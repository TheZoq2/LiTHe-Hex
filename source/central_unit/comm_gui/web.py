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


