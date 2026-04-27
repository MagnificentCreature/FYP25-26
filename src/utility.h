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

/** Print a uint16_t array as space-separated hex values. */
void print_uint16_array_hex(const uint16_t *arr, size_t size);

/** Print a 32-bit value in binary with byte separators. */
void print_32bit_binary(uint32_t val);

/** Print a uint32_t array as decimal with index labels. */
void print_uint32_array(const uint32_t *data, size_t len);

/**
 * @brief Fill a buffer with deterministic pseudo-random data (32-bit LFSR).
 *
 * Taps: bits 32, 22, 2, 1 (Galois form).  Produces a repeatable sequence
 * for a given seed, useful for BER testing on the receiver side.
 *
 * @param buf   Destination (>= PAYLOADSIZE bytes).
 * @param seed  LFSR seed (must be nonzero).
 */
void GeneratePRBSPayload(uint8_t *buf, uint32_t seed);

/**
 * @brief Initialise the low?power sleep utility.
 *        Must be called once before using SleepMs().
 */
void Sleep_Init(void);

/**
 * @brief Enter deep sleep for the specified number of milliseconds.
 * @param ms  Wake?up time in milliseconds (max ~24 hours for 32 kHz LSI).
 */
void SleepMs(uint32_t ms);

#endif /* UTILITY_H */
