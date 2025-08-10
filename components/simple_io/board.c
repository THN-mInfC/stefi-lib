#include "board.h"

#include "led.h"
#include <stddef.h>
#include <libstefi/systick.h>
#include <libstefi/uart.h>

/*
 * Board-specific configuration with default peripheral mapping
 *
 * Note:
 *  Timer 3  - used for fading the green, blue LED.
 *  Timer 4  - Button debouncing
 *  Timer 5  - controls the servo driver.
 *  Timer 6  - used for timed ADC/DAC reading/writing.
 */


button_config_t buttons[NUM_BUTTONS] = {
    [BUTTON_S0] = {B0, PULL_UP},
    [BUTTON_S1] = {B1, PULL_UP},
    [BUTTON_S2] = {B2, PULL_UP},
    [BUTTON_S3] = {B3, PULL_UP},
    [BUTTON_NUCLEO] = {C13, NONE},
};

led_config_t leds[NUM_LEDS] = {
    [LED0_RED] = {C4, HIGH},
    [LED1_YELLOW] = {C5, HIGH},
    [LED2_GREEN] = {C6, HIGH, TIMER3, 1, AF2},
    [LED3_BLUE] = {C7, HIGH, TIMER3, 2, AF2},
    [LED_NUCLEO]= {A5, LOW},
};

fm25cl64_config_t fram ={
    .spi_id = PERIPHERAL_SPI1,
    .cs_portpin = A4,
    .sck_portpin = A5,
    .miso_portpin = A6,
    .mosi_portpin = A7,
};

freepin_config_t freepin[NUM_GPIO_PINS] = {
    /* Arduino-Connector CN9, right */
    {A3,   TIMER_INVALID,  0, PERIPHERAL_ADC1, 8}, //Reserved UART2
    {A2,   TIMER_INVALID,  0, PERIPHERAL_ADC1, 7}, //Reserved UART2
    {A10,   TIMER_INVALID,  0, NULL, 0},
    {B3, TIMER2,  2, NULL, 0}, //Reserved Button
    {B5, TIMER_INVALID,  0, NULL, 0},
    {B4, TIMER3,  1, NULL, 0},
    {B10, TIMER2,  3, NULL, 0},
    {A8, TIMER_INVALID,  0, NULL, 0},
    /* Arduino-Connector CN5, right */
    {A9, TIMER_INVALID,  0, NULL, 0},
    {C7, TIMER3,  2, NULL, 0}, //Reserved Blue LED
    {B6, TIMER4,  1, NULL, 0},
    {A7, TIMER17,  1, PERIPHERAL_ADC1, 12}, //SPI1 MOSI
    {A6, TIMER3,  1, PERIPHERAL_ADC1, 11}, //SPI1 MISO
    {A5, TIMER_INVALID,  0, PERIPHERAL_ADC1, 10},
    {B9, TIMER4,  4, NULL, 0}, //Reserve I2C
    {B8, TIMER4,  3, NULL, 0}, //Reserved I2C
    /* Arduino-Connector CN8, left */
    {A0, TIMER2,  1, PERIPHERAL_ADC1, 5},
    {A1, TIMER2,  2, PERIPHERAL_ADC1, 6},
    {A4, TIMER_INVALID,  0, PERIPHERAL_ADC1, 9},
    {B0, TIMER3,  3, PERIPHERAL_ADC1, 15},
    {C1, TIMER_INVALID,  0, PERIPHERAL_ADC1, 2},
    {C0, TIMER_INVALID,  0, PERIPHERAL_ADC1, 1},

    /*ST-Morphoconnectors,CN7 on outside, inside same as arduino, only port A,B,C */
    {C10,   TIMER_INVALID,  0, NULL, 0}, //Reserved UART2
    {C12,   TIMER_INVALID,  0, NULL, 0}, //Reserved UART2
    {A13,   TIMER_INVALID,  0, NULL, 0},
    {A14, TIMER_INVALID,  0, NULL, 0},
    {A15, TIMER_INVALID,  0, NULL, 0},
    {B7, TIMER_INVALID,  0, NULL, 0},
    {C13, TIMER_INVALID,  0, NULL, 0},
    {C14, TIMER_INVALID,  0, NULL, 0},
    {C15, TIMER_INVALID,  0, NULL, 0},
    {C2, TIMER_INVALID,  0, PERIPHERAL_ADC1, 3},
    {C3, TIMER_INVALID,  0, PERIPHERAL_ADC1, 4},

    /*ST-Morphoconnectors,CN10 on outside, inside same as arduino, only port A,B,C */
    {C8,   TIMER_INVALID,  0, NULL, 0}, //Reserved UART2
    {C6,   TIMER_INVALID,  0, NULL, 0}, //Reserved UART2
    {C5,   TIMER_INVALID,  0, PERIPHERAL_ADC1, 14},
    {A12, TIMER_INVALID,  0, NULL, 0}, //Reserved Button
    {A11, TIMER_INVALID,  0, NULL, 0},
    {B12, TIMER_INVALID,  0, NULL, 0},
    {B2, TIMER_INVALID,  0, NULL, 0},
    {B1, TIMER_INVALID,  0, PERIPHERAL_ADC1, 16},
    {B15, TIMER_INVALID,  0, NULL, 0},
    {B14, TIMER_INVALID,  0, NULL, 0},
    {B13, TIMER_INVALID,  0, NULL, 0},
    {C4, TIMER_INVALID,  0, PERIPHERAL_ADC1, 13},
    /*CN7,10 inside not covered by ARDUINO headers*/
    {C11,   TIMER_INVALID,  0, NULL, 0}, //Reserved UART3
    {C9,   TIMER_INVALID,  0, NULL, 0}, //Reserved UART2
};

void board_init() {
    //UART
    uart_configure();

    //initialize all leds, default, output not PWM
    for(uint32_t i = 0; i < NUM_LEDS; i++) {
        led_init(i, OUTPUT);
    }

    //systick init
    systick_init();
    systick_start();


}