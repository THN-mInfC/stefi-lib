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
        default:
            break;
    }
}

pin_value_t pin_get_value(pin_id_t pin) {
    return gpio_read(pin);
}

void pin_set_value(pin_id_t pin, pin_value_t val) {
    gpio_write(pin, val);
}

void pin_low(pin_id_t pin) {

}

void pin_high(pin_id_t pin) {

}