#pragma once

#include "libstefi/gpio.h"
#include "libstefi/timer.h"
#include "libstefi/peripheral.h"

/*
 * RobOhmobil ("Rohmi") mainboard — NUCLEO-G431KB
 * Pin mapping from Schematic_RobOhmobil_V01 (Rohmi_Extended_Mainboard).
 *
 * NOTE: DIR MOTOR L is PF0 (and expansion pin PF1) — requires port F support
 * in libstefi (gpio_id_t: F0 = 0x500, F1; peripheral: PERIPHERAL_GPIOF).
 */

typedef enum {
    LED1 = 0,       // PA10 — shared net with the buzzer!
    LED_NUCLEO,     // LD2 on the G431KB (PB8)
    NUM_LEDS
} stefi_led_t;

/* No button is populated on the Rohmi board or the NUCLEO-G431KB. The
 * default mapping points at free expansion pins (PB4/PB5) so the button
 * component works out of the box — wire external buttons to GND there. */
typedef enum {
    BUTTON_USER0 = 0,
    BUTTON_USER1,
    NUM_BUTTONS
} stefi_button_t;

typedef enum {
    DEBOUNCE_TIMER,
    NUM_RESERVED_TIMER,
} stefi_timer_t;

typedef enum {
    MOTOR_L = 0,
    MOTOR_R,
    NUM_MOTORS
} stefi_motor_t;

typedef enum {
    LINE_SENSOR_L = 0,
    LINE_SENSOR_R,
    NUM_LINE_SENSORS
} stefi_line_sensor_t;

typedef struct {
    gpio_id_t portpin;
    sig_t off_state;
    tim_id_t timer_id;
    uint32_t channel;
    afr_t af_mode;
} led_config_t;

//Button Confguration
typedef struct {
    gpio_id_t portpin;
    pupdr_t pull;
} button_config_t;

typedef struct {
    tim_id_t timer;
    uint32_t prescaler;
    uint32_t period;
} hardware_timer_config_t;

/* STSPIN240 dual brushed-DC driver: one PWM (speed) + one DIR (phase) per
 * motor. EN/FAULT and STBY/RESET are strapped on-board, not MCU-controlled. */
typedef struct {
    gpio_id_t pwm_portpin;
    tim_id_t timer_id;
    uint32_t channel;
    afr_t af_mode;
    gpio_id_t dir_portpin;
} motor_config_t;

/* Reflectance sensor modules on J4/J5: 3V3, GND, digital signal out.
 * LINE_SENSOR_R (PB0) can alternatively be sampled as ADC1 channel 15. */
typedef struct {
    gpio_id_t portpin;
} line_sensor_config_t;

/* HC-SR04-style ranger on J6. Echo is level-shifted 5V->3V3 on-board and
 * routes to TIM4 CH1 for input-capture pulse measurement. */
typedef struct {
    gpio_id_t trigger_portpin;
    gpio_id_t echo_portpin;
    tim_id_t echo_timer_id;
    uint32_t echo_channel;
    afr_t echo_af_mode;
} ultrasound_config_t;

/* Buzzer shares PA10 with LED1: any beep also lights the LED.
 * Tone generation: TIM1 CH3 (AF6) PWM, or plain GPIO toggling. */
typedef struct {
    gpio_id_t portpin;
    tim_id_t timer_id;
    uint32_t channel;
    afr_t af_mode;
} buzzer_config_t;

typedef peripheral_i2c_t i2c_id_t;

/* LSM6DSO IMU on I2C2. */
typedef struct {
    i2c_id_t i2c_id;
    uint8_t i2c_addr;
    gpio_id_t scl;
    gpio_id_t sda;
    gpio_id_t int1_portpin;
} lsm6dso_config_t;

typedef peripheral_adc_t adc_id_t;

typedef struct {
    gpio_id_t portpin;
    tim_id_t timer_id;
    uint32_t timer_channel;
    adc_id_t *adc_id;
    uint32_t adc_channel;
} freepin_config_t;

/* Pins routed to the (currently unused) expansion socket; free for apps. */
#define NUM_GPIO_PINS 12

extern led_config_t leds[NUM_LEDS];
extern button_config_t buttons[NUM_BUTTONS];
extern hardware_timer_config_t hardware_timers[NUM_RESERVED_TIMER];
extern motor_config_t motors[NUM_MOTORS];
extern line_sensor_config_t line_sensors[NUM_LINE_SENSORS];
extern ultrasound_config_t ultrasound;
extern buzzer_config_t buzzer;
extern lsm6dso_config_t imu;
extern freepin_config_t freepin[NUM_GPIO_PINS];

void board_init();
