#pragma once

/**********************************************************************/
/* RCC (Reset Clock Control) Hardware Abstraction layer               */
/**********************************************************************/
typedef enum peripheral_id_gpio peripheral_id_gpio_t;
typedef enum peripheral_id_tim peripheral_id_tim_t;
typedef enum peripheral_id_uart peripheral_id_uart_t;
typedef enum peripheral_id_i2c peripheral_id_i2c_t;
typedef enum peripheral_id_spi peripheral_id_spi_t;
typedef enum peripheral_id_adc peripheral_id_adc_t;

void peripheral_gpio_enable(peripheral_id_gpio_t);
void peripheral_tim_enable(peripheral_id_tim_t);
void peripheral_uart_enable(peripheral_id_uart_t);
void peripheral_i2c_enable(peripheral_id_i2c_t id);
void peripheral_spi_enable(peripheral_id_spi_t id);
void peripheral_adc_enable(peripheral_id_adc_t id);
void peripheral_exti_enable();
void peripheral_rng_enable();

enum peripheral_id_gpio {
    PERIPHERAL_ID_GPIOA,
    PERIPHERAL_ID_GPIOB,
    PERIPHERAL_ID_GPIOC,
    PERIPHERAL_GPIO_COUNT
};

enum peripheral_id_tim {
    PERIPHERAL_ID_TIM1,
    PERIPHERAL_ID_TIM2,
    PERIPHERAL_ID_TIM3,
    PERIPHERAL_ID_TIM4,
    PERIPHERAL_ID_TIM5,
    PERIPHERAL_ID_TIM6,
    PERIPHERAL_ID_TIM7,
    PERIPHERAL_ID_TIM8,
    PERIPHERAL_ID_TIM15 = 14,
    PERIPHERAL_ID_TIM16,
    PERIPHERAL_ID_TIM17,
    PERIPHERAL_TIM_COUNT
};

enum peripheral_id_uart {
    PERIPHERAL_ID_UART1,
    PERIPHERAL_ID_UART2,
    PERIPHERAL_UART_COUNT
};

enum peripheral_id_i2c {
    PERIPHERAL_ID_I2C1,
    PERIPHERAL_ID_I2C2,
    PERIPHERAL_ID_I2C3,
    PERIPHERAL_I2C_COUNT
};

enum peripheral_id_spi {
    PERIPHERAL_ID_SPI1,
    PERIPHERAL_ID_SPI2,
    PERIPHERAL_ID_SPI3,
    PERIPHERAL_SPI_COUNT
};

enum peripheral_id_adc {
    PERIPHERAL_ID_ADC1,
    PERIPHERAL_ID_ADC2,
    PERIPHERAL_ID_ADC3,
    PERIPHERAL_ADC_COUNT
};
