#include "board.h"
#include "button.h"
#include "led.h"
#include "ssd1306.h"

#include <libstefi/gpio.h>
#include <libstefi/spi.h>
#include <libstefi/systick.h>

#include <stdio.h>
#include <string.h>

/*
 * STefi (NUCLEO-L476) full board smoke test — logs to the ST-Link VCP.
 *
 * Open a terminal at 115200 8N1 (e.g. `screen /dev/tty.usbmodem* 115200`).
 * printf is reliable now that uart_configure() clocks USART2 from HSI16.
 * The OLED mirrors the current section (it is itself under test).
 *
 * Sections:
 *   1. OLED      — visual: confirm the banner shows on the display.
 *   2. LEDs      — walk all 5, then PWM-fade green/blue (TIM3). Visual.
 *   3. Buttons   — press each (S0..S3 + NUCLEO); logged as they register.
 *   4. FRAM      — FM25CL64 over SPI1: write a pattern, read back, verify.
 *
 * NOTE: A5 is shared between LED_NUCLEO and FRAM SCK, so the LED walk runs
 * before SPI init; after the FRAM section A5 is the SPI clock.
 */

#define FRAM_CS    A4
#define FRAM_WREN  0x06
#define FRAM_WRITE 0x02
#define FRAM_READ  0x03

static void oled_banner(const char *l0, const char *l1) {
    ssd1306_clear_screen();
    if (l0) ssd1306_putstring(0,  0, (char *)l0);
    if (l1) ssd1306_putstring(0, 24, (char *)l1);
    ssd1306_update_screen();
}

/* ---- LEDs ------------------------------------------------------------- */
static void test_leds(void) {
    printf("[2] LEDs: walking RED YELLOW GREEN BLUE NUCLEO\r\n");
    oled_banner("2. LEDs", "walking...");

    const stefi_led_t order[] = {LED0_RED, LED1_YELLOW, LED2_GREEN, LED3_BLUE, LED_NUCLEO};
    const char *name[] = {"RED", "YELLOW", "GREEN", "BLUE", "NUCLEO"};
    for (uint32_t i = 0; i < 5; i++) {
        printf("    %s on\r\n", name[i]);
        led_on(order[i]);
        systick_delay_ms(300);
        led_off(order[i]);
    }

    printf("    PWM fade green/blue (TIM3)\r\n");
    oled_banner("2. LEDs", "PWM fade G/B");
    led_init(LED2_GREEN, PWM);
    led_init(LED3_BLUE, PWM);
    for (uint32_t d = 0; d <= 255; d += 5) {
        led_set_brightness(LED2_GREEN, d);
        led_set_brightness(LED3_BLUE, 255 - d);
        systick_delay_ms(15);
    }
    led_set_brightness(LED2_GREEN, 0);
    led_set_brightness(LED3_BLUE, 0);
    printf("    LEDs done\r\n");
}

/* ---- Buttons ---------------------------------------------------------- */
static bool test_buttons(void) {
    const stefi_button_t btn[] = {BUTTON_S0, BUTTON_S1, BUTTON_S2, BUTTON_S3, BUTTON_NUCLEO};
    const char *name[] = {"S0", "S1", "S2", "S3", "NUCLEO"};
    uint32_t seen = 0;
    const uint32_t all = 0x1F;

    printf("[3] Buttons: press each once (S0 S1 S2 S3 NUCLEO)\r\n");
    oled_banner("3. Buttons", "press each once");

    uint32_t start = systick_get_ms();
    while (seen != all && (systick_get_ms() - start) < 25000) {
        for (uint32_t i = 0; i < 5; i++) {
            if (button_is_pressed(btn[i]) && !(seen & (1u << i))) {
                seen |= (1u << i);
                printf("    %s ok\r\n", name[i]);
                char line[24];
                sprintf(line, "%s ok", name[i]);
                oled_banner("3. Buttons", line);
            }
        }
        systick_delay_ms(30);
    }

    bool ok = (seen == all);
    if (ok) {
        printf("    all buttons OK\r\n");
    } else {
        printf("    TIMEOUT, missing:");
        for (uint32_t i = 0; i < 5; i++)
            if (!(seen & (1u << i))) printf(" %s", name[i]);
        printf("\r\n");
    }
    oled_banner("3. Buttons", ok ? "ALL OK" : "missing some");
    systick_delay_ms(1200);
    return ok;
}

