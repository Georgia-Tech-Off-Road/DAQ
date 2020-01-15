EESchema Schematic File Version 4
LIBS:MainDataAquisitionBoard-cache
EELAYER 30 0
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
Wire Wire Line
	4450 3050 4800 3050
Wire Wire Line
	4450 3150 4800 3150
Wire Wire Line
	4450 3250 4800 3250
Wire Wire Line
	4450 3350 4800 3350
Wire Wire Line
	4450 3450 4800 3450
Wire Wire Line
	4450 3550 4800 3550
Wire Wire Line
	4450 3650 4800 3650
Wire Wire Line
	4450 3750 4800 3750
Wire Wire Line
	4450 3850 4800 3850
Wire Wire Line
	4450 3950 4800 3950
Wire Wire Line
	4450 4150 4800 4150
Wire Wire Line
	4450 4250 4800 4250
Wire Wire Line
	5600 3050 5950 3050
Wire Wire Line
	5600 3150 5950 3150
Wire Wire Line
	5600 3250 5950 3250
Wire Wire Line
	5600 3350 5950 3350
Wire Wire Line
	5600 3450 5950 3450
Wire Wire Line
	5600 3550 5950 3550
Wire Wire Line
	5600 3850 5950 3850
Wire Wire Line
	5600 3950 5950 3950
Wire Wire Line
	5600 4050 5950 4050
Wire Wire Line
	5600 4250 5950 4250
Wire Wire Line
	6600 4150 6600 4100
Wire Wire Line
	5600 3650 5950 3650
Wire Wire Line
	5600 3750 5950 3750
Wire Wire Line
	4450 4050 4800 4050
Wire Wire Line
	4300 4300 6100 4300
$Comp
L Connector_Generic:Conn_01x01 J2
U 1 1 5DCE29A1
P 5600 5800
F 0 "J2" H 5680 5842 50  0000 L CNN
F 1 "Conn_01x01" H 5680 5751 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 5600 5800 50  0001 C CNN
F 3 "~" H 5600 5800 50  0001 C CNN
	1    5600 5800
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J3
U 1 1 5DCE4593
P 5600 6500
F 0 "J3" H 5680 6542 50  0000 L CNN
F 1 "Conn_01x01" H 5680 6451 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 5600 6500 50  0001 C CNN
F 3 "~" H 5600 6500 50  0001 C CNN
	1    5600 6500
	1    0    0    -1  
$EndComp
NoConn ~ 5400 5800
NoConn ~ 5400 6500
Wire Wire Line
	4450 4350 4800 4350
Wire Wire Line
	4450 4450 4800 4450
Wire Wire Line
	4450 4550 4800 4550
Wire Wire Line
	4450 4650 4800 4650
Wire Wire Line
	4450 4750 4800 4750
Wire Wire Line
	4450 4850 4800 4850
Wire Wire Line
	4450 4950 4800 4950
Wire Wire Line
	4450 5050 4800 5050
Wire Wire Line
	4450 5150 4800 5150
Wire Wire Line
	4450 5250 4800 5250
Wire Wire Line
	5600 4350 5950 4350
Wire Wire Line
	5600 4450 5950 4450
Wire Wire Line
	5600 4550 5950 4550
Wire Wire Line
	5600 4650 5950 4650
Wire Wire Line
	5600 4750 5950 4750
Wire Wire Line
	5600 4850 5950 4850
Wire Wire Line
	5600 4950 5950 4950
Wire Wire Line
	5600 5050 5950 5050
Wire Wire Line
	5600 5150 5950 5150
Wire Wire Line
	5600 5250 5950 5250
$Comp
L Connector_Generic:Conn_01x04 J12
U 1 1 5DD1CAD9
P 8000 3050
F 0 "J12" H 8080 3042 50  0000 L CNN
F 1 "Hall Effect" H 8080 2951 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8000 3050 50  0001 C CNN
F 3 "~" H 8000 3050 50  0001 C CNN
	1    8000 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 2950 4450 2950
Wire Wire Line
	5600 2950 5950 2950
Wire Wire Line
	5600 4150 5950 4150
