EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Module RS485 Oya"
Date "2023-02-05"
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
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-4-5.08_1x04_P5.08mm_Horizontal" H 1000 1400 50  0001 C CNN
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
	1200 1300 1600 1300
Wire Wire Line
	1200 1400 1700 1400
$Comp
L Connector:Screw_Terminal_01x04 J2
U 1 1 6383A568
P 1000 2400
F 0 "J2" H 918 1975 50  0000 C CNN
F 1 "NETWORK OUT" H 918 2066 50  0000 C CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-4-5.08_1x04_P5.08mm_Horizontal" H 1000 2400 50  0001 C CNN
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
L Connector:Screw_Terminal_01x02 J6
U 1 1 63853958
P 10200 2950
F 0 "J6" H 10280 2942 50  0000 L CNN
F 1 "Electrovanne 12V" H 10280 2851 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-2-5.08_1x02_P5.08mm_Horizontal" H 10200 2950 50  0001 C CNN
F 3 "~" H 10200 2950 50  0001 C CNN
	1    10200 2950
	1    0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J7
U 1 1 63854B40
P 10300 4600
F 0 "J7" H 10380 4592 50  0000 L CNN
F 1 "Niveau Haut" H 10380 4501 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-2-5.08_1x02_P5.08mm_Horizontal" H 10300 4600 50  0001 C CNN
F 3 "~" H 10300 4600 50  0001 C CNN
	1    10300 4600
	1    0    0    1   
$EndComp
$Comp
L Connector:Screw_Terminal_01x02 J5
U 1 1 638553D9
P 10000 5750
F 0 "J5" H 10080 5742 50  0000 L CNN
F 1 "Niveau bas" H 10080 5651 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-2-5.08_1x02_P5.08mm_Horizontal" H 10000 5750 50  0001 C CNN
F 3 "~" H 10000 5750 50  0001 C CNN
	1    10000 5750
	1    0    0    1   
$EndComp
Wire Wire Line
	9650 1800 9300 1800
$Comp
L Device:CP C12
U 1 1 6385AE49
P 9300 2050
F 0 "C12" H 9418 2096 50  0000 L CNN
F 1 "20µF" H 9418 2005 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D12.5mm_P5.00mm" H 9338 1900 50  0001 C CNN
F 3 "~" H 9300 2050 50  0001 C CNN
	1    9300 2050
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 6385B8D1
P 8900 2050
F 0 "C11" H 9015 2096 50  0000 L CNN
F 1 "10nF" H 9015 2005 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 8938 1900 50  0001 C CNN
F 3 "~" H 8900 2050 50  0001 C CNN
	1    8900 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 6385BF0C
P 8900 2350
F 0 "#PWR012" H 8900 2100 50  0001 C CNN
F 1 "GND" H 8905 2177 50  0000 C CNN
F 2 "" H 8900 2350 50  0001 C CNN
F 3 "" H 8900 2350 50  0001 C CNN
	1    8900 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 6385C214
P 9300 2350
F 0 "#PWR014" H 9300 2100 50  0001 C CNN
F 1 "GND" H 9305 2177 50  0000 C CNN
F 2 "" H 9300 2350 50  0001 C CNN
F 3 "" H 9300 2350 50  0001 C CNN
	1    9300 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 2350 9300 2200
Wire Wire Line
	9300 1900 9300 1800
Connection ~ 9300 1800
Wire Wire Line
	9300 1800 8900 1800
Wire Wire Line
	8900 1900 8900 1800
Wire Wire Line
	8900 2200 8900 2350
$Comp
L power:GND #PWR016
U 1 1 638614E8
P 9800 5950
F 0 "#PWR016" H 9800 5700 50  0001 C CNN
F 1 "GND" H 9805 5777 50  0000 C CNN
F 2 "" H 9800 5950 50  0001 C CNN
F 3 "" H 9800 5950 50  0001 C CNN
	1    9800 5950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 63861F42
P 10100 4800
F 0 "#PWR017" H 10100 4550 50  0001 C CNN
F 1 "GND" H 10105 4627 50  0000 C CNN
F 2 "" H 10100 4800 50  0001 C CNN
F 3 "" H 10100 4800 50  0001 C CNN
	1    10100 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 5750 9800 5950
