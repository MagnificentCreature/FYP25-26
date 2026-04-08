/**
 * @file packet_gen.c
 * @brief BLE-compatible packet header construction.
 *
 * The header layout (HEADER_LEN = 10 bytes):
 *   [0..3]  Preamble   : 0xAA 0xAA 0xAA 0xAA
 *   [4..7]  Sync / Access Address : 0x93 0x0B 0x51 0xDE
 *   [8]     Payload length (1 + PAYLOADSIZE, per CC2500 convention)
 *   [9]     Sequence number
 */

#include "CH57x_common.h"
#include "packet_gen.h"

/** Default header template: preamble + sync word (length & seq filled later). */
static uint8_t packet_hdr[HEADER_LEN] = {
    0xAA, 0xAA, 0xAA, 0xAA,    /* preamble */
    0x93, 0x0B, 0x51, 0xDE,    /* sync / access address */
    0x00,                       /* length  (set by add_header) */
    0x00                        /* seq num (set by add_header) */
};

uint8_t *packet_hdr_template(void)
{
    return packet_hdr;
}

void add_header(uint8_t *packet, uint8_t seq, uint8_t *header_template)
{
    /* Copy preamble + sync word */
    for (int i = 0; i < HEADER_LEN - 2; i++) {
        packet[i] = header_template[i];
    }

    /*
     * Length byte: payload data size + 1 (excludes the length byte itself
     * and optional CRC, per CC2500 datasheet p. 30).
     */
    packet[HEADER_LEN - 2] = 1 + PAYLOADSIZE;

    /* Sequence number */
    packet[HEADER_LEN - 1] = seq;
}
