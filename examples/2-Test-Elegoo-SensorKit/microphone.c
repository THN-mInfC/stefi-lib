#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"
#include <libstefi/adc.h>

/* Sensor: Sound Sensor, reuse Linear magnetic hall sensor  */
/* Demo: Connect VCC-3.3 V, GND, and Analog -> Pin A0, Digital to Pin A1 on CN7 */
/* Function: User led lights up on large sound, pot. to change sensetivity */

#define USER_LED A5
#define SOUND_SENSOR_ANALOG_PIN A0
#define SOUND_SENSOR_DIGITAL_PIN A1

void main(){
    uart_configure();
    peripheral_gpio_enable(PERIPHERAL_ID_GPIOA);
    gpio_set_mode(USER_LED, MODER_OUTPUT);
    gpio_set_mode(SOUND_SENSOR_DIGITAL_PIN, MODER_INPUT);

    adc_gpio_init(SOUND_SENSOR_ANALOG_PIN);
    adc_init(PERIPHERAL_ID_ADC1);

    while(1) {
        uint16_t x = adc_read(PERIPHERAL_ID_ADC1,5);
        uint16_t detect = gpio_read(SOUND_SENSOR_DIGITAL_PIN);
        printf("x:%d detect: %d \r\n", x, detect);
        if(detect) {
            gpio_write(USER_LED, LOW);
        } else {
            gpio_write(USER_LED, HIGH);
        }
        soft_delay_ms(200);
    }
}