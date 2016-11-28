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

import threading
import pika
import json

CENTRAL_UNIT_KEY_RECEIVE = 'to_pi'
CENTRAL_UNIT_KEY_SEND = 'from_pi'

QUEUE_CHECK_DELAY = 0.05


class ServerReceivedPacket(object):
    """
    Data structure containing the information of parsed JSON-strings
    from the web server. 
    """

    def __init__(self, json_string):
        data = json.loads(json_string)
        self.x = data['x_spd']
        self.y = data['y_spd'] 
        self.rotation = data['rotation'] 
        self.thrust = data['thrust'] 
        self.auto = data['auto']


class ServerSendPacket(object):
    """
    Data structure containing information to be converted to JSON-strings
    and sent to the web server.
    """

    def __init__(self, sensor_data_packet, corridor_data_packet):
        """
        Constructs a ServerSendPacket from a SensorDataPacket and 
        CorridorDataPacket.
        """
        self.sensor = sensor_data_packet
        self.corridor = corridor_data_packet
    
    def get_json(self):
        data = {
            "ir_down": self.sensor.ir_down,
            "ir_fl": self.sensor.ir_front_left,
            "ir_fr": self.sensor.ir_front_right,
            "ir_bl": self.sensor.ir_back_left,
            "ir_br": self.sensor.ir_back_right,
            "lidar": self.sensor.lidar,
            "dist_f": self.corridor.front_dist,
            "dist_l": self.corridor.left_dist,
            "dist_r": self.corridor.right_dist,
            "dist_d": self.corridor.down_dist,
            "corr_angle": self.corridor.corr_angle
        }
        return json.dumps(data)


class ServerSenderThread(threading.Thread):

    def __init__(self, queue, channel, connection):
        super().__init__()
        self._stop_flag = threading.Event()
        self.queue = queue
        self.channel = channel
        self.connection = connection

    def run(self):
        while not self._stop_flag.wait(QUEUE_CHECK_DELAY):
            if not self.queue.empty():
                packet = self.queue.get()
                self.channel.basic_publish(exchange='', 
                                           routing_key=CENTRAL_UNIT_KEY_SEND,
                                           body=packet.get_json())

    def stop(self):
        self._stop_flag.set()


class ServerReceiverThread(threading.Thread):

    def __init__(self, queue, channel, connection):
        super().__init__()

        def _receive_callback(ch, method, properties, body):
            self.queue.put(ServerReceivedPacket(body))

        self.queue = queue
        self.channel = channel
        self.connection = connection
        self.channel.basic_consume(_receive_callback,
                                   queue=CENTRAL_UNIT_KEY_RECEIVE,
                                   no_ack=True)

    def run(self):
        self.channel.start_consuming()

    def stop(self):
        self.channel.close()
    

class CommunicationThread(threading.Thread):

    def __init__(self, send_queue, receive_queue):
        super().__init__()

        connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))

        channel_receive = connection.channel()
        channel_send = connection.channel()

        channel_receive.queue_declare(queue=CENTRAL_UNIT_KEY_RECEIVE)
        channel_send.queue_declare(queue=CENTRAL_UNIT_KEY_SEND)

        self.receive_thread = ServerReceiverThread(receive_queue, channel_receive, 
                                                   connection)
        self.send_thread = ServerSenderThread(send_queue, channel_send, connection)

    def run(self):
        self.receive_thread.start()
        self.send_thread.start()
        self.receive_thread.join()
        self.send_thread.join()

    def stop(self):
        self.receive_thread.stop()
        self.send_thread.stop()


# parameters = pika.ConnectionParameters()
# connection = pika.SelectConnection(parameters, on_connected)
# 
# try:
#     # Loop so we can communicate with RabbitMQ
#     connection.ioloop.start()
# except KeyboardInterrupt:
#     # Gracefully close the connection
#     connection.close()


