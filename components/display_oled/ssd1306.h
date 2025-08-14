#pragma once

#include <libstefi/peripheral.h>

#include "stdint.h"

#define SSD1306_HEIGHT          64
#define SSD1306_WIDTH           128
#define SSD1306_BUFFER_SIZE   SSD1306_WIDTH * SSD1306_HEIGHT / 8

extern uint8_t SSD1306_Buffer[];

struct render_region {
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
};

void test_init();

// Public API
void ssd1306_init(peripheral_i2c_t i2c_id, uint8_t addr);
void ssd1306_update_screen(void);
void ssd1306_clear_screen(void);
void ssd1306_render_region(uint8_t *buf, struct render_region *region);
void ssd1306_set_pixel(int x, int y, int on);
void ssd1306_putchar(uint32_t x, uint32_t y, uint8_t ch);
void ssd1306_putstring(uint32_t x, uint32_t y, char *str);