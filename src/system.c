#include "libstefi/system.h"

#include <rcc_internal.h>
#include <libstefi/util.h>

#include "internal/cortexm4_internal.h"
#include "internal/pwr_internal.h"

//L4: APBx Clock same as Sys Clock
enum {
    APB_PSC_1 = 1, //Default
    APB_PSC_2 = 4,
    APB_PSC_4 = 5,
    APB_PSC_8 = 6,
    APB_PSC_16 = 7,
};

enum {
    HSI16_CLK = 16,
    PLL_M = 1, //Div Factor(1) FOR PLL Input(1..8)
    PLL_N = 10, //Mul Factor(10) for VC0
    PLL_R = 2, //Div Factor(2) for PLL Out(2,4,6,8)
};
#define APB1_PSC APB_PSC_1
#define APB2_PSC APB_PSC_1

const uint32_t sys_clk = ((HSI16_CLK/ PLL_M) * PLL_N) / PLL_R;


//Initialize to 80 Mhz using HSI:
void system_init() {
    SCB->CPACR |= (0XF << 20); //FPU Enable
    //1. Enable HSI (G4: Default, L4: MSI) and Voltage scaling (Range 1 for L4, G4) for high perf.
    RCC->APB1ENR1 |= BIT(28);
    PWR->CR1 |= BIT(9); //range 1
    while((PWR->SR2 & BIT(10)) == BIT(10)); // While Voltage is changing

    RCC->CR |= BIT(8); //(explicit for L4, HSI)
    while((RCC->CR & BIT(10)) != BIT(10)); //bit 10 is zero, while oscillator not ready

    //2. Configure Flash Latency (g4: 2WS, l4: 4 WS (5cycles) for 80 MHZ)
    FLASH->ACR &= ~(0x7); //0 WS
    FLASH->ACR |= BIT(2); //4 WS
    FLASH->ACR |= BIT(8) | BIT(9); // Enable Instruction Cache, Prefetch

    //3. Configure PLL
    RCC->PLLCFGR = 0;
    RCC->PLLCFGR |= (PLL_M - 1) << 4;               // Set PLL_M
    RCC->PLLCFGR |= PLL_N << 8;                     // Set PLL_N
    RCC->PLLCFGR |= (((PLL_R - 2)/2) & 3) << 25 ;   // Set PLL_R
    RCC->PLLCFGR |= 2 << 0;                         //HSI Clock as PLL Input

    //4. Enable PLL
    RCC->CR |= BIT(24); //enable pll clock output
    while((RCC->CR & BIT(25)) != BIT(25)); //while pll unlocked
    RCC->PLLCFGR |= BIT(24); //PLL output enable

    //5. Select PLL as System Clock
    RCC->CFGR |= ((RCC->CFGR & (~3)) | 3);
    while (((RCC->CFGR >> 2) & 0b11) != 0b11); //Wait until PLL used as system clock
}