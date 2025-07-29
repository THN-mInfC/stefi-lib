#pragma once

/**********************************************************************/
/* Interrupts Hardware Abstraction layer */
/**********************************************************************/

//Datatypes
typedef void (*callbackfn_t)();

enum _nvic_interrupt_sources {
    INTERRUPT_SOURCE_NONE = -1,
    INTERRUPT_SOURCE_EXTI0 = 6,
    INTERRUPT_SOURCE_EXTI1 = 7,
    INTERRUPT_SOURCE_EXTI2 = 8,
    INTERRUPT_SOURCE_EXTI3 = 9,
    INTERRUPT_SOURCE_EXTI4 = 10,
    INTERRUPT_SOURCE_EXTI5_9 = 23,
    INTERRUPT_SOURCE_EXTI10_15 = 40,
    INTERRUPT_SOURCE_TIM2 = 28,
    INTERRUPT_SOURCE_TIM3 = 29,
    INTERRUPT_SOURCE_TIM4 = 30,
    INTERRUPT_SOURCE_TIM6 = 54,
    INTERRUPT_SOURCE_TIM7 = 55,
    INTERRUPT_SOURCE_TIM8 = 44,
    INTERRUPT_SOURCE_TIM15 = 24,
    INTERRUPT_SOURCE_TIM1_TIM16 = 25,
    INTERRUPT_SOURCE_TIM17 = 26,
};

typedef enum _nvic_interrupt_sources nvic_source_t;

/* Interrupt initialization functions*/

/**
 * @brief Enable interrupt module
 *
 */
void interrupts_init(void);

/**
 * @brief Enable interrupt handlers
 *
 */
void interrupts_global_enable(void);

/**
 * @brief Disable interrupt handlers
 *
 */
void interrupts_global_disable(void);

/**
 * @brief Enable interrupt #source
 *
 */
void interrupts_enable_source(nvic_source_t source);

/**
 * @brief Disable interrupt #source
 *
 */
void interrupts_disable_source(nvic_source_t source);

/**
 * @brief Register a callback for the given interrupt source
 *
 */
void interrupts_register_handler(nvic_source_t source, callbackfn_t fn);
