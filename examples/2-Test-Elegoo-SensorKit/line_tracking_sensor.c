#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"

/* Sensor: Line Tracking Module, PIR Motion Sensor, Magnetic Spring, Flame Sensor */
/* Demo: Connect VCC-3.3 V, GND, and S -> Pin A0 on CN7 */
/* Function: USER LED (A5) lights up on white surface, and off on black area*/

#define USER_LED A5
#define TRACKING_SENSOR_PIN A0

void main(){
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    gpio_set_mode(USER_LED, MODER_OUTPUT);
    gpio_set_mode(TRACKING_SENSOR_PIN, MODER_INPUT);

    while(1) {
        if(gpio_read(TRACKING_SENSOR_PIN) == HIGH) {
            //sensor detects WHITE surface, turn on led (low-aktiv)
            gpio_write(USER_LED, LOW);
        } else {
            gpio_write(USER_LED, HIGH);
        }
    }
}