/*
 * pi_spiwriter.c
 *
 * Created: 11/14/2016 3:45:16 PM
 *  Author: robsl733
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

#define ACK 0x7E

void SPI_SlaveInit(void)
{
	/*Set MISO output, all others input*/
	DDRB = (1<<DDB6);
	/*Enable SPI*/
	SPCR = (1<<SPE) | (1<<SPIE);
}

uint16_t SPI_SlaveRecieve(uint16_t data)
{
	SPDR = data;
	/*Wait for reception complete*/
	while(!(SPSR) & (1<<SPIF)){}
		
	/*Return Data Register*/
	return SPDR;
}
int main(void)
{
	//SPIMasterInit();
	SPI_SlaveInit();
	uint16_t data;
	
    while(1)
    {
		data = SPI_SlaveRecieve(ACK);
		data |= (SPI_SlaveRecieve(ACK)<<8);
    }
}