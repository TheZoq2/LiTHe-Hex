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
MAYBE_CORRIDOR = 2

GO_FORWARD = 0
TURN_LEFT = 1
TURN_RIGHT = 2
STOP = 3

FRONT = 0
LEFT = 1
RIGHT = 2

DEAD_END_DISTANCE = 1 # meters

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
    """
    Returns the dead ends and corridors detected in the maze
    """
    corridors_and_dead_ends = [DEAD_END, DEAD_END, DEAD_END]

    if (sensor_data[FRONT_SENSOR] >= DEAD_END_DISTANCE):
        corridors_and_dead_ends[FRONT] = CORRIDOR
    else:
        corridors_and_dead_ends[FRONT] = DEAD_END

    if (sensor_data[FRONT_LEFT_SENSOR] >= DEAD_END_DISTANCE):
        # front left sensor detects a long distance to wall
        corridors_and_dead_ends[LEFT] = MAYBE_CORRIDOR

        # check if back sensor also detects long distance
        if (sensor_data[BACK_LEFT_SENSOR] >= DEAD_END_DISTANCE and
            corridors_and_dead_ends[LEFT] == MAYBE_CORRIDOR):
            corridors_and_dead_ends[LEFT] = CORRIDOR

    else:
        corridors_and_dead_ends[LEFT] = DEAD_END

    if (sensor_data[FRONT_RIGHT_SENSOR] >= DEAD_END_DISTANCE):
        # front right sensor detects long distance to wall
        corridors_and_dead_ends[RIGHT] = MAYBE_CORRIDOR

        # check if back sensor also detects long distance
        if (sensor_data[BACK_RIGHT_SENSOR] >= DEAD_END_DISTANCE and
            corridors_and_dead_ends[RIGHT] == MAYBE_CORRIDOR):
            corridors_and_dead_ends[RIGHT] = CORRIDOT

    else:
        corridors_and_dead_ends[RIGHT] = DEAD_END

    return corridors_and_dead_ends


def get_decision(corridors_and_dead_ends):
    """
    Returns the decision made based on the dead ends and corridors detected
    """
    # robot will always move forward until it detects a dead end forward
    decision = GO_FORWARD;

    for value in corridors_and_dead_ends:
        # if more than one corridor to choose from
        if (corridors_and_dead_ends.count(0) > 1):
            print("Maze too complicated")

        if ((corridors_and_dead_ends[LEFT] == CORRIDOR) and
            corridors_and_dead_ends[RIGHT] == DEAD_END and
            corridors_and_dead_ends[FRONT] == DEAD_END):
            decision = TURN_LEFT

        elif (corridors_and_dead_ends[RIGHT] == CORRIDOR and
            corridors_and_dead_ends[LEFT] == DEAD_END and
            corridors_and_dead_ends[FRONT] == DEAD_END):
            decision = TURN_RIGHT

        elif (corridors_and_dead_ends[RIGHT] == DEAD_END and
            corridors_and_dead_ends[LEFT] == DEAD_END and
            corridors_and_dead_ends[FRONT] == DEAD_END):
            decision = STOP

        else:
            decision = GO_FORWARD

    return decision


def main():
    while 1:
        sensor_data = get_sensor_data()
        corridors_and_dead_ends = get_corridors_and_dead_ends(sensor_data)
        decision = get_decision(corridors_and_dead_ends)

        print(decision)

main()
