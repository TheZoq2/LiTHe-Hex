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

import spidev
import time
import pdb

MOTOR_ADDR = (0, 0)
SENSOR_ADDR = (0, 1)

DATA_REQ    = 0x02

SEND_FAIL   = 0x1F
ACK         = 0x0F

MAX_BYTE_SIZE = 255
MAX_16BIT_SIZE = MAX_BYTE_SIZE**2

SET_OBSTACLE = 0x03

SET_SERVO_SPEED = 0x20
SET_SERVO_SPEED_LENGTH = 2

WALK = 0x21
WALK_LENGTH = 3

RETURN_TO_NEUTRAL = 0x05

SERVO_STATUS = 0x22
MOTOR_DEBUG = 0x23

SENSOR_DATA = 0x24

SENSOR_CORRIDOR = 0x25


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
    if sum([(not isinstance(x, int)) or x > MAX_BYTE_SIZE or x < 0 for x in data]):
        raise InvalidCommandException("Data sequence {} contains non-bytes".format(data))
    spi.writebytes([66])
    spi.writebytes(list(data))
    return spi.readbytes(1)
        

def _recieve_muliple_bytes(spi, type_):
    length = spi.readbytes(1)[0]
    return spi.readbytes(length)
    

def _recieve_single_byte(spi, type_):
    return spi.readbytes(1)[0]


def _recieve_bytes(spi):
    type_ = spi.readbytes(1)[0]

    # if the most significant bit in the
    # type message is 1, then the message
    # is of variable length
    if (type_ & 0x80):
        return _recieve_muliple_bytes(spi, type_)
    else:
        return _recieve_single_byte(spi, type_)


def _is_multibyte_msg(data_id):
    tmp = data_id << 2
    tmp_str = '{0:08b}'.format(tmp)
    if tmp[0] == '1':
        return True
    return False


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
    res = str(length) if length > 1 else ""
    for d in data:
        res += str(d)
    return int(res)


def communication_init():
    spi = spidev.SpiDev()
    spi.open(*MOTOR_ADDR)
    spi.max_speed_hz = 25000
    return spi


def set_obstacle_mode(spi, value):
    if value not in (True, False):
        raise InvalidCommandException("Value \"{}\" is not a valid mode.".format(value))
    # yes i'm paranoid
    value = 0x01 if value else 0x00
    _select_device(spi, MOTOR_ADDR)
    response = _send_bytes(spi, _add_parity(SET_OBSTACLE, value), value)
    _check_response(response)


def set_servo_speed(spi, speed):
    if speed < 0 or speed > MAX_16BIT_SIZE:
        raise InvalidCommandException("Speed \"{}\" is not a 16-bit value".format(speed))
    _select_device(spi, MOTOR_ADDR)
    least = speed & 0x00FF
    most = (speed & 0xFF00) >> 8
    total_msg = _get_total_msg(SET_SERVO_SPEED_LENGTH, least, most)
    response = _send_bytes(spi, _add_parity(SET_SERVO_SPEED, total_msg), 
                           SET_SERVO_SPEED_LENGTH, least, most)
    _check_response(response)


def walk(spi, x_speed, y_speed, turn_speed):
    _select_device(spi, MOTOR_ADDR)
    total_msg = _get_total_msg(x_speed, y_speed, turn_speed)
    response = _send_bytes(spi, _add_parity(WALK, total_msg),
                           WALK_LENGTH, x_speed, y_speed, turn_speed)
    _check_response(response)


def back_to_neutral(spi):
    _select_device(spi, MOTOR_ADDR)
    pass


def get_servo_status(spi):
    _select_device(spi, MOTOR_ADDR)
    return _request_data(spi, SERVO_STATUS)


def get_motor_debug(spi):
    _select_device(spi, MOTOR_ADDR)
    return _request_data(spi, MOTOR_DEBUG)


def get_sensor_data(spi):
    _select_device(spi, SENSOR_ADDR)
    return _request_data(spi, SENSOR_DATA)


def get_corridor_data(spi):
    _select_device(spi, SENSOR_ADDR)
    return _request_data(spi, SENSOR_DATA)


