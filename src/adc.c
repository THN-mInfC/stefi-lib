#include "libstefi/adc.h"

#include <assert.h>
#include <stddef.h>
#include <libstefi/peripheral.h>
#include <libstefi/util.h>
#include <libstefi/gpio.h>

#include "internal/adc_internal.h"

ADC_t *adc_peripherals[] = {(ADC_t *) ADC1_BASE, NULL, NULL};

void adc_init(const peripheral_adc_t adc_id) {
    assert(adc_id >= 0);
    peripheral_adc_enable(adc_id);

    ADC_t *adc = adc_peripherals[adc_id];

    adc->CR &= ~BIT(29);//Wake up ADC from DEEP POWER DOWN mode
    adc->CR |= BIT(28);//Switch on ADC voltage regulator
    soft_delay_us(20); //wait at least 20 µsuntil it is ready

    //Select ADC clock source, default no prescale, synchronous to bus clock/1
    ADC123_COMMON_BASE->CCR |= BIT(16);
    // - Calibrate ADC
    if ((adc->CR & BIT(0)) != 0) {
        adc->CR |= BIT(1);
        while (adc->CR & BIT(1)); // Wait until fully disabled
    }
    adc->CR |= BIT(31);
    while (adc->CR & BIT(31));

    //- adc_set_sample rate
    adc->SMPR1 |= (4 << 15) | (4 << 18); //47.5 cycles for ADC

    // - Switch on ADC
    adc->ISR |= BIT(0);   // Clear ready flag
    adc->CR |= BIT(0);      // Enable ADC
    while (!(adc->ISR & BIT(0))); // Wait for ADC ready//Clear ADC ready bit
}

void adc_gpio_init(gpio_id_t adc_pin) {
    //enable clock
    peripheral_gpio_enable(gpio_get_port_from_portpin(adc_pin));
    gpio_set_mode(adc_pin, MODER_ANALOG);
    gpio_set_analog_switch(adc_pin);
}

uint16_t adc_read(const peripheral_adc_t adc_id, uint8_t channel) {
    assert(adc_id >= 0);
    ADC_t *adc = adc_peripherals[adc_id];
    //- Set number of conversions to 1
    adc->SQR1 &= ~(0xF << 0);    // 1 conversion
    //- Set channel to be converted (ADC1->SQRx)
    adc->SQR1 &= ~(0x1F << 6);   // Clear SQ1 bits
    adc->SQR1 |= (channel << 6);       // SQ1 = channel 5

    // Start ADC conversion sequence
    adc->CR |= BIT(2);

    while (!(adc->ISR & BIT(2))); //channel conversion
    return (uint16_t)(adc->DR);
}

