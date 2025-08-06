
#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "test.h"

extern void gpio_toggle_test(void);

const tests_t tests[] = {
    {"gpio_readwrite", gpio_toggle_test},
};

void main() {
    uart_configure();
    run_all_tests(tests, countof(tests));
}