// Task: test api functions on stm32 nucleo
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"
#include "libstefi/uart.h"


#define LED A5
#define BUTTON C13

void gpio_init() {
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    peripheral_gpio_enable(PERIPHERAL_GPIOC);
    gpio_set_mode(LED, MODER_OUTPUT);
    gpio_set_mode(BUTTON, MODER_INPUT);
    gpio_set_pupd(BUTTON, PULL_UP);
}

void gpio_toggle_test() {
    while(1) {
        gpio_toggle(LED);
        soft_delay_ms(500);
    }
}

void gpio_readwrite_test()
{
    // USER LED on Nucleo Board in High-Active
    gpio_write(LED, LOW);
    if(gpio_read(LED) == HIGH)
    {
        printf("GPIO read/write test passed \r\n");
    } else
    {
        printf("GPIO read/write test failed\r\n");
    }
}

void button_interrupt_callback()
{
    gpio_toggle(LED);
}

void gpio_interrupt_test() {
    gpio_enable_interrupt(BUTTON, FALLING_EDGE);
    gpio_interrupt_register_handler(BUTTON, button_interrupt_callback);
    while (1) {
    }
}

void main(){
    uart_configure();
    gpio_init();
    //gpio_toggle_test();
    //gpio_readwrite_test();
    gpio_interrupt_test();
}