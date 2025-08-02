// Task: Empty Template: turning on LED1 on libstefi
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"


#define LED A5

void gpio_init() {
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    gpio_set_mode(LED, MODER_OUTPUT); // LED1 on Port C pin 5 on STefi
}

void gpio_process() {
    while(1) {
        gpio_toggle(LED);
        soft_delay_ms(50);
    }
}

void main(){
    gpio_init();
    gpio_process();
}