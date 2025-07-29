#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"
#include <libstefi/systick.h>
#include "libstefi/timer.h"

/* Sensor: Ultrasound  */
/* Demo: Connect VCC-3.3 V, GND */
/* Function: pulse out, pulseIn length (Timer Capture) proportional to distance  */

#define USER_LED A5
#define US_IN_PIN C9 //echo
#define US_OUT_PIN A1

volatile uint16_t ticks = 0;
uint16_t trigger = 0;
const int capture_timer = TIMER3;
const uint8_t capture_channel = TIMER_CHANNEL4;

static enum { WAIT_FOR_RISING, WAIT_FOR_FALLING } state = WAIT_FOR_RISING;
static uint16_t rising = 0, falling = 0;

void capture_callback()
{
    uint16_t current_capture_value = 0;
    current_capture_value = timer_get_compare(capture_timer, capture_channel);

    if (state == WAIT_FOR_RISING) {
        rising = current_capture_value;
        timer_set_ic_edge(capture_timer, capture_channel, TIMER_EDGE_FALLING);
        state = WAIT_FOR_FALLING;
    } else {
        falling = current_capture_value;
        if (falling >= rising)
            ticks = falling - rising;
        else
            ticks = (timer_get_arr(capture_timer) - rising + falling);
        timer_set_ic_edge(capture_timer, capture_channel, TIMER_EDGE_RISING);
        state = WAIT_FOR_RISING;
        trigger = 0;
    }
}

void main(){
    uart_configure();

    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    peripheral_gpio_enable(PERIPHERAL_GPIOC);

    //Trigger (10us) on out ping
    gpio_set_mode(US_OUT_PIN, MODER_OUTPUT);

    //Setup Timer3 to capture input on C9
    gpio_set_mode(US_IN_PIN, MODER_AF);
    gpio_set_alternate_function(US_IN_PIN, AF2);

    timer_init(capture_timer);
    timer_set_mode_ic(capture_timer, capture_channel);
    timer_cc_interrupt_register_handler(capture_timer, capture_channel, capture_callback);
    timer_cc_enable(capture_timer, capture_channel, true);
    timer_start(capture_timer);

    while(1) {
        //send pulse every 0.5s
        gpio_write(US_OUT_PIN, LOW); soft_delay_us(2);
        gpio_write(US_OUT_PIN, HIGH); soft_delay_us(20);
        gpio_write(US_OUT_PIN, LOW);
        trigger = 1;
        soft_delay_ms(200);
        if(trigger == 0) {
            //Factor: 0.125 as CPU Clock: 4MHz + round trip time
            printf("Distance: %.2f cm\r\n", (ticks * 0.125) * 0.0343f);
        }
    }
}
