#include <button.h>
#include <pin.h>
#include <ssd1306.h>
#include <stdio.h>
#include <string.h>
#include <internal/i2c_internal.h>

#include "led.h"
#include "board.h"
#include "hardware_timer.h"

#include <libstefi/systick.h>
#include <libstefi/util.h>

#define DELAY_MS 100

void led_chaser() {
    //Forward sweep
    for(uint32_t i = 0; i < 3; i++) {
        led_on(i);
        systick_delay_ms(DELAY_MS);
        led_off(i);
    }
    //Reverse sweep
    for(uint32_t i = 3; i > 0; i--) {
        led_on(i);
        systick_delay_ms(DELAY_MS);
        led_off(i);
    }
}

void test_led_button() {
    //Works for all buttons and leds, maybe move LED Nucleo out
    while(1) {
        if(button_is_pressed(BUTTON_S0)){
           led_chaser();
        }
    }
}

void blink_cb() {
    led_toggle(LED0_RED);
    led_toggle(LED1_YELLOW);
    led_toggle(LED2_GREEN);
    led_toggle(LED3_BLUE);
}

void button_callback() {
    led_toggle(LED0_RED);
}

void test_button_interrupt() {
    //Works for all buttons, including Nucleo, Debounce necessary
    button_interrupt_init(BUTTON_S0, button_callback);
    while(1) {
    }
}

void test_button_debounce() {
    //Debounce Flag set in button.c
    button_interrupt_init(BUTTON_NUCLEO, button_callback);
    while(1) {
    }
}

void test_hardware_timer_periodic() {
    htimer_init_periodic_ms(TIMER4, 50, blink_cb);
    htimer_start(TIMER4);
    while(1) {
    }
}

void test_hardware_timer_pwm() {
    pin_init(C6, PIN_ALT);
    pin_set_alt(C6, AF2);
    htimer_init_pwm(TIMER3, HTIMER_CHANNEL_1);
    htimer_start(TIMER3);
    while(1) {
        //Fade in
        for (int duty = 0; duty <255; duty++) {
            htimer_set_pwm(TIMER3, HTIMER_CHANNEL_1, duty);
            systick_delay_ms(8);
        }
        // Fade-out
        for (int duty = 256; duty > 0; duty--) {
            htimer_set_pwm(TIMER3, HTIMER_CHANNEL_1, duty);
            systick_delay_ms(8);
        }
    }
}

volatile uint16_t ticks;
const int capture_timer = TIMER3;
const uint8_t capture_channel = 4;
#define OUT_PIN C11 //OC1B: Pin 6 on J1
#define CAPTURE_PIN C9 //OC2: Pin 8 on J1

void timer_callback()
{
    pin_set_value(OUT_PIN, !pin_get_value(OUT_PIN));
}

void capture_callback()
{
    static uint16_t last_capture_value = 0, current_capture_value = 0;
    current_capture_value = htimer_get_capture(capture_timer, capture_channel);

    if(last_capture_value < current_capture_value)
    {
        ticks = current_capture_value - last_capture_value;
    }
    else
    {
        ticks = (htimer_get_overflow(capture_timer) - last_capture_value) + current_capture_value;
    }
    last_capture_value = current_capture_value;
}

void test_hardware_timer_input_capture() {
    pin_init(OUT_PIN, PIN_OUT);
    htimer_init_periodic_ms(TIMER4, 5, timer_callback); //Timer freq = 100Hz
    htimer_start(TIMER4);

    pin_init(C9, PIN_ALT);
    pin_set_alt(C9, AF2);
    htimer_init_input_capture(capture_timer,capture_channel, capture_callback);
    htimer_start(capture_timer);

    while(1) {
        printf("Frequency:%d \r\n", 4000000/ticks);
    }
}


void test_display() {
    char buffer[10];
    led_off(LED0_RED);
    ssd1306_clear_screen();
    sprintf(buffer, "TIME: ");
    ssd1306_putstring( 5, 0, buffer);
    ssd1306_update_screen();
    while(1) {
    }
}

void main() {
    board_init();
    //test_led_button();
    //test_button_interrupt();
    //test_button_debounce();
    //test_hardware_timer_periodic();
    //test_hardware_timer_pwm();
    //test_hardware_timer_input_capture();
    test_display();
}


