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
U 1 1 57FDEFB3
P 2400 3050
F 0 "IC?" H 1550 4930 50  0000 L BNN
F 1 "ATMEGA1284-P" H 2800 1100 50  0000 L BNN
F 2 "DIL40" H 2400 3050 50  0000 C CIN
F 3 "" H 2400 3050 50  0000 C CNN
	1    2400 3050
	1    0    0    -1  
$EndComp
Text Notes 6900 2300 0    60   ~ 0
Centralenheten
Text Notes 5500 2650 0    60   Italic 0
SPI
Text Notes 4100 3800 0    60   ~ 0
JTAG
Text Notes 7200 3350 0    60   ~ 0
LIDAR
$Comp
L C 680µF
U 1 1 57FDF7AD
P 6600 3300
F 0 "680µF" H 6625 3400 50  0000 L CNN
F 1 "C" H 6625 3200 50  0001 L CNN
F 2 "" H 6638 3150 50  0000 C CNN
F 3 "" H 6600 3300 50  0000 C CNN
	1    6600 3300
	1    0    0    -1  
$EndComp
Text Label 6900 3150 0    39   ~ 0
GND
Text Label 6900 3450 0    39   ~ 0
Vcc
Text Label 7000 3700 0    39   ~ 0
SDA
Text Label 7000 3800 0    39   ~ 0
SCL
$Comp
L R R?
U 1 1 57FDFADA
P 4900 3400
F 0 "R?" V 4980 3400 50  0001 C CNN
F 1 "4.7k" V 4900 3400 50  0000 C CNN
F 2 "" V 4830 3400 50  0000 C CNN
F 3 "" H 4900 3400 50  0000 C CNN
	1    4900 3400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57FDFF7C
P 6300 2150
F 0 "#PWR?" H 6300 1900 50  0001 C CNN
F 1 "GND" H 6300 2000 50  0000 C CNN
F 2 "" H 6300 2150 50  0000 C CNN
F 3 "" H 6300 2150 50  0000 C CNN
	1    6300 2150
	1    0    0    -1  
$EndComp
Text Notes 6100 1400 0    60   ~ 0
GP2Y0A02YK
Text Notes 6100 1500 0    60   ~ 0
GP2Y0A02YK
Text Notes 6100 1600 0    60   ~ 0
GP2Y0A02YK
Text Notes 6100 1700 0    60   ~ 0
GP2Y0A02YK
Text Notes 6100 1800 0    60   ~ 0
GP2D120
Connection ~ 7000 2400
Connection ~ 7150 2400
Connection ~ 7300 2400
Connection ~ 7450 2400
Wire Wire Line
	7000 2400 7000 2650
Wire Wire Line
	7000 2650 3400 2650
Wire Wire Line
	7150 2400 7150 2750
Wire Wire Line
	7150 2750 3400 2750
Wire Wire Line
	7300 2400 7300 2850
Wire Wire Line
	7300 2850 3400 2850
Wire Wire Line
	7450 2400 7450 2950
Wire Wire Line
	7450 2950 3400 2950
Connection ~ 4150 3650
Connection ~ 4250 3650
Connection ~ 4350 3650
Connection ~ 4050 3650
Wire Wire Line
	3400 3650 4050 3650
Wire Wire Line
	3400 3550 4150 3550
Wire Wire Line
	4150 3550 4150 3650
Wire Wire Line
	3400 3450 4250 3450
Wire Wire Line
	4250 3450 4250 3650
Wire Wire Line
	3400 3350 4350 3350
Wire Wire Line
	4350 3350 4350 3650
Connection ~ 7100 3150
Connection ~ 7100 3450
Connection ~ 6600 3150
Connection ~ 6600 3450
Wire Wire Line
	6400 3150 7100 3150
Wire Wire Line
	6400 3450 7100 3450
Connection ~ 7250 3450
Connection ~ 7400 3450
Wire Wire Line
	3400 3150 6250 3150
Wire Wire Line
	6250 3150 6250 3700
Wire Wire Line
	6250 3700 7250 3700
Wire Wire Line
	7250 3700 7250 3450
Wire Wire Line
	3400 3250 6100 3250
Wire Wire Line
	6100 3250 6100 3800
Wire Wire Line
	6100 3800 7400 3800
Wire Wire Line
	7400 3800 7400 3450
Connection ~ 6400 3450
Wire Wire Line
	6400 3150 6400 3050
