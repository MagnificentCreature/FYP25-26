/**
 * @file Main.c
 * @brief CH570D BLE Backscatter Transmitter
 *
 * Generates an FSK-modulated PWM waveform for BLE backscatter communication
 * on the WCH CH570D / CH572 RISC-V microcontroller.
 *
 * How it works:
 *   1. A BLE-compatible packet is assembled (preamble + sync + length + seq + payload).
 *   2. For every bit in the packet, PWM period and edge-count values are
 *      pre-computed into lookup tables (pwm_periods[] and bit_counts[]).
 *   3. PWM2 drives the backscatter antenna. A hardware timer (TMR0) counts
 *      falling edges of the PWM output; when enough edges have elapsed for
 *      one bit period, an ISR switches to the next bit's frequency.
 *   4. Transmission is either UART-triggered (single-shot) or periodic,
 *      controlled by BACKSCATTER_PERIOD_MS in backscatter_config.h.
 *
 * All tunable parameters (frequencies, bit rate, packet size, period, pins)
 * are in backscatter_config.h.
 */

#include "CH57x_common.h"
#include "backscatter_config.h"
#include "packet_gen.h"
#include "utility.h"

// Bring in the I2C and Sensor headers
#include "I2C/app_i2c.h"
#include "I2C/mcp9808.h"

#define PRINT_ON 0

/*============================================================================
 * TRANSMISSION STATE
 *============================================================================*/

/** Pre-computed PWM period register value for each bit in the packet. */
static uint16_t pwm_periods[TOTAL_BITS];

/** Pre-computed number of PWM edges (timer endpoint) for each bit. */
static uint16_t bit_counts[TOTAL_BITS];

/** Index of the bit currently being transmitted (written by ISR). */
static volatile uint16_t bit_index = 0;

/** Set to 1 by the ISR when the last bit has been sent. */
static volatile uint8_t tx_done = 0;

/** Nonzero once the lookup tables have been built for the current message. */
static uint8_t tables_built = 0;

/** PWM period counter values for FSK freq0 and freq1. */
static uint16_t period_f0, period_f1;

/** Number of PWM edges per bit for freq0 and freq1 (after compensation). */
static uint16_t edges_per_bit_f0, edges_per_bit_f1;

/** Packet buffer, 4-byte aligned for the RISC-V core. */
static uint8_t message[MESSAGE_BYTES] = {0};

/** Rolling packet sequence number (incremented each transmission). */
static uint8_t seq = 0;

/*============================================================================
 * PERIPHERAL INITIALIZATION
 *============================================================================*/

/**
 * @brief Initialize UART0 for debug serial output.
 *
 * Remaps TX -> PA11, RX -> PA10. Uses default baud rate from UART_DefInit().
 */
static void Debug_Init(void)
{
    GPIOA_SetBits(GPIO_Pin_10);
    GPIOA_ModeCfg(GPIO_Pin_10, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(GPIO_Pin_11, GPIO_ModeOut_PP_5mA);
    UART_Remap(ENABLE, UART_TX_REMAP_PA11, UART_RX_REMAP_PA10);
    UART_DefInit();
}

/**
 * @brief Initialize PWM peripheral for backscatter output.
 *
 * Configures BACKSCATTER_PWM_PIN as push-pull output, sets the PWM clock
 * prescaler, and enables 16-bit cycle mode.  Does NOT start PWM output;
 * that happens in StartFSK().
 */
static void PWM_Init(void)
{
    GPIOA_ModeCfg(BACKSCATTER_PWM_PIN, BACKSCATTER_PWM_DRIVE);
    PWMX_CLKCfg(CLK_TICKS_PER_CYC);
    PWM_16bit_CycleEnable();
}

/**
 * @brief Configure TMR0 as a falling-edge counter on the PWM2 output.
 *
 * When the count reaches the programmed endpoint (R32_TMR_CNT_END), a
 * cycle-end interrupt fires so the ISR can advance to the next FSK bit.
 */
static void Timer_EdgeCounter_Init(void)
{    
    // Remap timer PWM0 to PA2, and also timer input to PA2 (for internal feedback)
    // Clear both timer pin remap bits
    R16_PIN_ALTERNATE_H &= ~RB_TMR_PIN;
    R16_PIN_ALTERNATE_H |= (1 << 6);   // because bit6 is the lower bit of the field
    

    R8_TMR_CTRL_MOD  = RB_TMR_ALL_CLEAR;
    R8_TMR_CTRL_MOD |= RB_TMR_MODE_IN | RB_TMR_CAP_COUNT;
    R8_TMR_CTRL_MOD |= (2 << 6);               /* count on falling edges */
    R8_TMR_INTER_EN |= RB_TMR_IE_CYC_END;
}

void Configure_EnergyGate_Interrupt(void) {
    GPIOA_ModeCfg(V_OK_PIN, GPIO_ModeIN_PU);
    GPIOA_ITModeCfg(V_OK_PIN, GPIO_ITMode_RiseEdge); // ÏÂ½µÑØ»½ÐÑ
    PFIC_EnableIRQ(GPIO_A_IRQn);
    PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_GPIO_WAKE, Fsys_Delay_4096);
}

