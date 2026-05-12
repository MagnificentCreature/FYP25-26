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

    //Set polarity to output high
    R8_PWM_POLAR  |= RB_PWM2_POLAR;
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
    GPIOA_ITModeCfg(V_OK_PIN, GPIO_ITMode_FallEdge); // ÏÂ½µÑØ»½ÐÑ
    PFIC_EnableIRQ(GPIO_A_IRQn);
    PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_GPIO_WAKE, Fsys_Delay_8191);
    // Configure the base wake behavior
    // Note: The 3rd parameter in the WCH HAL is usually Edge/Level config, 
    // passing a delay macro here can overwrite edge settings.

    // FIX 1: Latch the GPIO event so short pulses don't abort the wake up
    // 1: Enable memory, support short-pulse event wake-up 
    R8_SLP_WAKE_CTRL |= RB_WAKE_EV_MODE; 
    
    // FIX 2: Maximize the Wake Delay for the HSE
    // Set RB_WAKE_DLY_MOD to 100b (8191 cycles) to give the HSE maximum time 
    // at 16MHz.
    R8_SLP_POWER_CTRL = (R8_SLP_POWER_CTRL & ~RB_WAKE_DLY_MOD) | 0x04;
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
            pwm_periods[idx] = b ? PWM_CYC_COUNT1 : PWM_CYC_COUNT0;
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
            pwm_periods[bit_idx] = b ? PWM_CYC_COUNT1 : PWM_CYC_COUNT0;
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

    SetSysClock(BACKSCATTER_SYS_FREQ);

    /* Enable PWM2 output, active-high polarity */
    R8_PWM_OUT_EN |= RB_PWM2_OUT_EN;

    do
    {
        __nop();        
    }while(!tx_done);
}

/**Resume FSK transmission of the packet**/
__HIGH_CODE
static void ResumeFSK(void)
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
    R8_RST_WDOG_CTRL &= ~RB_WDOG_INT_EN;   // disable IWDG
    
    #if PRINT_ON
    Debug_Init();
    PRINT("Power on\n");
    uint8_t val = R8_CLK_SYS_CFG;  // read the register
    printf("R8_CLK_SYS_CFG = 0x%02X\r\n", val);    
    #endif

    Configure_EnergyGate_Interrupt();
    Timer_EdgeCounter_Init();

    // // Warm?start peripherals (PWM, timer, I2C)
    PWM_Init();    
    I2C_setup();

    MCP9808_SetResolution_0_5C(); 
    MCP9808_Sleep();
    
    #if PRINT_ON
    // printf("Clock source : %s\n", Is_PLL_Clock() ? "PLL" : "HSE");
    printf("Freq0 period : %u,  edges/bit: %u\n", PWM_CYC_COUNT0, edges_per_bit_f0);
    printf("Freq1 period : %u,  edges/bit: %u\n", PWM_CYC_COUNT1, edges_per_bit_f1);
    #endif

    add_header(message, seq, packet_hdr_template());
    BuildLookupTables(message, MESSAGE_BYTES);

    #if PRINT_ON
    PRINT("Initialised\n");
    DelayMs(1);
    #endif

    while (1) {
        LowPower_Sleep( RB_PWR_RAM12K | RB_PWR_EXTEND);

        // --- Wake-up from GPIO interrupt ---
        #if PRINT_ON
        PRINT("UP\n");
        #endif

        // Read sensor, update packet, transmit...
        MCP9808_Wake();
        SleepMs(30);
        uint16_t raw_temp = (SENSING_MODE ? MCP9808_ReadRawTemperature() : 0);
        MCP9808_Sleep();
        seq++;
        FastUpdatePacketAndTables(raw_temp);

        #if PRINT_ON
        print_uint8_array_hex(message, 12);
        DelayMs(1);
        #endif

        StartFSK();
        for (int i = 0; i < 2; i++) {
            DelayUs(5); //Takes 100us on a 8MHz clock
            ResumeFSK();
        }
    }
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

        /* Load next bit's PWM period (~50 % duty) */
        R16_PWM2_DATA     = pwm_periods[bit_index] >> 1;
        R16_PWM_CYC_VALUE = pwm_periods[bit_index];
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