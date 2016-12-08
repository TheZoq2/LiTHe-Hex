#include "spi.h"
#include <stdio.h>

void spi_init()
{

}
uint8_t spi_receive_byte()
{
	return 0;
}

uint8_t spi_transmit_byte(uint8_t data)
{
	return 0;
}
void spi_transmit_ack()
{

}
void spi_transmit_fail()
{

}
void spi_set_interrupts(bool value)
{
	printf("Setting spi interrupts to %i", value);
}
