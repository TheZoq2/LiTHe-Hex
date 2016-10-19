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
LIBS:sensor-cache
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
P 3500 3800
F 0 "IC?" H 2650 5680 50  0001 L BNN
F 1 "ATMEGA1284-P" H 3900 1850 50  0000 L BNN
F 2 "DIL40" H 3500 3800 50  0000 C CIN
F 3 "" H 3500 3800 50  0000 C CNN
	1    3500 3800
	1    0    0    -1  
$EndComp
Text Notes 8000 3050 0    60   ~ 0
Centralenheten
Text Notes 6600 3400 0    60   Italic 0
SPI
Text Notes 5200 4550 0    60   ~ 0
JTAG
Text Notes 8300 4100 0    60   ~ 0
LIDAR
$Comp
L C 680µF
U 1 1 57FDF7AD
P 7700 4050
F 0 "680µF" H 7725 4150 50  0000 L CNN
F 1 "C" H 7725 3950 50  0001 L CNN
F 2 "" H 7738 3900 50  0000 C CNN
F 3 "" H 7700 4050 50  0000 C CNN
	1    7700 4050
	1    0    0    -1  
$EndComp
Text Label 8000 3900 0    39   ~ 0
GND
Text Label 8000 4200 0    39   ~ 0
Vcc
Text Label 8100 4450 0    39   ~ 0
SDA
Text Label 8100 4550 0    39   ~ 0
SCL
$Comp
L R R?
U 1 1 57FDFADA
P 6000 4150
F 0 "R?" V 6080 4150 50  0001 C CNN
F 1 "4.7k" V 6000 4150 50  0000 C CNN
F 2 "" V 5930 4150 50  0000 C CNN
F 3 "" H 6000 4150 50  0000 C CNN
	1    6000 4150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR?
U 1 1 57FDFF7C
P 7400 2900
F 0 "#PWR?" H 7400 2650 50  0001 C CNN
F 1 "GND" H 7400 2750 50  0000 C CNN
F 2 "" H 7400 2900 50  0000 C CNN
F 3 "" H 7400 2900 50  0000 C CNN
	1    7400 2900
	1    0    0    -1  
$EndComp
Text Notes 7200 2150 0    60   ~ 0
GP2Y0A02YK
Text Notes 7200 2250 0    60   ~ 0
GP2Y0A02YK
Text Notes 7200 2350 0    60   ~ 0
GP2Y0A02YK
Text Notes 7200 2450 0    60   ~ 0
GP2Y0A02YK
Text Notes 7200 2550 0    60   ~ 0
GP2D120
Text Label 7850 2100 0    39   ~ 0
Vcc
Text Label 7850 2200 0    39   ~ 0
Vcc
Text Label 7850 2300 0    39   ~ 0
Vcc
Text Label 7850 2400 0    39   ~ 0
Vcc
Text Label 7850 2500 0    39   ~ 0
Vcc
$Comp
L R R?
U 1 1 57FE09D7
P 6700 2100
F 0 "R?" V 6780 2100 50  0001 C CNN
F 1 "18k" V 6700 2100 50  0000 C CNN
F 2 "" V 6630 2100 50  0000 C CNN
F 3 "" H 6700 2100 50  0000 C CNN
	1    6700 2100
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57FE0AA2
P 6700 2200
F 0 "R?" V 6780 2200 50  0001 C CNN
F 1 "18k" V 6700 2200 50  0000 C CNN
F 2 "" V 6630 2200 50  0000 C CNN
F 3 "" H 6700 2200 50  0000 C CNN
	1    6700 2200
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57FE0AB5
P 6700 2300
F 0 "R?" V 6780 2300 50  0001 C CNN
F 1 "18k" V 6700 2300 50  0000 C CNN
F 2 "" V 6630 2300 50  0000 C CNN
F 3 "" H 6700 2300 50  0000 C CNN
	1    6700 2300
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57FE0AC8
P 6700 2400
F 0 "R?" V 6780 2400 50  0001 C CNN
F 1 "18k" V 6700 2400 50  0000 C CNN
F 2 "" V 6630 2400 50  0000 C CNN
F 3 "" H 6700 2400 50  0000 C CNN
	1    6700 2400
	0    1    1    0   
