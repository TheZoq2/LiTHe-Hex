import sys
import math

input_file = "/tmp/hexsim/sensors"
output_file = "/tmp/hexsim/command"

ANGLE = 0
FRONT_RIGHT_SENSOR = 1
BACK_RIGHT_SENSOR = 2
FRONT_LEFT_SENSOR = 3
BACK_LEFT_SENSOR = 4



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
        offset = (0.8 * (sensor_data[FRONT_LEFT_SENSOR] + 0.1)/(sensor_data[FRONT_RIGHT_SENSOR] + 0.2 + sensor_data[FRONT_LEFT_SENSOR])) - 0.4

        command_y = (sensor_data[FRONT_RIGHT_SENSOR] - sensor_data[FRONT_LEFT_SENSOR])/10

        if abs(offset) > 0.1:
            goal_angle = -sensor_data[ANGLE] - math.pi * offset / 2
        else:
            goal_angle = -sensor_data[ANGLE]

        command = "0.1," + str(command_y) + "," + str(goal_angle)
        write_output_command(command)

main()
