#include <libstefi/timer.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"

/* Sensor: RGB Module */
/* Demo: Connect VCC-3.3 V, GND, and RED, GREEN, BLUE -> Pin A0 on CN7 */
/* Function: pwm driven rgb, red does not work in smd rgb led */

#define USER_LED A5
#define RED_RGBLED_PIN A7 //D11 {A7, TIMER3, 2, AF2}, D6{B10, TIM2, 3, AF1}
#define GREEN_RGBLED_PIN B6 //D10 {B6, TIM4, 1, AF2}, D5 {B4, TIM3, 1, AF2}
#define BLUE_RGBLED_PIN B3 // D3 {B3, TIM2, 2, AF1}, D9 {C7, TIM3, 2, AF2},

void main(){
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    peripheral_gpio_enable(PERIPHERAL_GPIOB);
    gpio_set_mode(USER_LED, MODER_OUTPUT);

    gpio_set_mode(RED_RGBLED_PIN, MODER_AF);
    gpio_set_mode(GREEN_RGBLED_PIN, MODER_AF);
    gpio_set_mode(BLUE_RGBLED_PIN, MODER_AF);

    gpio_set_alternate_function(RED_RGBLED_PIN, AF2);
    gpio_set_alternate_function(GREEN_RGBLED_PIN, AF2);
    gpio_set_alternate_function(BLUE_RGBLED_PIN, AF1);

    timer_init(TIMER3);
    timer_init(TIMER4);
    timer_init(TIMER2);

    timer_set_period(TIMER3, 16, 255); //1000 Hz-PWM
    timer_set_period(TIMER4, 16, 255);
    timer_set_period(TIMER2, 16, 255);

    timer_set_mode_pwm(TIMER3, TIMER_CHANNEL2);
    timer_set_mode_pwm(TIMER4, TIMER_CHANNEL1);
    timer_set_mode_pwm(TIMER2, TIMER_CHANNEL2);

    timer_cc_enable(TIMER3, TIMER_CHANNEL2, false);
    timer_cc_enable(TIMER4, TIMER_CHANNEL1, false);
    timer_cc_enable(TIMER2, TIMER_CHANNEL2, false);

    timer_set_compare(TIMER3, TIMER_CHANNEL2, 1);
    timer_set_compare(TIMER4, TIMER_CHANNEL1, 1);
    timer_set_compare(TIMER2, TIMER_CHANNEL2, 1);

    timer_start(TIMER3);
    timer_start(TIMER4);
    timer_start(TIMER2);

    while(1) {
        for(uint32_t val = 0; val <  255; val++) {
            timer_set_compare(TIMER3, TIMER_CHANNEL2, 255);
            timer_set_compare(TIMER4, TIMER_CHANNEL1, 128);
            timer_set_compare(TIMER2, TIMER_CHANNEL2,256);
            soft_delay_ms(50);
        }
    }
}
