#include "hardware_timer.h"

#include <stddef.h>

void htimer_init(htimer_id_t id, htimer_channel_t channel, htimer_mode_t mode, uint32_t period_us, htimer_callbackfn_t cb) {
    timer_init(id);
    // Get a board define for 4-> 4MHz
    switch(mode) {
        case HTIMER_MODE_PERIODIC:
            timer_set_period(id, 4000, period_us);
            break;
        case HTIMER_MODE_PWM:
            timer_set_period(id, 16, 255); //PWM Freq=4 MhZ/(16*256) = 1 KHZ
            timer_set_mode_pwm(id, channel);
            timer_cc_enable(id, channel, false);
            timer_set_compare(id, channel, 0);
            break;
        case HTIMER_MODE_INPUTCAPTURE:
            timer_set_mode_ic(id, channel);
            timer_cc_enable(id, channel, true);
            break;
        case HTIMER_MODE_ONE_SHOT:
            break;
    }

    if (channel == HTIMER_NOCHANNEL) {
        timer_update_interrupt_register_handler(id, cb);
        timer_enable_interrupt(id);
    } else {
        timer_cc_interrupt_register_handler(id, channel, cb);
    }
}

void htimer_init_periodic_ms(htimer_id_t id, uint32_t period_ms, htimer_callbackfn_t cb) {
    htimer_init(id, HTIMER_NOCHANNEL, HTIMER_MODE_PERIODIC, period_ms, cb);
}

void htimer_init_pwm(htimer_id_t id, htimer_channel_t channel) {
    htimer_init(id, channel, HTIMER_MODE_PWM, 0, NULL);
    timer_set_compare(id, channel, 0);
}

void htimer_init_input_capture(htimer_id_t id, htimer_channel_t channel, htimer_callbackfn_t cb) {
    htimer_init(id, channel, HTIMER_MODE_INPUTCAPTURE, 0, cb);
}

void htimer_start(htimer_id_t id) {
    timer_start(id);
}

void htimer_stop(htimer_id_t id) {
    timer_stop(id);
}

void htimer_set_pwm(htimer_id_t id, htimer_channel_t channel, uint32_t duty_0_255) {
    timer_set_compare(id, channel, duty_0_255);
}

uint32_t htimer_get_overflow(htimer_id_t id) {
    return timer_get_arr(id);
}

uint32_t htimer_get_count(htimer_id_t id) {
    return timer_get_count(id);
}

uint32_t htimer_get_capture(htimer_id_t id, htimer_channel_t channel) {
    return timer_get_compare(id, channel);
}