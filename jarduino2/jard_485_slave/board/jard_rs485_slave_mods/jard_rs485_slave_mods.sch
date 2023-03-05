EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Module RS485 Oya"
Date "2023-03-05"
Rev "A0"
Comp "BPC"
Comment1 "Module de contrôle d'un oya"
Comment2 "Technologie avec modules Arduino"
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector:Screw_Terminal_01x04 J1
U 1 1 6382A878
P 1000 1400
F 0 "J1" H 918 975 50  0000 C CNN
F 1 "NETWORK IN" H 918 1066 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-4-2.54_1x04_P2.54mm_Horizontal" H 1000 1400 50  0001 C CNN
F 3 "~" H 1000 1400 50  0001 C CNN
	1    1000 1400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1200 1600 1350 1600
Wire Wire Line
	1350 1600 1350 1700
Wire Wire Line
	1350 1700 1900 1700
Wire Wire Line
	1200 1300 1300 1300
Wire Wire Line
	1200 1400 1700 1400
$Comp
L Connector:Screw_Terminal_01x04 J2
U 1 1 6383A568
P 1000 2400
F 0 "J2" H 918 1975 50  0000 C CNN
F 1 "NETWORK OUT" H 918 2066 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-4-2.54_1x04_P2.54mm_Horizontal" H 1000 2400 50  0001 C CNN
F 3 "~" H 1000 2400 50  0001 C CNN
	1    1000 2400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1200 2300 1600 2300
Wire Wire Line
	1600 2300 1600 1300
Wire Wire Line
	1700 1400 1700 2400
Wire Wire Line
	1700 2400 1200 2400
Wire Wire Line
	1200 2500 1800 2500
Wire Wire Line
	1900 1700 1900 2600
Wire Wire Line
	1900 2600 1800 2600
Wire Wire Line
	1800 2500 1800 1500
Wire Wire Line
	1800 1500 1200 1500
$Comp
L Connector:Screw_Terminal_01x02 J7
U 1 1 63853958
P 9900 2250
F 0 "J7" H 9980 2242 50  0000 L CNN
F 1 "Electrovanne 12V" H 9980 2151 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-2-2.54_1x02_P2.54mm_Horizontal" H 9900 2250 50  0001 C CNN
F 3 "~" H 9900 2250 50  0001 C CNN
	1    9900 2250
	1    0    0    1   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J6
U 1 1 638553D9
P 9700 5700
F 0 "J6" H 9780 5692 50  0000 L CNN
F 1 "Niveau" H 9780 5601 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-3-2.54_1x03_P2.54mm_Horizontal" H 9700 5700 50  0001 C CNN
F 3 "~" H 9700 5700 50  0001 C CNN
	1    9700 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 638614E8
P 8950 6000
F 0 "#PWR011" H 8950 5750 50  0001 C CNN
F 1 "GND" H 8955 5827 50  0000 C CNN
F 2 "" H 8950 6000 50  0001 C CNN
F 3 "" H 8950 6000 50  0001 C CNN
	1    8950 6000
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 63884FD3
P 1400 5850
F 0 "J3" V 1272 5930 50  0000 L CNN
F 1 "LED" V 1363 5930 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1400 5850 50  0001 C CNN
F 3 "~" H 1400 5850 50  0001 C CNN
	1    1400 5850
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 63886143
P 1700 5450
F 0 "R1" H 1770 5496 50  0000 L CNN
F 1 "150" H 1770 5405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 1630 5450 50  0001 C CNN
F 3 "~" H 1700 5450 50  0001 C CNN
	1    1700 5450
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 6388CA92
P 1700 6150
F 0 "#PWR01" H 1700 5900 50  0001 C CNN
F 1 "GND" V 1705 6022 50  0000 R CNN
F 2 "" H 1700 6150 50  0001 C CNN
F 3 "" H 1700 6150 50  0001 C CNN
	1    1700 6150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1800 1500 2100 1500