$Comp
L Connector_Generic:Conn_01x24 J5
U 1 1 5DD20EDA
P 5000 4050
F 0 "J5" H 4950 5450 50  0000 L CNN
F 1 "Teensy 3.6L" H 4800 5300 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x24_P2.54mm_Vertical" H 5000 4050 50  0001 C CNN
F 3 "~" H 5000 4050 50  0001 C CNN
	1    5000 4050
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x24 J4
U 1 1 5DD21E75
P 4650 4050
F 0 "J4" H 4600 5400 50  0000 L CNN
F 1 "1x24" H 4350 5300 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x24_P2.54mm_Vertical" H 4650 4050 50  0001 C CNN
F 3 "~" H 4650 4050 50  0001 C CNN
	1    4650 4050
	1    0    0    -1  
$EndComp
Connection ~ 4450 2950
Wire Wire Line
	4450 2950 4800 2950
$Comp
L Connector_Generic:Conn_01x24 J6
U 1 1 5DD23B4D
P 5400 4050
F 0 "J6" H 5318 5367 50  0000 C CNN
F 1 "Teensy 3.6R" H 5318 5276 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x24_P2.54mm_Vertical" H 5400 4050 50  0001 C CNN
F 3 "~" H 5400 4050 50  0001 C CNN
	1    5400 4050
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x24 J7
U 1 1 5DD26121
P 5750 4050
F 0 "J7" H 5668 5367 50  0000 C CNN
F 1 "1x24" H 5668 5276 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x24_P2.54mm_Vertical" H 5750 4050 50  0001 C CNN
F 3 "~" H 5750 4050 50  0001 C CNN
	1    5750 4050
	-1   0    0    -1  
$EndComp
Connection ~ 5950 2950
$Comp
L Device:R R2
U 1 1 5DD2B190
P 7400 3650
F 0 "R2" V 7193 3650 50  0000 C CNN
F 1 "8.2k" V 7284 3650 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7330 3650 50  0001 C CNN
F 3 "~" H 7400 3650 50  0001 C CNN
	1    7400 3650
	0    1    1    0   
$EndComp
$Comp
L Device:R R1
U 1 1 5DD2BDB5
P 7400 3350
F 0 "R1" V 7193 3350 50  0000 C CNN
F 1 "5.1k" V 7284 3350 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7330 3350 50  0001 C CNN
F 3 "~" H 7400 3350 50  0001 C CNN
	1    7400 3350
	0    1    1    0   
$EndComp
Connection ~ 5950 3150
Wire Wire Line
	6100 4300 6100 4550
Wire Wire Line
	7150 4550 7150 4450
Wire Wire Line
	7150 3050 7800 3050
Wire Wire Line
	7250 3350 7250 2950
Connection ~ 7250 2950
Wire Wire Line
	7250 2950 7800 2950
Wire Wire Line
	7250 3650 7150 3650
Connection ~ 7150 3650
Wire Wire Line
	7150 3650 7150 3050
Wire Wire Line
	7550 3350 7550 3550
$Comp
L Connector_Generic:Conn_01x08 J1
U 1 1 5DCE18DD
P 4900 6100
F 0 "J1" H 4980 6092 50  0000 L CNN
F 1 "Motor Driver" H 4980 6001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 4900 6100 50  0001 C CNN
F 3 "~" H 4900 6100 50  0001 C CNN
	1    4900 6100
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x08 J15
U 1 1 5DDF828E
P 4500 6100
F 0 "J15" H 4580 6092 50  0000 L CNN
F 1 "Conn_01x08" H 4580 6001 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x08_P2.54mm_Vertical" H 4500 6100 50  0001 C CNN
F 3 "~" H 4500 6100 50  0001 C CNN
	1    4500 6100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 5800 4700 5800
Wire Wire Line
	4300 5900 4700 5900
Wire Wire Line
	4300 6000 4700 6000
Wire Wire Line
	4300 6100 4700 6100
Wire Wire Line
	4300 6200 4700 6200
Wire Wire Line
	4300 6300 4700 6300
Wire Wire Line
	4300 6400 4700 6400
