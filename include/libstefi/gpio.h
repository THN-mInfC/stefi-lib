#pragma once

#include <../src/internal/gpio_internal.h>

#include "interrupts.h"
#include "stdint.h"
/**********************************************************************/
/* GPIO (General Purpose Input and Output) Hardware Abstraction layer */
/**********************************************************************/
typedef enum _stefilite_ids gpio_id_t;
typedef enum _stefi_exti_ids exti_id_t;

enum _stefilite_ids
{
    A0 = 0x000, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15,
    B0 = 0x100, B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12, B13, B14, B15,
    C0 = 0x200, C1, C2, C3, C4, C5, C6, C7, C8, C9, C10, C11, C12, C13, C14, C15,
};

typedef enum {LOW, HIGH} sig_t;
typedef enum {MODER_INPUT, MODER_OUTPUT, MODER_AF, MODER_ANALOG} moder_t;
typedef enum {PUSH_PULL, OPEN_DRAIN} otype_t;
typedef enum {LOW_S, MEDIUM_S, HIGH_S, VERY_HIGH_S} ospeed_t;
typedef enum {NONE, PULL_UP, PULL_DOWN} pupdr_t;
typedef enum {AF0, AF1, AF2, AF3, AF4, AF5, AF6, AF7, AF8, AF9, AF10, AF11, AF12, AF13, AF14, AF15,} afr_t;
typedef enum {RISING_EDGE, FALLING_EDGE} edge_t;

/* GPIO initialization functions*/
void gpio_set_mode(const gpio_id_t portpin, moder_t mode);
void gpio_set_output_type(const gpio_id_t portpin, otype_t otype);
void gpio_set_output_speed(const gpio_id_t portpin, ospeed_t ospeed);
void gpio_set_pupd(const gpio_id_t portpin, pupdr_t pupd);
void gpio_set_alternate_function(const gpio_id_t portpin, afr_t af);
void gpio_set_analog_switch(const gpio_id_t portpin);

/* GPIO read, write, functions */
static inline GPIO_typeDef * gpio_get_base_address(const gpio_id_t portpin)
{
    uint16_t port = (portpin >> 8);
    uint32_t baseoffset = port * GPIO_PORTOFFSET;
    return (GPIO_typeDef *) (GPIO_BASE +  baseoffset);
}

static inline sig_t gpio_read(const gpio_id_t portpin)
{
    uint16_t pin = portpin & 0xFF;
    GPIO_typeDef *gpio = gpio_get_base_address(portpin);

    return (gpio->IDR & (1<<pin) ? HIGH : LOW);
}

static inline void gpio_write(const gpio_id_t portpin, sig_t val)
{
    uint16_t pin = portpin & 0xFF;
    GPIO_typeDef *gpio = gpio_get_base_address(portpin);

    //low 16 bit odr high, upper 16 bit odr low
    gpio->BSRR = (1 << pin)<<( val ? 0: 16);
}

static inline void gpio_toggle(const gpio_id_t portpin)
{
    uint16_t pin = portpin & 0xFF;
    GPIO_typeDef *gpio = gpio_get_base_address(portpin);

    gpio->ODR ^= (1 << pin);
}

/* EXTI Functions(extended interrupts mapped to GPIO Pins)*/
void gpio_enable_interrupt(const gpio_id_t, const edge_t);
void gpio_disable_interrupt(const gpio_id_t, const edge_t);

void gpio_clear_interruptflag(const uint8_t);
void gpio_interrupt_register_handler(const gpio_id_t portpin, callbackfn_typeDef);

/* For testing*/
moder_t gpio_get_mode(const gpio_id_t portpin);
uint16_t gpio_get_port(const gpio_id_t portpin);