Wire Wire Line
	2100 1500 2100 3600
Connection ~ 1800 1500
Wire Wire Line
	2500 3500 2250 3500
Wire Wire Line
	2250 3500 2250 1400
Wire Wire Line
	2250 1400 1700 1400
Connection ~ 1700 1400
Text GLabel 5200 3700 1    50   Output ~ 0
Vcc
Text GLabel 8600 5700 0    50   Output ~ 0
IN_LVL_HIGH
Text GLabel 8600 5600 0    50   Output ~ 0
IN_LVL_LOW
$Comp
L Device:R R7
U 1 1 63F26993
P 8800 5150
F 0 "R7" H 8870 5196 50  0000 L CNN
F 1 "100k" H 8870 5105 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 8730 5150 50  0001 C CNN
F 3 "~" H 8800 5150 50  0001 C CNN
	1    8800 5150
	1    0    0    -1  
$EndComp
Text GLabel 8800 4800 1    50   Input ~ 0
Vcc
Wire Wire Line
	8800 4800 8800 5000
Wire Wire Line
	8600 5600 8800 5600
$Comp
L Device:CP C2
U 1 1 63F706EA
P 6000 2400
F 0 "C2" H 6118 2446 50  0000 L CNN
F 1 "10µF" H 6118 2355 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P2.50mm" H 6038 2250 50  0001 C CNN
F 3 "~" H 6000 2400 50  0001 C CNN
	1    6000 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 63F712E0
P 5600 2400
F 0 "C1" H 5715 2446 50  0000 L CNN
F 1 "10n" H 5715 2355 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 5638 2250 50  0001 C CNN
F 3 "~" H 5600 2400 50  0001 C CNN
	1    5600 2400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR07
U 1 1 63F71989
P 5800 2750
F 0 "#PWR07" H 5800 2500 50  0001 C CNN
F 1 "GND" H 5805 2577 50  0000 C CNN
F 2 "" H 5800 2750 50  0001 C CNN
F 3 "" H 5800 2750 50  0001 C CNN
	1    5800 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2550 5800 2550
Wire Wire Line
	5800 2750 5800 2550
Connection ~ 5800 2550
Wire Wire Line
	5800 2550 6000 2550
Wire Wire Line
	5600 2250 5600 2100
Wire Wire Line
	4900 2100 5600 2100
Wire Wire Line
	5600 2100 6000 2100
Connection ~ 5600 2100
Wire Wire Line
	6000 2250 6000 2100
Wire Wire Line
	8800 5300 8800 5600
Connection ~ 8800 5600
$Comp
L Device:D D1
U 1 1 63E8068B
P 2000 950
F 0 "D1" H 2000 733 50  0000 C CNN
F 1 "D" H 2000 824 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P12.70mm_Horizontal" H 2000 950 50  0001 C CNN
F 3 "~" H 2000 950 50  0001 C CNN
	1    2000 950 
	-1   0    0    1   
$EndComp
Wire Wire Line
	1850 950  1600 950 
Wire Wire Line
	1600 950  1600 1300
Connection ~ 1600 1300
$Comp
L bpc_scm:BPC_Mod485 U1
U 1 1 63E332DA
P 2600 3700
F 0 "U1" H 3100 4355 50  0000 C CNN
F 1 "BPC_Mod485" H 3100 4264 50  0000 C CNN
F 2 "bpc:BPC_Mod485" H 3100 4173 50  0000 C CNN
F 3 "" H 3000 3700 50  0001 C CNN
	1    2600 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3600 2100 3600
$Comp
L power:GND #PWR04
U 1 1 63E36370
P 2650 3000
F 0 "#PWR04" H 2650 2750 50  0001 C CNN
F 1 "GND" H 2655 2827 50  0000 C CNN
F 2 "" H 2650 3000 50  0001 C CNN
F 3 "" H 2650 3000 50  0001 C CNN
	1    2650 3000
	1    0    0    -1  
