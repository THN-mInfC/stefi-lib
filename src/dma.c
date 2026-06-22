/**********************************************************************/
/* DMA  (Direct Memory Access)  Hardware Abstraction layer            */
/**********************************************************************/
#include "libstefi/dma.h"
#include "libstefi/peripheral.h"
#include "libstefi/interrupts.h"
#include "libstefi/util.h"
#include "internal/dma_internal.h"

/*
 * DMA_CCRx bit positions (RM0440 / RM0351):
 *   EN      [0]    channel enable — set LAST; this starts the transfer
 *   TCIE    [1]    transfer-complete interrupt enable
 *   TEIE    [3]    transfer-error interrupt enable
 *   DIR     [4]    0 = read from peripheral addr (CPAR) -> memory (CMAR)
 *   PINC    [6]    increment the peripheral (CPAR) pointer after each item
 *   MINC    [7]    increment the memory   (CMAR) pointer after each item
 *   MEM2MEM [14]   memory-to-memory mode (runs without a peripheral request)
 */
#define CCR_EN        0u
#define CCR_TCIE      1u
#define CCR_TEIE      3u
#define CCR_DIR       4u
#define CCR_PINC      6u
#define CCR_MINC      7u
#define CCR_MEM2MEM  14u

/* Only DMA1 channels 1..7 have wired NVIC vectors (see startup.c). */
#define DMA1_NUM_CHANNELS 7u

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

/****************************** Interrupts ******************************/
/*
 * DMA owns its own per-channel handler table (like timer.c / gpio.c own
 * theirs) because each DMA1 channel is a distinct NVIC source. Indexed by
 * channel-1, so channel 1 -> dma1_handlers[0].
 */
static struct {
    callbackfn_t callback;
    void *aux_data;
} dma1_handlers[DMA1_NUM_CHANNELS];

void dma_register_handler(uint8_t channel, callbackfn_t fn, void *aux)
{
    assert(channel >= 1 && channel <= DMA1_NUM_CHANNELS);
    dma1_handlers[channel - 1u].callback = fn;
    dma1_handlers[channel - 1u].aux_data = aux;
}

void dma_mem_to_mem_irq(uint8_t channel,
                        void *dst, const void *src, uint32_t len)
{
    assert(channel >= 1 && channel <= DMA1_NUM_CHANNELS);
    peripheral_dma_enable(PERIPHERAL_DMA1);

    DMA_Channel_t *ch = DMA_CHANNEL(DMA1, channel);
    ch->CCR &= ~BIT(CCR_EN);

    ch->CPAR  = (uint32_t) src;
    ch->CMAR  = (uint32_t) dst;
    ch->CNDTR = len;

    // Same as the polled M2M setup, plus transfer-complete/error interrupts.
    ch->CCR = BIT(CCR_MEM2MEM) | BIT(CCR_MINC) | BIT(CCR_PINC)
            | BIT(CCR_TCIE) | BIT(CCR_TEIE);

    uint32_t shift = channel_flag_shift(channel);
    DMA1->IFCR = 0xFu << shift;
    // NVIC sources for DMA1 ch1..7 are contiguous from INTERRUPT_SOURCE_DMA1_CH1.
    interrupts_enable_source(INTERRUPT_SOURCE_DMA1_CH1 + (channel - 1));
    ch->CCR |= BIT(CCR_EN);              // GO
}

static inline void handle_dma1_irq(uint8_t channel)
{
    uint32_t shift = channel_flag_shift(channel);
    uint32_t flags = DMA1->ISR;

    // Transfer-complete (TCIF = shift+1) or transfer-error (TEIF = shift+3).
    // Either way this M2M transfer is a one-shot: clear flags, release channel.
    if (flags & ((1u << (shift + 1u)) | (1u << (shift + 3u)))) {
        DMA1->IFCR = 0xFu << shift;
        DMA_CHANNEL(DMA1, channel)->CCR &= ~BIT(CCR_EN);

        callbackfn_t fn = dma1_handlers[channel - 1u].callback;
        if (fn) {                        // guard: never call a NULL handler
            fn(dma1_handlers[channel - 1u].aux_data);
        }
    }
}

void DMA1_Channel1_IRQHandler(void) { handle_dma1_irq(1); }
void DMA1_Channel2_IRQHandler(void) { handle_dma1_irq(2); }
void DMA1_Channel3_IRQHandler(void) { handle_dma1_irq(3); }
void DMA1_Channel4_IRQHandler(void) { handle_dma1_irq(4); }
void DMA1_Channel5_IRQHandler(void) { handle_dma1_irq(5); }
void DMA1_Channel6_IRQHandler(void) { handle_dma1_irq(6); }
void DMA1_Channel7_IRQHandler(void) { handle_dma1_irq(7); }
