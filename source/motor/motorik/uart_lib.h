#ifndef H_UART_LIB
#define H_UART_LIB

#include "avr_helper.h"

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

#include <stdint.h>

enum UsartDirection
{
	RX,
	TX
};

enum UartError
{
	Ok,
	Timeout
};

typedef struct UartResult
{
	uint8_t value;
	enum UartError error;
} UartResult;

void usart_init();
void uart_wait();
void usart_transmit(uint8_t data);
UartResult usart_receive();
void usart_set_direction(enum UsartDirection direction);
void clear_uart_buffer();

#endif
