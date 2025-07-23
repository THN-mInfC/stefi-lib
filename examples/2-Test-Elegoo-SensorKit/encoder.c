#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"
#include <libstefi/adc.h>
#include <libstefi/systick.h>

/* Sensor: Encoder  */
/* Demo: Connect VCC-3.3 V, GND */
/* Function: Count +1,-1, depending on direction. SW RESET TO 0  */

#define USER_LED A5
#define ENCODER_CLK_PIN A0 //Interrupt
#define ENCODER_DT_PIN A1
#define ENCODER_SW_PIN A10

volatile long count = 0;
unsigned long t = 0;

void rotaryEncoderChanged(){ // when CLK_PIN is FALLING
    unsigned long temp = systick_get_ms();
    if(temp - t < 10) // remove bounce: primitive
        return;
    t = temp;

    // DT_PIN state represents a forward or reverse.
    count += gpio_read(ENCODER_DT_PIN) == HIGH ? 1 : -1;
    printf("Count: %ld t: %d\r\n", count, t);
}

void main(){
    uart_configure();

    peripheral_gpio_enable(PERIPHERAL_ID_GPIOA);
    gpio_set_mode(USER_LED, MODER_OUTPUT);

    gpio_set_mode(ENCODER_CLK_PIN, MODER_INPUT);
    gpio_set_mode(ENCODER_DT_PIN, MODER_INPUT);
    gpio_set_mode(ENCODER_SW_PIN, MODER_INPUT);
    //
    gpio_set_pupd(ENCODER_CLK_PIN, PULL_UP);
    gpio_set_pupd(ENCODER_DT_PIN, PULL_UP);
    gpio_set_pupd(ENCODER_SW_PIN, PULL_UP);

    /* Interrupt init*/
    gpio_enable_interrupt(ENCODER_CLK_PIN, FALLING_EDGE);
    gpio_interrupt_register_handler(ENCODER_CLK_PIN, rotaryEncoderChanged);

    systick_init();
    systick_start();

    while(1) {
        if(gpio_read(ENCODER_SW_PIN) == LOW){
            count = 0;
            printf("t (%d): count reset to 0 \r\n", systick_get_ms());
            soft_delay_ms(500);
        }
    }
}
