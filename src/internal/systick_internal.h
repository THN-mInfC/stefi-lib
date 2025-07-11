#pragma once
#include "libstefi/board.h"
#include <stdint.h>

#define TICKS_PER_S SYSCLK
#define TICKS_PER_MS (SYSCLK/1000U)
#define TICKS_PER_US (SYSCLK/1000000U)

//Add __weak if irq handler defined in example or main
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
} SysTick_t;

#define SysTick ((SysTick_t *) 0xE000E010)