#pragma once
#include <stdint.h>

/** NVIC register map type. */
typedef struct {
    volatile uint32_t ISER[8];      /**< Interrupt Set Enable Registers */
    volatile uint32_t RESERVED0[24];
    volatile uint32_t ICER[8];      /**< Interrupt Clear Enable Registers */
    volatile uint32_t RESERVED1[24];
    volatile uint32_t ISPR[8];      /**< Interrupt Set Pending Registers */
    volatile uint32_t RESERVED2[24];
    volatile uint32_t ICPR[8];      /**< Interrupt Clear Pending Registers */
    uint32_t RESERVED3[24];
    volatile uint32_t IABR[8];      /**< Interrupt Active bit Registers */
    volatile uint32_t RESERVED5[644];
    volatile uint32_t STIR;         /**< Software Trigger Interrupt Registers */
} NVIC_typeDef;

#define NVIC_BASE ((NVIC_typeDef *) 0xE000E100)


#define NVIC_GlobalEnable() do { __asm volatile("cpsie i"); } while (0)
#define NVIC_GlobalDisable() do { __asm volatile("cpsid i"); } while(0)
