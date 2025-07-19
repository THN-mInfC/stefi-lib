// Task: Empty Template: turning on LED1 on libstefi
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"


#define LED A5
void main(){
    peripheral_gpio_enable(PERIPHERAL_ID_GPIOA);
    gpio_set_mode(LED, MODER_OUTPUT); // LED1 on Port C pin 5 on STefi

    while(1) {
        gpio_toggle(LED);
        soft_delay_ms(500);
    }
}