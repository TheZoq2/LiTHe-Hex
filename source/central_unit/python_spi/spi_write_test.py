import spidev
import time

spi = spidev.SpiDev()
spi.open(0,0)
data_to_send = 0
# reset the spi on avr
spi.xfer2([0])
while True:
    data_to_send+=1
    response = spi.xfer2([data_to_send])
    if (response == [126]):
        print(data_to_send)
        print("She got the message!")
    time.sleep(0.1)
