#pragma once
#include <stdint.h>

//Add __weak if irq handler defined in example or main
typedef struct {
    volatile uint32_t CTRL;
    volatile uint32_t LOAD;
    volatile uint32_t VAL;
} SysTick_t;

#define SysTick ((SysTick_t *) 0xE000E010)