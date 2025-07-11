#pragma once
#include "gpio.h"
#include "peripheral.h"

void adc_init(const peripheral_id_adc_t adc_id);
void adc_gpio_init (gpio_id_t adc_pin);
uint16_t adc_read(const peripheral_id_adc_t adc_id, uint8_t channel);