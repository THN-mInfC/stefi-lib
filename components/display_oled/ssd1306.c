#include "ssd1306.h"

#include <ctype.h>
#include <libstefi/systick.h>
#include <libstefi/util.h>

#include "font.h"
#include "libstefi/i2c.h"
#include "libstefi/gpio.h"

// Screenbuffer
uint8_t SSD1306_Buffer[SSD1306_BUFFER_SIZE];
static peripheral_i2c_t I2C_ID;
static uint8_t SSD1306_I2C_ADDR;

static void ssd1306_write_cmd(uint8_t byte) {
    i2c_writeto_reg(I2C_ID, SSD1306_I2C_ADDR, 0x00, &byte, 1);

}

static void ssd1306_write_data(uint8_t* buffer, uint32_t buff_size) {
    i2c_writeto_reg(I2C_ID, SSD1306_I2C_ADDR, 0x40, buffer, buff_size);
}

static void ssd1306_write_cmd_list(uint8_t *buf, uint32_t num) {
    for (int i=0;i<num;i++)
        ssd1306_write_cmd(buf[i]);
}

/* update display with actual fb */
void ssd1306_update_screen(void) {
    for(uint8_t i = 0; i < SSD1306_HEIGHT/8; i++) {
        ssd1306_write_cmd(0xB0 + i); // Set the current RAM page address.
        ssd1306_write_cmd(0x00);
        ssd1306_write_cmd(0x10);
        ssd1306_write_data(&SSD1306_Buffer[SSD1306_WIDTH*i],SSD1306_WIDTH);
    }
}

void ssd1306_render_region(uint8_t *buf, struct render_region *region) {
    uint32_t start_page = (region->y / 8);
    uint32_t end_page = ((region->y + region->height -1) / 8);
    uint32_t end_col = (region->x + region->width - 1);
    // update a portion of the display with a render area
    uint8_t cmds[] = {
        0x21,
        region->x,
        end_col,
        0x22,
        start_page,
        end_page,
    };

    ssd1306_write_cmd_list(cmds, countof(cmds));

    const uint32_t MAX_CHUNK = 128;
    uint32_t buflen = (end_col - region->x + 1) * (end_page - start_page + 1);
    while (buflen > 0) {
        uint32_t chunk = (buflen > MAX_CHUNK) ? MAX_CHUNK : buflen;
        ssd1306_write_data(buf, chunk);

        buf += chunk;
        buflen -= chunk;
    }

    //ssd1306_write_data(buf, buflen);
}

void ssd1306_set_pixel(int x, int y, int on) {
    if (x < 0 || x >= SSD1306_WIDTH || y < 0 || y >= SSD1306_HEIGHT) return;
    uint32_t page = y / 8;
    uint32_t bit = y % 8;
    if(on) {
        SSD1306_Buffer[x + SSD1306_WIDTH*page] |= 1 << bit;
    } else {
        SSD1306_Buffer[x + SSD1306_WIDTH*page] &= ~(1 << bit);
    }
}

static inline uint32_t get_font_index(uint8_t ch) {
    if (ch >= 'A' && ch <='Z') {
        return  ch - 'A' + 1;
    }
    else if (ch >= '0' && ch <='9') {
        return  ch - '0' + 27;
    }
    else return  0;
}

void ssd1306_putchar(uint32_t x, uint32_t y, uint8_t ch) {
    y = y/8;
    volatile uint32_t font_idx = get_font_index(toupper(ch))*8;
    uint32_t frame_buffer_idx = y*128 + x;

    for(uint32_t i = 0; i < 8; i++) {
        SSD1306_Buffer[frame_buffer_idx++] = font[font_idx];
        font_idx++;
    }
}

void ssd1306_putstring(uint32_t x, uint32_t y, char *str) {
    while(*str) {
        ssd1306_putchar(x, y, *str++);
        x+=8;
    }
}

void ssd1306_init(peripheral_i2c_t i2c_id, uint8_t addr) {
    i2c_init(i2c_id);
    I2C_ID = i2c_id;
    SSD1306_I2C_ADDR = addr;

    systick_delay_ms(100); //Wait for OLED to boot

    uint8_t cmds[] = {
        0xAE, 0x20, 0x00,//On, Horizontal Address Mode
        0xB0, 0xC8, //Start Page 0, Vertical Flip (Normal, C0)
        0x00, 0x10, 0x40, //low and high column start at 0,0, ram row 0 mapped to column 0
        0x81, 0xFF, //Set Contrast, i.e brightest lit pixels
        0xA1, 0xA6, //Mirror Image (used with C8), Normal 1 is ON pixel
        0xA8, 0x3F, //set multiplex ratio for 64px high display
        0xA4, //Entire Display on
        0xD3, 0x00, //Display offset 00
        0xD5, 0xF0,
        0xD9, 0x22,
        0xDA, 0x12,
        0xDB, 0x20,
        0x8D, 0x14,
        0xAF
    };

    //uint8_t cmds[] = {0xAE, 0x20, 0x00, 0xB0, 0xC8};
    ssd1306_write_cmd_list(cmds, countof(cmds));
}

// Clear screen (set buffer to 0)
void ssd1306_clear_screen(void) {
    ssd1306_write_cmd(0xAE); // Display off
    ssd1306_write_cmd(0x20); // Set Memory Addressing Mode
    ssd1306_write_cmd(0x00); // Horizontal Addressing Mode
    memset(SSD1306_Buffer, 0x00, SSD1306_BUFFER_SIZE);
    ssd1306_write_cmd(0xAF);

}