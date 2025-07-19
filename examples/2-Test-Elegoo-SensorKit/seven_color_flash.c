#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"

/* Sensor: Seven Color Module, Laser */
/* Demo: Connect VCC-3.3 V, GND, and S -> Pin A0 on CN7 */
/* Function: flashes seven color pre-programmed sequence on High signal */

#define USER_LED A5
#define SEVENCOLORLED_SIGNAL_PIN A0

void main(){
    peripheral_gpio_enable(PERIPHERAL_ID_GPIOA);
    gpio_set_mode(USER_LED, MODER_OUTPUT);
    gpio_set_mode(SEVENCOLORLED_SIGNAL_PIN, MODER_OUTPUT);

    while(1) {
        gpio_write(SEVENCOLORLED_SIGNAL_PIN, HIGH);
    }
}