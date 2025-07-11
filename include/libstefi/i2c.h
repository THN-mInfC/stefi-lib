#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "libstefi/gpio.h"


void i2c_init(const uint16_t id, gpio_id_t scl_pin, gpio_id_t sda_pin);
void i2c_readfrom(const uint16_t id, uint8_t address7b, uint8_t *buf, uint32_t len);

void i2c_writeto(const uint16_t id, uint8_t address7b, uint8_t *buf, uint32_t len, bool repeat);
void i2c_writeto_reg(const uint16_t id, uint8_t address7b, uint8_t reg, uint8_t *buf, uint32_t len);