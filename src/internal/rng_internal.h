#pragma once
#include <stdint.h>

typedef struct {
    volatile uint32_t CR;
    volatile uint32_t SR;
    volatile uint32_t DR;
} RNG_typeDef;

#define RNG ((RNG_typeDef *) 0x50060800)