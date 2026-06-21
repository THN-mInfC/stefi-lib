#pragma once

#include <stddef.h>
#include <stdint.h>
#include "libstefi/peripheral.h"

/**********************************************************************/
/* DMA  (Direct Memory Access)  Hardware Abstraction layer            */
/**********************************************************************/

/**
 * @brief Blocking memory-to-memory copy using one DMA channel.
 *
 * Configures the given channel for an 8-bit, pointer-incrementing
 * memory-to-memory transfer, starts it, and busy-waits for completion.
 * Enables the DMA controller clock internally.
 *
 * @param dma      PERIPHERAL_DMA1 or PERIPHERAL_DMA2
 * @param channel  1-based channel number (1..8 on G4, 1..7 on L4)
 * @param dst      destination buffer
 * @param src      source buffer
 * @param len      number of bytes (1..65535)
 */
void dma_mem_to_mem(peripheral_dma_t dma, uint8_t channel,
                    void *dst, const void *src, uint32_t len);
