#include "libstefi/gpio.h"
#include <libstefi/peripheral.h>
#include <libstefi/util.h>
#include "board.h"
#include "test.h"


void gpio_init() {
    peripheral_gpio_enable(PERIPHERAL_GPIOC);
    gpio_set_mode(DIG_OUT, MODER_OUTPUT);
    gpio_set_mode(DIG_IN, MODER_INPUT);
}

void gpio_toggle_test() {
    gpio_init();
    sig_t last_state = gpio_read(DIG_IN);
    gpio_toggle(DIG_OUT);
    if(gpio_read(DIG_IN) == last_state) {
        test_fail("GPIO Toggle fails, Check Jumper \r\n");
    } else {
        test_pass("GPIO Toggle works \r\n");
    }
}

void gpio_readwrite_test() {
    gpio_init();
    gpio_write(DIG_OUT, HIGH); //Note: Default state gpio pin is LOW
    if(gpio_read(DIG_IN) == LOW) {
        test_fail("GPIO loopback fails, Check Jumper \r\n");
    } else {
        test_pass("GPIO loopback works \r\n");
    }
}
