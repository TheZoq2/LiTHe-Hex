# Copyright 2016 Noak Ringman, Emil Segerbäck, Robin Sliwa, Frans Skarman, Hannes Tuhkala, Malcolm Wigren, Olav Övrebö
#

# This file is part of LiTHe Hex.

# LiTHe Hex is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# LiTHe Hex is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with LiTHe Hex.  If not, see <http://www.gnu.org/licenses/>.

try:
    import spidev
except ImportError:
    import tests.fake_spi as spidev

import time
import math
import communication.angle_calculation as angle_calculation
import pdb
import constants


MOTOR_ADDR = (0, 0)
SENSOR_ADDR = (0, 1)

DATA_REQ    = 0x02

SEND_FAIL   = 0x1F
ACK         = 0x0F
GARBAGE     = 0x00

SET_OBSTACLE = 0x03

SET_SERVO_SPEED = 0x20
SET_SERVO_SPEED_LENGTH = 2

WALK = 0x21
WALK_LENGTH = 3

RETURN_TO_NEUTRAL = 0x05

SERVO_STATUS = 0x22
MOTOR_DEBUG = 0x23

SENSOR_DATA = 0x24


class SensorDataPacket(object):
    """
    Data structure containing the values of all sensors. All angles
    are measured in degrees and all distances in meters.
    """

    def __init__(self, ir_down, ir_front_left, ir_back_left,
                 ir_front_right, ir_back_right, lidar_msd, lidar_lsd):
        # divide by hundred to convert from cm -> m
        self.ir_front_left  = ir_front_left / 100
        self.ir_front_right = ir_front_right / 100
        self.ir_back_left   = ir_back_left / 100
        self.ir_back_right  = ir_back_right / 100
        self.ir_down        = ir_down / 100
        self.lidar          = (lidar_lsd | (lidar_msd << 8)) / 100
        self.left_angle     = angle_calculation.get_left_angle(self)
        self.right_angle    = angle_calculation.get_right_angle(self)
        self.average_angle  = angle_calculation.get_average_angle(self)

    def __str__(self):
        return """
Front left:  {}
Front right: {}
Back left:   {}
Back right:  {}
Down:        {}
Lidar:       {}
                """.format(
                        self.ir_front_left,
                        self.ir_front_right,
                        self.ir_back_left,
                        self.ir_back_right,
                        self.ir_down,
                        self.lidar)


class InvalidCommandException(Exception):

    def __init__(self, message, command=""):
        self.message = message
        self.command = command

    def __repr__(self):
        return self.message

    def __str__(self):
        return self.message


class CommunicationError(Exception):

    def __init__(self, message, cause=None):
        self.message = message
        self.cause = cause

    def __repr__(self):
        return self.message

    def __str__(self):
        return self.message


def _calculate_parity(data):
    data_str = "{0:032b}".format(data)
    num_ones = data_str.count("1")
    if num_ones % 2 == 0:
        return 0
    else:
        return 1


def _add_parity(type_, msg):
    msg_parity = _calculate_parity(msg)
    result = type_ << 2
    result |= msg_parity << 1
    type_parity = _calculate_parity(result)
    result |= type_parity
    return result


def _send_bytes(spi, *data):
    # check if all are bytes
    if sum([(not isinstance(x, int)) or x > constants.MAX_BYTE_SIZE or x < 0 for x in data]):
        raise InvalidCommandException("Data sequence {} contains non-bytes".format(data))
    spi.writebytes([GARBAGE])
    spi.writebytes(list(data))
    return spi.readbytes(1)


def _recieve_muliple_bytes(spi, type_):
    length = spi.readbytes(1)[0]
    return spi.readbytes(length)


def _recieve_single_byte(spi, type_):
    return spi.readbytes(1)


