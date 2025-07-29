#include <libstefi/system.h>
#include <libstefi/timer.h>
#include <libstefi/uart.h>
#include <libstefi/util.h>

#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"

#define LED A5
#define DHT11_PIN A0

typedef struct {
    uint8_t temperature;
    uint8_t humidity;
    uint8_t error;
} dht11_data_t;

volatile uint32_t *DWT_CYCCNT = (volatile uint32_t *)0xE0001004;
volatile uint32_t *DWT_CONTROL = (volatile uint32_t *)0xE0001000;
volatile uint32_t *SCB_DEMCR = (volatile uint32_t *)0xE000EDFC;

void timer_delay_us(uint32_t us) {
    uint32_t start_ticks = *DWT_CYCCNT;
    uint32_t ticks = us * (80000000 / 1000000);

    while ((*DWT_CYCCNT - start_ticks) < ticks);
}

// void timer_delay_us(const tim_id_t timer_id, uint16_t delay_us) {
//     timer_init(timer_id);
//     timer_set_period(timer_id, 80, delay_us);
//     timer_set_mode_opm(timer_id);
//     timer_start(timer_id);
//     timer_wait_for_update_flag(timer_id);
// }

static uint8_t dht11_read_val(void) {
    uint8_t byte = 0;
    for (int i = 0; i < 8; i++) {
        while (!gpio_read(DHT11_PIN));        // wait for HIGH
        timer_delay_us(30);
        if (gpio_read(DHT11_PIN))
            byte |= (1 << (7 - i)); // bit = 1

        while (gpio_read(DHT11_PIN));         // wait for LOW
    }
    return byte;
}

dht11_data_t dht11_bitbang_read(void) {
    dht11_data_t data = {0};

    gpio_set_mode(DHT11_PIN, MODER_OUTPUT);
    gpio_write(DHT11_PIN, LOW);
    timer_delay_us(20000);// >18 ms start signal
    gpio_write(DHT11_PIN, HIGH);
    gpio_set_mode(DHT11_PIN, MODER_INPUT);
    timer_delay_us( 30);    // >20us pull-up



    uint32_t timeout = 0;
    while (gpio_read(DHT11_PIN) && timeout < 100) {
        timer_delay_us(1);
        timeout++;
    }

    if (timeout >= 100) {
        data.error = 1;  // no response
        return data;
    }

    timeout = 0;

    while(!gpio_read(DHT11_PIN) && timeout < 100) {
        timer_delay_us(1);
        timeout++;
    }
    if (timeout >= 100) {
        data.error = 1;  // no response
        return data;
    }
    timer_delay_us(90);
    uint8_t h_int = dht11_read_val();
    uint8_t h_dec = dht11_read_val();
    uint8_t t_int = dht11_read_val();
    uint8_t t_dec = dht11_read_val();
    uint8_t checksum = dht11_read_val();
    if ((h_int + h_dec + t_int + t_dec) != checksum) {
        data.error = 2;  // checksum failed
        return data;
    }

    data.humidity = h_int;
    data.temperature = t_int;
    return data;
}

void dwt_init(void) {
    *SCB_DEMCR |= 0x01000000;
    *DWT_CONTROL |= 1;
}

void main(){
    uart_configure();
    //80MHz
    system_init();

    //DHT11 Init
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    gpio_set_mode(LED, MODER_OUTPUT);

    gpio_set_mode(DHT11_PIN, MODER_OUTPUT);
    //gpio_set_output_type(DHT11_PIN, VERY_HIGH_S);

    dwt_init();

    printf("DHT 11- Bit banging \r\n");
    while(1) {
        dht11_data_t data = dht11_bitbang_read();
        printf("-----\n Humidity: %d \r\n", data.humidity);
        printf("Temperature: %d \r\n", data.temperature);
        soft_delay_ms(50000);
    }
}