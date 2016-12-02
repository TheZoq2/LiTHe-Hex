# Copyright 2016 Noak Ringman, Emil Segerbäck, Robin Sliwa, Frans Skarman, Hannes Tuhkala, Malcolm Wigren, Olav Övrebö

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


class UnexpectedDataException(Exception):

    def __init__(self, expected, actual):
        super().__init__()
        self.expected = expected
        self.actual = actual


class SpiDev(object):
    """
    Fake SpiDev class for testing purposes.
    """
    
    def __init__(self):
        self.max_speed_hz = 500000
        self.spi_port = 0
        self.slave_select = 0
        self.data_sequence = []
        self.expected_seq = []

    def set_fake_read_sequence(self, data):
        """
        Sets a data sequence that readbytes will
        return when invoked.
        """
        assert isinstance(data, list)
        self.data_sequence = data

    def set_expected_write_sequence(self, data):
        """
        Sets a sequence that writebytes expects
        when invoked.
        """
        self.expected_seq = data 

    def open(self, spi_port, slave_select):
        self.spi_port = spi_port
        self.slave_select = slave_select

    def writebytes(self, data):
        for i in range(len(data)):
            if data[i] != self.expected_seq[i]:
                raise UnexpectedDataException(self.expected_seq, data)
        self.expected_seq = self.expected_seq[len(data):]

    def readbytes(self, length):
        data = self.data_sequence[:length]
        self.data_sequence = self.data_sequence[length:]
        return data

