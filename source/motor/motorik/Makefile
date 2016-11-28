SOURCES=motorik.c uart_lib.c servo.c  \
		../../communication/spi.c ../../communication/communication.c \
		../../IK/ik.c gangstil.c
build:
	avr-gcc -mmcu=atmega1284p -Wall -Os -o src.elf ${SOURCES} -DF_CPU=16000000

test:
	gcc ${SOURCES} -DIS_X86
	./a.out

#avr-objcopy -j .text -j .data -O ihex src.elf src.hex