/* ---- FRAM (FM25CL64 on SPI1) ----------------------------------------- */
static void fram_write(uint16_t addr, const uint8_t *data, uint32_t len) {
    gpio_write(FRAM_CS, LOW);
    spi_transfer(PERIPHERAL_SPI1, FRAM_WREN);
    gpio_write(FRAM_CS, HIGH);

    gpio_write(FRAM_CS, LOW);
    spi_transfer(PERIPHERAL_SPI1, FRAM_WRITE);
    spi_transfer(PERIPHERAL_SPI1, (addr >> 8) & 0xFF);
    spi_transfer(PERIPHERAL_SPI1, addr & 0xFF);
    for (uint32_t i = 0; i < len; i++)
        spi_transfer(PERIPHERAL_SPI1, data[i]);
    gpio_write(FRAM_CS, HIGH);
}

static void fram_read(uint16_t addr, uint8_t *data, uint32_t len) {
    gpio_write(FRAM_CS, LOW);
    spi_transfer(PERIPHERAL_SPI1, FRAM_READ);
    spi_transfer(PERIPHERAL_SPI1, (addr >> 8) & 0xFF);
    spi_transfer(PERIPHERAL_SPI1, addr & 0xFF);
    for (uint32_t i = 0; i < len; i++)
        data[i] = spi_transfer(PERIPHERAL_SPI1, 0xFF);
    gpio_write(FRAM_CS, HIGH);
}

static bool test_fram(void) {
    printf("[4] FRAM (FM25CL64 on SPI1, takes over A5/SCK)\r\n");
    oled_banner("4. FRAM SPI1", "write/read...");

    spi_init(PERIPHERAL_SPI1);
    spi_gpio_init(A4, A5, A6, A7);
    gpio_write(FRAM_CS, HIGH);

    const uint8_t pattern[8] = {0xDE,0xAD,0xBE,0xEF,0x01,0x23,0x45,0x67};
    uint8_t back[8] = {0};
    fram_write(0x0010, pattern, 8);
    fram_read(0x0010, back, 8);
    bool ok = (memcmp(pattern, back, 8) == 0);

    printf("    wrote ");
    for (int i = 0; i < 8; i++) printf("%02X ", pattern[i]);
    printf("\r\n    read  ");
    for (int i = 0; i < 8; i++) printf("%02X ", back[i]);
    printf("-> %s\r\n", ok ? "PASS" : "FAIL");
    oled_banner("4. FRAM SPI1", ok ? "PASS" : "FAIL");
    systick_delay_ms(1500);
    return ok;
}

void main(void) {
    board_init();   // UART(HSI16), LEDs, buttons, debounce TIMER6, SysTick, OLED

    printf("\r\n=== STefi Board Test ===\r\n");
    printf("[1] OLED: confirm this banner shows on the display\r\n");
    oled_banner("STefi BoardTest", "1. OLED OK");
    systick_delay_ms(1500);

    test_leds();
    bool btn_ok  = test_buttons();
    bool fram_ok = test_fram();

    printf("=== summary: LEDs=visual  BUTTONS=%s  FRAM=%s ===\r\n",
           btn_ok ? "OK" : "CHECK", fram_ok ? "PASS" : "FAIL");

    char line[24];
    sprintf(line, "BTN:%s FRAM:%s", btn_ok ? "OK" : "??", fram_ok ? "OK" : "X");
    led_init(LED0_RED, OUTPUT);   // A5 now belongs to SPI, blink RED instead
    uint32_t s = 0;
    while (1) {
        printf("uptime %lus  %s\r\n", (unsigned long)s++, line);
        oled_banner("Board test done", line);
        led_toggle(LED0_RED);
        systick_delay_ms(1000);
    }
}
