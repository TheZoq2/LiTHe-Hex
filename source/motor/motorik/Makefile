SOURCES=motorik.c uart_lib.c servo.c  \
		../../IK/ik.c gangstil.c

SOURCES_AVR=../../communication/spi.c ../../communication/communication.c
build:
	avr-gcc -mmcu=atmega1284p -Wall -Os -o src.elf ${SOURCES} ${SOURCES_AVR} -DF_CPU=16000000

test:
	gcc ${SOURCES} -DIS_X86 -lm -g
	./a.out

#avr-objcopy -j .text -j .data -O ihex src.elf src.hex