Wire Wire Line
	10100 4600 10100 4800
$Comp
L power:GND #PWR015
U 1 1 638676E5
P 9650 4000
F 0 "#PWR015" H 9650 3750 50  0001 C CNN
F 1 "GND" H 9655 3827 50  0000 C CNN
F 2 "" H 9650 4000 50  0001 C CNN
F 3 "" H 9650 4000 50  0001 C CNN
	1    9650 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 63867DC1
P 8950 3300
F 0 "R3" H 9020 3346 50  0000 L CNN
F 1 "50" H 9020 3255 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 8880 3300 50  0001 C CNN
F 3 "~" H 8950 3300 50  0001 C CNN
	1    8950 3300
	0    1    1    0   
$EndComp
$Comp
L Device:R R4
U 1 1 6386FF1B
P 9350 3600
F 0 "R4" H 9420 3646 50  0000 L CNN
F 1 "100k" H 9420 3555 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9280 3600 50  0001 C CNN
F 3 "~" H 9350 3600 50  0001 C CNN
	1    9350 3600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 638704FF
P 9350 4000
F 0 "#PWR013" H 9350 3750 50  0001 C CNN
F 1 "GND" H 9355 3827 50  0000 C CNN
F 2 "" H 9350 4000 50  0001 C CNN
F 3 "" H 9350 4000 50  0001 C CNN
	1    9350 4000
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 3500 9650 4000
Wire Wire Line
	9350 3750 9350 4000
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 63884FD3
P 2500 6050
F 0 "J4" V 2372 6130 50  0000 L CNN
F 1 "LED" V 2463 6130 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2500 6050 50  0001 C CNN
F 3 "~" H 2500 6050 50  0001 C CNN
	1    2500 6050
	0    -1   1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 63886143
P 3050 5550
F 0 "R2" H 3120 5596 50  0000 L CNN
F 1 "150" H 3120 5505 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 2980 5550 50  0001 C CNN
F 3 "~" H 3050 5550 50  0001 C CNN
	1    3050 5550
	0    -1   1    0   
$EndComp
Wire Wire Line
	2600 5550 2600 5850
$Comp
L power:GND #PWR011
U 1 1 6388CA92
P 2250 5550
F 0 "#PWR011" H 2250 5300 50  0001 C CNN
F 1 "GND" V 2255 5422 50  0000 R CNN
F 2 "" H 2250 5550 50  0001 C CNN
F 3 "" H 2250 5550 50  0001 C CNN
	1    2250 5550
	0    1    -1   0   
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
Wire Wire Line
	2600 5550 2900 5550
Text GLabel 5200 3700 1    50   Output ~ 0
Vcc
Text GLabel 8550 3300 0    50   Input ~ 0
CMD_EV
Wire Wire Line
	9350 3450 9350 3300
Wire Wire Line
	9100 3300 9350 3300
Wire Wire Line
	8800 3300 8550 3300
$Comp
L Device:R R6
U 1 1 63F08001
P 10100 3950
F 0 "R6" H 10170 3996 50  0000 L CNN
F 1 "100k" H 10170 3905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 10030 3950 50  0001 C CNN
F 3 "~" H 10100 3950 50  0001 C CNN
	1    10100 3950
	1    0    0    -1  
$EndComp
Text GLabel 10100 3600 1    50   Input ~ 0
Vcc
Wire Wire Line
	10100 3600 10100 3800
Text GLabel 9750 4500 0    50   Output ~ 0
IN_LVL_HIGH
Wire Wire Line
	9750 4500 10100 4500
Text GLabel 9600 5650 0    50   Output ~ 0
IN_LVL_LOW
$Comp
L Device:R R5
U 1 1 63F26993
P 9800 5200
F 0 "R5" H 9870 5246 50  0000 L CNN
F 1 "100k" H 9870 5155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9730 5200 50  0001 C CNN
F 3 "~" H 9800 5200 50  0001 C CNN
	1    9800 5200
	1    0    0    -1  
$EndComp
Text GLabel 9800 4850 1    50   Input ~ 0
Vcc
Wire Wire Line
	9800 4850 9800 5050
Wire Wire Line
	9600 5650 9800 5650
