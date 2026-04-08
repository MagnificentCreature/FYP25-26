/**
 * @file utility.c
 * @brief Debug printing helpers and PWM interrupt configuration.
 */

#include "CH57x_common.h"
#include "utility.h"



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
    for (size_t i = 0; i < size; i++) {
        printf("0x%02X%s", arr[i], (i == size - 1) ? "" : ", ");
        
        // Wrap lines every 12 hex values for readability
        if ((i + 1) % 12 == 0 && i != size - 1) {
            printf("\n    ");
        }
    }
}

void print_uint8_array_dec(const uint8_t *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
        printf("%02d ", arr[i]);
    printf("\n");
}

void print_8bit_binary(uint8_t val)
{
    // Count UP from bit 0 (t=0) to bit 7 (t=7)
    for (int i = 0; i <= 7; i++) {
        printf("%d", (val >> i) & 1);
    }
    printf(" ");
}

void print_uint8_array_bin(const uint8_t *arr, size_t size)
{
    for (size_t i = 0; i < size/4; i++){
        for (size_t j = 0; j < 4; j++){
            print_8bit_binary(arr[i * 4 + j]);
        }
        printf("\n");
    }
}

void print_uint16_array_hex(const uint16_t *arr, size_t size)
{
    for (size_t i = 0; i < size; i++)
        printf("%04x ", arr[i]);
    printf("\n");
}


void print_32bit_binary(uint32_t val)
{
    // Count UP from bit 0 (t=0) to bit 31 (t=31)
    for (int i = 0; i <= 31; i++) {
        printf("%d", (val >> i) & 1);
        
        // Add a space every 8 bits for readability
        if ((i + 1) % 8 == 0 && i != 31) {
            printf(" ");
        }
    }
}

void print_uint32_array(const uint32_t *data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        print_32bit_binary(data[i]);
        printf("\n"); // Add a newline so you can easily read the chunks
    }
}