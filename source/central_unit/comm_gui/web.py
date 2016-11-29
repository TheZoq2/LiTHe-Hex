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
    from the web server. Fields are marked None if the particular key
    was not included in this message.
    """

    def __init__(self, json_string):
        """
        Construct a ServerReceivedPacket from a json string
        """
        data = json.loads(json_string)
        self.x = data.get('x', None)
        self.y = data.get('y', None)
        self.rotation = data('rotation', None)
        self.thrust = data('thrust', None)
        self.auto = data.get('auto', None)


class ServerSendPacket(object):
    """
    Data structure containing information to be converted to JSON-strings
    and sent to the web server.
    """

    def __init__(self, sensor_data_packet=None, corridor_data_packet=None,
                 auto_mode=None, debug_string=None):
        """
        Constructs a ServerSendPacket from an optional SensorDataPacket and 
        CorridorDataPacket, an optional flag indicating whether automatic
        mode is engaged, and an optional debug-string.
        """
        assert sensor_data_packet is None or isinstance(sensor_data_packet, SensorDataPacket)
        assert corridor_data_packet is None or isinstance(sensor_data_packet, CorridorDataPacket)
        assert auto_mode is None or auto_mode in (True, False)
        assert isinstance(debug_string, str)
        self.sensor = sensor_data_packet
        self.corridor = corridor_data_packet
        self.auto_mode = auto_mode
        self.debug_string = debug_string
    
    def get_json(self):
        data = {}
        if self.sensor is not None:
            data["ir_down"] = self.sensor.ir_down
            data["ir_fl"] = self.sensor.ir_front_left
            data["ir_fr"] = self.sensor.ir_front_right
            data["ir_bl"] = self.sensor.ir_back_left
            data["ir_br"] = self.sensor.ir_back_right
            data["lidar"] = self.sensor.lidar
        if self.corridor is not None:
            data["dist_f"] = self.corridor.front_dist
            data["dist_l"] = self.corridor.left_dist
            data["dist_r"] = self.corridor.right_dist
            data["dist_d"] = self.corridor.down_dist
            data["corr_angle"] = self.corridor.corr_angle
        if self.auto_mode is not None:
            data["auto"] = self.auto_mode
        if self.debug_string is not None:
            data["debug"] = self.debug_string
        return json.dumps(data)


class ServerSenderThread(threading.Thread):
    """
    Thread whose job is to send packets put in the send queue
    to the server. Dequeues an element (if available) and sends
    it 1/QUEUE_CHECK_DELAY times per second.
    """

    def __init__(self, queue, channel, connection):
        super().__init__()
        # create a flag that, when set, stops this thread
        self._stop_flag = threading.Event()
        self.queue = queue
        self.channel = channel
        self.connection = connection

    def run(self):
        """
        Loop and wait QUEUE_CHECK_DELAY seconds. 
        Exits the thread if the stop flag is set.
        """
        while not self._stop_flag.wait(QUEUE_CHECK_DELAY):
            if not self.queue.empty():
                packet = self.queue.get()
                self.channel.basic_publish(exchange='', 
                                           routing_key=CENTRAL_UNIT_KEY_SEND,
                                           body=packet.get_json())

    def stop(self):
        self._stop_flag.set()
        return 0


class ServerReceiverThread(threading.Thread):
    """
    Thread whose job is to receive packets from the server and put
    in the receive queue.
    """

    def __init__(self, queue, channel, connection):
        super().__init__()

        def _receive_callback(ch, method, properties, body):
            self.queue.put(ServerReceivedPacket(body.decode("utf-8")))
            print(body.decode("utf-8"))

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
        return 0
    

class CommunicationThread(threading.Thread):
    """
    The main communication thread which is started in the background. Creates
    a thread that sends data packets put in the shared send queue to the
    web server, and another thread that receives data from the web server and 
    puts it in the shared receive queue.
    """

    def __init__(self, send_queue, receive_queue):
        super().__init__()

        # setup connection
        connection = pika.BlockingConnection(pika.ConnectionParameters(host='localhost'))

        # create the send and receive channels
        channel_receive = connection.channel()
        channel_send = connection.channel()
        channel_receive.queue_declare(queue=CENTRAL_UNIT_KEY_RECEIVE)
        channel_send.queue_declare(queue=CENTRAL_UNIT_KEY_SEND)

        # create the send and receive threads
        self.receive_thread = ServerReceiverThread(receive_queue, channel_receive, 
                                                   connection)
        self.send_thread = ServerSenderThread(send_queue, channel_send, connection)

    def run(self):
        # start the threads
        self.receive_thread.start()
        self.send_thread.start()

        # wait for the threads to stop (if stop is invoked)
        self.receive_thread.join()
        self.send_thread.join()

    def stop(self):
        """
        Stops the communication to the webserver and exits the thread.
        Returns 0 on success and 1 on fail.
        """
        status_recv = self.receive_thread.stop()
        status_send = self.send_thread.stop()
        return 0 if status_recv == 0 and status_send == 0 else 1

