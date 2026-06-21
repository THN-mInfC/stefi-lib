/**********************************************************************/
/* DMA  (Direct Memory Access)  Hardware Abstraction layer            */
/**********************************************************************/
#include "libstefi/dma.h"
#include "libstefi/peripheral.h"
#include "libstefi/util.h"
#include "internal/dma_internal.h"

/*
 * DMA_CCRx bit positions (RM0440 / RM0351). Only the ones phase 3 needs:
 *   EN      [0]    channel enable — set LAST; this starts the transfer
 *   DIR     [4]    0 = read from peripheral addr (CPAR) -> memory (CMAR)
 *   PINC    [6]    increment the peripheral (CPAR) pointer after each item
 *   MINC    [7]    increment the memory   (CMAR) pointer after each item
 *   MEM2MEM [14]   memory-to-memory mode (runs without a peripheral request)
 */
#define CCR_EN        0u
#define CCR_DIR       4u
#define CCR_PINC      6u
#define CCR_MINC      7u
#define CCR_MEM2MEM  14u

/* Each channel owns 4 bits in ISR/IFCR at (channel-1)*4: GIF/TCIF/HTIF/TEIF. */
static inline uint32_t channel_flag_shift(uint8_t channel)
{
    return (uint32_t)(channel - 1u) * 4u;
}

void dma_mem_to_mem(peripheral_dma_t dma, uint8_t channel,
                    void *dst, const void *src, uint32_t len)
{
    peripheral_dma_enable(dma);

    DMA_t *ctrl = (dma == PERIPHERAL_DMA2) ? DMA2 : DMA1;
    DMA_Channel_t *ch = DMA_CHANNEL(ctrl, channel);

    // A channel must be disabled before CPAR/CMAR/CNDTR can be written.
    ch->CCR &= ~BIT(CCR_EN);

    ch->CPAR  = (uint32_t) src;   // source      (read side, DIR=0)
    ch->CMAR  = (uint32_t) dst;   // destination (write side)
    ch->CNDTR = len;              // 8-bit items -> byte count

    // 8-bit, increment both pointers, memory-to-memory; DIR/PSIZE/MSIZE/PL = 0.
    ch->CCR = BIT(CCR_MEM2MEM) | BIT(CCR_MINC) | BIT(CCR_PINC);

    uint32_t shift = channel_flag_shift(channel);
    ctrl->IFCR = 0xFu << shift;          // clear stale flags for this channel
    ch->CCR |= BIT(CCR_EN);              // GO

    // Wait for transfer-complete (TCIF = bit shift+1).
    while (!(ctrl->ISR & (1u << (shift + 1u))));

    ctrl->IFCR = 0xFu << shift;          // clear flags
    ch->CCR &= ~BIT(CCR_EN);             // release the channel
}
