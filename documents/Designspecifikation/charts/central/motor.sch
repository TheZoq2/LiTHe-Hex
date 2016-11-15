EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:hex
LIBS:motor-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATMEGA1284-P IC?
U 1 1 581A0EFD
P 5600 4050
F 0 "IC?" H 4750 5930 50  0000 L BNN
F 1 "ATMEGA1284-P" H 6000 2100 50  0000 L BNN
F 2 "DIL40" H 5600 4050 50  0000 C CIN
F 3 "" H 5600 4050 50  0000 C CNN
	1    5600 4050
	1    0    0    -1  
$EndComp
$Comp
L ATMEGA1284-P IC?
U 1 1 581A13B7
P 8500 4000
F 0 "IC?" H 7650 5880 50  0000 L BNN
F 1 "ATMEGA1284-P" H 8900 2050 50  0000 L BNN
F 2 "DIL40" H 8500 4000 50  0000 C CIN
F 3 "" H 8500 4000 50  0000 C CNN
	1    8500 4000
	1    0    0    -1  
$EndComp
$Comp
L BOB-12009 -
U 1 1 581A1EC5
P 3250 2200
F 0 "-" H 3250 2000 60  0000 C CNN
F 1 "TXB0104" H 3250 2150 60  0000 C CNN
F 2 "" H 3250 2150 60  0001 C CNN
F 3 "" H 3250 2150 60  0001 C CNN
	1    3250 2200
	1    0    0    -1  
$EndComp
$Comp
L RaspberryPi3 Rpi
U 1 1 581A2F39
P 1350 3050
F 0 "Rpi" H 1300 3000 60  0000 C CNN
F 1 "RaspberryPi3" H 1300 3150 60  0000 C CNN
F 2 "" H 1300 3150 60  0001 C CNN
F 3 "" H 1300 3150 60  0001 C CNN
	1    1350 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	6550 6950 6750 6950
Connection ~ 6550 6950
Wire Wire Line
	2550 2250 2550 3000
Wire Wire Line
	4450 3400 3950 3400
Wire Wire Line
	4450 1600 4450 3400
Wire Wire Line
	4450 1600 9900 1600
Wire Wire Line
	7000 1600 7000 3950
Wire Wire Line
	7000 3950 6600 3950
Wire Wire Line
	4350 3300 3950 3300
Wire Wire Line
	4350 1700 4350 3300
Wire Wire Line
	4350 1700 9800 1700
Wire Wire Line
	6900 1700 6900 3850
Wire Wire Line
	6900 3850 6600 3850
Wire Wire Line
	4250 3200 3950 3200
Wire Wire Line
	4250 1800 4250 3200
Wire Wire Line
	4250 1800 9700 1800
Wire Wire Line
	6800 1800 6800 3750
Wire Wire Line
	6800 3750 6600 3750
Wire Wire Line
	4150 3100 3950 3100
Wire Wire Line
	4150 1900 4150 3100
Wire Wire Line
	4150 1900 6700 1900
Wire Wire Line
	6700 1900 6700 3650
Wire Wire Line
	6700 3650 6600 3650
Wire Wire Line
	9700 1800 9700 3650
Wire Wire Line
	9700 3700 9500 3700
Connection ~ 6800 1800
Wire Wire Line
	9800 1700 9800 3750
Wire Wire Line
	9800 3800 9500 3800
Connection ~ 6900 1700
Wire Wire Line
	9900 1600 9900 3850
Wire Wire Line
	9900 3900 9500 3900
Connection ~ 7000 1600
Wire Wire Line
	3950 2250 4050 2250
Wire Wire Line
	4050 2250 4050 1450
Wire Wire Line
	4050 1450 10050 1450
Wire Wire Line
	10050 1450 10050 3550
Wire Wire Line
	10050 3600 9500 3600
$Comp
L BOB-12009 -
U 1 1 581A1E82
P 3250 3350
F 0 "-" H 3250 3150 60  0000 C CNN
F 1 "TXB0104" H 3250 3300 60  0000 C CNN
F 2 "" H 3250 3300 60  0001 C CNN
F 3 "" H 3250 3300 60  0001 C CNN
	1    3250 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2350 3400 2550 3400
Wire Wire Line
	2350 3300 2550 3300
Wire Wire Line
	2350 3200 2550 3200
Wire Wire Line
	2350 3100 2550 3100
Wire Wire Line
	2550 3000 2350 3000
Wire Wire Line
	1600 2150 1600 1500
Wire Wire Line
	1600 1500 3100 1500
Wire Wire Line
	3100 1500 3100 1650
Wire Wire Line
	1450 2150 1450 1250
Wire Wire Line
	1450 1250 8300 1250
Wire Wire Line
	3400 1100 3400 1650
Text Notes 5050 3900 0    60   ~ 0
Sensorenhet\n
Text Notes 7900 3850 0    60   ~ 0
Motorikenhet\n
Text Notes 1100 3600 0    60   ~ 0
Centralenhet\n
Wire Wire Line
	3100 2800 3100 2550
Wire Wire Line
	3100 2550 2400 2550
Wire Wire Line
	2400 2550 2400 1500
Connection ~ 2400 1500
Wire Wire Line
	3400 2800 3400 2500
Wire Wire Line
	3400 2500 2450 2500
Wire Wire Line
	2450 2500 2450 1250
Connection ~ 2450 1250
Wire Wire Line
	5400 1250 5400 2050
Connection ~ 3400 1250
Wire Wire Line
	8300 1250 8300 2000
Connection ~ 5400 1250
$Comp
L +5V #PWR?
U 1 1 581A6F63
P 3400 1100
F 0 "#PWR?" H 3400 950 50  0001 C CNN
F 1 "+5V" H 3400 1240 50  0000 C CNN
F 2 "" H 3400 1100 50  0000 C CNN
F 3 "" H 3400 1100 50  0000 C CNN
	1    3400 1100
	1    0    0    -1  
$EndComp
$EndSCHEMATC
