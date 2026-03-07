/**
 * @file backscatter_config.h
 * @brief Centralized configuration for CH570D BLE backscatter transmitter.
 *
 * All tunable parameters for the backscatter system are defined here.
 * Modify these values to adjust FSK modulation, timing, packet structure,
 * and transmission behavior.
 *
 * Target: WCH CH570D / CH572 RISC-V microcontroller
 */

#ifndef BACKSCATTER_CONFIG_H
#define BACKSCATTER_CONFIG_H

/*============================================================================
 * SYSTEM CLOCK CONFIGURATION
 *============================================================================*/

/**
 * System clock frequency in Hz.
 * Must match the SetSysClock() call in main().
 * Common values: 6400000 (HSE 6.4 MHz), 16000000 (PLL 16 MHz)
 */
#define SYS_CLK_FREQ           6400000
//Note: We override the value in CH57x_common.h by calling #define  FREQ_SYS	SYS_CLK_FREQ (line 53)


/**
 * PWM clock prescaler divider.
 * PWM base frequency = SYS_CLK_FREQ / CLK_TICKS_PER_CYC.
 * Must be >= 1. Increase only if PWM period counters would overflow 16 bits.
 */
#define CLK_TICKS_PER_CYC     1

/*============================================================================
 * FSK MODULATION PARAMETERS
 *============================================================================*/

/** FSK bit rate in bits per second. */
#define FSK_BIT_RATE           3000

/**
 * FSK frequency for logic '0' in Hz.
 * Must be less than SYS_CLK_FREQ / CLK_TICKS_PER_CYC.
 */
#define FSK_FREQ0              80000

/**
 * FSK frequency for logic '1' in Hz.
 * Must be less than SYS_CLK_FREQ / CLK_TICKS_PER_CYC.
 */
#define FSK_FREQ1              100000

/*============================================================================
 * PACKET CONFIGURATION
 *============================================================================*/

/** Header length in bytes (preamble + sync word + length byte + seq byte). */
#define HEADER_LEN             10

/** Payload size in bytes (excluding header). */
#define PAYLOADSIZE            14

/** LFSR seed for pseudo-random payload generation (must be nonzero). */
#define PAYLOAD_LFSR_SEED      0xABCDE

/*============================================================================
 * TRANSMISSION TIMING
 *============================================================================*/

/**
 * Backscatter transmission period in milliseconds.
 *
 *   0     = UART-triggered mode (waits for serial command to transmit once)
 *   > 0   = Periodic mode (automatically retransmits every N ms)
 *
 * Examples:
 *   #define BACKSCATTER_PERIOD_MS  0       // single-shot, UART trigger
 *   #define BACKSCATTER_PERIOD_MS  1000    // transmit every 1 second
 *   #define BACKSCATTER_PERIOD_MS  500     // transmit every 500 ms
 */
#define BACKSCATTER_PERIOD_MS  500

/*============================================================================
 * HARDWARE PIN CONFIGURATION
 *============================================================================*/

/** GPIO pin used for PWM backscatter output (PA7 = PWM1). */
#define BACKSCATTER_PWM_PIN    GPIO_Pin_7

/** PWM output drive strength for the backscatter pin. */
#define BACKSCATTER_PWM_DRIVE  GPIO_ModeOut_PP_20mA

/*============================================================================
 * CLOCK-SOURCE COMPENSATION CONSTANTS
 *
 * The timer counts PWM edges to time each FSK bit. When the ISR fires and
 * updates PWM registers, a few edges are lost during the switch. These
 * constants add extra edges to compensate.
 *
 * Positive values = add extra edges (transmission slightly slower).
 * Negative values = subtract edges (transmission slightly faster).
 *
 * Tune empirically with a spectrum analyzer or SDR receiver if you change
 * the clock source or frequency.
 *============================================================================*/

/** Cycle count correction for freq0 / freq1 when running from PLL. */
#define PLL_SWITCH_CONSTANT_0  (-1)
#define PLL_SWITCH_CONSTANT_1  (-1)

/** Cycle count correction for freq0 / freq1 when running from HSE. */
#define HSE_SWITCH_CONSTANT_0  (-3)
#define HSE_SWITCH_CONSTANT_1  (-2)

/*============================================================================
 * DERIVED CONSTANTS  (do not modify below this line)
 *============================================================================*/

/** PWM base frequency after prescaler division. */
#define BASE_FREQ              (SYS_CLK_FREQ / CLK_TICKS_PER_CYC)

/** 16-bit PWM period register value for FSK frequency 0. */
#define PWM_CYC_COUNT0         ((BASE_FREQ / FSK_FREQ0) - 1)

/** 16-bit PWM period register value for FSK frequency 1. */
#define PWM_CYC_COUNT1         ((BASE_FREQ / FSK_FREQ1) - 1)

/**
 * Compute the number of 4-byte words needed to hold (payload + header) bytes.
 * Used for 32-bit aligned buffer allocation on the RISC-V core.
 */
#define PACKET_BUF_WORDS(payload, header) \
    ((((payload) + (header)) % 4 == 0)   \
     ? (((payload) + (header)) / 4)      \
     : (((payload) + (header)) / 4 + 1))

/** Message buffer length in 4-byte words. */
#define MESSAGE_WORDS          PACKET_BUF_WORDS(PAYLOADSIZE, HEADER_LEN)

/** Message buffer length in bytes (4-byte aligned). */
#define MESSAGE_BYTES          (MESSAGE_WORDS * 4)

/** Total number of bits to transmit per packet. */
#define TOTAL_BITS             (MESSAGE_BYTES * 8)

/* Compile-time sanity checks */
#if FSK_FREQ0 >= BASE_FREQ
  #error "FSK_FREQ0 must be less than BASE_FREQ"
#endif
#if FSK_FREQ1 >= BASE_FREQ
  #error "FSK_FREQ1 must be less than BASE_FREQ"
#endif
#if PAYLOAD_LFSR_SEED == 0
  #error "PAYLOAD_LFSR_SEED must be nonzero"
#endif

#endif /* BACKSCATTER_CONFIG_H */
