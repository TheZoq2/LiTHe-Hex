import sys
import math

input_file = "/tmp/decisions/sensors"
output_file = "/tmp/decisions/desicion"

def get_sensor_data():
    with open(input_file, 'r') as data:
        file_content = data.read()
        while len(file_content) == 0:
            file_content = data.read()
        split_content = file_content.split(',')

        return (
            float(split_content[0]), # forward
            float(split_content[1]), # left front
            float(split_content[2]), # left back
            float(split_content[3]), # right front
            float(split_content[4])  # right back
        )


def write_output_command(command):
    with open(output_file, 'w') as txt:
        txt.write(command)


def get_possible_dead_ends(sensor_data):
    dead_ends = [0,0,0]
    if (sensor_data[0] <= 1): # dead-end forward
        dead_ends[0] = 1

    if (sensor_data[1] <= 1): # front left sensor < 1m
        if (sensor_data[2] < 1): # back left sensor < 1m
            dead_ends[1] = 1
        else:
            print("Maybe dead-end left")

    if (sensor_data[3] <= 1): #front right sensor < 1m
        if (sensor_data[4] <= 1): # back right sensor < 1m
            dead_ends[2] = 1
        else:
            print("Maybe dead-end left")

    return dead_ends


def main():
    while 1:
        sensor_data = get_sensor_data()

        possible_corridors = get_possible_dead_ends(sensor_data)

        print(possible_corridors)

main()