$EndComp
Text GLabel 2400 3700 0    50   Input ~ 0
Vcc
Wire Wire Line
	2500 3700 2400 3700
$Comp
L power:GND #PWR06
U 1 1 63E40FB6
P 5050 6150
F 0 "#PWR06" H 5050 5900 50  0001 C CNN
F 1 "GND" H 5055 5977 50  0000 C CNN
F 2 "" H 5050 6150 50  0001 C CNN
F 3 "" H 5050 6150 50  0001 C CNN
	1    5050 6150
	1    0    0    -1  
$EndComp
Wire Wire Line
	5000 5950 5000 6050
Wire Wire Line
	5000 6050 5050 6050
Wire Wire Line
	5100 6050 5100 5950
Wire Wire Line
	5050 6050 5050 6150
Connection ~ 5050 6050
Wire Wire Line
	5050 6050 5100 6050
Wire Wire Line
	5200 3700 5200 3950
Connection ~ 4900 950 
Wire Wire Line
	4900 950  9200 950 
Wire Wire Line
	2150 950  3100 950 
Wire Wire Line
	4900 2100 4900 950 
Text GLabel 4050 3400 2    50   Input ~ 0
TX
Text GLabel 4050 3700 2    50   Output ~ 0
RX
Text GLabel 4550 3550 2    50   Input ~ 0
TXEN
Wire Wire Line
	4050 3400 3700 3400
Wire Wire Line
	3700 3500 3800 3500
Wire Wire Line
	3800 3500 3800 3550
Wire Wire Line
	3800 3600 3700 3600
Connection ~ 3800 3550
Wire Wire Line
	3800 3550 3800 3600
Wire Wire Line
	4050 3700 3700 3700
Text GLabel 4250 4950 0    50   Output ~ 0
TXEN
Text GLabel 4300 4350 0    50   Input ~ 0
RX
Text GLabel 4300 4450 0    50   Output ~ 0
TX
Wire Wire Line
	2650 3000 2650 2900
Wire Wire Line
	2500 3400 2400 3400
Wire Wire Line
	2400 3400 2400 2900
Wire Wire Line
	2400 2900 2650 2900
$Comp
L MCU_Module:Arduino_Nano_v2.x A1
U 1 1 63E31F27
P 5000 4950
F 0 "A1" H 4550 3900 50  0000 C CNN
F 1 "Arduino_Nano_v2.x" H 4400 3750 50  0000 C CNN
F 2 "bpc:BPC_Arduino_Nano" H 5000 4950 50  0001 C CIN
F 3 "https://www.arduino.cc/en/uploads/Main/ArduinoNanoManual23.pdf" H 5000 4950 50  0001 C CNN
	1    5000 4950
	1    0    0    -1  
$EndComp
Text GLabel 4300 4650 0    50   Input ~ 0
IN_LVL_HIGH
Text GLabel 4300 4550 0    50   Input ~ 0
IN_LVL_LOW
Wire Wire Line
	4500 4950 4250 4950
Text GLabel 4250 5050 0    50   Output ~ 0
CMD_EV
$Comp
L Device:R R3
U 1 1 63EB8273
P 4400 3750
F 0 "R3" H 4470 3796 50  0000 L CNN
F 1 "100k" H 4470 3705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 4330 3750 50  0001 C CNN
F 3 "~" H 4400 3750 50  0001 C CNN
	1    4400 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 63EB915D
P 4400 3950
F 0 "#PWR05" H 4400 3700 50  0001 C CNN
F 1 "GND" H 4405 3777 50  0000 C CNN
F 2 "" H 4400 3950 50  0001 C CNN
F 3 "" H 4400 3950 50  0001 C CNN
	1    4400 3950
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3550 4400 3600
Wire Wire Line
	3800 3550 4400 3550
Wire Wire Line
	4550 3550 4400 3550
Connection ~ 4400 3550
Wire Wire Line
	4400 3900 4400 3950
