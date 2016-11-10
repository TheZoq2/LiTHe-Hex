import sys
import math

input_file = "/tmp/hexsim/sensors"
output_file = "/tmp/hexsim/command"

ANGLE = 0
FRONT_RIGHT_SENSOR = 1
BACK_RIGHT_SENSOR = 2
FRONT_LEFT_SENSOR = 3
BACK_LEFT_SENSOR = 4



CORRIDOR_WIDTH = 0.8
SENSOR_OFFSET = 0.1
ANGLE_SCALEDOWN = 0.5
MOVEMENT_SCALEDOWN = 0.1
ANGULAR_ADJUSTMENT_BORDER = 0.1
BASE_MOVEMENT = "0.1" #placeholder forward movement.

def get_sensor_data():
    """
    Returns the fetched sensor data from the input file
    """
    with open(input_file, 'r') as txt:
        file_content = txt.read()
        while len(file_content) == 0:
            file_content = txt.read()
        split_content = file_content.split(",")

        return (
            float(split_content[ANGLE]),
            float(split_content[FRONT_RIGHT_SENSOR]),
            float(split_content[BACK_RIGHT_SENSOR]),
            float(split_content[FRONT_LEFT_SENSOR]),
            float(split_content[BACK_LEFT_SENSOR])
        )

def write_output_command(command):
    """
    Writes the command to the output file
    """
    with open(output_file, 'w') as txt:
        txt.write(command)

def main():
    while 1:
        sensor_data = get_sensor_data()

        # offset for the robot length from mid in a corridor
        offset = (CORRIDOR_WIDTH * (sensor_data[FRONT_LEFT_SENSOR] + SENSOR_OFFSET)/(sensor_data[FRONT_RIGHT_SENSOR] + (2 * SENSOR_OFFSET) + sensor_data[FRONT_LEFT_SENSOR])) - (CORRIDOR_WIDTH/2)

        command_y = (sensor_data[FRONT_RIGHT_SENSOR] - sensor_data[FRONT_LEFT_SENSOR]) * MOVEMENT_SCALEDOWN

        if abs(offset) > ANGULAR_ADJUSTMENT_BORDER:
            goal_angle = -sensor_data[ANGLE] - math.pi * offset * ANGLE_SCALEDOWN
        else:
            goal_angle = -sensor_data[ANGLE]

        command = BASE_MOVEMENT + "," + str(command_y) + "," + str(goal_angle)
        write_output_command(command)

main()
