import sys
import math

input_file = "/tmp/decisions/sensors"
output_file = "/tmp/decisions/desicion"

FRONT_SENSOR = 0
FRONT_LEFT_SENSOR = 1
BACK_LEFT_SENSOR = 2
FRONT_RIGHT_SENSOR = 3
BACK_RIGHT_SENSOR = 4

CORRIDOR = 0
DEAD_END = 1

GO_FORWARD = 0
TURN_LEFT = 1
TURN_RIGHT = 2

FRONT = 0
LEFT = 1
RIGHT = 2

def get_sensor_data():
    with open(input_file, 'r') as data:
        file_content = data.read()
        while len(file_content) == 0:
            file_content = data.read()
        split_content = file_content.split(',')

        return (
            float(split_content[FRONT_SENSOR]),
            float(split_content[FRONT_LEFT_SENSOR]),
            float(split_content[BACK_LEFT_SENSOR]),
            float(split_content[FRONT_RIGHT_SENSOR]),
            float(split_content[BACK_RIGHT_SENSOR])
        )


def write_output_command(command):
    with open(output_file, 'w') as txt:
        txt.write(command)


def get_corridors_and_dead_ends(sensor_data):
    corridors_and_dead_ends = [0,0,0]

    if (sensor_data[FRONT_SENSOR] <= 1): # dead-end forward
        corridors_and_dead_ends[0] = DEAD_END
    else:
        corridors_and_dead_ends[0] = CORRIDOR

    if (sensor_data[FRONT_LEFT_SENSOR] <= 1):
        if (sensor_data[BACK_LEFT_SENSOR] < 1):
            corridors_and_dead_ends[1] = DEAD_END
        else:
            print("Maybe dead-end left")
    else:
        corridors_and_dead_ends[1] = CORRIDOR

    if (sensor_data[FRONT_RIGHT_SENSOR] <= 1): #front right sensor < 1m
        if (sensor_data[BACK_RIGHT_SENSOR] <= 1): # back right sensor < 1m
            corridors_and_dead_ends[2] = DEAD_END
        else:
            print("Maybe dead-end left")
    else:
        corridors_and_dead_ends[2] = CORRIDOR

    return corridors_and_dead_ends


def decision(corridors_and_dead_ends):
    decision = GO_FORWARD;

    for value in corridors_and_dead_ends:
        if ((corridors_and_dead_ends[LEFT] == CORRIDOR) and
            corridors_and_dead_ends[RIGHT] == DEAD_END and
            corridors_and_dead_ends[FRONT] == DEAD_END):
            decision = TURN_LEFT
        if (corridors_and_dead_ends[RIGHT] == CORRIDOR):
            decision = TURN_RIGHT



def main():
    while 1:
        sensor_data = get_sensor_data()

        corridors_and_dead_ends = get_corridors_and_dead_ends(sensor_data)

        print(corridors_and_dead_ends)

main()
