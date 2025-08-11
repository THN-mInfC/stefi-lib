#pragma once
#include "libstefi/gpio.h"
#include "libstefi/timer.h"
#include "libstefi/peripheral.h"

typedef enum {
    LED0_RED = 0,
    LED1_YELLOW,
    LED2_GREEN,
    LED3_BLUE,
    LED_NUCLEO,
    NUM_LEDS
} stefi_led_t;

typedef enum {
    BUTTON_S0 = 0,
    BUTTON_S1,
    BUTTON_S2,
    BUTTON_S3,
    BUTTON_NUCLEO,
    NUM_BUTTONS
} stefi_button_t;

typedef enum {
    DEBOUNCE_TIMER,
    NUM_RESERVED_TIMER,
} stefi_timer_t;

typedef struct {
    gpio_id_t portpin;
    sig_t off_state;
    tim_id_t timer_id;
    uint32_t channel;
    afr_t af_mode;
}  led_config_t;

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

//FRAM Configuration: later freq, mode, ...
typedef struct {
    peripheral_spi_t spi_id;
    gpio_id_t cs_portpin;
    gpio_id_t sck_portpin;
    gpio_id_t miso_portpin;
    gpio_id_t mosi_portpin;
} fm25cl64_config_t;

typedef peripheral_adc_t adc_id_t;

typedef struct {
    gpio_id_t portpin;
    tim_id_t timer_id;
    uint32_t timer_channel;
    adc_id_t *adc_id;
    uint32_t adc_channel;
} freepin_config_t;

#define NUM_GPIO_PINS 47

extern led_config_t leds[NUM_LEDS];
extern button_config_t buttons[NUM_BUTTONS];
extern hardware_timer_config_t hardware_timers[NUM_RESERVED_TIMER];
extern freepin_config_t freepin[NUM_GPIO_PINS];

void board_init();