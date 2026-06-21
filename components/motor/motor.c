#include "motor.h"

#include "libstefi/peripheral.h"
#include "libstefi/gpio.h"
#include "libstefi/timer.h"

/*
 * DIR (phase) level that drives the car forward. The wiring/gearing that makes
 * this "forward" is not known until the car is on the bench — verify on
 * hardware and flip if a positive speed_pct drives a wheel backward.
 */
#define MOTOR_FORWARD_LEVEL LOW

/* PWM resolution: timer period is 100 (see board_init), so duty is in percent
 * and the active-low compare for a duty d is (100 - d). */
#define MOTOR_PWM_PERIOD 100

static void motor_setup_one(stefi_motor_t id) {
    const motor_config_t *m = &motors[id];

    /* DIR (phase) pin: output, default forward. */
    peripheral_gpio_enable(gpio_get_port_from_portpin(m->dir_portpin));
    gpio_write(m->dir_portpin, MOTOR_FORWARD_LEVEL);
    gpio_set_mode(m->dir_portpin, MODER_OUTPUT);

    /* PWM (enable/speed) pin: alternate function on the timer channel. */
    peripheral_gpio_enable(gpio_get_port_from_portpin(m->pwm_portpin));
    gpio_set_mode(m->pwm_portpin, MODER_AF);
    gpio_set_alternate_function(m->pwm_portpin, m->af_mode);
    timer_set_mode_pwm(m->timer_id, m->channel);
    timer_cc_enable(m->timer_id, m->channel, false);
    timer_set_compare(m->timer_id, m->channel, MOTOR_PWM_PERIOD); /* 0 % duty */
}

void motor_init(void) {
    /* Both motors share one timer (TIMER2): init the period once. */
    timer_init(motors[MOTOR_L].timer_id);
    timer_set_period(motors[MOTOR_L].timer_id, 16, MOTOR_PWM_PERIOD);
    for (uint32_t i = 0; i < NUM_MOTORS; i++) {
        motor_setup_one((stefi_motor_t) i);
    }
    timer_start(motors[MOTOR_L].timer_id);
}

void motor_drive(stefi_motor_t id, int8_t speed_pct) {
    const motor_config_t *m = &motors[id];

    sig_t dir = (speed_pct >= 0) ? MOTOR_FORWARD_LEVEL : !MOTOR_FORWARD_LEVEL;
    gpio_write(m->dir_portpin, dir);

    int32_t mag = (speed_pct < 0) ? -(int32_t) speed_pct : (int32_t) speed_pct;
    if (mag > MOTOR_PWM_PERIOD) {
        mag = MOTOR_PWM_PERIOD;
    }
    /* Active-low compare: duty d -> compare (period - d). */
    timer_set_compare(m->timer_id, m->channel, MOTOR_PWM_PERIOD - mag);
}

void motor_stop(stefi_motor_t id) {
    timer_set_compare(motors[id].timer_id, motors[id].channel, MOTOR_PWM_PERIOD);
}

void motor_stop_all(void) {
    for (uint32_t i = 0; i < NUM_MOTORS; i++) {
        motor_stop((stefi_motor_t) i);
    }
}