$Comp
L power:GND #PWR02
U 1 1 63E899D1
P 1800 2800
F 0 "#PWR02" H 1800 2550 50  0001 C CNN
F 1 "GND" H 1805 2627 50  0000 C CNN
F 2 "" H 1800 2800 50  0001 C CNN
F 3 "" H 1800 2800 50  0001 C CNN
	1    1800 2800
	1    0    0    -1  
$EndComp
Connection ~ 1800 2600
Wire Wire Line
	1800 2600 1200 2600
Wire Wire Line
	1800 2600 1800 2800
Wire Wire Line
	8800 5600 9500 5600
$Comp
L Device:R R8
U 1 1 6405BD64
P 9150 5150
F 0 "R8" H 9220 5196 50  0000 L CNN
F 1 "100k" H 9220 5105 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9080 5150 50  0001 C CNN
F 3 "~" H 9150 5150 50  0001 C CNN
	1    9150 5150
	1    0    0    -1  
$EndComp
Text GLabel 9150 4800 1    50   Input ~ 0
Vcc
Wire Wire Line
	8950 6000 8950 5800
Wire Wire Line
	8950 5800 9500 5800
Wire Wire Line
	9500 5700 9150 5700
Wire Wire Line
	9150 5300 9150 5700
Connection ~ 9150 5700
Wire Wire Line
	9150 5700 8600 5700
Wire Wire Line
	9150 4800 9150 5000
$Comp
L bpc:P03P4L-04 Q2
U 1 1 6406C719
P 9550 1600
F 0 "Q2" H 9754 1646 50  0000 L CNN
F 1 "P03P4L-04" H 9754 1555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 9650 2100 50  0001 C CNN
F 3 "~" H 9550 1600 50  0001 C CNN
	1    9550 1600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 6406E3CB
P 9200 1250
F 0 "R9" H 9270 1296 50  0000 L CNN
F 1 "100k" H 9270 1205 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9130 1250 50  0001 C CNN
F 3 "~" H 9200 1250 50  0001 C CNN
	1    9200 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	9200 1600 9200 1400
Wire Wire Line
	9200 1600 9350 1600
Wire Wire Line
	9650 1400 9650 950 
Wire Wire Line
	9200 950  9200 1100
Connection ~ 9200 950 
Wire Wire Line
	9200 950  9650 950 
$Comp
L Device:D D2
U 1 1 64078D14
P 9400 2200
F 0 "D2" V 9354 2280 50  0000 L CNN
F 1 "D" V 9445 2280 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P2.54mm_Vertical_AnodeUp" H 9400 2200 50  0001 C CNN
F 3 "~" H 9400 2200 50  0001 C CNN
	1    9400 2200
	0    1    1    0   
$EndComp
Wire Wire Line
	9650 1800 9650 1950
Wire Wire Line
	9650 2150 9700 2150
Wire Wire Line
	9700 2250 9650 2250
Wire Wire Line
	9650 2250 9650 2450
Wire Wire Line
	9650 2450 9400 2450
Wire Wire Line
	9400 2450 9400 2350
Wire Wire Line
	9400 2050 9400 1950
Wire Wire Line
	9400 1950 9650 1950
Connection ~ 9650 1950
Wire Wire Line
	9650 1950 9650 2150
$Comp
L power:GND #PWR012
U 1 1 64081DCF
P 9650 2600
F 0 "#PWR012" H 9650 2350 50  0001 C CNN
F 1 "GND" H 9655 2427 50  0000 C CNN
F 2 "" H 9650 2600 50  0001 C CNN
F 3 "" H 9650 2600 50  0001 C CNN
	1    9650 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 2600 9650 2450
