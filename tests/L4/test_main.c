#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "test.h"

/* Note: plug in the appropriate jumpers*/

extern void gpio_toggle_test(void);
extern void gpio_readwrite_test(void);
extern void dma_polled_m2m_test(void);
extern void dma_irq_m2m_test(void);
extern void dma_uart_tx_test(void);

const tests_t tests[] = {
    {"gpio_readwrite", gpio_readwrite_test},
    {"gpio_toggle", gpio_toggle_test},
    {"dma_polled_m2m", dma_polled_m2m_test},
    {"dma_irq_m2m", dma_irq_m2m_test},
    {"dma_uart_tx", dma_uart_tx_test},
};

void main() {
    uart_configure();
    run_all_tests(tests, countof(tests));
}