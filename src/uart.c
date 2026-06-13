/**********************************************************************/
/* UART/USART                 Hardware Abstraction layer              */
/**********************************************************************/
#include "libstefi/uart.h"
#include "libstefi/gpio.h"
#include "libstefi/peripheral.h"
#include "internal/uart_internal.h"
#include "internal/rcc_internal.h"
#include "libstefi/util.h"

//Default Uart2 on A2,A3
gpio_id_t port_uart_tx = A2;
gpio_id_t port_uart_rx = A3;
UART_typeDef *uart = USART2;

void uart_configure(){
    //UM2397: USART2 (PA2/PA3) connected to STLINK-V3E Virtual COM port.
    peripheral_uart_enable(PERIPHERAL_UART2);
    peripheral_gpio_enable(PERIPHERAL_GPIOA);
    gpio_set_alternate_function(port_uart_tx, AF7);
    gpio_set_alternate_function(port_uart_rx, AF7);
    gpio_set_mode(port_uart_tx, MODER_AF);
    gpio_set_mode(port_uart_rx, MODER_AF);

    /*
     * Clock USART2 from HSI16 (fixed 16 MHz RC) rather than PCLK1. PCLK1 is
     * only 4 MHz at the L476 MSI boot — which made BRR below ~4x too fast and
     * garbled all output — and it changes again after system_init(). Sourcing
     * the kernel clock from HSI16 makes the baud correct on both boards and
     * keeps printf working across clock changes. (CCIPR USART2SEL[3:2] = 0b10
     * is HSI16 on both L4 and G4.)
     */
    RCC->CR |= BIT(8);                  // HSION
    while (!(RCC->CR & BIT(10)));        // wait HSIRDY
    RCC->CCIPR &= ~(0x3u << 2);
    RCC->CCIPR |=  (0x2u << 2);          // USART2SEL = HSI16

    //BRR from a 16 MHz kernel clock; rounded (-> 139) for ~-0.08% baud error
    USART2->BRR = (16000000 + 115200/2) / 115200;
    USART2->CR1 |= (1 << 0) | (1 << 2) | (1 << 3); //uart, receive, Transmit Enable

    while (!(USART2->ISR & BIT(21)) || !(USART2->ISR & BIT(22)));
}

void uart_poll_out(const char *buf, uint32_t len)
{
    uint32_t i = 0;
    //Check Data Register Empty
    while ((uart->ISR & BIT(7)) && (i < len))
    {
        uart->TDR  = buf[i++];
        while (!(uart->ISR & BIT(6))); //Check Transmission Complete
    }
}

void uart_putchar(char ch) {
    uart_poll_out(&ch, 1);
}

char uart_getchar() {
    return uart_poll_in();
}

//This is blocking and not polling, change name
unsigned char uart_poll_in()
{
    /* Check if uart is initialized */
    while ( (uart->ISR & (1 << 5)) != (1 << 5));
    return (unsigned char) (uart->RDR & 0xFF);
}

