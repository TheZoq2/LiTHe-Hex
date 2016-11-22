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

MOTOR_ADDR = (0, 0)
SENSOR_ADDR = (0, 1)

DATA_REQ    = 0x02

SEND_FAIL   = 0x1F
ACK         = 0x0F


def _send_bytes(spi, data):
    pass


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


def _select_device(addr):
    spi.open(*addr)


def communication_init():
    spi = spidev.SpiDev()
    spi.open(*MOTOR_ADDR)
    return spi


def toggle_obstacle_mode(spi):
    pass


def set_servo_speed(spi, speed):
    pass


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

