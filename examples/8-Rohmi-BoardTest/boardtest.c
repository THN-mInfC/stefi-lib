#include "board.h"
#include "led.h"

#include <libstefi/systick.h>

/*
 * Rohmi (NUCLEO-G431KB) board smoke test: blink the on-board LED at 1 Hz.
 * LED1 (PA10) is left alone — it shares its net with the buzzer.
 */
void main(void) {
    board_init();

    while (1) {
        led_toggle(LED_NUCLEO);
        systick_delay_ms(5000);
    }
}