$EndComp
$Comp
L R R?
U 1 1 57FE0ADB
P 6700 2500
F 0 "R?" V 6780 2500 50  0001 C CNN
F 1 "18k" V 6700 2500 50  0000 C CNN
F 2 "" V 6630 2500 50  0000 C CNN
F 3 "" H 6700 2500 50  0000 C CNN
	1    6700 2500
	0    1    1    0   
$EndComp
$Comp
L C C?
U 1 1 57FE0F05
P 5050 2700
F 0 "C?" H 5075 2800 50  0001 L CNN
F 1 "10nF" H 5075 2600 50  0000 L CNN
F 2 "" H 5088 2550 50  0000 C CNN
F 3 "" H 5050 2700 50  0000 C CNN
	1    5050 2700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57FE108A
P 5300 2700
F 0 "C?" H 5325 2800 50  0001 L CNN
F 1 "10nF" H 5325 2600 50  0000 L CNN
F 2 "" H 5338 2550 50  0000 C CNN
F 3 "" H 5300 2700 50  0000 C CNN
	1    5300 2700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57FE10A3
P 5550 2700
F 0 "C?" H 5575 2800 50  0001 L CNN
F 1 "10nF" H 5575 2600 50  0000 L CNN
F 2 "" H 5588 2550 50  0000 C CNN
F 3 "" H 5550 2700 50  0000 C CNN
	1    5550 2700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57FE10B6
P 5800 2700
F 0 "C?" H 5825 2800 50  0001 L CNN
F 1 "10nF" H 5825 2600 50  0000 L CNN
F 2 "" H 5838 2550 50  0000 C CNN
F 3 "" H 5800 2700 50  0000 C CNN
	1    5800 2700
	1    0    0    -1  
$EndComp
$Comp
L C C?
U 1 1 57FE10D1
P 6050 2700
F 0 "C?" H 6075 2800 50  0001 L CNN
F 1 "10nF" H 6075 2600 50  0000 L CNN
F 2 "" H 6088 2550 50  0000 C CNN
F 3 "" H 6050 2700 50  0000 C CNN
	1    6050 2700
	1    0    0    -1  
$EndComp
Text Notes 7450 4150 0    60   ~ 0
5V
Text Label 8600 2100 0    39   ~ 0
PD0
Text Label 8600 2200 0    39   ~ 0
PD1
Text Label 8600 2300 0    39   ~ 0
PD2
Text Label 8600 2400 0    39   ~ 0
PD3
Text Label 8600 2500 0    39   ~ 0
PD4
Text Notes 8800 3900 0    60   ~ 0
PD0...PD4
Text Notes 4600 1850 0    60   ~ 0
Gyro
Connection ~ 8100 3150
Connection ~ 8250 3150
Connection ~ 8400 3150
Connection ~ 8550 3150
Wire Wire Line
	8100 3150 8100 3400
Wire Wire Line
	8100 3400 4500 3400
Wire Wire Line
	8250 3150 8250 3500
Wire Wire Line
	8250 3500 4500 3500
Wire Wire Line
	8400 3150 8400 3600
Wire Wire Line
	8400 3600 4500 3600
Wire Wire Line
	8550 3150 8550 3700
Wire Wire Line
	8550 3700 4500 3700
Connection ~ 5250 4400
Connection ~ 5350 4400
Connection ~ 5450 4400
Connection ~ 5150 4400
Wire Wire Line
	4500 4400 5150 4400
Wire Wire Line
	4500 4300 5250 4300
Wire Wire Line
	5250 4300 5250 4400
Wire Wire Line
	4500 4200 5350 4200
Wire Wire Line
	5350 4200 5350 4400
Wire Wire Line
	4500 4100 5450 4100
Wire Wire Line
	5450 4100 5450 4400
Connection ~ 8200 3900
Connection ~ 8200 4200
Connection ~ 7700 3900
Connection ~ 7700 4200
Wire Wire Line
	7500 3900 8200 3900
Wire Wire Line
	7500 4200 8200 4200
Connection ~ 8350 4200
Connection ~ 8500 4200
Wire Wire Line
	4500 3900 7350 3900
