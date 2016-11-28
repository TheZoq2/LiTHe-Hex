import threading
import pika


connection = pika.BlockingConnection(pika.ConnectionParameters(
        host='localhost'))
channel = connection.channel()

channel.queue_declare(queue='hello')

channel.basic_publish(exchange='',
                      routing_key='hello',
                      body='Hello World!')
print(" [x] Sent 'Hello World!'")
connection.close()
# connection = pika.BlockingConnection(pika.ConnectionParameters(
#         host='localhost'))
# channel = connection.channel()
# 
# channel.queue_declare(queue='hello')
# 
# def callback(ch, method, properties, body):
#     print(" [x] Received %r" % body)
# 
# channel.basic_consume(callback,
#                       queue='hello',
#                       no_ack=True)
# 
# print(' [*] Waiting for messages. To exit press CTRL+C')
# channel.start_consuming()

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


# class CommunicationThread(threading.Thread):
# 
#     def __init__(self, send_queue, receive_queue):
#         super().__init__()
# 
#         self.channel = None
# 
#         def handle_delivery(channel, method, header, body):
#             print(body)
# 
#         def on_queue_declared(frame):
#             self.channel.basic_consume(handle_delivery, queue='hello')
# 
#         def on_channel_open(new_channel):
#             self.channel = new_channel
#             self.channel.queue_declare(queue="hello", durable=True, exclusive=False,
#                                   auto_delete=False, callback=on_queue_declared)
# 
#         def on_connected(connection):
#             self.connection.channel(on_channel_open)
# 
#         self.send_queue = send_queue
#         self.receive_queue = receive_queue
#         self.parameters = pika.ConnectionParameters()
#         self.connection = pika.SelectConnection(self.parameters, on_connected)
# 
#     def run(self):
#         self.connection.ioloop.start()
# 
#     def stop(self):
#         self.connection.close()


# parameters = pika.ConnectionParameters()
# connection = pika.SelectConnection(parameters, on_connected)
# 
# try:
#     # Loop so we can communicate with RabbitMQ
#     connection.ioloop.start()
# except KeyboardInterrupt:
#     # Gracefully close the connection
#     connection.close()


