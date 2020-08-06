EESchema Schematic File Version 4
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
	5600 3700 6000 3700
Wire Wire Line
	6000 3700 6500 3700
Wire Wire Line
	6000 3700 6000 3600
Connection ~ 6000 3700
Wire Wire Line
	5600 4800 6000 4800
Wire Wire Line
	6000 4800 6500 4800
Wire Wire Line
	6000 4800 6000 4700
Connection ~ 6000 4800
Wire Wire Line
	3900 2800 3900 3200
Wire Wire Line
	3900 3200 3000 3200
Wire Wire Line
	3000 3600 3000 3200
Wire Wire Line
	4600 4000 4600 3600
Wire Wire Line
	4600 3600 5000 3600
Wire Wire Line
	4600 4000 4600 4700
Wire Wire Line
	4600 4700 5000 4700
Connection ~ 4600 4000
Wire Wire Line
	3600 3600 3800 3600
Text Label 3800 3600 0    70   ~ 0
5V
Wire Wire Line
	5200 3400 5200 3200
Text Label 5200 3200 1    70   ~ 0
5V
Wire Wire Line
	5200 4500 5200 4400
Text Label 5200 4400 1    70   ~ 0
5V
Text Label 5200 4400 1    70   ~ 0
5V
Wire Wire Line
	4400 4200 4400 4400
Text Label 4400 4400 3    70   ~ 0
5V
Wire Wire Line
	3300 3900 3300 5100
Wire Wire Line
	3300 5100 3300 5300
Wire Wire Line
	3300 5100 3400 5100
Connection ~ 3300 5100
Text Label 3400 5100 0    70   ~ 0
GND
Wire Wire Line
	5300 5100 5300 5200
Text Label 5300 5200 3    70   ~ 0
GND
Wire Wire Line
	5300 4000 5300 4100
Text Label 5300 4100 3    70   ~ 0
GND
Wire Wire Line
	4400 3800 4400 3600
Text Label 4400 3600 1    70   ~ 0
GND
Wire Wire Line
	6900 3700 6900 3800
Text Label 6900 3800 3    70   ~ 0
GND
Wire Wire Line
	6900 4800 6900 4900
Text Label 6900 4900 3    70   ~ 0
GND
Wire Wire Line
	6000 3200 6000 3100
Text Label 6000 3100 1    70   ~ 0
GND
Wire Wire Line
	6000 4300 6000 4200
Text Label 6000 4200 1    70   ~ 0
GND
Wire Wire Line
	6900 3500 7200 3500
Wire Wire Line
	6900 4600 7200 4600
$Comp
L brakelight-eagle-import:LM311N IC1
U 1 1 22F7723E
P 5300 3700
F 0 "IC1" H 5400 3925 59  0000 L BNN
F 1 "LM311N" H 5400 3500 59  0000 L BNN
F 2 "Package_DIP:DIP-8-16_W7.62mm" H 5300 3700 50  0001 C CNN
F 3 "" H 5300 3700 50  0001 C CNN
	1    5300 3700
	1    0    0    -1  
$EndComp
$Comp
L brakelight-eagle-import:LM311N IC2
U 1 1 BC34DC86
P 5300 4800
F 0 "IC2" H 5400 5025 59  0000 L BNN
F 1 "LM311N" H 5400 4600 59  0000 L BNN
F 2 "Package_DIP:DIP-8-16_W7.62mm" H 5300 4800 50  0001 C CNN
F 3 "" H 5300 4800 50  0001 C CNN
	1    5300 4800
	1    0    0    -1  
$EndComp
$Comp
L brakelight-eagle-import:SUP75N06-08 Q1
U 1 1 22BC07EB
P 6700 3600
F 0 "Q1" H 6250 3700 59  0000 L BNN
F 1 "SUP75N06-08" H 6250 3600 59  0000 L BNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 6700 3600 50  0001 C CNN
F 3 "" H 6700 3600 50  0001 C CNN
	1    6700 3600
	1    0    0    -1  
