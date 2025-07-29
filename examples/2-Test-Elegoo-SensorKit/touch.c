#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"

/* Sensor: Touch Sensor */
/* Demo: Connect VCC-3.3 V, GND, and S -> Pin A0 on CN7 */
/* Function: User led lights up on touching the sensor */

#define USER_LED A5
#define TOUCH_SENSOR_PIN A0

void main(){
    uart_configure();
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    gpio_set_mode(USER_LED, MODER_OUTPUT);
    gpio_set_mode(TOUCH_SENSOR_PIN, MODER_INPUT);

    while(1) {
        //printf("Sensor Detects Touch: %s \r\n", ? "False" : "True");
        if(gpio_read(TOUCH_SENSOR_PIN)) {
            gpio_write(USER_LED, HIGH);
        } else {
            gpio_write(USER_LED, LOW);
        }
    }
}