//#define F_CPU 16000000UL

#include "avr_helper.h"

#ifndef IS_X86
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "communication.h"
#include "spi.h"
#endif

#include "macros.h"

#include "uart_lib.h"
#include "servo.h"
#include "gangstil.h"

#include <stdint.h>
#include <stdlib.h>


#ifndef IS_X86
void build_spi_reply_frame(Frame *frame_trans);
void handle_spi_frame(Frame *frame_recv);

// If SPI receive something
ISR(SPI_STC_vect) {

    Frame frame_recv;
	on_spi_recv(&frame_recv);

	uint8_t current_msg = get_id(&frame_recv);

	if(current_msg == DATA_REQUEST) {

		Frame frame_trans;
		build_spi_reply_frame(&frame_trans);
		send_frame(&frame_trans);
	} else {

		handle_spi_frame(&frame_recv);
	}
}
#endif

int main(void)
{
	// Enable global interrupts and init spi communication
#ifndef IS_X86
	spi_init();
	sei();
#endif

	set_ddr(DDRD, 0xfE);
	
	usart_init();
	
	_delay_ms(100);
	
	init_all_servos();

	send_servo_action();

	//Initialize all legs
	Point2D current_leg_positions[6];
	for(size_t i = 0; i < 6; i++)
	{
		Point2D* current = get_default_leg_position(i);
		current_leg_positions[i] = *current;

		free(current);
	}
	assume_standardized_stance(current_leg_positions);

	Point2D new_point;
	
	new_point.x = 1;
	new_point.y = 0;
	
	//work_towards_goal(0, &new_point, current_leg_positions);
		
	while(1)
	{
	}
}

#ifndef IS_X86
void build_spi_reply_frame(Frame *frame_trans) {

	switch(frame_trans->msg[0]) {
		case SERVO_STATUS :
			// Send servo status data
			frame_trans->control_byte = SERVO_STATUS << 2;
			frame_trans->len = 0x05; // SET LEN OF MSG
			frame_trans->msg[0] = 0x00; // SET MSG
			break;
		case DEBUG_STRING :
			// Send servo status data
			frame_trans->control_byte = DEBUG_STRING << 2;
			frame_trans->len = 0x05; // SET LEN OF MSG
			frame_trans->msg[0] = 0x00; // SET MSG
			break;
		case OBSTACLE :
			// Send servo status data
			frame_trans->control_byte = OBSTACLE << 2;
			frame_trans->len = 0x00; 
			frame_trans->msg[0] = 0x00; // SET MSG
			break;
	}
}

void handle_spi_frame(Frame *frame_recv) {

	switch(get_id(frame_recv)){
		case TOGGLE_OBSTACLE :
			// Toggle obstacle
			//uint8_t on_off = frame_recv->msg[0];
			break;
		case SET_SERVO_SPEED :
			// Set speed
			//uint16_t servo_speed = ((uint16_t) frame_recv->msg[1] << 8) | (uint16_t) frame_recv->msg[0];
			break;
		case WALK_COMMAMD :
			// Go command 
			//uint8_t len = frame_recv->msg[0];
			//uint8_t x_speed = frame_recv->msg[1];
			//uint8_t y_speed = frame_recv->msg[2];
			//uint8_t turn_speed = frame_recv->msg[3];
			break;
		case RETURN_TO_NEUTRAL :
			// Return to neutral
			break;
	}
}
#endif
