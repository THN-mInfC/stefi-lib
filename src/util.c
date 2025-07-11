#include "libstefi/util.h"

void inline soft_delay_ms(uint32_t time_in_ms) {
    uint32_t count = time_in_ms * 1865; //Approx Factor
    while (count--)
        __asm__("NOP"); //6-8 Cycles per loop => @16 Mhz
}

void inline soft_delay_us(uint32_t time_in_us) {
    uint32_t count = time_in_us * 2; //Approx Factor
    while (count--)
        __asm__("NOP"); //6-8 Cycles per loop => @16 Mhz
}