/**
 * @brief Compute FSK modulation parameters from the configuration defines.
 *
 * Calculates PWM period values and per-bit edge counts for both FSK
 * frequencies, applying clock-source-dependent compensation constants.
 */
static void Compute_FSK_Parameters(void)
{
    period_f0 = PWM_CYC_COUNT0;
    period_f1 = PWM_CYC_COUNT1;

    uint8_t pll = Is_PLL_Clock();

    /*
     * edges_per_bit = FSK_FREQx / FSK_BIT_RATE
     * Adjusted by a small constant to compensate for CPU cycles spent
     * updating PWM registers inside the ISR.
     */
    edges_per_bit_f0 = FSK_FREQ0 / FSK_BIT_RATE
                     - (pll ? PLL_SWITCH_CONSTANT_0 : HSE_SWITCH_CONSTANT_0);
    edges_per_bit_f1 = FSK_FREQ1 / FSK_BIT_RATE
                     - (pll ? PLL_SWITCH_CONSTANT_1 : HSE_SWITCH_CONSTANT_1);

    #if PRINT_ON
    printf("Clock source : %s\n", pll ? "PLL" : "HSE");
    printf("Freq0 period : %u,  edges/bit: %u\n", period_f0, edges_per_bit_f0);
    printf("Freq1 period : %u,  edges/bit: %u\n", period_f1, edges_per_bit_f1);
    #endif
}

/*============================================================================
 * FSK LOOKUP TABLE & TRANSMISSION
 *============================================================================*/

/**
 * @brief Build pre-computed lookup tables for every bit in the packet.
 *
 * For each bit (MSB-first), stores the corresponding PWM period and
 * edge count.  This eliminates per-bit computation in the time-critical ISR.
 *
 * @param data  Packet byte array.
 * @param len   Number of bytes in data.
 */
static void BuildLookupTables(const uint8_t *data, uint32_t len)
{
    if (tables_built)
        return;

    uint32_t idx = 0;
    for (uint32_t i = 0; i < len; i++) {
        uint8_t byte = data[i];
        for (int8_t bit = 7; bit >= 0; bit--) {        /* MSB first */
            uint8_t b = (byte >> bit) & 1;
            pwm_periods[idx] = b ? period_f1 : period_f0;
            bit_counts[idx]  = b ? edges_per_bit_f1 : edges_per_bit_f0;
            idx++;
        }
    }
    tables_built = 1;
}

// Change the function to accept an 'offset' and 'length' 
// so we only update the parts of the table that changed.
static void UpdateLookupTable(const uint8_t *data, uint32_t start_byte, uint32_t num_bytes)
{
    uint32_t bit_idx = start_byte * 8; 
    
    for (uint32_t i = 0; i < num_bytes; i++) {
        uint8_t byte = data[i];
        for (int8_t bit = 7; bit >= 0; bit--) {
            uint8_t b = (byte >> bit) & 1;
            // Directly overwrite the specific memory locations
            pwm_periods[bit_idx] = b ? period_f1 : period_f0;
            bit_counts[bit_idx]  = b ? edges_per_bit_f1 : edges_per_bit_f0;
            bit_idx++;
        }
    }
}


static void FastUpdatePacketAndTables(uint16_t raw_temp) {
    // 1. Update sequence byte
    message[HEADER_LEN - 1] = seq;
    
    // 2. Update payload bytes
    memcpy(&message[HEADER_LEN], &raw_temp, sizeof(uint16_t));

    // 3. Update lookup tables ONLY for the Sequence (1 byte) + Payload (size)
    // message offset is (HEADER_LEN - 1). 
    UpdateLookupTable(&message[HEADER_LEN - 1], HEADER_LEN - 1, 1 + PAYLOADSIZE);
}


/**
 * @brief Invalidate lookup tables so they are rebuilt before the next TX.
 *
 * Call after modifying the message buffer (e.g., new sequence number).
 */
static void InvalidateTables(void)
{
    tables_built = 0;
}

/**
 * @brief Begin FSK transmission of the packet.
 *
 * Builds lookup tables if needed, programmes PWM and timer for the first
 * bit, then enables the timer interrupt. The ISR drives the rest of the
 * packet bit-by-bit until TOTAL_BITS have been sent.
 *
 * @param data  Packet byte array.
 * @param len   Number of bytes.
 */
__HIGH_CODE
static void StartFSK(void)
{
    bit_index = 0;
    tx_done   = 0;

    PFIC_EnableIRQ(TMR_IRQn);

    /* Configure PWM2 for the first bit */
    R16_PWM_CYC_VALUE = pwm_periods[0];
    R16_PWM2_DATA     = pwm_periods[0] >> 1;

    /* Programme timer endpoint for the first bit */
    R32_TMR_CNT_END = bit_counts[0];

    /* Clear and start the edge counter */
    R8_TMR_CTRL_MOD |=  RB_TMR_ALL_CLEAR;
    R8_TMR_CTRL_MOD &= ~RB_TMR_ALL_CLEAR;
    R8_TMR_CTRL_MOD |=  RB_TMR_COUNT_EN;

    /* Enable PWM2 output, active-high polarity */
    R8_PWM_POLAR  |= RB_PWM2_POLAR;
    R8_PWM_OUT_EN |= RB_PWM2_OUT_EN;

    do
    {
        __nop();        
    }while(!tx_done);
}