Wire Wire Line
	6400 3050 5300 3050
Connection ~ 4900 3250
Wire Wire Line
	5300 3550 4900 3550
Connection ~ 6300 2150
Wire Wire Line
	3900 2150 6300 2150
Connection ~ 5300 3050
Wire Wire Line
	5300 2150 5300 3550
Connection ~ 5300 2150
Connection ~ 6700 1350
Connection ~ 6700 1450
Connection ~ 6700 1550
Connection ~ 6700 1650
Connection ~ 6700 1750
Connection ~ 7650 1350
Connection ~ 7650 1450
Connection ~ 7650 1550
Connection ~ 7650 1650
Connection ~ 7650 1750
Connection ~ 6050 1350
Connection ~ 6050 1450
Connection ~ 6050 1550
Connection ~ 6050 1650
Connection ~ 6050 1750
Wire Wire Line
	6700 1350 7650 1350
Wire Wire Line
	6700 1450 7650 1450
Wire Wire Line
	6700 1550 7650 1550
Wire Wire Line
	6700 1650 7650 1650
Wire Wire Line
	6700 1750 7650 1750
Text Label 6750 1350 0    39   ~ 0
Vcc
Text Label 6750 1450 0    39   ~ 0
Vcc
Text Label 6750 1550 0    39   ~ 0
Vcc
Text Label 6750 1650 0    39   ~ 0
Vcc
Text Label 6750 1750 0    39   ~ 0
Vcc
$Comp
L R R?
U 1 1 57FE09D7
P 5600 1350
F 0 "R?" V 5680 1350 50  0001 C CNN
F 1 "18k" V 5600 1350 50  0000 C CNN
F 2 "" V 5530 1350 50  0000 C CNN
F 3 "" H 5600 1350 50  0000 C CNN
	1    5600 1350
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57FE0AA2
P 5600 1450
F 0 "R?" V 5680 1450 50  0001 C CNN
F 1 "18k" V 5600 1450 50  0000 C CNN
F 2 "" V 5530 1450 50  0000 C CNN
F 3 "" H 5600 1450 50  0000 C CNN
	1    5600 1450
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57FE0AB5
P 5600 1550
F 0 "R?" V 5680 1550 50  0001 C CNN
F 1 "18k" V 5600 1550 50  0000 C CNN
F 2 "" V 5530 1550 50  0000 C CNN
F 3 "" H 5600 1550 50  0000 C CNN
	1    5600 1550
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57FE0AC8
P 5600 1650
F 0 "R?" V 5680 1650 50  0001 C CNN
F 1 "18k" V 5600 1650 50  0000 C CNN
F 2 "" V 5530 1650 50  0000 C CNN
F 3 "" H 5600 1650 50  0000 C CNN
	1    5600 1650
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57FE0ADB
P 5600 1750
F 0 "R?" V 5680 1750 50  0001 C CNN
F 1 "18k" V 5600 1750 50  0000 C CNN
F 2 "" V 5530 1750 50  0000 C CNN
F 3 "" H 5600 1750 50  0000 C CNN
	1    5600 1750
	0    1    1    0   
$EndComp
Wire Wire Line
	3400 1350 5450 1350
Wire Wire Line
	3400 1450 5450 1450
Wire Wire Line
	3400 1550 5450 1550
Wire Wire Line
	3400 1650 5450 1650
Wire Wire Line
	3400 1750 5450 1750
Wire Wire Line
	5750 1350 6050 1350
Wire Wire Line
	5750 1450 6050 1450
Wire Wire Line
	5750 1550 6050 1550
Wire Wire Line
	5750 1650 6050 1650
Wire Wire Line
	5750 1750 6050 1750
