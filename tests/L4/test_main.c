#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "test.h"

/* Note: plug in the appropriate jumpers*/

extern void gpio_toggle_test(void);
extern void gpio_readwrite_test(void);

const tests_t tests[] = {
    {"gpio_readwrite", gpio_readwrite_test},
    {"gpio_toggle", gpio_toggle_test},
};

void main() {
    uart_configure();
    run_all_tests(tests, countof(tests));
}