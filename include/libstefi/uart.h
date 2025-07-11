#pragma once

#include <stddef.h>
#include <stdint.h>

void uart_configure();

void uart_poll_out(const char *buf, uint32_t len);
unsigned char uart_poll_in();
void uart_putchar(char ch);
char uart_getchar();




