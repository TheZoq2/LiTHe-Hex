SOURCES=uart_lib.c servo.c  \
		../../IK/ik.c gangstil.c \
		status.c

SOURCES_X86=gangstil_x86.c servo_x86.c

SOURCES_AVR=../../communication/spi.c ../../communication/communication.c

SOURCES_MAIN=motorik.c

SOURCES_UNIT=unit.c

build:
	avr-gcc -mmcu=atmega1284p -Wall -Os -o src.elf \
		${SOURCES} ${SOURCES_AVR} ${SOURCES_MAIN} -DF_CPU=16000000

x86:
	gcc ${SOURCES} ${SOURCES_MAIN} ${SOURCES_X86} -DIS_X86 -Wall -lm -g

test:
	make x86
	./a.out

unit:
	gcc ${SOURCES} ${SOURCES_X86} ${SOURCES_UNIT} -DIS_X86 -Wall -lm -g -lcheck \
		-o unit_tests -DIS_UNIT_TEST -DCK_FORK=no
	./unit_tests

#avr-objcopy -j .text -j .data -O ihex src.elf src.hex

