#include "libstefi/i2c.h"

#include <stdbool.h>
#include <libstefi/util.h>
#include "libstefi/peripheral.h"
#include "internal/i2c_internal.h"

static inline I2C_typeDef * i2c_get_base_address(const uint16_t id)
{
    uint32_t baseoffset = (id-1) * I2C_PORTOFFSET;
    return (I2C_typeDef *) (I2C_BASE +  baseoffset);
}

void i2c_init(const uint16_t id) {
    peripheral_i2c_enable(id);

    I2C_typeDef *i2c = i2c_get_base_address(id);

    //formula later
    i2c->TIMINGR = (uint32_t) 0x00100D14;   // set I2C timing (100kHz with default clock)
    i2c->CR1 |= BIT(0);                 // Enable I2C
}

void i2c_gpio_init(gpio_id_t scl_pin, gpio_id_t sda_pin) {
    peripheral_gpio_enable(gpio_get_port_from_portpin(scl_pin));
    peripheral_gpio_enable(gpio_get_port_from_portpin(sda_pin));

    gpio_set_alternate_function(scl_pin, AF4);//all i2c on AF4
    gpio_set_alternate_function(sda_pin, AF4);
    gpio_set_mode(scl_pin, MODER_AF);
    gpio_set_mode(sda_pin, MODER_AF);

    gpio_set_output_type(scl_pin, OPEN_DRAIN);
    gpio_set_output_type(sda_pin, OPEN_DRAIN);
}



void i2c_readfrom(const uint16_t id, uint8_t address7b, uint8_t *buf, uint32_t len) {
    I2C_typeDef *i2c = i2c_get_base_address(id);

    i2c->CR2 = ((address7b << 1) & 0x3FF); //Slave address
    i2c->CR2 |= BIT(25); //Stop=True
    i2c->CR2 |= (len & 0xFF) << 16;
    i2c->CR2 |= BIT(10); //Master request read, before start
    i2c->CR2 |= BIT(13); //START


    for(uint32_t i = 0; i < len; i++) {
        while ((i2c->ISR & BIT(2)) != BIT(2)); //Receive Data Register empty
        buf[i] = i2c->RXDR; //READ BYTE
    }
}

void i2c_writeto(const uint16_t id, uint8_t address7b, uint8_t *buf, uint32_t len, bool repeat) {
    I2C_typeDef *i2c = i2c_get_base_address(id);
    uint32_t cr2 = 0;

    cr2 = ((address7b << 1) & 0x3FF);//Slave address
    cr2 |= (len & 0xFF) << 16;
    if (!repeat) cr2 |= BIT(25); //Autoend
    cr2 &= ~BIT(10);
    cr2 |= BIT(13); //START

    i2c->CR2 = cr2;

    for(uint32_t i = 0; i < len; i++) {
        while ((i2c->ISR & BIT(0)) != BIT(0)); //Wait for TXE
        i2c->TXDR = buf[i]; //Write BYTE
    }

    if (repeat) {
        // Wait for transfer complete (not auto-end)
        while ((i2c->ISR & BIT(6)) != BIT(6));
    } else {
        // Wait for STOP
        while ((i2c->ISR & BIT(5)) != BIT(5));
    }
}

void i2c_writeto_reg(const uint16_t id, uint8_t address7b, uint8_t reg, uint8_t *buf, uint32_t len) {

    I2C_typeDef *i2c = i2c_get_base_address(id);
    uint32_t cr2 = 0;
    while ((i2c->ISR & BIT(15)) == BIT(15));

    cr2 = ((address7b << 1) & 0x3FF);//Slave address
    cr2 |= ((len+1) & 0xFF) << 16; //+1 for reg
    cr2 |= BIT(25); //Autoend
    cr2 |= BIT(13); //START

    i2c->CR2 = cr2;
    while ((i2c->ISR & BIT(0)) != BIT(0));
    i2c->TXDR = reg;

    for(uint32_t i = 0; i < len; i++) {
        while ((i2c->ISR & BIT(0)) != BIT(0)); //Wait for TXE
        i2c->TXDR = buf[i]; //Write BYTE
    }

    // Wait for STOP
    while ((i2c->ISR & BIT(5)) != BIT(5));
}