/**
 * @brief Stop FSK output and tri-state the antenna pin.
 */
__HIGH_CODE
static void StopFSK(void)
{
    PWMX_16bit_ACTOUT(CH_PWM2, 0, Low_Level, DISABLE);
}

/*============================================================================
 * PACKET CONSTRUCTION
 *============================================================================*/

/**
 * @brief Assemble a complete packet (header + PRBS payload) into message[].
 *
 * Invalidates the lookup tables so they are rebuilt on the next StartFSK().
 */
static void BuildPacket(uint16_t raw_temp) {
    add_header(message, seq, packet_hdr_template());

    uint8_t payload[PAYLOADSIZE];

    #if SENSING_MODE == 0
    GeneratePRBSPayload(payload, PAYLOAD_LFSR_SEED);
    # else
    // Copy the sensor data directly in
    memcpy(payload, &raw_temp, sizeof(uint16_t));
    # endif

    // Stick it to the header
    memcpy(&message[HEADER_LEN], payload, PAYLOADSIZE);    

    InvalidateTables();
}

void I2C_setup(void){
    I2C_Init(I2C_Mode_I2C, 400000, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, 0x42);
    i2c_app_init(0x42); 
    
    // Add a simple timeout to prevent freezing if the 2.6V sensor fails
    uint32_t timeout = 100000; 
    while(I2C_GetFlagStatus(I2C_FLAG_BUSY) != RESET && --timeout > 0);
    
    if (timeout == 0) {
        #if PRINT_ON
        PRINT("I2C BUSY Timeout! Sensor might be locked.\n");
        #endif
        // Hardware reset the I2C peripheral to recover
        R16_I2C_CTRL1 |= RB_I2C_SWRST;
        R16_I2C_CTRL1 &= ~RB_I2C_SWRST;
    }
}

/*============================================================================
 * MAIN
 *============================================================================*/

int main(void)
{
    /* --- 1. COLD START --- */
    R16_PIN_ALTERNATE &= ~RB_PIN_DEBUG_EN;
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_PLL_100MHz); 
    GPIOA_ModeCfg(GPIO_Pin_All, GPIO_ModeIN_PU);

    
    Timer_EdgeCounter_Init();

    // Warm?start peripherals (PWM, timer, I2C)
    PWM_Init();    
    I2C_setup();

    Compute_FSK_Parameters();
    add_header(message, seq, packet_hdr_template());
    BuildLookupTables(message, MESSAGE_BYTES);

    uint16_t raw_temp = (SENSING_MODE ? MCP9808_ReadRawTemperature() : 0);
        
    seq++;
    FastUpdatePacketAndTables(raw_temp);

    for (int i = 0; i < 2; i++) {
        StartFSK();
        DelayUs(50);
    }
    LowPower_Shutdown(0);
}
/*============================================================================
 * INTERRUPT HANDLER
 *============================================================================*/

/**
 * @brief TMR0 cycle-end ISR -- advances FSK modulation one bit at a time.
 *
 * Fires when the edge counter reaches bit_counts[bit_index].  Updates PWM
 * period and duty for the next bit, reloads the timer endpoint.  After the
 * last bit, stops the timer/PWM and sets tx_done.
 */
__INTERRUPT
__HIGH_CODE
void TMR_IRQHandler(void)
{
    if (R8_TMR_INT_FLAG & RB_TMR_IF_CYC_END) {
        R8_TMR_INT_FLAG = RB_TMR_IF_CYC_END;       /* clear flag */

        bit_index++;
        if (bit_index >= TOTAL_BITS) {
            /* Packet complete -- shut down */
            R8_TMR_CTRL_MOD &= ~RB_TMR_COUNT_EN;
            R8_PWM_OUT_EN   &= ~RB_PWM2_OUT_EN;
            PFIC_DisableIRQ(TMR_IRQn);
            tx_done = 1;
            return;
        }

        /* Reload timer endpoint for next bit */
        R32_TMR_CNT_END = bit_counts[bit_index];

        /* Load next bit's PWM period (50 % duty) */
        R16_PWM_CYC_VALUE = pwm_periods[bit_index];
        R16_PWM2_DATA     = pwm_periods[bit_index] >> 1;
    }
}

/**
 * @brief RTC trigger interrupt handler ¨C clears flag and wakes the CPU.
 */
__INTERRUPT
__HIGH_CODE
void RTC_IRQHandler(void)
{
    if (RTC_GetITFlag(RTC_TRIG_EVENT)) {
        RTC_ClearITFlag(RTC_TRIG_EVENT);
    }
}

/*********************************************************************
 * @fn      GPIOA_IRQHandler
 *
 * @brief   GPIOAÖÐ¶Ïº¯Êý
 *
 * @return  none
 */
__INTERRUPT
__HIGH_CODE
void GPIOA_IRQHandler(void)
{
    GPIOA_ClearITFlagBit(V_OK_PIN);
}