Connection ~ 9650 2450
$Comp
L bpc:BPC_2N2222A Q1
U 1 1 640851E6
P 8150 2100
F 0 "Q1" H 8688 2146 50  0000 L CNN
F 1 "BPC_2N2222A" H 8688 2055 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 8700 1950 50  0001 L CNN
F 3 "https://www.arrow.com/en/products/2n2222a/microsemi" H 8700 1850 50  0001 L CNN
F 4 "Trans GP BJT NPN 50V 0.8A 3-Pin TO-18" H 8700 1750 50  0001 L CNN "Description"
F 5 "Microsemi Corporation" H 8700 1550 50  0001 L CNN "Manufacturer_Name"
F 6 "2N2222A" H 8700 1450 50  0001 L CNN "Manufacturer_Part_Number"
F 7 "494-2N2222A" H 8700 1350 50  0001 L CNN "Mouser Part Number"
F 8 "https://www.mouser.co.uk/ProductDetail/Microchip-Microsemi/2N2222A?qs=TXMzd3F6EylR6f6YErRW3Q%3D%3D" H 8700 1250 50  0001 L CNN "Mouser Price/Stock"
F 9 "2N2222A" H 8700 1150 50  0001 L CNN "Arrow Part Number"
F 10 "https://www.arrow.com/en/products/2n2222a/microsemi" H 8700 1050 50  0001 L CNN "Arrow Price/Stock"
	1    8150 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR010
U 1 1 64086C04
P 8550 2550
F 0 "#PWR010" H 8550 2300 50  0001 C CNN
F 1 "GND" H 8555 2377 50  0000 C CNN
F 2 "" H 8550 2550 50  0001 C CNN
F 3 "" H 8550 2550 50  0001 C CNN
	1    8550 2550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 2400 8550 2550
Wire Wire Line
	9200 1600 8550 1600
Wire Wire Line
	8550 1600 8550 1800
Connection ~ 9200 1600
$Comp
L Device:R R6
U 1 1 6408CE3A
P 7950 2350
F 0 "R6" H 8020 2396 50  0000 L CNN
F 1 "100k" H 8020 2305 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7880 2350 50  0001 C CNN
F 3 "~" H 7950 2350 50  0001 C CNN
	1    7950 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 6408D4D4
P 7950 2600
F 0 "#PWR09" H 7950 2350 50  0001 C CNN
F 1 "GND" H 7955 2427 50  0000 C CNN
F 2 "" H 7950 2600 50  0001 C CNN
F 3 "" H 7950 2600 50  0001 C CNN
	1    7950 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	7950 2600 7950 2500
Wire Wire Line
	8150 2100 7950 2100
Wire Wire Line
	7950 2100 7950 2200
$Comp
L Device:R R5
U 1 1 64093B69
P 7550 2100
F 0 "R5" H 7620 2146 50  0000 L CNN
F 1 "100" H 7620 2055 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7480 2100 50  0001 C CNN
F 3 "~" H 7550 2100 50  0001 C CNN
	1    7550 2100
	0    1    1    0   
$EndComp
Wire Wire Line
	7950 2100 7700 2100
Connection ~ 7950 2100
Wire Wire Line
	7400 2100 7200 2100
Text GLabel 7200 2100 0    50   Input ~ 0
CMD_EV
Wire Wire Line
	4250 5050 4500 5050
Wire Wire Line
	4500 4550 4300 4550
Wire Wire Line
	4300 4350 4500 4350
Wire Wire Line
	4300 4450 4500 4450
Wire Wire Line
	4300 4650 4500 4650
Text GLabel 4250 5150 0    50   Output ~ 0
LED1
Wire Wire Line
	4250 5150 4500 5150
Text GLabel 4250 5250 0    50   Output ~ 0
LED2
Wire Wire Line
	4250 5250 4500 5250
Text GLabel 1700 5100 1    50   Input ~ 0
LED1
Wire Wire Line
	1700 5300 1700 5100
Wire Wire Line
	1700 5600 1700 5850
Wire Wire Line
	1700 5850 1600 5850
Wire Wire Line
	1600 5950 1700 5950
