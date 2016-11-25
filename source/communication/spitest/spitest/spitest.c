/*
 * spitest.c
 *
 * Created: 11/14/2016 3:45:16 PM
 *  Author: robsl733
 */


//#include "communication.h"
#include <avr/interrupt.h>

// volatile uint8_t data1;
// volatile uint8_t data2;
// 
// uint8_t if_len = 0;
// 
// uint8_t i = 0;
// 
// uint8_t SET_FLAG = 0;
// 
// typedef struct Frame {
// 	uint8_t control_byte;
// 	uint8_t len;
// 	uint8_t msg[];
// } Frame;
// 
// Frame frame;

ISR(SPI_STC_vect) {

	// if(!if_len) {
	// 	if(frame.control_byte > 0) {

	// 		frame.control_byte = spi_receive_byte();

	// 		if(frame.control_byte & 0x80) {
	// 			if_len = 1;
	// 		} else {
	// 			if_len = 0;
	// 		}
	// 	} else {
	// 		frame.msg[0] = spi_receive_byte();
	// 	}
	// } else {
	// 	if(!frame.len) {
	// 		frame.len = spi_receive_byte();

	// 	} else {
	// 		frame.msg[i] = spi_receive_byte();
	// 		i++;
	// 		if(i >= frame.len) {
	// 			i = 0;
	// 			if_len = 0;
	// 			SET_FLAG = 1;
	// 		}
	// 	}
	// }

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