$Comp
L Device:CP C7
U 1 1 63F706EA
P 6000 2400
F 0 "C7" H 6118 2446 50  0000 L CNN
F 1 "10µF" H 6118 2355 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D8.0mm_P2.50mm" H 6038 2250 50  0001 C CNN
F 3 "~" H 6000 2400 50  0001 C CNN
	1    6000 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 63F712E0
P 5600 2400
F 0 "C5" H 5715 2446 50  0000 L CNN
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
	10100 4100 10100 4500
Connection ~ 10100 4500
Wire Wire Line
	9800 5350 9800 5650
Connection ~ 9800 5650
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
L Transistor_BJT:TIP41C Q1
U 1 1 63EAF4AF
P 9550 3300
F 0 "Q1" H 9741 3346 50  0000 L CNN
F 1 "TIP41C" H 9741 3255 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 9800 3225 50  0001 L CIN
F 3 "https://www.centralsemi.com/get_document.php?cmp=1&mergetype=pd&mergepath=pd&pdf_id=tip41.PDF" H 9550 3300 50  0001 L CNN
	1    9550 3300
	1    0    0    -1  
$EndComp
Connection ~ 9350 3300
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
L power:GND #PWR01
U 1 1 63E36370
P 2650 3000
F 0 "#PWR01" H 2650 2750 50  0001 C CNN
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
L power:GND #PWR02
U 1 1 63E40FB6
P 5050 6150
F 0 "#PWR02" H 5050 5900 50  0001 C CNN
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
Wire Wire Line
	4900 3950 4900 2100
Connection ~ 4900 950 
Wire Wire Line
	4900 950  9650 950 
Wire Wire Line
	2150 950  4900 950 
Connection ~ 4900 2100
Wire Wire Line
	4900 2100 4900 950 
Wire Wire Line
	2500 5550 2250 5550
Wire Wire Line
	2500 5550 2500 5850
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
Text GLabel 4300 4550 0    50   Output ~ 0
TXEN
Text GLabel 4300 4250 0    50   Input ~ 0
RX
Text GLabel 4300 4400 0    50   Output ~ 0
TX
Wire Wire Line
	4500 4550 4300 4550
Wire Wire Line
	4500 4450 4400 4450
Wire Wire Line
	4400 4450 4400 4400
Wire Wire Line
	4400 4400 4300 4400
Wire Wire Line
	4500 4350 4450 4350
Wire Wire Line
	4450 4350 4450 4250
Wire Wire Line
	4450 4250 4300 4250
Wire Wire Line
	2650 3000 2650 2900
Wire Wire Line
	2500 3400 2400 3400
Wire Wire Line
	2400 3400 2400 2900
Wire Wire Line
	2400 2900 2650 2900
Wire Wire Line
	9650 2950 9650 3100
Wire Wire Line
	9650 2950 10000 2950
Wire Wire Line
	9650 2850 9650 1800
Connection ~ 9650 1800
Wire Wire Line
	9650 1800 9650 950 
Wire Wire Line
	9650 2850 10000 2850
Wire Wire Line
	4500 4850 3600 4850
Wire Wire Line
	3600 4850 3600 5550
Wire Wire Line
	3600 5550 3200 5550
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
Text GLabel 4250 4950 0    50   Input ~ 0
IN_LVL_HIGH
Text GLabel 4250 5100 0    50   Input ~ 0
IN_LVL_LOW
Wire Wire Line
	4500 4950 4250 4950
Wire Wire Line
	4500 5050 4350 5050
Wire Wire Line
	4350 5050 4350 5100
Wire Wire Line
	4350 5100 4250 5100
Text GLabel 4250 4750 0    50   Output ~ 0
CMD_EV
Wire Wire Line
	4500 4750 4250 4750
$Comp
L Device:R R1
U 1 1 63EB8273
P 4400 3750
F 0 "R1" H 4470 3796 50  0000 L CNN
F 1 "100k" H 4470 3705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 4330 3750 50  0001 C CNN
F 3 "~" H 4400 3750 50  0001 C CNN
	1    4400 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 63EB915D
P 4400 3950
F 0 "#PWR03" H 4400 3700 50  0001 C CNN
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
L power:GND #PWR?
U 1 1 63E899D1
P 1800 2800
F 0 "#PWR?" H 1800 2550 50  0001 C CNN
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
$EndSCHEMATC
