import sys
import math

input_file = "/tmp/hexsim/sensors"
utput_file = "/tmp/hexsim/command"

def get_sensor_data():
    """
    Returns the fetched sensor data from the input file
    """
    #TODO: There are 5 values in the input file. Make them 3 and fix the indexes here.
    with open(input_file, 'r') as txt:
        file_content = txt.read()
        while len(file_content) == 0:
            file_content = txt.read()
        split_content = file_content.split(",")

        return (
            float(split_content[0]),
            float(split_content[1]),
            float(split_content[3])
        )

def write_utput_command(command):
    """
    Writes the command to the utput file
    """
    with open(utput_file, 'w') as txt:
        txt.write(command)

def main():
    while 1:
        sensor_data = get_sensor_data()

        # offset for the robot length from mid in a corridor
        offset = (0.8 * (sensor_data[2] + 0.1)/(sensor_data[1] + 0.2 + sensor_data[2])) - 0.4

        command_y = (sensor_data[1] - sensor_data[2])/10

        if abs(offset) > 0.1:
            goal_angle = -sensor_data[0] - math.pi * offset / 2
        else:
            goal_angle = -sensor_data[0]

        command = "0.1," + str(command_y) + "," + str(goal_angle)
        write_utput_command(command)

main()
