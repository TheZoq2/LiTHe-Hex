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
	
	reset_servo_max_angles(sevo_id);
	enable_servo_torque(sevo_id);
	set_servo_angle(sevo_id, 0x01ff);
		
	while(1)
	{
		
		//_delay_ms(100);
	}
}

