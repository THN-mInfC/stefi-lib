#include "board.h"

#include "button.h"
#include "led.h"

#include <libstefi/system.h>
#include <libstefi/systick.h>
#include <libstefi/uart.h>

/*
 * Board-specific configuration with default peripheral mapping
 * NUCLEO-F767ZI
 *
 * Note (timer budget, all timer kernels at 100 MHz):
 *  Systick  - delay functions
 *  Timer 3  - button debouncing (10 ms one-shot)
 * DMA budget:
 *  DMA1 stream 3 / channel 4 (USART3_TX) - uart_dma_out()
 */

led_config_t leds[NUM_LEDS] = {
    [LED_GREEN] = {B0,  LOW},
    [LED_BLUE]  = {B7,  LOW},
    [LED_RED]   = {B14, LOW},
};

button_config_t buttons[NUM_BUTTONS] = {
    [BUTTON_USER] = {C13, PULL_DOWN},   // board has its own pull-down; active high
};

_Static_assert(NUM_BUTTONS <= STEFI_MAX_BUTTONS,
        "button.c sizes its per-button state by STEFI_MAX_BUTTONS");

hardware_timer_config_t hardware_timers[NUM_RESERVED_TIMER] = {
    // 100 MHz / 10000 = 10 kHz tick, 100 ticks = 10 ms
    [DEBOUNCE_TIMER] = {TIMER3, 10000, 100},
};

void board_init() {
    //200 MHz core, 100 MHz timers, 48 MHz USB
    system_init();

    //UART (USART3/PD8/PD9 — ST-Link VCP), HSI-clocked
    uart_configure();

    //initialize all leds, default, output not PWM
    for(uint32_t i = 0; i < NUM_LEDS; i++) {
        led_init(i, OUTPUT);
    }

    for(uint32_t i = 0; i < NUM_BUTTONS; i++) {
        button_init(i);
    }

    //intialize default timers
    for(uint32_t i = 0; i < NUM_RESERVED_TIMER; i++) {
        timer_init(hardware_timers[i].timer);
        timer_set_period(hardware_timers[i].timer, hardware_timers[i].prescaler, hardware_timers[i].period);
    }

    systick_init(BOARD_SYSCLK);
    systick_start();
}
