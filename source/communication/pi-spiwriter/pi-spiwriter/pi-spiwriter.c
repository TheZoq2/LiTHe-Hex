/*
 * pi_spiwriter.c
 *
 * Created: 11/14/2016 3:45:16 PM
 *  Author: robsl733
 */


#include <avr/io.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "communication.h"

#define ACK 0x7E
volatile uint16_t data;

ISR(SPI_STC_vect)
{
  on_spi_recv();
}

int main(void)
{
	spi_init();
	sei();

    while(1)
    {
      int a = 0;
    }
}
