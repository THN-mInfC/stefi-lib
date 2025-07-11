#pragma once

#include <stdint.h>

#include "libstefi/peripheral.h"
#include "libstefi/gpio.h"


void spi_init(const peripheral_id_spi_t spi_id, gpio_id_t cs_pin, gpio_id_t sck_pin, gpio_id_t miso_pin, gpio_id_t mosi_pin);
uint8_t spi_transfer(const peripheral_id_spi_t spi_id, uint8_t data);
