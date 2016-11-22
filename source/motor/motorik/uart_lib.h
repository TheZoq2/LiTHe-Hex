#ifndef H_UART_LIB
#define H_UART_LIB

#include "avr_helper.h"

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

#include <stdint.h>

void usart_init();
void uart_wait();
void usart_transmit(uint8_t data);
uint8_t usart_receive();

#endif
