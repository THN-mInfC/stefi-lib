#pragma once
#include <stdint.h>

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t CFR;
    volatile uint32_t SR;
} wwdg_t;

#define WWDG_BASE ((wwdg_t*) 0x40002C00)