$EndComp
$Comp
L brakelight-eagle-import:SUP75N06-08 Q2
U 1 1 FC4D4FCD
P 6700 4700
F 0 "Q2" H 6250 4800 59  0000 L BNN
F 1 "SUP75N06-08" H 6250 4700 59  0000 L BNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 6700 4700 50  0001 C CNN
F 3 "" H 6700 4700 50  0001 C CNN
	1    6700 4700
	1    0    0    -1  
$EndComp
$Comp
L brakelight-eagle-import:78XXL IC3
U 1 1 B686635D
P 3300 3600
F 0 "IC3" H 3400 3300 59  0000 L BNN
F 1 "78XXL" H 3400 3200 59  0000 L BNN
F 2 "Package_TO_SOT_THT:TO-220-3_Horizontal_TabDown" H 3300 3600 50  0001 C CNN
F 3 "" H 3300 3600 50  0001 C CNN
	1    3300 3600
	1    0    0    -1  
$EndComp
$Comp
L brakelight-eagle-import:V+ #P+01
U 1 1 6A081B6E
P 3900 2700
F 0 "#P+01" H 3900 2700 50  0001 C CNN
F 1 "V+" V 3800 2600 59  0000 L BNN
F 2 "" H 3900 2700 50  0001 C CNN
F 3 "" H 3900 2700 50  0001 C CNN
	1    3900 2700
	1    0    0    -1  
$EndComp
$Comp
L brakelight-eagle-import:GND #GND01
U 1 1 8DA2BB24
P 3300 5400
F 0 "#GND01" H 3300 5400 50  0001 C CNN
F 1 "GND" H 3200 5300 59  0000 L BNN
F 2 "" H 3300 5400 50  0001 C CNN
F 3 "" H 3300 5400 50  0001 C CNN
	1    3300 5400
	1    0    0    -1  
$EndComp
$Comp
L brakelight-eagle-import:R-TRIMM74W R1
U 1 1 54673866
P 4400 4000
F 0 "R1" V 4165 3850 59  0000 L BNN
F 1 "R-TRIMM74W" V 4250 3850 59  0000 L BNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 4400 4000 50  0001 C CNN
F 3 "" H 4400 4000 50  0001 C CNN
	1    4400 4000
	1    0    0    -1  
$EndComp
$Comp
L brakelight-eagle-import:R-US_0204_2V R2
U 1 1 7B6FF4A5
P 6000 3400
F 0 "R2" H 5850 3459 59  0000 L BNN
F 1 "100K" H 5850 3270 59  0000 L BNN
F 2 "CustomKiCADLibrary:PinHeader_1x03_P5.08mm_Vertical" H 6000 3400 50  0001 C CNN
F 3 "" H 6000 3400 50  0001 C CNN
	1    6000 3400
	0    -1   -1   0   
$EndComp
$Comp
L brakelight-eagle-import:R-US_0204_2V R3
U 1 1 5E3BF956
P 6000 4500
F 0 "R3" H 5850 4559 59  0000 L BNN
F 1 "100K" H 5850 4370 59  0000 L BNN
F 2 "CustomKiCADLibrary:PinHeader_1x03_P5.08mm_Vertical" H 6000 4500 50  0001 C CNN
F 3 "" H 6000 4500 50  0001 C CNN
	1    6000 4500
	0    -1   -1   0   
$EndComp
$Comp
L brakelight-eagle-import:PINHD-1X2 JP1
U 1 1 0B60073D
P 8262 4303
F 0 "JP1" H 8012 4528 59  0000 L BNN
F 1 "PINHD-1X2" H 8012 4103 59  0000 L BNN
F 2 "CustomKiCADLibrary:PinHeader_1x02_P5.08mm_Vertical" H 8262 4303 50  0001 C CNN
F 3 "" H 8262 4303 50  0001 C CNN
	1    8262 4303
	1    0    0    -1  
$EndComp
Text GLabel 5200 4000 0    10   BiDi ~ 0
V-
Text GLabel 5200 5100 0    10   BiDi ~ 0
V-
$EndSCHEMATC