Wire Wire Line
	4300 6500 4700 6500
Connection ~ 4300 2950
Wire Wire Line
	4300 2950 4300 4300
Wire Wire Line
	4300 5800 4300 5700
Connection ~ 4300 5800
Connection ~ 4300 4300
$Comp
L Regulator_Linear:L7805 U1
U 1 1 5DE3C705
P 4850 1750
F 0 "U1" H 4850 1992 50  0000 C CNN
F 1 "L7805" H 4850 1901 50  0000 C CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 4875 1600 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 4850 1700 50  0001 C CNN
	1    4850 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 2200 4550 2200
Wire Wire Line
	4550 2200 4550 1750
Wire Wire Line
	4850 2300 4850 2050
Wire Wire Line
	4300 2300 4300 2950
Wire Wire Line
	4300 2300 4850 2300
Wire Wire Line
	5150 1750 7250 1750
Wire Wire Line
	4450 3250 4200 3250
Wire Wire Line
	4200 3250 4200 5900
Wire Wire Line
	4200 5900 4300 5900
Connection ~ 4450 3250
Connection ~ 4300 5900
Wire Wire Line
	4450 3350 4100 3350
Wire Wire Line
	4100 3350 4100 6000
Wire Wire Line
	4100 6000 4300 6000
Connection ~ 4450 3350
Connection ~ 4300 6000
$Comp
L Connector_Generic:Conn_01x02 J16
U 1 1 5DE62186
P 5000 5500
F 0 "J16" V 4964 5312 50  0000 R CNN
F 1 "MotorDriver2" V 4873 5312 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 5000 5500 50  0001 C CNN
F 3 "~" H 5000 5500 50  0001 C CNN
	1    5000 5500
	0    -1   -1   0   
$EndComp
Connection ~ 4300 5700
Wire Wire Line
	4300 5700 4300 4300
Wire Wire Line
	4300 5700 5000 5700
Wire Wire Line
	5100 5700 7050 5700
Wire Wire Line
	7050 5700 7050 3150
Wire Wire Line
	7550 3150 7550 3350
Wire Wire Line
	7550 3150 7800 3150
Connection ~ 7550 3350
Wire Wire Line
	7250 1750 7250 2950
$Comp
L Device:R R3
U 1 1 5DD277A0
P 7400 3950
F 0 "R3" V 7193 3950 50  0000 C CNN
F 1 "5.1k" V 7284 3950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7330 3950 50  0001 C CNN
F 3 "~" H 7400 3950 50  0001 C CNN
	1    7400 3950
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 5DD27B35
P 7400 4250
F 0 "R4" V 7193 4250 50  0000 C CNN
F 1 "8.2k" V 7284 4250 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7330 4250 50  0001 C CNN
F 3 "~" H 7400 4250 50  0001 C CNN
	1    7400 4250
	0    1    1    0   
$EndComp
$Comp
L Device:R R5
U 1 1 5DD27EB6
P 7400 4550
F 0 "R5" V 7193 4550 50  0000 C CNN
F 1 "5.1k" V 7284 4550 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7330 4550 50  0001 C CNN
F 3 "~" H 7400 4550 50  0001 C CNN
	1    7400 4550
	0    1    1    0   
$EndComp
$Comp
L Device:R R6
U 1 1 5DD28541
P 7400 4850
F 0 "R6" V 7193 4850 50  0000 C CNN
F 1 "8.2k" V 7284 4850 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7330 4850 50  0001 C CNN
F 3 "~" H 7400 4850 50  0001 C CNN
	1    7400 4850
	0    1    1    0   
$EndComp
$Comp
L Device:R R7
U 1 1 5DD287BC
P 7400 5150
F 0 "R7" V 7193 5150 50  0000 C CNN
F 1 "5.1k" V 7284 5150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7330 5150 50  0001 C CNN
F 3 "~" H 7400 5150 50  0001 C CNN
	1    7400 5150
	0    1    1    0   