Wire Wire Line
	1700 5950 1700 6150
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 640CB04F
P 2050 5850
F 0 "J4" V 1922 5930 50  0000 L CNN
F 1 "LED" V 2013 5930 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2050 5850 50  0001 C CNN
F 3 "~" H 2050 5850 50  0001 C CNN
	1    2050 5850
	-1   0    0    -1  
$EndComp
$Comp
L Device:R R2
U 1 1 640CB055
P 2350 5450
F 0 "R2" H 2420 5496 50  0000 L CNN
F 1 "150" H 2420 5405 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 2280 5450 50  0001 C CNN
F 3 "~" H 2350 5450 50  0001 C CNN
	1    2350 5450
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 640CB05B
P 2350 6150
F 0 "#PWR03" H 2350 5900 50  0001 C CNN
F 1 "GND" V 2355 6022 50  0000 R CNN
F 2 "" H 2350 6150 50  0001 C CNN
F 3 "" H 2350 6150 50  0001 C CNN
	1    2350 6150
	-1   0    0    -1  
$EndComp
Text GLabel 2350 5100 1    50   Input ~ 0
LED2
Wire Wire Line
	2350 5300 2350 5100
Wire Wire Line
	2350 5600 2350 5850
Wire Wire Line
	2350 5850 2250 5850
Wire Wire Line
	2250 5950 2350 5950
Wire Wire Line
	2350 5950 2350 6150
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 640D46DE
P 7900 4300
F 0 "J5" H 7980 4292 50  0000 L CNN
F 1 "Debit" H 7980 4201 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-3-2.54_1x03_P2.54mm_Horizontal" H 7900 4300 50  0001 C CNN
F 3 "~" H 7900 4300 50  0001 C CNN
	1    7900 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 640D46E4
P 7150 4600
F 0 "#PWR08" H 7150 4350 50  0001 C CNN
F 1 "GND" H 7155 4427 50  0000 C CNN
F 2 "" H 7150 4600 50  0001 C CNN
F 3 "" H 7150 4600 50  0001 C CNN
	1    7150 4600
	1    0    0    -1  
$EndComp
Text GLabel 6800 4300 0    50   Output ~ 0
FLOW_IN
Text GLabel 7700 3400 1    50   Input ~ 0
Vcc
$Comp
L Device:R R4
U 1 1 640D46F8
P 7350 3750
F 0 "R4" H 7420 3796 50  0000 L CNN
F 1 "100k" H 7420 3705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7280 3750 50  0001 C CNN
F 3 "~" H 7350 3750 50  0001 C CNN
	1    7350 3750
	1    0    0    -1  
$EndComp
Text GLabel 7350 3400 1    50   Input ~ 0
Vcc
Wire Wire Line
	7150 4600 7150 4400
Wire Wire Line
	7150 4400 7700 4400
Wire Wire Line
	7700 4300 7350 4300
Wire Wire Line
	7350 3900 7350 4300
Connection ~ 7350 4300
Wire Wire Line
	7350 4300 6800 4300
Wire Wire Line
	7350 3400 7350 3600
Wire Wire Line
	7700 3400 7700 4200
Text GLabel 4300 4750 0    50   Input ~ 0
FLOW_IN
Wire Wire Line
	4300 4750 4500 4750
NoConn ~ 4400 4850
NoConn ~ 4400 5450
NoConn ~ 4400 5550
NoConn ~ 4400 5650
NoConn ~ 5600 5650
NoConn ~ 5600 5550
NoConn ~ 5600 5450
NoConn ~ 5600 5350
NoConn ~ 5600 5250
NoConn ~ 5600 5150
NoConn ~ 5600 4950
NoConn ~ 5600 5050
NoConn ~ 5600 4750
NoConn ~ 5600 4450
NoConn ~ 5600 4350
NoConn ~ 5100 3850
Wire Wire Line
	5100 3950 5100 3850
Wire Wire Line
	4500 4850 4400 4850
Wire Wire Line
	4500 5450 4400 5450
