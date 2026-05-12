/**
 * @file utility.c
 * @brief Debug printing helpers and PWM interrupt configuration.
 */

#include "CH57x_common.h"
#include "utility.h"
#include "backscatter_config.h"

void PWM_INTCfg(FunctionalState s, uint8_t irq_mask)
{
    if (s)
        R8_PWM_INT_EN |= irq_mask;
    else
        R8_PWM_INT_EN &= ~irq_mask;
}

/**
 * @brief Detect whether the system clock is sourced from PLL.
 * @return 1 if PLL, 0 if HSE (CK32M).
 */
uint8_t Is_PLL_Clock(void)
{
    return (((R8_CLK_SYS_CFG >> 6) & 0x03) == 0x01) ? 1 : 0;
}

/**
 * @brief Get the actual system clock frequency in Hz by reading hardware registers.
 * @return frequency in Hz
 */
uint32_t GetSysClockFreq(void)
{
    uint8_t clk_cfg = R8_CLK_SYS_CFG;
    uint8_t src = (clk_cfg >> 6) & 0x03;   // clock source
    uint8_t div = clk_cfg & 0x1F;           // divisor field (5 bits)

    // Divisor: if div == 0, it means 32 (max), otherwise it's the divisor value.
    uint32_t divisor = (div == 0) ? 32 : div;

    uint32_t freq;
    if (src == 0x01) {  // PLL mode (default 600 MHz)
        // Assuming PLL is at 600 MHz (can be changed, but typically fixed)
        freq = 600000000UL / divisor;
    } else {            // HSE mode (default 32 MHz)
        // HSE frequency is 32 MHz (from external crystal)
        freq = 32000000UL / divisor;
    }
    return freq;
}

void VerifySysClock(u_long sys_clk_freq){
    uint32_t actual_freq = GetSysClockFreq();
    if (actual_freq != sys_clk_freq) {
        printf("ERROR: System clock mismatch! Expected %lu Hz, actual %lu Hz\n",
            (unsigned long)sys_clk_freq, (unsigned long)actual_freq);
    }
}


void print_uint8_array_hex(const uint8_t *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
        printf("%02x ", arr[i]);
    printf("\n");
}

void print_uint16_array_hex(const uint16_t *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
        printf("%04x ", arr[i]);
    printf("\n");
}

void print_32bit_binary(uint32_t val)
{
    for (int i = 31; i >= 0; i--) {
        printf("%d", (val >> i) & 1);
        if (i % 8 == 0 && i != 0)
            printf(" ");
    }
}

void print_uint32_array(const uint32_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++)
        printf("[%u] %u\n", (unsigned)i, data[i]);
}

/**
 * @brief Fill a buffer with deterministic pseudo-random data (32-bit LFSR).
 *
 * Taps: bits 32, 22, 2, 1 (Galois form).  Produces a repeatable sequence
 * for a given seed, useful for BER testing on the receiver side.
 *
 * @param buf   Destination (>= PAYLOADSIZE bytes).
 * @param seed  LFSR seed (must be nonzero).
 */
void GeneratePRBSPayload(uint8_t *buf, uint32_t seed)
{
    uint32_t lfsr = seed;
    for (uint8_t i = 0; i < PAYLOADSIZE; i++) {
        uint32_t fb = ((lfsr >> 0) ^ (lfsr >> 1)
                     ^ (lfsr >> 21) ^ (lfsr >> 31)) & 1;
        lfsr = (lfsr >> 1) | (fb << 31);
        buf[i] = (uint8_t)(lfsr & 0xFF);
    }
}

// External variable from CH57x_clk.c ¨C holds measured LSI frequency
extern uint32_t Freq_LSI;

// Flag to remember if LSI has been calibrated
static uint8_t lsi_calibrated = 0;

void Sleep_Init(void)
{
    if (!lsi_calibrated) {
        // Calibrate LSI oscillator (takes a few milliseconds)
        RTC_InitClock(Count_1024);
        lsi_calibrated = 1;
    }
}

/**
 * @brief Enter deep sleep (Sleep mode) and wake up after given milliseconds using RTC.
 * @param ms  Wake-up time in milliseconds (max ~1300 ms with 32kHz LSI)
 */
void SleepMs(uint32_t ms)
{
    if (ms == 0) return;
    
    // Ensure LSI frequency is known
    if (!lsi_calibrated) {
        Sleep_Init();
    }

    // Compute number of LSI cycles for the desired delay
    uint32_t cycles = (ms * Freq_LSI) / 1000;

    // Configure RTC as a one-shot wake-up timer
    RTC_TRIGFunCfg(cycles);                 // set trigger after 'cycles' LSI ticks
    PWR_PeriphWakeUpCfg(ENABLE, RB_SLP_RTC_WAKE, Fsys_Delay_4096);
    PFIC_EnableIRQ(RTC_IRQn);               // enable RTC interrupt in NVIC

    // Enter deep sleep (Sleep mode). RAM retained, only RTC runs.
    LowPower_Sleep(RB_PWR_RAM12K | RB_PWR_EXTEND);

    // ----- Woken by RTC interrupt -----
    // Restore HSE bias current (LowPower_Sleep may have changed it)
    HSECFG_Current(HSE_RCur_100);

    // Disable RTC wake-up and clear the trigger
    // PWR_PeriphWakeUpCfg(DISABLE, RB_SLP_RTC_WAKE, Fsys_Delay_4096);
    RTC_ModeFunDisable(RTC_TRIG_MODE);
    // PFIC_DisableIRQ(RTC_IRQn);
}

