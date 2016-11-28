//#define F_CPU 16000000UL

#include "avr_helper.h"

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#endif

#include "macros.h"

#include "uart_lib.h"
#include "servo.h"

#include <stdint.h>
#include <stdlib.h>



int main(void)
{
	//spi_slave_init();
	

	set_ddr(DDRD, 0xfE);
	//
	usart_init();
	
	_delay_ms(100);
	
	
	uint8_t sevo_id = 13;
	
	init_all_servos();

	send_servo_action();
	
	uint16_t angles[3] = {0x1ff, 0x1ff, 0x1ff};
	for(uint8_t i = 0; i < 6; ++i)
	{
		set_leg_angles(i, angles);
	}
	
	send_servo_action();
	
	//uint16_t angles_1[3] = {0x0ff, 0x144, 0x0ff};
	//uint16_t angles_2[3] = {0x1ff, 0x2CC, 0x1ff};
		
	while(1)
	{

	}
}

