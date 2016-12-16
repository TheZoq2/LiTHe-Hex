import sys
import math
import communication.avr_communication as avr_communication
import time

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
MAZE_TOO_COMPLICATED = 6 # Can not take decision

# General directions
FRONT = 0
LEFT = 1
RIGHT = 2

# Angle
ANGLE_10_DEGREE = 10

TILE_SIZE = 0.8

# Distances to different objects in meters
DEAD_END_DISTANCE = 1.35
LIDAR_DEAD_END_DISTANCE = 1.08
LIDAR_STOP_DISTANCE = 0.35
DISTANCE_TO_OBSTACLE = 0.0
DISTANCE_TO_WALL_IN_CORRIDOR = 0.5
FAULTY_LIDAR_DISTANCE = 0.05
ANGLE_SCALEDOWN = 0.5
MOVEMENT_SCALEDOWN = 5
ANGLE_THRESHOLD = 4

AUTONOMOUS_SPEED = 0x00FF


class DecisionPacket():
    def __init__(self):
        self.decision = GO_FORWARD
        self.decisions = [GO_FORWARD, GO_FORWARD, GO_FORWARD]
        self.previous_decision = GO_FORWARD
        self.speed = AUTONOMOUS_SPEED
        self.regulate_base_movement = 0;
        self.regulate_command_y = 0;
        self.regulate_goal_angle = 0;
        self.regulate_angle_scaledown = ANGLE_SCALEDOWN
        self.regulate_movement_scaledown = MOVEMENT_SCALEDOWN
        self.regulate_angular_adjustment_border = ANGLE_THRESHOLD


# Returns the dead ends and corridors detected in the maze
def _get_corridors_and_dead_ends(sensor_data):
    corridors_and_dead_ends = [DEAD_END, DEAD_END, DEAD_END]

    if (sensor_data.lidar >= LIDAR_DEAD_END_DISTANCE or sensor_data.lidar <= FAULTY_LIDAR_DISTANCE):
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
    if (sensor_data.ir_down <= DISTANCE_TO_OBSTACLE and
        sensor_data.lidar >= LIDAR_DEAD_END_DISTANCE):
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
def _is_inside_corridor(sensor_data):
    if (sensor_data.ir_front_left <= DISTANCE_TO_WALL_IN_CORRIDOR and
        sensor_data.ir_back_left <= DISTANCE_TO_WALL_IN_CORRIDOR and
        sensor_data.ir_front_right <= DISTANCE_TO_WALL_IN_CORRIDOR and
        sensor_data.ir_back_right <= DISTANCE_TO_WALL_IN_CORRIDOR):
        return True
    else:
        return False


# Returns the decision made based on the dead ends and corridors detected
def get_decision(sensor_data, decision_packet, motor_spi):
    corridors_and_dead_ends = _get_corridors_and_dead_ends(sensor_data)

    # Robot will always move forward until it detects a dead end forward
    decision_packet.decisions[0] = GO_FORWARD;

    if (_found_obstacle(sensor_data)):
        if (corridors_and_dead_ends[LEFT] == CORRIDOR):
            decision_packet.decisions[0] = TURN_LEFT
        elif (corridors_and_dead_ends[RIGHT] == CORRIDOR):
            decision_packet.decisiona[0] == TURN_RIGHT
        else:
            decision_packet.decisions[0] = CLIMB_OBSTACLE

    else:

        for value in corridors_and_dead_ends:
            # If more than one corridor to choose from
            if (corridors_and_dead_ends.count(CORRIDOR) == 3):
                decision_packet.decisions[0] = GO_FORWARD;

            elif (corridors_and_dead_ends.count(CORRIDOR) == 2):
                if (sensor_data.lidar > LIDAR_DEAD_END_DISTANCE):
                    decision_packet.decisions[0] = GO_FORWARD
                else:
                    if (corridors_and_dead_ends[LEFT] == CORRIDOR):
                        decision_packet.decisions[0] = TURN_LEFT
                    else:
                        decision_packet.decisions[0] = TURN_RIGHT

            elif (corridors_and_dead_ends.count(CORRIDOR) == 1):
                if (sensor_data.lidar < TILE_SIZE and sensor_data.lidar > LIDAR_STOP_DISTANCE):
                    decision_packet.decisions[0] = GO_FORWARD
                elif (corridors_and_dead_ends[LEFT] == CORRIDOR):
                    decision_packet.decisions[0] = TURN_LEFT
                elif (corridors_and_dead_ends[RIGHT] == CORRIDOR):
                    decision_packet.decisions[0] = TURN_RIGHT
                else:
                    decision_packet.decisions[0] = GO_FORWARD

            elif (corridors_and_dead_ends.count(CORRIDOR) == 0):
                if (sensor_data.lidar > LIDAR_STOP_DISTANCE):
                    decision_packet.decisions[0] = GO_FORWARD
                else:
                    decision_packet.decisions[0] = TURN_LEFT


    # Check if previous decision was to make a turn.
    # If it was we need to let the robot make a full turn before using
    # the sensor data because they will give bad values during a turn.
    if (decision_packet.previous_decision == TURN_LEFT or
        decision_packet.previous_decision == TURN_RIGHT):
        #print("Robot is turning left!")

        # After the robot has started turning the angle will be
        # larger than 5 (0 ideally), so we don't make new decisions until
        # the robot is back at straight angle and robot has stopped rotating.
        if (abs(sensor_data.average_angle) <= ANGLE_10_DEGREE and
            not avr_communication.is_busy_rotating(motor_spi, timeout=0)):
            decision_packet.decisions[0] = GO_FORWARD
            decision_packet.previous_decision = COMPLETE_TURN
            #print("Turning left complete.")

    # When the robot has rotated but yet not entered the new corridor
    elif (decision_packet.previous_decision == COMPLETE_TURN):
        decision_packet.decisions[0] = GO_FORWARD
        decision_packet.previous_decision = COMPLETE_TURN

        if (_is_inside_corridor(sensor_data)):
            decision_packet.previous_decision = GO_FORWARD
    
    average_decision(decision_packet)

def average_decision(decision_packet): 
    decision_packet.decisions[2] = decision_packet.decisions[1]
    decision_packet.decisions[1] = decision_packet.decisions[0]
    if (decision_packet.decisions[0] == decision_packet.decisions[1] and 
        decision_packet.decisions[0] == decision_packet.decisions[2]):
        
        decision_packet.decision = decision_packet.decisions[0]

def int_to_string_command(command):
    if (command == GO_FORWARD):
        return "GO_FORWARD"
    elif (command == TURN_LEFT):
        return "TURN_LEFT"
    elif (command == TURN_RIGHT):
        return "TURN_RIGHT"
    elif (command == STOP):
        return "STOP"
    elif (command == COMPLETE_TURN):
        return "COMPLETE_TURN"
    elif (command == MAZE_TOO_COMPLICATED):
        return "MAZE_TOO_COMPLICATED"
