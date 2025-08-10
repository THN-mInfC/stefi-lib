#include <stdio.h>

#include "led.h"
#include "board.h"

#include <libstefi/systick.h>

#define DELAY_MS 100
void main() {
    board_init();
    printf("LED Chaser \r\n");

    uint32_t count = 0;
    while(1) {
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
}