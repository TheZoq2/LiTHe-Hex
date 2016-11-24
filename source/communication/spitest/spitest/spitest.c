/*
 * spitest.c
 *
 * Created: 11/14/2016 3:45:16 PM
 *  Author: robsl733
 */


#include "communication.h"
#include <avr/interrupt.h>

volatile uint8_t data1;
volatile uint8_t data2;

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
