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
 *   3. PWM1 drives the backscatter antenna. A hardware timer (TMR0) counts
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
 * Remaps TX -> PA3, RX -> PA2. Uses default baud rate from UART_DefInit().
 */
static void Debug_Init(void)
{
    GPIOA_SetBits(bTXD_0);
    GPIOA_ModeCfg(bRXD_0, GPIO_ModeIN_PU);
    GPIOA_ModeCfg(bTXD_0, GPIO_ModeOut_PP_5mA);
    UART_Remap(ENABLE, UART_TX_REMAP_PA3, UART_RX_REMAP_PA2);
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
 * @brief Configure TMR0 as a falling-edge counter on the PWM1 output.
 *
 * When the count reaches the programmed endpoint (R32_TMR_CNT_END), a
 * cycle-end interrupt fires so the ISR can advance to the next FSK bit.
 */
static void Timer_EdgeCounter_Init(void)
{
    R8_TMR_CTRL_MOD  = RB_TMR_ALL_CLEAR;
    R8_TMR_CTRL_MOD |= RB_TMR_MODE_IN | RB_TMR_CAP_COUNT;
    R8_TMR_CTRL_MOD |= (2 << 6);               /* count on falling edges */
    R8_TMR_INTER_EN |= RB_TMR_IE_CYC_END;
}

/**
 * @brief Detect whether the system clock is sourced from PLL.
 * @return 1 if PLL, 0 if HSE (CK32M).
 */
static uint8_t Is_PLL_Clock(void)
{
    return (((R8_CLK_SYS_CFG >> 6) & 0x03) == 0x01) ? 1 : 0;
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

    printf("Clock source : %s\n", pll ? "PLL" : "HSE");
    printf("Freq0 period : %u,  edges/bit: %u\n", period_f0, edges_per_bit_f0);
    printf("Freq1 period : %u,  edges/bit: %u\n", period_f1, edges_per_bit_f1);
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
static void StartFSK(const uint8_t *data, uint32_t len)
{
    BuildLookupTables(data, len);
    bit_index = 0;
    tx_done   = 0;

    PFIC_EnableIRQ(TMR_IRQn);

    /* Configure PWM for the first bit (50 % duty cycle) */
    R16_PWM_CYC_VALUE = pwm_periods[0];
    R16_PWM1_DATA     = pwm_periods[0] >> 1;

    /* Programme timer endpoint for the first bit */
    R32_TMR_CNT_END = bit_counts[0];

    /* Clear and start the edge counter */
    R8_TMR_CTRL_MOD |=  RB_TMR_ALL_CLEAR;
    R8_TMR_CTRL_MOD &= ~RB_TMR_ALL_CLEAR;
    R8_TMR_CTRL_MOD |=  RB_TMR_COUNT_EN;

    /* Enable PWM1 output, active-high polarity */
    R8_PWM_POLAR  |= RB_PWM1_POLAR;
    R8_PWM_OUT_EN |= RB_PWM1_OUT_EN;

    while(!tx_done)
    {
        __nop();        
    }
}

/**
 * @brief Stop FSK output and tri-state the antenna pin.
 */
static void StopFSK(void)
{
    PWMX_16bit_ACTOUT(CH_PWM1, 0, High_Level, DISABLE);
    GPIOA_ModeCfg(BACKSCATTER_PWM_PIN, GPIO_ModeIN_PU);
}

/*============================================================================
 * PACKET CONSTRUCTION
 *============================================================================*/

/**
 * @brief Fill a buffer with deterministic pseudo-random data (32-bit LFSR).
 *
 * Taps: bits 32, 22, 2, 1 (Galois form).  Produces a repeatable sequence
 * for a given seed, useful for BER testing on the receiver side.
 *
 * @param buf   Destination (>= PAYLOADSIZE bytes).
 * @param seed  LFSR seed (must be nonzero).
 */
static void GeneratePRBSPayload(uint8_t *buf, uint32_t seed)
{
    uint32_t lfsr = seed;
    for (uint8_t i = 0; i < PAYLOADSIZE; i++) {
        uint32_t fb = ((lfsr >> 0) ^ (lfsr >> 1)
                     ^ (lfsr >> 21) ^ (lfsr >> 31)) & 1;
        lfsr = (lfsr >> 1) | (fb << 31);
        buf[i] = (uint8_t)(lfsr & 0xFF);
    }
}

/**
 * @brief Assemble a complete packet (header + PRBS payload) into message[].
 *
 * Invalidates the lookup tables so they are rebuilt on the next StartFSK().
 */
static void BuildPacket(void)
{
    add_header(message, seq, packet_hdr_template());

    uint8_t payload[PAYLOADSIZE];
    GeneratePRBSPayload(payload, PAYLOAD_LFSR_SEED);
    memcpy(&message[HEADER_LEN], payload, PAYLOADSIZE);

    InvalidateTables();
}

/*============================================================================
 * MAIN
 *============================================================================*/

int main(void)
{
    /* Disable SWD debug pins to free GPIOs */
    R16_PIN_ALTERNATE &= ~RB_PIN_DEBUG_EN;

    /* Configure system clock */
    HSECFG_Capacitance(HSECap_18p);
    SetSysClock(CLK_SOURCE_HSE_6_4MHz);

    /* Serial debug output */
    Debug_Init();
    PRINT("CH570D Backscatter V3\n");
    DelayMs(10);

    /* Compute FSK parameters from config defines */
    Compute_FSK_Parameters();
    DelayMs(100);

    /* Initialise peripherals */
    PWM_Init();
    Timer_EdgeCounter_Init();

    /* Build and display the packet */
    BuildPacket();
    print_uint8_array_hex(message, MESSAGE_BYTES);
    DelayMs(100);

#if BACKSCATTER_PERIOD_MS > 0
    /* ----- Periodic transmission mode ----- */
    printf("Mode: periodic TX every %d ms\n", BACKSCATTER_PERIOD_MS);
    DelayMs(10);

    while (1) {
        StartFSK(message, MESSAGE_BYTES);

        /* increment sequence number for each packet */
        seq++;
        BuildPacket();

        PRINT("Periodic TX end\n");
        DelayMs(BACKSCATTER_PERIOD_MS);
    }

#else
    /* ----- UART-triggered single-shot mode ----- */
    printf("Mode: UART-triggered TX\n");
    DelayMs(10);

    uint8_t len;
    uint8_t rx_buf[100];
    while (1) {
        len = UART_RecvString(rx_buf);
        if (len) {
            len = 0;
            PRINT("Transmitting\n");
            StartFSK(message, MESSAGE_BYTES);
            PRINT("Transmitted\n");
            DelayMs(1);
        }
    }
#endif
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
            R8_PWM_OUT_EN   &= ~RB_PWM1_OUT_EN;
            PFIC_DisableIRQ(TMR_IRQn);
            tx_done = 1;
            return;
        }

        /* Load next bit's PWM period (50 % duty) */
        R16_PWM_CYC_VALUE = pwm_periods[bit_index];
        R16_PWM1_DATA     = pwm_periods[bit_index] >> 1;

        /* Reload timer endpoint for next bit */
        R32_TMR_CNT_END = bit_counts[bit_index];
    }
}
