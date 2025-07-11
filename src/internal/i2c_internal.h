#pragma once

typedef struct
{
    volatile uint32_t CR1;         /*!< I2C Control register 1,            Address offset: 0x00 */
    volatile uint32_t CR2;         /*!< I2C Control register 2,            Address offset: 0x04 */
    volatile uint32_t OAR1;        /*!< I2C Own address 1 register,        Address offset: 0x08 */
    volatile uint32_t OAR2;        /*!< I2C Own address 2 register,        Address offset: 0x0C */
    volatile uint32_t TIMINGR;     /*!< I2C Timing register,               Address offset: 0x10 */
    volatile uint32_t TIMEOUTR;    /*!< I2C Timeout register,              Address offset: 0x14 */
    volatile uint32_t ISR;         /*!< I2C Interrupt and status register, Address offset: 0x18 */
    volatile uint32_t ICR;         /*!< I2C Interrupt clear register,      Address offset: 0x1C */
    volatile uint32_t PECR;        /*!< I2C PEC register,                  Address offset: 0x20 */
    volatile uint32_t RXDR;        /*!< I2C Receive data register,         Address offset: 0x24 */
    volatile uint32_t TXDR;        /*!< I2C Transmit data register,        Address offset: 0x28 */
} I2C_typeDef;

#define I2C_BASE (0x40005400)
#define I2C_PORTOFFSET (0x400)

#define I2C1 ((I2C_typeDef *) 0x40005400)
#define I2C2 ((I2C_typeDef *) 0x40005800)
#define I2C3 ((I2C_typeDef *) 0x40005C00)

#define I2C_ISR_BUSY BIT(15)
#define I2C_CR2_AUTOEND BIT(25)
#define I2C_CR2_START BIT(13)
#define I2C_ISR_TXE BIT(0)
#define I2C_ISR_STOPF BIT(5)
