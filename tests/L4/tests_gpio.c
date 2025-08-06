#include "libstefi/gpio.h"
#include <libstefi/peripheral.h>
#include <libstefi/util.h>
#include "board.h"

void gpio_toggle_test() {
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    peripheral_gpio_enable(PERIPHERAL_GPIOC);
    gpio_set_mode(LED, MODER_OUTPUT);
    gpio_set_mode(BUTTON, MODER_INPUT);
    gpio_set_pupd(BUTTON, PULL_UP);
    while(1) {
        gpio_toggle(LED);
        soft_delay_ms(500);
    }
}
