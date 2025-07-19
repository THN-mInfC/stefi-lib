#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"

/* Sensor: Passive Buzzer Module */
/* Demo: Connect VCC-3.3 V, GND, and S -> Pin A0 on CN7 */
/* Function: Passive Buzzer driven by pwm signal@2KHZ, No PWM Silence*/
/* Function: Active Buzzer, HIGH->SOUND ON */

#define USER_LED A5
#define BUZZER_SIGNAL_PIN A0

void main(){
    peripheral_gpio_enable(PERIPHERAL_ID_GPIOA);
    gpio_set_mode(USER_LED, MODER_OUTPUT);
    gpio_set_mode(BUZZER_SIGNAL_PIN, MODER_OUTPUT);

    while(1) {
        for(uint32_t i = 0; i < 40; i++) {
            gpio_write(BUZZER_SIGNAL_PIN, HIGH);
            soft_delay_us(20000);
            gpio_write(BUZZER_SIGNAL_PIN, LOW);
            soft_delay_us(20000);
        }
        for(uint32_t i = 0; i < 160; i++) {
            gpio_write(BUZZER_SIGNAL_PIN, HIGH);
            soft_delay_us(80000);
            gpio_write(BUZZER_SIGNAL_PIN, LOW);
            soft_delay_us(80000);
        }
    }
}