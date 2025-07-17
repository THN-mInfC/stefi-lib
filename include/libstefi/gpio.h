#pragma once

#include "../src/internal/gpio_internal.h"

#include "interrupts.h"
#include "stdint.h"
/**********************************************************************/
/* GPIO (General Purpose Input and Output) Hardware Abstraction layer */
/**********************************************************************/
typedef enum
{
    A0 = 0x000, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
    B0 = 0x100, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
    C0 = 0x200, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15,
} gpio_id_t;

typedef enum {LOW = 0, HIGH = 1} sig_t;
typedef enum {MODER_INPUT = 0, MODER_OUTPUT = 1, MODER_AF = 2, MODER_ANALOG = 3} moder_t;
typedef enum {PUSH_PULL = 0, OPEN_DRAIN = 1} otype_t;
typedef enum {LOW_S = 0, MEDIUM_S = 1, HIGH_S = 2, VERY_HIGH_S = 3} ospeed_t;
typedef enum {NONE = 0, PULL_UP = 1, PULL_DOWN = 2} pupdr_t;
typedef enum {AF0 = 0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15,} afr_t;
typedef enum {RISING_EDGE = 0, FALLING_EDGE = 1} edge_t;

/* GPIO initialization functions*/
void gpio_set_mode(gpio_id_t portpin, moder_t mode);
void gpio_set_output_type(gpio_id_t portpin, otype_t otype);
void gpio_set_output_speed(gpio_id_t portpin, ospeed_t ospeed);
void gpio_set_pupd(gpio_id_t portpin, pupdr_t pupd);
void gpio_set_alternate_function(gpio_id_t portpin, afr_t af);
void gpio_set_analog_switch(gpio_id_t portpin);

/* GPIO read, write, functions */
static inline GPIO_typeDef * gpio_get_base_address(gpio_id_t portpin)
{
    uint16_t port = (portpin >> 8);
    uint32_t baseoffset = port * GPIO_PORTOFFSET;
    return (GPIO_typeDef *) (GPIO_BASE +  baseoffset);
}

static inline sig_t gpio_read(gpio_id_t portpin)
{
    uint16_t pin = portpin & 0xFF;
    GPIO_typeDef *gpio = gpio_get_base_address(portpin);
    return (gpio->IDR & (1<<pin) ? HIGH : LOW);
}

static inline void gpio_write(gpio_id_t portpin, sig_t val)
{
    uint16_t pin = portpin & 0xFF;
    GPIO_typeDef *gpio = gpio_get_base_address(portpin);
    //low 16 bit odr high, upper 16 bit odr low
    gpio->BSRR = (1 << pin)<<( val ? 0: 16);
}

static inline void gpio_toggle(gpio_id_t portpin)
{
    uint16_t pin = portpin & 0xFF;
    GPIO_typeDef *gpio = gpio_get_base_address(portpin);
    gpio->ODR ^= (1 << pin);
}

/* EXTI Functions(extended interrupts mapped to GPIO Pins)*/
void gpio_enable_interrupt(gpio_id_t portpin,  edge_t edge);
void gpio_disable_interrupt(gpio_id_t portpin,  edge_t edge);
void gpio_interrupt_register_handler(gpio_id_t portpin, callbackfn_typeDef);

/* For testing*/
moder_t gpio_get_mode(gpio_id_t portpin);
uint16_t gpio_get_port_from_portpin(gpio_id_t portpin);