Connection ~ 3950 1350
$Comp
L C C?
U 1 1 57FE0F05
P 3950 1950
F 0 "C?" H 3975 2050 50  0001 L CNN
F 1 "10nF" H 3975 1850 50  0000 L CNN
F 2 "" H 3988 1800 50  0000 C CNN
F 3 "" H 3950 1950 50  0000 C CNN
	1    3950 1950
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57FE108A
P 4200 1950
F 0 "C?" H 4225 2050 50  0001 L CNN
F 1 "10nF" H 4225 1850 50  0000 L CNN
F 2 "" H 4238 1800 50  0000 C CNN
F 3 "" H 4200 1950 50  0000 C CNN
	1    4200 1950
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57FE10A3
P 4450 1950
F 0 "C?" H 4475 2050 50  0001 L CNN
F 1 "10nF" H 4475 1850 50  0000 L CNN
F 2 "" H 4488 1800 50  0000 C CNN
F 3 "" H 4450 1950 50  0000 C CNN
	1    4450 1950
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57FE10B6
P 4700 1950
F 0 "C?" H 4725 2050 50  0001 L CNN
F 1 "10nF" H 4725 1850 50  0000 L CNN
F 2 "" H 4738 1800 50  0000 C CNN
F 3 "" H 4700 1950 50  0000 C CNN
	1    4700 1950
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57FE10D1
P 4950 1950
F 0 "C?" H 4975 2050 50  0001 L CNN
F 1 "10nF" H 4975 1850 50  0000 L CNN
F 2 "" H 4988 1800 50  0000 C CNN
F 3 "" H 4950 1950 50  0000 C CNN
	1    4950 1950
	1    0    0    -1  
$EndComp
Connection ~ 4200 1450
Connection ~ 4450 1550
Connection ~ 4700 1650
Connection ~ 4950 1750
Wire Wire Line
	3950 1350 3950 1800
Wire Wire Line
	4200 1450 4200 1800
Wire Wire Line
	4450 1550 4450 1800
Wire Wire Line
	4700 1650 4700 1800
Wire Wire Line
	4950 1750 4950 1800
Connection ~ 3950 2150
Connection ~ 4200 2150
Connection ~ 4450 2150
Connection ~ 4700 2150
Connection ~ 4950 2150
Wire Wire Line
	3950 2100 3950 2150
Wire Wire Line
	4200 2100 4200 2150
Wire Wire Line
	4450 2100 4450 2150
Wire Wire Line
	4700 2100 4700 2150
Wire Wire Line
	4950 2100 4950 2150
Connection ~ 3900 2150
Text Notes 7750 1550 0    59   ~ 0
5V
Text Notes 6350 3400 0    60   ~ 0
5V
Text Notes 4250 5100 0    59   ~ 0
AD7820
Connection ~ 4050 4950
Connection ~ 4150 4950
Connection ~ 4250 4950
Connection ~ 4350 4950
Connection ~ 4450 4950
Connection ~ 4550 4950
Connection ~ 4650 4950
Connection ~ 4750 4950
Wire Wire Line
	3400 4750 4050 4750
Wire Wire Line
	4050 4750 4050 4950
Wire Wire Line
	3400 4650 4150 4650
Wire Wire Line
	4150 4650 4150 4950
Wire Wire Line
	3400 4550 4250 4550
Wire Wire Line
	4250 4550 4250 4950
Wire Wire Line
	3400 4450 4350 4450
Wire Wire Line
	4350 4450 4350 4950
Wire Wire Line
	3400 4350 4450 4350
Wire Wire Line
	4450 4350 4450 4950
Wire Wire Line
	3400 4250 4550 4250
Wire Wire Line
	4550 4250 4550 4950
Wire Wire Line
	3400 4150 4650 4150
Wire Wire Line
	4650 4150 4650 4950
Wire Wire Line
	3400 4050 4750 4050
Wire Wire Line
	4750 4050 4750 4950
Text Notes 5150 5350 0    59   ~ 0
Gyro
Connection ~ 4450 5150
Connection ~ 5050 5300
Wire Wire Line
	4450 5150 4450 5300
Wire Wire Line
	4450 5300 5050 5300
Connection ~ 4750 5050
Connection ~ 4750 5150
Wire Wire Line
	3400 3750 4000 3750
Wire Wire Line
	4000 3750 4000 3850
Wire Wire Line
	4000 3850 4400 3850
Wire Wire Line
	4400 3850 4400 3750
Wire Wire Line
	4400 3750 5050 3750
Wire Wire Line
	5050 3750 5050 5150
Wire Wire Line
	5050 5150 4750 5150
Wire Wire Line
	3400 3850 3850 3850
Wire Wire Line
	3850 3850 3850 3950
Wire Wire Line
	3850 3950 4500 3950
Wire Wire Line
	4500 3950 4500 3850
Wire Wire Line
	4500 3850 4950 3850
Wire Wire Line
	4950 3850 4950 5050
Wire Wire Line
	4950 5050 4750 5050
Text Label 4800 5050 0    39   ~ 0
WR
Text Label 4800 5150 0    39   ~ 0
INT
Text Label 4850 5300 0    39   ~ 0
OUT
$EndSCHEMATC