Wire Wire Line
	7350 3900 7350 4450
Wire Wire Line
	7350 4450 8350 4450
Wire Wire Line
	8350 4450 8350 4200
Wire Wire Line
	4500 4000 7200 4000
Wire Wire Line
	7200 4000 7200 4550
Wire Wire Line
	7200 4550 8500 4550
Wire Wire Line
	8500 4550 8500 4200
Connection ~ 7500 4200
Wire Wire Line
	7500 3900 7500 3800
Wire Wire Line
	7500 3800 6400 3800
Connection ~ 6000 4000
Wire Wire Line
	6400 4300 6000 4300
Connection ~ 7400 2900
Wire Wire Line
	5000 2900 7400 2900
Connection ~ 6400 3800
Wire Wire Line
	6400 2900 6400 4300
Connection ~ 6400 2900
Connection ~ 7800 2100
Connection ~ 7800 2200
Connection ~ 7800 2300
Connection ~ 7800 2400
Connection ~ 7800 2500
Connection ~ 8750 2100
Connection ~ 8750 2200
Connection ~ 8750 2300
Connection ~ 8750 2400
Connection ~ 8750 2500
Connection ~ 7150 2100
Connection ~ 7150 2200
Connection ~ 7150 2300
Connection ~ 7150 2400
Connection ~ 7150 2500
Wire Wire Line
	7800 2100 8750 2100
Wire Wire Line
	7800 2200 8750 2200
Wire Wire Line
	7800 2300 8750 2300
Wire Wire Line
	7800 2400 8750 2400
Wire Wire Line
	7800 2500 8750 2500
Wire Wire Line
	4500 2100 6550 2100
Wire Wire Line
	4500 2200 6550 2200
Wire Wire Line
	4500 2300 6550 2300
Wire Wire Line
	4500 2400 6550 2400
Wire Wire Line
	4500 2500 6550 2500
Wire Wire Line
	6850 2100 7150 2100
Wire Wire Line
	6850 2200 7150 2200
Wire Wire Line
	6850 2300 7150 2300
Wire Wire Line
	6850 2400 7150 2400
Wire Wire Line
	6850 2500 7150 2500
Connection ~ 5050 2100
Connection ~ 5300 2200
Connection ~ 5550 2300
Connection ~ 5800 2400
Connection ~ 6050 2500
Wire Wire Line
	5050 2100 5050 2550
Wire Wire Line
	5300 2200 5300 2550
Wire Wire Line
	5550 2300 5550 2550
Wire Wire Line
	5800 2400 5800 2550
Wire Wire Line
	6050 2500 6050 2550
Connection ~ 5050 2900
Connection ~ 5300 2900
Connection ~ 5550 2900
Connection ~ 5800 2900
Connection ~ 6050 2900
Wire Wire Line
	5050 2850 5050 2900
Wire Wire Line
	5300 2850 5300 2900
Wire Wire Line
	5550 2850 5550 2900
Wire Wire Line
	5800 2850 5800 2900
Wire Wire Line
	6050 2850 6050 2900
Connection ~ 5000 2900
Connection ~ 8750 1950
Wire Bus Line
	8750 1950 8750 5650
Connection ~ 8750 5650
Wire Wire Line
	4500 4800 8750 4800
Wire Wire Line
	4500 4900 8750 4900
Wire Wire Line
	4500 5000 8750 5000
Wire Wire Line
	4500 5100 8750 5100
Wire Wire Line
	4500 5200 8750 5200
Connection ~ 8750 4800
Connection ~ 8750 4900
Connection ~ 8750 5000
Connection ~ 8750 5100
Connection ~ 8750 5200
Wire Wire Line
	4500 2600 4700 2600
Wire Wire Line
	4700 2600 4700 1900
Connection ~ 4700 1900
Text Notes 5300 1850 0    60   ~ 0
5V
Connection ~ 5250 1800
Connection ~ 4850 1800
Wire Wire Line
	4850 1800 5250 1800
Text Notes 1600 2150 0    60   ~ 0
Reset
Connection ~ 1900 2100
Wire Wire Line
	1900 2100 2500 2100
$EndSCHEMATC
