#pragma once

#include "stefi_board.h"

/*
 * Bare NUCLEO-F767ZI (NUCLEO-144):
 *
 *   USB device (CN13, OTG_FS)                  PA11/PA12
 *   ST-Link virtual COM port (printf)          USART3, PD8/PD9
 */

/* board_init() switches to 200 MHz (needed for the 48 MHz USB clock; the
 * timer kernels run at 100 MHz). */
#define BOARD_SYSCLK      (200000000U)
#define BOARD_TIMER_CLK   (100000000U)   /* APB1/APB2 timer kernel clock */

enum {
    LED_GREEN = 0,  // LD1, PB0
    LED_BLUE,       // LD2, PB7
    LED_RED,        // LD3, PB14
    NUM_LEDS
};

enum {
    BUTTON_USER = 0, // B1, PC13 (active high, external pull-down)
    NUM_BUTTONS
};

extern led_config_t leds[NUM_LEDS];
extern button_config_t buttons[NUM_BUTTONS];
