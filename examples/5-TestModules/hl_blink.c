#include <button.h>
#include <stdio.h>

#include "led.h"
#include "board.h"

#include <libstefi/systick.h>
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
    printf("LED Chaser \r\n");

    while(1) {
        if(button_is_pressed(BUTTON_S0)){
           led_chaser();
        }
    }
}

void button_callback() {
    led_toggle(LED0_RED);
}

void test_button_interrupt() {
    button_interrupt_init(BUTTON_S0, button_callback);
    while(1) {
    }
}

void test_debounced_button() {

}

void main() {
    board_init();
    //test_led_button();
    test_button_interrupt();
}