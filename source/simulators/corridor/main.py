from vec import Vec2, vec2_from_direction

import sys
#from sdl2.ext import *
import sdl2.ext as sdl
import sdl2
import math
import time
import os
import pdb


command_filename = "/tmp/hexsim/command"
START_X_POS = -0.3
START_Y_POS = 1
START_ANGLE = -0.2 * math.pi

class Robot:
    def __init__(self, sensor_positions):
        self.position = Vec2(START_X_POS,START_Y_POS)
        self.angle = START_ANGLE
        self.sensor_positions = sensor_positions


def set_command_file():
    with open(command_filename, 'w') as cmd_file:
        cmd_file.write("0.0,0.0,0.0")
    cmd_file.close()

def read_commands():
    """
    Returns a tuple with speed forward, speed left/right and turn speed
    """

    with open(command_filename) as txt:
        file_content = txt.read()
        while len(file_content) == 0:
            file_content = txt.read()
        split_content = file_content.split(",")

        return (
                float(split_content[0]),
                float(split_content[1]),
                float(split_content[2])
                )


def add_noise_function(command, function):
    command = function(command)
    return command


def update_robot(robot, command, delta_time):
    forward, strafe, turn = command

    robot.position += vec2_from_direction(robot.angle - math.pi/2, forward * delta_time)
    robot.position += vec2_from_direction(robot.angle, strafe * delta_time)
    robot.angle += turn * delta_time

    return robot


WINDOW_SIZE = (1300, 700)

def init_window():
    window = sdl.Window("Hello World!", size=WINDOW_SIZE)
    window.show()

    return window


def meters_to_pixels(m):
    return int(m * 350)


def draw_robot(surface, robot):
    pos_x = meters_to_pixels(robot.position.x) + WINDOW_SIZE[0] // 2
    pos_y = meters_to_pixels(robot.position.y) % WINDOW_SIZE[1]

    line = (
            pos_x,
            pos_y,
            pos_x + int(meters_to_pixels(0.10) * math.cos(robot.angle - math.pi / 2)),
            pos_y + int(meters_to_pixels(0.10) * math.sin(robot.angle - math.pi / 2)),
            pos_x,
            pos_y,
            pos_x + int(meters_to_pixels(0.05) * math.cos(robot.angle + math.pi / 2)),
            pos_y + int(meters_to_pixels(0.05) * math.sin(robot.angle + math.pi / 2)),
            pos_x,
            pos_y,
            pos_x + int(meters_to_pixels(0.05) * math.cos(robot.angle + 0)),
            pos_y + int(meters_to_pixels(0.05) * math.sin(robot.angle + 0)),
            pos_x,
            pos_y,
            pos_x + int(meters_to_pixels(0.05) * math.cos(robot.angle - math.pi)),
            pos_y + int(meters_to_pixels(0.05) * math.sin(robot.angle - math.pi))
        )

    sdl.line(surface, sdl.Color(255,255,100), line)


def draw(window, robot):
    events = sdl.get_events()
    for event in events:
        if event.type == sdl2.SDL_QUIT:
            running = False
            break

    surface = window.get_surface()
    sdl.fill(surface, sdl.Color(0,0,0))

    #Drawing the walls
    sdl.line(surface, sdl.Color(255,255,255),
            (
                - meters_to_pixels(0.4) + WINDOW_SIZE[0] // 2, 0,
                - meters_to_pixels(0.4) + WINDOW_SIZE[0] // 2, WINDOW_SIZE[1],
                meters_to_pixels(0.4) + WINDOW_SIZE[0] // 2, 0,
                meters_to_pixels(0.4) + WINDOW_SIZE[0] // 2, WINDOW_SIZE[1]
            ))

    draw_robot(surface, robot)

    window.refresh()



def get_senor_position(robot, sensor_pos):
    sensor_angle = math.atan2(sensor_pos.y, sensor_pos.x)
    sensor_length = abs(sensor_pos)

    total_angle = robot.angle + sensor_angle

    new_pos = robot.position + vec2_from_direction(total_angle, sensor_length)

    return new_pos


def write_sensor_data(robot):
    result = "" + str(robot.angle)

    for i in range(len(robot.sensor_positions)):
        sensor = robot.sensor_positions[i]

        sensor_pos = get_senor_position(robot, sensor)
        sensor_value = 0
        if i <= 1:
            sensor_angle = robot.angle

            wall_pos = 0.4

            x_distance = wall_pos - sensor_pos.x

            sensor_value = x_distance / math.cos(sensor_angle)
        else:
            sensor_angle = robot.angle + math.pi

            wall_pos = - 0.4

            x_distance = wall_pos - sensor_pos.x

            sensor_value = x_distance / math.cos(sensor_angle)

        result += "," + str(sensor_value) #+ math.sin(time.time() * 7)/20)

    file_path = "/tmp/hexsim/sensors"

    with open(file_path, mode="w") as file:
        file.write(result)

    return result


def set_start_values():
    noise_function = lambda cmd: (
            cmd[0],
            cmd[1], #+ math.sin(time.time())/15,
            cmd[2]  #+ math.sin(time.time() * 3)/15
        )
    return (noise_function, [Vec2(0.1,0.1), Vec2(0.1,-0.1), Vec2(-0.1,0.1), Vec2(-0.1,-0.1)])

def main():
    set_command_file()

    noise_func, sensor_positions = set_start_values()

    robot = Robot(sensor_positions)

    window = init_window()

    old_time = time.time()

    while True:
        new_time = time.time()
        delta_time = new_time - old_time
        old_time = new_time

        commands = read_commands()

        commands = add_noise_function(commands, noise_func)

        robot = update_robot(robot, commands, delta_time)

        draw(window, robot)

        write_sensor_data(robot)


if __name__ == "__main__":
    main()
