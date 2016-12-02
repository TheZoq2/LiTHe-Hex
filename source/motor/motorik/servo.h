#ifndef H_SERVO
#define H_SERVO

#include <stdint.h>
#include <stdlib.h>
#include "uart_lib.h"

#include "macros.h"


#define DD_MOSI 6

//0 Indexed
#define PIN_RX_TOGGLE 2

#define DIRECTION_TX 1
#define DIRECTION_RX 0

extern const uint8_t WRITE_DATA_INSTRUCTION;
extern const uint8_t WRITE_REG_INSTRUCTIO;
extern const uint8_t ACTION_INSTRUCTION;


extern const uint8_t ANGLE_LIMIT_ADDRESS;
extern const uint8_t TORQUE_ENABLE_ADDRESS;
extern const uint8_t GOAL_POSITION_ADDRESS;

enum LegIds
{
	LEFT_FRONT = 0,
	LEFT_MID = 1,
	LEFT_BACK = 2,
	RIGHT_FRONT = 3,
	RIGHT_MID = 4,
	RIGHT_BACK = 5
};

extern const uint8_t SERVO_MAP[6][3];

extern const uint8_t BROADCAST_ID;

/*
	Struct that contains a response from a servo. 

	Remember to free the data pointed to by the parameters
	pointer when an instance goes out of scope.
 */
typedef struct
{
	uint8_t id;
	uint8_t length;
	uint8_t error;

	uint8_t parameter_amount;
	uint8_t* parameters;

	uint8_t checksum;
} ServoReply;

/*
	Receives a reply from the servo. Remember to free the reply once done with it
 */
ServoReply receive_servo_reply();

void free_servo_reply(ServoReply reply);



/*
	Sends a command to the servo.

	ID is the ID of the servo to send the command to. Set to BROADCAST_ID to
		send the message to all servos

	Instruction is the instruction ID that should be executed by the servo

	The data pointer should point to a sequence of bytes to send to the 
		server. The length of that data should be data_amount
 */
void send_servo_command(
		uint8_t id, uint8_t instruction, void* data, uint8_t data_amount);

/*
	Writes data to the servo control table. This is done using the WRITE_REG
	instruction which means that an 'action' instruction needs to be sent
	to  the servo in order to use the new value.

	ID is the ID of the servo to send the command to. Set to BROADCAST_ID to
		send the message to all servos
	
	Address is the address in the control table to write to. A list of addresses
	can be seen in the AX-12 datasheet on page 13
 */
void write_servo_data(
		uint8_t id, uint8_t address, const uint8_t* data, uint8_t data_amount);

/*
	Writes a single byte to a servo. See write_servo_command for details
 */
void write_servo_single_byte(uint8_t id, uint8_t address, uint8_t value);


/*Resets the min and max angles of a servo*/
void reset_servo_bounds(uint8_t id);
/*Sets torque_enable to 1 on a servo*/
void enable_servo_torque(uint8_t id);
void disable_servo_torque(uint8_t id);

/*
	Sets  the angle of a specific servo. The angles are defined in the
 	ax-12 datasheet on page 17
*/
void set_servo_angle(uint8_t id, uint16_t angle);
/*
	Sends the 'action' instruction to all servos which makes them perform
	previously sent commads
*/
void send_servo_action();

/*
	Sets default values, disables replies and enables  torque on all servos
 */
void init_all_servos();

/*
	Sets the angle of the 3 limbs on a specific leg.

	One of the elements in the LEG struct. 

	Angles should be a  list of 3 unsigned angle values where
	the first angle is the desired angle of the limb closest
	to  the body. See set_servo_angle for details.
 */
void set_leg_angles(enum LegIds leg_index, uint16_t* angles);

/*
	Sends a read request to a servo and returns the response
*/
ServoReply read_servo_data(uint8_t id, uint8_t address, uint8_t length);



#endif
