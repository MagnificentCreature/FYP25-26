/**
 * @file utility.h
 * @brief Debug printing helpers and PWM interrupt configuration.
 */

#ifndef UTILITY_H
#define UTILITY_H

#include "CH57x_common.h"
#include <stdint.h>
#include <stddef.h>

/** Enable or disable PWM interrupt sources. */
void PWM_INTCfg(FunctionalState s, uint8_t irq_mask);

/** Check if PPL clock is on*/ 
uint8_t Is_PLL_Clock(void);

/**Verify if the system clock setting matches actual clock*/
void VerifySysClock(u_long sys_clk_freq);

/** Print a uint8_t array as space-separated hex bytes. */
void print_uint8_array_hex(const uint8_t *arr, size_t size);

/** Print a uint8_t array as space-separated dec. */
void print_uint8_array_dec(const uint8_t *arr, size_t size);

/** Print a uint8_t array as space-separated binary. */
void print_uint8_array_bin(const uint8_t *arr, size_t size);

/** Print a uint16_t array as space-separated hex values. */
void print_uint16_array_hex(const uint16_t *arr, size_t size);

/** Print a 32-bit value in binary with byte separators. */
void print_32bit_binary(uint32_t val);

/** Print a uint32_t array as decimal with index labels. */
void print_uint32_array(const uint32_t *data, size_t len);

#endif /* UTILITY_H */
