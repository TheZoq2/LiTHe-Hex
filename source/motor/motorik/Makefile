build:
	avr-gcc -mmcu=atmega1284p -Wall -Os -o src.elf motorik.c uart_lib.c -DF_CPU=16000000

test:
	gcc motorik.c uart_lib.c -DIS_X86
	./a.out

#avr-objcopy -j .text -j .data -O ihex src.elf src.hex

