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
	
	
	
	//reset_servo_bounds(0);
	//enable_servo_torque(0);
	//set_servo_angle(0, 0x0200);
	init_all_servos();

	send_servo_action();

	//uint16_t angles[3] = {0x1ff, 0x1ff, 0x1ff};
	//set_leg_angles(LEFT_FRONT, angles);
		
	while(1)
	{
		
		//_delay_ms(100);
	}
}

