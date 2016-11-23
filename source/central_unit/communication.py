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

MOTOR_ADDR = (0, 0)
SENSOR_ADDR = (0, 1)

DATA_REQ    = 0x02

SEND_FAIL   = 0x1F
ACK         = 0x0F

MAX_BYTE_SIZE = 255
MAX_16BIT_SIZE = MAX_BYTE_SIZE**2


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


def _send_bytes(spi, *data):
    # check if all are bytes
    if sum([(not isinstance(x, int)) or x > MAX_BYTE_SIZE or x < 0 for x in data]):
        raise InvalidCommandException("Data sequence {} contains non-bytes".format(data))
    spi.writebytes([66])
    spi.writebytes(list(data))
    return spi.readbytes(1)
    # res = []
    # for d in data:
    #     res.append(spi.xfer2([d]))
    # return res
        

def _recieve_muliple_bytes(spi, type_):
    length = spi.readbytes(1)
    return spi.readbytes(length)
    

def _recieve_single_byte(spi, type_):
    return spi.readbytes(1)


def _recieve_bytes(spi):
    type_ = spi.readbytes(1)

    # if the most significant bit in the
    # type message is 1, then the message
    # is of variable length
    if (type_ & 0x80):
        return _recieve_muliple_bytes(spi, type_)
    else:
        return _recieve_single_byte(spi, type_)


def _request_data(data_id):
    response = spi.xfer2([DATA_REQ, data_id])
    if response == ACK:
        return True
    else:
        return False


def _select_device(spi, addr):
    spi.open(*addr)


def _check_response(response):
    if response == SEND_FAIL:
        raise CommunicationError("Send fail revieved")
    elif response != ACK:
        raise CommunicationError("No acknowledge message recieved")


def communication_init():
    spi = spidev.SpiDev()
    spi.open(*MOTOR_ADDR)
    return spi


def set_obstacle_mode(spi, value):
    if value not in (True, False):
        raise InvalidCommandException("Value \"{}\" is not a valid mode.".format(value))
    # yes i'm paranoid
    value = 0x01 if value else 0x00
    _select_device(spi, MOTOR_ADDR)
    response = _send_bytes(spi, 0x03, value)
    _check_response(response)


def set_servo_speed(spi, speed):
    if speed < 0 or speed > MAX_16BIT_SIZE:
        raise InvalidCommandException("Speed \"{}\" is not a 16-bit value".format(speed))
    least = speed & 0x00FF
    most = (speed & 0xFF00) >> 8
    response = _send_bytes(spi, least, most)
    _check_response(response)


def walk(spi, x_speed, y_speed, turn_speed):
    pass


def back_to_neutral(spi):
    pass


def get_servo_status(spi):
    pass


def get_motor_debug(spi):
    pass


def get_sensor_data(spi):
    pass


def get_corridor_data(spi):
    pass

