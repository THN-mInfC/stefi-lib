#include "pin.h"

#include <libstefi/peripheral.h>

void pin_init(pin_id_t pin, pin_mode_t mode) {
    uint16_t port = gpio_get_port_from_portpin(pin);
    peripheral_gpio_enable(port);
    switch(mode) {
        case PIN_OUT:
            gpio_set_mode(pin, MODER_OUTPUT);
            break;
        case PIN_IN:
            gpio_set_mode(pin, MODER_INPUT);
            break;
        case PIN_ALT:
            gpio_set_mode(pin, MODER_AF);
            break;
        case PIN_ANALOG:
            gpio_set_mode(pin, MODER_ANALOG);
            break;
        case PIN_OPEN_DRAIN:
            gpio_set_mode(pin, MODER_OUTPUT);
            gpio_set_output_type(pin, OPEN_DRAIN);
            break;
        case PIN_ALT_OPEN_DRAIN:
            gpio_set_mode(pin, MODER_AF);
            gpio_set_output_type(pin, OPEN_DRAIN);
            break;
        case PIN_IN_PULLUP:
            gpio_set_mode(pin, MODER_INPUT);
            gpio_set_pupd(pin, PULL_UP);
            break;
        default:
            break;
    }
}

void pin_set_pull(pin_id_t pin, pin_pull_t pupd) {
    gpio_set_pupd(pin, pupd);
}

void pin_set_drive(pin_id_t pin, pin_drive_t drive) {
    gpio_set_output_speed(pin, drive);
}

void pin_set_alt(pin_id_t pin, pin_alt_t af) {
    gpio_set_alternate_function(pin, af);
}