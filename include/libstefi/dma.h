#pragma once

#include <stddef.h>
#include <stdint.h>
#include "libstefi/peripheral.h"
#include "libstefi/interrupts.h"

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

/**
 * @brief Register the transfer-complete callback for a DMA1 channel.
 *
 * The callback runs in interrupt context when the channel finishes.
 * Only DMA1 channels 1..7 are wired (their NVIC vectors).
 *
 * @param channel  1-based DMA1 channel number (1..7)
 * @param fn       callback invoked on transfer-complete
 * @param aux      opaque pointer passed to the callback
 */
void dma_register_handler(uint8_t channel, callbackfn_t fn, void *aux);

/**
 * @brief Non-blocking, interrupt-driven memory-to-memory copy on DMA1.
 *
 * Same configuration as dma_mem_to_mem() but enables the transfer-complete
 * interrupt and returns immediately. The registered handler (if any) fires
 * when the copy finishes; the channel is released automatically.
 *
 * @param channel  1-based DMA1 channel number (1..7)
 */
void dma_mem_to_mem_irq(uint8_t channel,
                        void *dst, const void *src, uint32_t len);
