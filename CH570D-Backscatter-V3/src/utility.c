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
