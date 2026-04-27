/**
 * @file packet_gen.h
 * @brief BLE-compatible packet header generation.
 *
 * Provides functions to build a packet header containing:
 *   - 4-byte preamble (0xAA pattern)
 *   - 4-byte sync / access address
 *   - 1-byte payload length
 *   - 1-byte sequence number
 *
 * Header template and sizes are configured in backscatter_config.h.
 */

#ifndef PACKET_GEN_H
#define PACKET_GEN_H

#include "backscatter_config.h"
#include <stdint.h>

/**
 * @brief Return a pointer to the default packet header template.
 *
 * The template contains the preamble and sync word bytes.  Length and
 * sequence fields are filled in by add_header().
 *
 * @return Pointer to a HEADER_LEN-byte array (static storage).
 */
uint8_t *packet_hdr_template(void);

/**
 * @brief Write a complete header into the beginning of a packet buffer.
 *
 * Copies the template (preamble + sync), then appends the payload length
 * byte and the sequence number.
 *
 * @param packet           Destination buffer (must be >= HEADER_LEN bytes).
 * @param seq              Packet sequence number (0-255, wraps).
 * @param header_template  Pointer returned by packet_hdr_template().
 */
void add_header(uint8_t *packet, uint8_t seq, uint8_t *header_template);

#endif /* PACKET_GEN_H */