Wire Wire Line
	4500 5550 4400 5550
Wire Wire Line
	4500 5650 4400 5650
Wire Wire Line
	5500 5650 5600 5650
Wire Wire Line
	5500 5550 5600 5550
Wire Wire Line
	5500 5450 5600 5450
Wire Wire Line
	5500 5350 5600 5350
Wire Wire Line
	5500 5250 5600 5250
Wire Wire Line
	5500 5150 5600 5150
Wire Wire Line
	5500 5050 5600 5050
Wire Wire Line
	5500 4950 5600 4950
Wire Wire Line
	5500 4750 5600 4750
Wire Wire Line
	5500 4450 5600 4450
Wire Wire Line
	5500 4350 5600 4350
$Comp
L bpc:BPC_DHT22 U2
U 1 1 641ABE55
P 3450 1800
F 0 "U2" H 3778 1701 50  0000 L CNN
F 1 "BPC_DHT22" H 3778 1610 50  0000 L CNN
F 2 "bpc:BPC_DHT22" H 3600 2000 50  0001 C CNN
F 3 "" H 3450 1900 50  0001 C CNN
	1    3450 1800
	1    0    0    -1  
$EndComp
Text GLabel 3150 1550 1    50   Input ~ 0
Vcc
Wire Wire Line
	3350 1800 3150 1800
Wire Wire Line
	3150 1800 3150 1550
$Comp
L power:GND #PWR013
U 1 1 641B1133
P 3150 2250
F 0 "#PWR013" H 3150 2000 50  0001 C CNN
F 1 "GND" H 3155 2077 50  0000 C CNN
F 2 "" H 3150 2250 50  0001 C CNN
F 3 "" H 3150 2250 50  0001 C CNN
	1    3150 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3150 2250 3150 2100
Wire Wire Line
	3150 2100 3350 2100
Text GLabel 3150 1900 0    50   Output ~ 0
TEMP
Wire Wire Line
	3350 1900 3150 1900
Text GLabel 4250 5350 0    50   Input ~ 0
TEMP
Wire Wire Line
	4250 5350 4500 5350
$Comp
L power:PWR_FLAG #FLG0101
U 1 1 641C2607
P 1300 1150
F 0 "#FLG0101" H 1300 1225 50  0001 C CNN
F 1 "PWR_FLAG" H 1300 1323 50  0000 C CNN
F 2 "" H 1300 1150 50  0001 C CNN
F 3 "~" H 1300 1150 50  0001 C CNN
	1    1300 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1300 1150 1300 1300
Connection ~ 1300 1300
Wire Wire Line
	1300 1300 1600 1300
$Comp
L power:PWR_FLAG #FLG0102
U 1 1 641C805B
P 850 800
F 0 "#FLG0102" H 850 875 50  0001 C CNN
F 1 "PWR_FLAG" H 850 973 50  0000 C CNN
F 2 "" H 850 800 50  0001 C CNN
F 3 "~" H 850 800 50  0001 C CNN
	1    850  800 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0101
U 1 1 641C923E
P 850 900
F 0 "#PWR0101" H 850 650 50  0001 C CNN
F 1 "GND" H 855 727 50  0000 C CNN
F 2 "" H 850 900 50  0001 C CNN
F 3 "" H 850 900 50  0001 C CNN
	1    850  900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	850  900  850  800 
Wire Wire Line
	4900 3950 4900 2100
Connection ~ 4900 2100
$Comp
L power:PWR_FLAG #FLG0103
U 1 1 641D46ED
P 3100 800
F 0 "#FLG0103" H 3100 875 50  0001 C CNN
F 1 "PWR_FLAG" H 3100 973 50  0000 C CNN
F 2 "" H 3100 800 50  0001 C CNN
F 3 "~" H 3100 800 50  0001 C CNN
	1    3100 800 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 800  3100 950 
Connection ~ 3100 950 
Wire Wire Line
	3100 950  4900 950 
$EndSCHEMATC
