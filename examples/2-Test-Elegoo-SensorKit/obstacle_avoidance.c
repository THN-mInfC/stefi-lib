#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"

/* Sensor: Obstacle Avoidance Sensor Module */
/* Demo: Connect VCC-3.3 V, GND, and S -> Pin A0 on CN7 */
/* Function: Signal Pin Low if object detected, adjust pot for detection length */

#define USER_LED A5
#define AVOIDANCE_SIGNAL_PIN A0

void main(){
    uart_configure();
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    gpio_set_mode(USER_LED, MODER_OUTPUT);
    gpio_set_mode(AVOIDANCE_SIGNAL_PIN, MODER_INPUT);

    while(1) {
        printf("Sensor Detects Object: %s \r\n", gpio_read(AVOIDANCE_SIGNAL_PIN)? "False" : "True");
        soft_delay_ms(100);
    }
}

