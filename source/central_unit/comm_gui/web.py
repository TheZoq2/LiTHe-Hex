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

import pika
import threading

# channel = None
# 
# def on_connected(connection):
#     connection.channel()
# 
# 
# def on_channel_open(new_channel):
#     global channel
#     channel = new_channel
#     channel.queue_declare(queue="test", durable=True, exclusive=False, auto_delete=False, callback=on_queue_declared)
# 
# 
# def on_queue_declared(frame):
#     channel.basic_consume(handle_delivery, queue='test')
# 
# 
# def handle_delivery(channel, method, header, body):
#     print(body)


class CommunicationThread(threading.Thread):

    def __init__(self, send_queue, receive_queue):
        super().__init__()

        self.channel = None

        def handle_delivery(channel, method, header, body):
            print(body)

        def on_queue_declared(frame):
            self.channel.basic_consume(handle_delivery, queue='test')

        def on_channel_open(new_channel):
            self.channel = new_channel
            self.channel.queue_declare(queue="test", durable=True, exclusive=False,
                                  auto_delete=False, callback=on_queue_declared)

        def on_connected(connection):
            self.connection.channel(on_channel_open)

        self.send_queue = send_queue
        self.receive_queue = receive_queue
        self.parameters = pika.ConnectionParameters()
        self.connection = pika.SelectConnection(self.parameters, on_connected)

    def run(self):
        self.connection.ioloop.start()

    def stop(self):
        self.connection.close()


# parameters = pika.ConnectionParameters()
# connection = pika.SelectConnection(parameters, on_connected)
# 
# try:
#     # Loop so we can communicate with RabbitMQ
#     connection.ioloop.start()
# except KeyboardInterrupt:
#     # Gracefully close the connection
#     connection.close()


