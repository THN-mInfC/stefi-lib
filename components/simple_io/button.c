#include "button.h"

void button_init(stefi_button_t id) {
    peripheral_gpio_enable(gpio_get_port_from_portpin(buttons[id].portpin));
    gpio_set_mode(buttons[id].portpin, MODER_INPUT);
    gpio_set_pupd(buttons[id].portpin, buttons[id].pull);
}

void button_interrupt_init(stefi_button_t id, callbackfn_t fn) {
    //Falling Edge Hard-Coded
    gpio_enable_interrupt(buttons[id].portpin, FALLING_EDGE);
    gpio_interrupt_register_handler(buttons[id].portpin, fn);
}

bool button_is_pressed(stefi_button_t id) {
    sig_t button_state = gpio_read(buttons[id].portpin);
    if(buttons[id].pull != PULL_DOWN) {
        return button_state ? false : true;
    } else {
        return button_state ? true : false;
    }
}