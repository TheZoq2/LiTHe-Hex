import sys
import math

input_file = "/tmp/hexsim/sensors"
output_file = "/tmp/hexsim/command"

CORRIDOR_WIDTH = 0.8
SENSOR_OFFSET = 0.1
ANGLE_SCALEDOWN = 0.5
MOVEMENT_SCALEDOWN = 0.1
ANGULAR_ADJUSTMENT_BORDER = 0.1
BASE_MOVEMENT = "0.1" #placeholder forward movement.


def write_output_command(command):
    """
    Writes the command to the output file
    """
    with open(output_file, 'w') as txt:
        txt.write(command)

def reglate(sensor_data, corridor_data):

   # offset for the robot length from mid in a corridor
   offset = (CORRIDOR_WIDTH * (sensor_data.ir_front_left + SENSOR_OFFSET)/(sensor_data.ir_front_right + (2 * SENSOR_OFFSET) + sensor_data.ir_front_left)) - (CORRIDOR_WIDTH/2)

   command_y = (sensor_data.ir_front_right - sensor_data.ir_front_left) * MOVEMENT_SCALEDOWN

   if abs(offset) > ANGULAR_ADJUSTMENT_BORDER:
       goal_angle = -corridor_data.corr_angle - math.pi * offset * ANGLE_SCALEDOWN
   else:
       goal_angle = -corridor_data.corr_angle

   command = BASE_MOVEMENT + "," + str(command_y) + "," + str(goal_angle)
   print(command)