$EndComp
$Comp
L Device:R R8
U 1 1 5DD289F0
P 7400 5450
F 0 "R8" V 7193 5450 50  0000 C CNN
F 1 "8.2k" V 7284 5450 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal" V 7330 5450 50  0001 C CNN
F 3 "~" H 7400 5450 50  0001 C CNN
	1    7400 5450
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x04 J13
U 1 1 5DD28CE4
P 8000 4450
F 0 "J13" H 8080 4442 50  0000 L CNN
F 1 "Hall Effect 2" H 8080 4351 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 8000 4450 50  0001 C CNN
F 3 "~" H 8000 4450 50  0001 C CNN
	1    8000 4450
	1    0    0    -1  
$EndComp
Wire Wire Line
	7150 4550 7150 4850
Wire Wire Line
	7150 5450 7250 5450
Connection ~ 7150 4550
Wire Wire Line
	7150 4850 7250 4850
Connection ~ 7150 4850
Wire Wire Line
	7150 4850 7150 5450
Wire Wire Line
	7150 4250 7250 4250
Connection ~ 7150 4250
Wire Wire Line
	7150 4250 7150 3650
Connection ~ 7250 3350
Wire Wire Line
	7250 3350 7250 3950
Connection ~ 7250 3950
Connection ~ 7250 4550
Wire Wire Line
	7250 4550 7250 5150
Wire Wire Line
	7250 3950 7250 4350
Wire Wire Line
	7250 4350 7800 4350
Connection ~ 7250 4350
Wire Wire Line
	7250 4350 7250 4550
Wire Wire Line
	7150 4450 7800 4450
Connection ~ 7150 4450
Wire Wire Line
	7150 4450 7150 4250
Wire Wire Line
	7800 4550 7550 4550
Wire Wire Line
	7550 4850 7550 4550
Connection ~ 7550 4550
Wire Wire Line
	7800 3950 7550 3950
Wire Wire Line
	7800 3250 7800 3750
Wire Wire Line
	7550 3950 7550 4250
Connection ~ 7550 3950
Wire Wire Line
	7800 4650 7800 5000
Wire Wire Line
	7800 5150 7550 5150
Connection ~ 7550 5150
Wire Wire Line
	7550 5450 7550 5150
Wire Wire Line
	5950 3550 7550 3550
Connection ~ 5950 3550
Connection ~ 7550 3550
Wire Wire Line
	7550 3550 7550 3650
Wire Wire Line
	5950 3650 6900 3650
Wire Wire Line
	6900 3650 6900 3750
Wire Wire Line
	6900 3750 7800 3750
Connection ~ 5950 3650
Connection ~ 7800 3750
Wire Wire Line
	7800 3750 7800 3950
Wire Wire Line
	5950 3950 6200 3950
Wire Wire Line
	6200 3950 6200 4950
Wire Wire Line
	6200 4950 7550 4950
Wire Wire Line
	7550 4950 7550 4850
Connection ~ 5950 3950
Connection ~ 7550 4850
Wire Wire Line
	5950 4050 6050 4050
Wire Wire Line
	6050 4050 6050 5000
Wire Wire Line
	6050 5000 7800 5000
Connection ~ 5950 4050
Connection ~ 7800 5000
Wire Wire Line
	7800 5000 7800 5150
$Comp
L Connector_Generic:Conn_01x01 J14
U 1 1 5DDB2E37
P 4000 2200
F 0 "J14" H 3800 2150 50  0000 C CNN
F 1 "12V In" H 4200 2200 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 4000 2200 50  0001 C CNN
F 3 "~" H 4000 2200 50  0001 C CNN
	1    4000 2200
	-1   0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x01 J17
U 1 1 5DDB375B
P 4000 2300
F 0 "J17" H 3800 2250 50  0000 C CNN
F 1 "GND In" H 4200 2300 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical" H 4000 2300 50  0001 C CNN
F 3 "~" H 4000 2300 50  0001 C CNN
	1    4000 2300
	-1   0    0    1   
$EndComp
Wire Wire Line
	5950 3150 7050 3150
Wire Wire Line
	6100 4550 7150 4550
Wire Wire Line
	5950 2950 7250 2950
$EndSCHEMATC
