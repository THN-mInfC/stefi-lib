#pragma once

#include "libstefi/gpio.h"

typedef gpio_id_t pin_id_t;

typedef enum {
        PIN_IN = MODER_INPUT,
        PIN_OUT = MODER_OUTPUT,
        PIN_ALT = MODER_AF,
        PIN_ANALOG = MODER_ANALOG,
        PIN_OPEN_DRAIN,
        PIN_ALT_OPEN_DRAIN,
        PIN_IN_PULLUP,
} pin_mode_t;

typedef enum {
        PIN_PULL_UP = PULL_UP,
        PIN_PULL_DOWN = PULL_DOWN,
} pin_pull_t;

typedef sig_t pin_value_t;

typedef enum {
        PIN_DRIVE_0 = LOW_S,
        PIN_DRIVE_1 = MEDIUM_S,
        PIN_DRIVE_2 = HIGH_S,
        PIN_DRIVE_3 = VERY_HIGH_S,
} pin_drive_t;

typedef afr_t pin_alt_t;


void pin_init(pin_id_t, pin_mode_t);

void pin_set_pull(pin_id_t, pin_pull_t);
void pin_set_drive(pin_id_t, pin_drive_t);
void pin_set_alt(pin_id_t, pin_alt_t);

static inline pin_value_t pin_get_value(pin_id_t pin) {
        return gpio_read(pin);
}
static inline void pin_set_value(pin_id_t pin, pin_value_t val) {
        gpio_write(pin, val);
}


