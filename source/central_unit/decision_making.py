import sys
import math
import communication

# Types of corridors
CORRIDOR = 0
DEAD_END = 1
MAYBE_CORRIDOR = 2 # when front sensor has detected a corridor but back has yet not

# Commands
GO_FORWARD = 0
TURN_LEFT = 1
TURN_RIGHT = 2
STOP = 3
CLIMB_OBSTACLE = 4
COMPLETE_TURN = 5 # After a turn we want the robot to walk forward to enter corridor

# General directions
FRONT = 0
LEFT = 1
RIGHT = 2

# Distances to different objects in meters
DEAD_END_DISTANCE = 1
DISTANCE_TO_OBSTACLE = 0.0

# Distance between the sensors in the mount on same side
DISTANCE_BETWEEN_SENSORS = 0.16

previous_decision = GO_FORWARD


# Returns the dead ends and corridors detected in the maze
def _get_corridors_and_dead_ends(sensor_data):
    corridors_and_dead_ends = [DEAD_END, DEAD_END, DEAD_END]

    if (sensor_data.lidar >= DEAD_END_DISTANCE):
        corridors_and_dead_ends[FRONT] = CORRIDOR
    else:
        corridors_and_dead_ends[FRONT] = DEAD_END

    if (sensor_data.ir_front_left >= DEAD_END_DISTANCE):
        # Front left sensor detects a long distance to wall
        corridors_and_dead_ends[LEFT] = MAYBE_CORRIDOR

        # Check if back sensor also detects long distance
        if (sensor_data.ir_back_left >= DEAD_END_DISTANCE and
            corridors_and_dead_ends[LEFT] == MAYBE_CORRIDOR):
            corridors_and_dead_ends[LEFT] = CORRIDOR

    else:
        corridors_and_dead_ends[LEFT] = DEAD_END

    if (sensor_data.ir_front_right >= DEAD_END_DISTANCE):
        # Front right sensor detects long distance to wall
        corridors_and_dead_ends[RIGHT] = MAYBE_CORRIDOR

        # Check if back sensor also detects long distance
        if (sensor_data.ir_back_right >= DEAD_END_DISTANCE and
            corridors_and_dead_ends[RIGHT] == MAYBE_CORRIDOR):
            corridors_and_dead_ends[RIGHT] = CORRIDOR

    else:
        corridors_and_dead_ends[RIGHT] = DEAD_END

    print(corridors_and_dead_ends)
    return corridors_and_dead_ends


# Returns whether an obstacle has been detected or not
def _found_obstacle(sensor_data):
    obstacle_found = False
    if (sensor_data.ir_down <= DISTANCE_TO_OBSTACLE):
        obstacle_found = True
    else:
        obstacle_found = False
    return obstacle_found


# Checks if the expected path exists or not
def _expected_path(corridors_and_dead_ends, front, left, right):
    if (corridors_and_dead_ends[FRONT] == front and
        corridors_and_dead_ends[LEFT] == left and
        corridors_and_dead_ends[RIGHT] == right):
        return True
    else:
        return False


# Checks if all the sidesensors give small values
# TODO: Tweak this and remove magic constant!
def _is_inside_corridor(sensor_data):
    if (sensor_data.ir_front_left <= 0.5 and
        sensor_data.ir_back_left <= 0.5 and
        sensor_data.ir_front_right <= 0.5 and
        sensor_data.ir_back_right <= 0.5):
        return True
    else:
        return False


# Returns the decision made based on the dead ends and corridors detected
def get_decision(sensor_data):
    corridors_and_dead_ends = _get_corridors_and_dead_ends(sensor_data)

    # Robot will always move forward until it detects a dead end forward
    decision = GO_FORWARD;

    if (_found_obstacle(sensor_data)):
        decision = CLIMB_OBSTACLE

    else:

        for value in corridors_and_dead_ends:
            # If more than one corridor to choose from
            if (corridors_and_dead_ends.count(CORRIDOR) > 1):
                print("Maze too complicated")

            else:
                if (_expected_path(corridors_and_dead_ends, DEAD_END, CORRIDOR, DEAD_END)):
                    decision = TURN_LEFT

                elif (_expected_path(corridors_and_dead_ends, DEAD_END, DEAD_END, CORRIDOR)):
                    decision = TURN_RIGHT

                elif (_expected_path(corridors_and_dead_ends, DEAD_END, DEAD_END, DEAD_END)):
                    decision = STOP

                else:
                    decision = GO_FORWARD

    # Check if previous decision was to make a turn.
    # If it was we need to let the robot make a full turn before using
    # the sensor data because they will give bad values during a turn.
    if (previous_decision == TURN_LEFT):
        print("Robot is turning left!")

        # After the robot has started turning the angle will be
        # larger than 5 (0 ideally), so we don't make new decisions until
        # the robot is back at straight angle.
        if (sensor_data.right_angle <= 5): #TODO: test and tweak this
            decision = GO_FORWARD
            previous_decision = COMPLETE_TURN
            print("Turning left complete.")

    elif (previous_decision == TURN_RIGHT):
        print("Robot is turning left!")

        if (sensor_data.left_angle <= 5): #TODO: test and tweak this
            decision = GO_FORWARD
            previous_decision = COMPLETE_TURN
            print("Turning right complete.")

    # When the robot has rotated but yet not entered the new corridor
    elif (previous_decision == COMPLETE_TURN):
        decision = GO_FORWARD
        previous_decision = COMPLETE_TURN

        if (_is_inside_corridor(sensor_data)):
            previous_decision = GO_FORWARD


    return decision