def _recieve_bytes(spi):
    type_ = spi.readbytes(1)[0]

    # if the most significant bit in the
    # type message is 1, then the message
    # is of variable length
    if _is_multibyte_msg(type_):
        return _recieve_muliple_bytes(spi, type_)
    else:
        return _recieve_single_byte(spi, type_)


def _is_multibyte_msg(data_id):
    return (data_id & 0x80) != 0


def _request_data(spi, data_id):
    # response = spi.xfer2([DATA_REQ, data_id])
    # pdb.set_trace()
    response = _send_bytes(spi, _add_parity(DATA_REQ, data_id), data_id)
    _check_response(response)
    return _recieve_bytes(spi)


def _select_device(spi, addr):
    spi.open(*addr)


def _check_response(response):
    if isinstance(response, list):
        _check_response(response[0])
    elif response == SEND_FAIL:
        raise CommunicationError("Send fail received")
    elif response != ACK:
        raise CommunicationError("No acknowledge message received")


def _get_total_msg(*data):
    length = len(data)
    res = ""
    for d in data:
        res += "{0:08b}".format(d)
    return int(res)


def communication_init():
    """Initializes the SPI-communication"""
    spi = spidev.SpiDev()
    spi.open(*MOTOR_ADDR)
    spi.max_speed_hz = 25000
    spi.open(*SENSOR_ADDR)
    spi.max_speed_hz = 25000
    return spi


def set_obstacle_mode(spi, value):
    """Sets the obstacle mode to either True or False on the motor unit"""
    if value not in (True, False):
        raise InvalidCommandException("Value \"{}\" is not a valid mode.".format(value))
    # yes i'm paranoid
    value = 0x01 if value else 0x00
    _select_device(spi, MOTOR_ADDR)
    response = _send_bytes(spi, _add_parity(SET_OBSTACLE, value), value)
    _check_response(response)


def set_servo_speed(spi, speed):
    """Sets the global servo speed on the motor unit"""
    if speed < 0 or speed > constants.MAX_16BIT_SIZE:
        raise InvalidCommandException("Speed \"{}\" is not a 16-bit value".format(speed))
    _select_device(spi, MOTOR_ADDR)
    least = speed & 0x00FF
    most = (speed & 0xFF00) >> 8
    total_msg = _get_total_msg(SET_SERVO_SPEED_LENGTH, least, most)
    response = _send_bytes(spi, _add_parity(SET_SERVO_SPEED, total_msg),
                           SET_SERVO_SPEED_LENGTH, least, most)
    _check_response(response)


def walk(spi, x_speed, y_speed, turn_speed):
    """
    Commands the motor unit to walk in the direction and speed 
    given by x_speed, y_speed and turn_speed
    """
    _select_device(spi, MOTOR_ADDR)
    total_msg = _get_total_msg(WALK_LENGTH, x_speed, y_speed, turn_speed)
    response = _send_bytes(spi, _add_parity(WALK, total_msg),
                           WALK_LENGTH, x_speed, y_speed, turn_speed)
    _check_response(response)


def back_to_neutral(spi):
    """Commands the motor unit to return to the neutral state"""
    _select_device(spi, MOTOR_ADDR)
    # we send a zero byte as args
    response = _send_bytes(spi, _add_parity(RETURN_TO_NEUTRAL, 0), 0)
    _check_response(response)


def get_servo_status(spi):
    """Fetches the servo status"""
    # TODO create appropriate data structure
    _select_device(spi, MOTOR_ADDR)
    return _request_data(spi, SERVO_STATUS)


def get_motor_debug(spi):
    """Gets a motor unit debug string"""
    # TODO create string
    _select_device(spi, MOTOR_ADDR)
    return _request_data(spi, MOTOR_DEBUG)


def get_sensor_data(spi):
    """
    Gets the values of all sensors in the sensor unit,
    and puts them in a SensorDataPacket
    """
    _select_device(spi, SENSOR_ADDR)
    raw_data = _request_data(spi, SENSOR_DATA)
    return SensorDataPacket(*raw_data)


