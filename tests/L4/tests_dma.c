#include <string.h>
#include <stdint.h>
#include "libstefi/dma.h"
#include "libstefi/interrupts.h"
#include "test.h"

/*
 * DMA memory-to-memory tests. No jumpers required (M2M needs no peripheral
 * request routing), so these run on any board: DMA1 ch1-7 = IRQ 11-17 on
 * both L4 and G4.
 */
#define N 64

static volatile int dma_done;

static void on_dma_complete(void *aux)
{
    (void) aux;
    dma_done = 1;
}

void dma_polled_m2m_test(void)
{
    uint8_t s[N], d[N];
    for (int i = 0; i < N; i++) { s[i] = (uint8_t)(i * 7 + 1); d[i] = 0; }

    dma_mem_to_mem(PERIPHERAL_DMA1, 1, d, s, sizeof s);

    if (memcmp(s, d, sizeof s) == 0)
        test_pass("DMA polled M2M \r\n");
    else
        test_fail("DMA polled M2M \r\n");
}

void dma_irq_m2m_test(void)
{
    interrupts_global_enable();

    uint8_t s[N], d[N];
    for (int i = 0; i < N; i++) { s[i] = (uint8_t)(255 - i); d[i] = 0; }

    dma_done = 0;
    dma_register_handler(2, on_dma_complete, NULL);
    dma_mem_to_mem_irq(2, d, s, sizeof s);

    volatile uint32_t spins = 0;
    while (!dma_done && ++spins < 5000000u) { }

    if (!dma_done)
        test_fail("DMA IRQ M2M: interrupt never fired \r\n");
    else if (memcmp(s, d, sizeof s) != 0)
        test_fail("DMA IRQ M2M: data mismatch \r\n");
    else
        test_pass("DMA IRQ M2M \r\n");
}
