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
L Device:CP C12
U 1 1 6385AE49
P 7750 850
F 0 "C12" H 7868 896 50  0000 L CNN
F 1 "20µF" H 7868 805 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D12.5mm_P5.00mm" H 7788 700 50  0001 C CNN
F 3 "~" H 7750 850 50  0001 C CNN
	1    7750 850 
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 6385B8D1
P 7350 850
F 0 "C11" H 7465 896 50  0000 L CNN
F 1 "10nF" H 7465 805 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 7388 700 50  0001 C CNN
F 3 "~" H 7350 850 50  0001 C CNN
	1    7350 850 
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 6385BF0C
P 7350 1150
F 0 "#PWR012" H 7350 900 50  0001 C CNN
F 1 "GND" H 7355 977 50  0000 C CNN
F 2 "" H 7350 1150 50  0001 C CNN
F 3 "" H 7350 1150 50  0001 C CNN
	1    7350 1150
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 6385C214
P 7750 1150
F 0 "#PWR014" H 7750 900 50  0001 C CNN
F 1 "GND" H 7755 977 50  0000 C CNN
F 2 "" H 7750 1150 50  0001 C CNN
F 3 "" H 7750 1150 50  0001 C CNN
	1    7750 1150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7750 1150 7750 1000
Wire Wire Line
	7750 700  7750 600 
Wire Wire Line
	7750 600  7350 600 
Wire Wire Line
	7350 700  7350 600 
Wire Wire Line
	7350 1000 7350 1150
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 63884FD3
P 1000 7100
F 0 "J4" V 872 7180 50  0000 L CNN
F 1 "LED" V 963 7180 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1000 7100 50  0001 C CNN
F 3 "~" H 1000 7100 50  0001 C CNN
	1    1000 7100
	1    0    0    1   
$EndComp
$Comp
L Device:R R2
U 1 1 63886143
P 800 6700
F 0 "R2" H 870 6746 50  0000 L CNN
F 1 "150" H 870 6655 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 730 6700 50  0001 C CNN
F 3 "~" H 800 6700 50  0001 C CNN
	1    800  6700
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 6388CA92
P 800 7400
F 0 "#PWR011" H 800 7150 50  0001 C CNN
F 1 "GND" V 805 7272 50  0000 R CNN
F 2 "" H 800 7400 50  0001 C CNN
F 3 "" H 800 7400 50  0001 C CNN
	1    800  7400
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1800 1500 2100 1500
Connection ~ 1800 1500
Wire Wire Line
	2500 3100 2250 3100
Wire Wire Line
	2250 3100 2250 1400
Wire Wire Line
	2250 1400 1700 1400
Connection ~ 1700 1400
Text GLabel 2400 5050 1    50   Output ~ 0
Vcc
$Comp
L Device:D D1
U 1 1 63E8068B
P 1600 950
F 0 "D1" H 1600 733 50  0000 C CNN
F 1 "D" H 1600 824 50  0000 C CNN
F 2 "Diode_THT:D_A-405_P12.70mm_Horizontal" H 1600 950 50  0001 C CNN
F 3 "~" H 1600 950 50  0001 C CNN
	1    1600 950 
	0    1    1    0   
$EndComp
$Comp
L bpc_scm:BPC_Mod485 U1
U 1 1 63E332DA
P 2600 3300
F 0 "U1" H 3100 3955 50  0000 C CNN
F 1 "BPC_Mod485" H 3100 3864 50  0000 C CNN
F 2 "bpc:BPC_Mod485" H 3100 3773 50  0000 C CNN
F 3 "" H 3000 3300 50  0001 C CNN
	1    2600 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	2500 3200 2100 3200
$Comp
L power:GND #PWR01
U 1 1 63E36370
P 2650 2600
F 0 "#PWR01" H 2650 2350 50  0001 C CNN
F 1 "GND" H 2655 2427 50  0000 C CNN
F 2 "" H 2650 2600 50  0001 C CNN
F 3 "" H 2650 2600 50  0001 C CNN
	1    2650 2600
	1    0    0    -1  
$EndComp
Text GLabel 2400 3300 0    50   Input ~ 0
Vcc
Wire Wire Line
	2500 3300 2400 3300
$Comp
L power:GND #PWR02
U 1 1 63E40FB6
P 2250 7500
F 0 "#PWR02" H 2250 7250 50  0001 C CNN
F 1 "GND" H 2255 7327 50  0000 C CNN
F 2 "" H 2250 7500 50  0001 C CNN
F 3 "" H 2250 7500 50  0001 C CNN
	1    2250 7500
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 7300 2200 7400
Wire Wire Line
	2200 7400 2250 7400
Wire Wire Line
	2300 7400 2300 7300
Wire Wire Line
	2250 7400 2250 7500
Connection ~ 2250 7400
Wire Wire Line
	2250 7400 2300 7400
Wire Wire Line
	2400 5050 2400 5300
Text GLabel 4050 3000 2    50   Input ~ 0
TX
Text GLabel 4050 3300 2    50   Output ~ 0
RX
Text GLabel 4550 3150 2    50   Input ~ 0
TXEN
Wire Wire Line
	4050 3000 3700 3000
Wire Wire Line
	3700 3100 3800 3100
Wire Wire Line
	3800 3100 3800 3150
Wire Wire Line
	3800 3200 3700 3200
Connection ~ 3800 3150
Wire Wire Line
	3800 3150 3800 3200
Wire Wire Line
	4050 3300 3700 3300
Text GLabel 1500 5900 0    50   Output ~ 0
TXEN
Text GLabel 1500 5600 0    50   Input ~ 0
RX
Text GLabel 1500 5750 0    50   Output ~ 0
TX
Wire Wire Line
	1700 5900 1500 5900
Wire Wire Line
	1700 5800 1600 5800
Wire Wire Line
	1600 5800 1600 5750
Wire Wire Line
	1600 5750 1500 5750
Wire Wire Line
	1700 5700 1650 5700
Wire Wire Line
	1650 5700 1650 5600
Wire Wire Line
	1650 5600 1500 5600
Wire Wire Line
	2650 2600 2650 2500
Wire Wire Line
	2500 3000 2400 3000
Wire Wire Line
	2400 3000 2400 2500
Wire Wire Line
	2400 2500 2650 2500
Wire Wire Line
	1700 6200 800  6200
$Comp
L MCU_Module:Arduino_Nano_v2.x A1
U 1 1 63E31F27
P 2200 6300
F 0 "A1" H 1750 5250 50  0000 C CNN
F 1 "Arduino_Nano_v2.x" H 1600 5100 50  0000 C CNN
F 2 "bpc:BPC_Arduino_Nano" H 2200 6300 50  0001 C CIN
F 3 "https://www.arduino.cc/en/uploads/Main/ArduinoNanoManual23.pdf" H 2200 6300 50  0001 C CNN
	1    2200 6300
	1    0    0    -1  
$EndComp
Text GLabel 1450 6300 0    50   Input ~ 0
IN_1
Text GLabel 1450 6450 0    50   Input ~ 0
IN_2
Wire Wire Line
	1700 6300 1450 6300
Wire Wire Line
	1700 6400 1550 6400
Wire Wire Line
	1550 6400 1550 6450
Wire Wire Line
	1550 6450 1450 6450
Text GLabel 1450 6100 0    50   Output ~ 0
CMD_P2
Wire Wire Line
	1700 6100 1450 6100
$Comp
L Device:R R1
U 1 1 63EB8273
P 4400 3350
F 0 "R1" H 4470 3396 50  0000 L CNN
F 1 "100k" H 4470 3305 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 4330 3350 50  0001 C CNN
F 3 "~" H 4400 3350 50  0001 C CNN
	1    4400 3350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 63EB915D
P 4400 3550
F 0 "#PWR03" H 4400 3300 50  0001 C CNN
F 1 "GND" H 4405 3377 50  0000 C CNN
F 2 "" H 4400 3550 50  0001 C CNN
F 3 "" H 4400 3550 50  0001 C CNN
	1    4400 3550
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 3150 4400 3200
Wire Wire Line
	3800 3150 4400 3150
Wire Wire Line
	4550 3150 4400 3150
Connection ~ 4400 3150
Wire Wire Line
	4400 3500 4400 3550
$Comp
L power:GND #PWR04
U 1 1 63E899D1
P 1800 2800
F 0 "#PWR04" H 1800 2550 50  0001 C CNN
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
	800  6850 800  7000
Wire Wire Line
	800  7100 800  7400
Wire Wire Line
	800  6200 800  6550
$Comp
L bpc_scm:bpc_mos_p03p4 Q3
U 1 1 63F922FB
P 8750 1250
F 0 "Q3" H 8955 1296 50  0000 L CNN
F 1 "bpc_mos_p03p4" H 8955 1205 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 8850 1750 50  0001 C CNN
F 3 "~" H 8750 1250 50  0001 C CNN
	1    8750 1250
	1    0    0    -1  
$EndComp
$Comp
L Device:R R13
U 1 1 63F9346A
P 8200 950
F 0 "R13" H 8270 996 50  0000 L CNN
F 1 "100k" H 8270 905 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 8130 950 50  0001 C CNN
F 3 "~" H 8200 950 50  0001 C CNN
	1    8200 950 
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:2N2222A Q1
U 1 1 63F94181
P 7800 1700
F 0 "Q1" H 8338 1746 50  0000 L CNN
F 1 "2N2222A" H 8338 1655 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 8350 1550 50  0001 L CNN
F 3 "https://www.arrow.com/en/products/2n2222a/microsemi" H 8350 1450 50  0001 L CNN
F 4 "Trans GP BJT NPN 50V 0.8A 3-Pin TO-18" H 8350 1350 50  0001 L CNN "Description"
F 5 "Microsemi Corporation" H 8350 1150 50  0001 L CNN "Manufacturer_Name"
F 6 "2N2222A" H 8350 1050 50  0001 L CNN "Manufacturer_Part_Number"
F 7 "494-2N2222A" H 8350 950 50  0001 L CNN "Mouser Part Number"
F 8 "https://www.mouser.co.uk/ProductDetail/Microchip-Microsemi/2N2222A?qs=TXMzd3F6EylR6f6YErRW3Q%3D%3D" H 8350 850 50  0001 L CNN "Mouser Price/Stock"
F 9 "2N2222A" H 8350 750 50  0001 L CNN "Arrow Part Number"
F 10 "https://www.arrow.com/en/products/2n2222a/microsemi" H 8350 650 50  0001 L CNN "Arrow Price/Stock"
	1    7800 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R11
U 1 1 63F9595B
P 7600 2000
F 0 "R11" H 7670 2046 50  0000 L CNN
F 1 "100k" H 7670 1955 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7530 2000 50  0001 C CNN
F 3 "~" H 7600 2000 50  0001 C CNN
	1    7600 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R9
U 1 1 63F95FDC
P 7300 1700
F 0 "R9" V 7093 1700 50  0000 C CNN
F 1 "50" V 7184 1700 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7230 1700 50  0001 C CNN
F 3 "~" H 7300 1700 50  0001 C CNN
	1    7300 1700
	0    1    1    0   
$EndComp
Wire Wire Line
	7800 1700 7600 1700
Wire Wire Line
	7600 1850 7600 1700
Connection ~ 7600 1700
Wire Wire Line
	7600 1700 7450 1700
Wire Wire Line
	8200 1400 8200 1250
Connection ~ 8200 1250
Wire Wire Line
	8200 1250 8200 1100
Wire Wire Line
	8200 1250 8550 1250
Wire Wire Line
	8200 800  8200 600 
Wire Wire Line
	8200 600  8850 600 
Wire Wire Line
	8850 600  8850 1050
$Comp
L power:GND #PWR015
U 1 1 63FA347A
P 7600 2300
F 0 "#PWR015" H 7600 2050 50  0001 C CNN
F 1 "GND" H 7605 2127 50  0000 C CNN
F 2 "" H 7600 2300 50  0001 C CNN
F 3 "" H 7600 2300 50  0001 C CNN
	1    7600 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 63FA3DC3
P 8200 2300
F 0 "#PWR017" H 8200 2050 50  0001 C CNN
F 1 "GND" H 8205 2127 50  0000 C CNN
F 2 "" H 8200 2300 50  0001 C CNN
F 3 "" H 8200 2300 50  0001 C CNN
	1    8200 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8200 2000 8200 2300
Wire Wire Line
	7600 2300 7600 2150
$Comp
L Connector_Generic:Conn_01x02 J9
U 1 1 63FA8E9A
P 9350 1650
F 0 "J9" H 9430 1642 50  0000 L CNN
F 1 "Conn_01x02" H 9430 1551 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-2-5.08_1x02_P5.08mm_Horizontal" H 9350 1650 50  0001 C CNN
F 3 "~" H 9350 1650 50  0001 C CNN
	1    9350 1650
	1    0    0    -1  
$EndComp
$Comp
L Device:D D10
U 1 1 63FAC5E3
P 8850 1700
F 0 "D10" V 8804 1780 50  0000 L CNN
F 1 "D" V 8895 1780 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P12.70mm_Horizontal" H 8850 1700 50  0001 C CNN
F 3 "~" H 8850 1700 50  0001 C CNN
	1    8850 1700
	0    1    1    0   
$EndComp
Wire Wire Line
	8850 1450 8850 1500
Wire Wire Line
	9150 1650 9150 1500
Wire Wire Line
	9150 1500 8850 1500
Connection ~ 8850 1500
Wire Wire Line
	8850 1500 8850 1550
Wire Wire Line
	9150 1750 9150 1900
Wire Wire Line
	9150 1900 8850 1900
Wire Wire Line
	8850 1900 8850 1850
$Comp
L Device:R R15
U 1 1 63FB4E62
P 8850 2100
F 0 "R15" H 8920 2146 50  0000 L CNN
F 1 "30m" H 8920 2055 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 8780 2100 50  0001 C CNN
F 3 "~" H 8850 2100 50  0001 C CNN
	1    8850 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 63FB5434
P 8850 2350
F 0 "#PWR020" H 8850 2100 50  0001 C CNN
F 1 "GND" H 8855 2177 50  0000 C CNN
F 2 "" H 8850 2350 50  0001 C CNN
F 3 "" H 8850 2350 50  0001 C CNN
	1    8850 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 1900 8850 1950
Connection ~ 8850 1900
Wire Wire Line
	8850 2250 8850 2350
$Comp
L Device:R R17
U 1 1 63FD327D
P 9400 1900
F 0 "R17" V 9500 1900 50  0000 C CNN
F 1 "R" V 9600 1900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9330 1900 50  0001 C CNN
F 3 "~" H 9400 1900 50  0001 C CNN
	1    9400 1900
	0    1    1    0   
$EndComp
Wire Wire Line
	9250 1900 9150 1900
Connection ~ 9150 1900
Wire Wire Line
	9550 1900 9600 1900
$Comp
L Device:C C1
U 1 1 63FDF1BF
P 9600 2100
F 0 "C1" H 9715 2146 50  0000 L CNN
F 1 "C" H 9715 2055 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 9638 1950 50  0001 C CNN
F 3 "~" H 9600 2100 50  0001 C CNN
	1    9600 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 63FE0145
P 9600 2350
F 0 "#PWR023" H 9600 2100 50  0001 C CNN
F 1 "GND" H 9605 2177 50  0000 C CNN
F 2 "" H 9600 2350 50  0001 C CNN
F 3 "" H 9600 2350 50  0001 C CNN
	1    9600 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	9600 2350 9600 2250
Wire Wire Line
	9600 1950 9600 1900
Connection ~ 9600 1900
Wire Wire Line
	9600 1900 9900 1900
$Comp
L Amplifier_Operational:MCP6002-xP U2
U 1 1 63FCBBC7
P 10250 2000
F 0 "U2" H 10250 2367 50  0000 C CNN
F 1 "MCP6002-xP" H 10250 2276 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 10250 2000 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 10250 2000 50  0001 C CNN
	1    10250 2000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R19
U 1 1 63FE91BD
P 9950 2500
F 0 "R19" H 10020 2546 50  0000 L CNN
F 1 "R" H 10020 2455 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9880 2500 50  0001 C CNN
F 3 "~" H 9950 2500 50  0001 C CNN
	1    9950 2500
	1    0    0    -1  
$EndComp
$Comp
L Device:R R21
U 1 1 63FE98C8
P 10350 2300
F 0 "R21" V 10450 2300 50  0000 C CNN
F 1 "R" V 10550 2300 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 10280 2300 50  0001 C CNN
F 3 "~" H 10350 2300 50  0001 C CNN
	1    10350 2300
	0    1    1    0   
$EndComp
Wire Wire Line
	9950 2100 9950 2300
Wire Wire Line
	10200 2300 9950 2300
Connection ~ 9950 2300
Wire Wire Line
	9950 2300 9950 2350
$Comp
L power:GND #PWR026
U 1 1 63FF0405
P 9950 2750
F 0 "#PWR026" H 9950 2500 50  0001 C CNN
F 1 "GND" H 9955 2577 50  0000 C CNN
F 2 "" H 9950 2750 50  0001 C CNN
F 3 "" H 9950 2750 50  0001 C CNN
	1    9950 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	9950 2750 9950 2650
Wire Wire Line
	10700 2000 10700 2300
Wire Wire Line
	10700 2300 10500 2300
Wire Wire Line
	10550 2000 10700 2000
Text GLabel 10800 2000 2    50   Output ~ 0
MESI_1
Wire Wire Line
	10800 2000 10700 2000
Connection ~ 10700 2000
Text GLabel 7000 1700 0    50   Input ~ 0
CMD_P1
Wire Wire Line
	7000 1700 7150 1700
$Comp
L bpc_scm:bpc_mos_p03p4 Q4
U 1 1 640245A7
P 8800 3450
F 0 "Q4" H 9005 3496 50  0000 L CNN
F 1 "bpc_mos_p03p4" H 9005 3405 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 8900 3950 50  0001 C CNN
F 3 "~" H 8800 3450 50  0001 C CNN
	1    8800 3450
	1    0    0    -1  
$EndComp
$Comp
L Device:R R14
U 1 1 640245AD
P 8250 3150
F 0 "R14" H 8320 3196 50  0000 L CNN
F 1 "100k" H 8320 3105 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 8180 3150 50  0001 C CNN
F 3 "~" H 8250 3150 50  0001 C CNN
	1    8250 3150
	1    0    0    -1  
$EndComp
$Comp
L Transistor_BJT:2N2222A Q2
U 1 1 640245BA
P 7850 3900
F 0 "Q2" H 8388 3946 50  0000 L CNN
F 1 "2N2222A" H 8388 3855 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 8400 3750 50  0001 L CNN
F 3 "https://www.arrow.com/en/products/2n2222a/microsemi" H 8400 3650 50  0001 L CNN
F 4 "Trans GP BJT NPN 50V 0.8A 3-Pin TO-18" H 8400 3550 50  0001 L CNN "Description"
F 5 "Microsemi Corporation" H 8400 3350 50  0001 L CNN "Manufacturer_Name"
F 6 "2N2222A" H 8400 3250 50  0001 L CNN "Manufacturer_Part_Number"
F 7 "494-2N2222A" H 8400 3150 50  0001 L CNN "Mouser Part Number"
F 8 "https://www.mouser.co.uk/ProductDetail/Microchip-Microsemi/2N2222A?qs=TXMzd3F6EylR6f6YErRW3Q%3D%3D" H 8400 3050 50  0001 L CNN "Mouser Price/Stock"
F 9 "2N2222A" H 8400 2950 50  0001 L CNN "Arrow Part Number"
F 10 "https://www.arrow.com/en/products/2n2222a/microsemi" H 8400 2850 50  0001 L CNN "Arrow Price/Stock"
	1    7850 3900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R12
U 1 1 640245C0
P 7650 4200
F 0 "R12" H 7720 4246 50  0000 L CNN
F 1 "100k" H 7720 4155 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7580 4200 50  0001 C CNN
F 3 "~" H 7650 4200 50  0001 C CNN
	1    7650 4200
	1    0    0    -1  
$EndComp
$Comp
L Device:R R10
U 1 1 640245C6
P 7350 3900
F 0 "R10" V 7143 3900 50  0000 C CNN
F 1 "50" V 7234 3900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 7280 3900 50  0001 C CNN
F 3 "~" H 7350 3900 50  0001 C CNN
	1    7350 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	7850 3900 7650 3900
Wire Wire Line
	7650 4050 7650 3900
Connection ~ 7650 3900
Wire Wire Line
	7650 3900 7500 3900
Wire Wire Line
	8250 3600 8250 3450
Connection ~ 8250 3450
Wire Wire Line
	8250 3450 8250 3300
Wire Wire Line
	8250 3450 8600 3450
Wire Wire Line
	8250 3000 8250 2800
Wire Wire Line
	8250 2800 8900 2800
Wire Wire Line
	8900 2800 8900 3250
$Comp
L power:GND #PWR016
U 1 1 640245D7
P 7650 4500
F 0 "#PWR016" H 7650 4250 50  0001 C CNN
F 1 "GND" H 7655 4327 50  0000 C CNN
F 2 "" H 7650 4500 50  0001 C CNN
F 3 "" H 7650 4500 50  0001 C CNN
	1    7650 4500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR018
U 1 1 640245DD
P 8250 4500
F 0 "#PWR018" H 8250 4250 50  0001 C CNN
F 1 "GND" H 8255 4327 50  0000 C CNN
F 2 "" H 8250 4500 50  0001 C CNN
F 3 "" H 8250 4500 50  0001 C CNN
	1    8250 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 4200 8250 4500
Wire Wire Line
	7650 4500 7650 4350
$Comp
L Connector_Generic:Conn_01x02 J10
U 1 1 640245E5
P 9400 3850
F 0 "J10" H 9480 3842 50  0000 L CNN
F 1 "Conn_01x02" H 9480 3751 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-3-2-5.08_1x02_P5.08mm_Horizontal" H 9400 3850 50  0001 C CNN
F 3 "~" H 9400 3850 50  0001 C CNN
	1    9400 3850
	1    0    0    -1  
$EndComp
$Comp
L Device:D D11
U 1 1 640245EB
P 8900 3900
F 0 "D11" V 8854 3980 50  0000 L CNN
F 1 "D" V 8945 3980 50  0000 L CNN
F 2 "Diode_THT:D_A-405_P12.70mm_Horizontal" H 8900 3900 50  0001 C CNN
F 3 "~" H 8900 3900 50  0001 C CNN
	1    8900 3900
	0    1    1    0   
$EndComp
Wire Wire Line
	8900 3650 8900 3700
Wire Wire Line
	9200 3850 9200 3700
Wire Wire Line
	9200 3700 8900 3700
Connection ~ 8900 3700
Wire Wire Line
	8900 3700 8900 3750
Wire Wire Line
	9200 3950 9200 4100
Wire Wire Line
	9200 4100 8900 4100
Wire Wire Line
	8900 4100 8900 4050
$Comp
L Device:R R16
U 1 1 640245F9
P 8900 4300
F 0 "R16" H 8970 4346 50  0000 L CNN
F 1 "30m" H 8970 4255 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 8830 4300 50  0001 C CNN
F 3 "~" H 8900 4300 50  0001 C CNN
	1    8900 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 640245FF
P 8900 4550
F 0 "#PWR021" H 8900 4300 50  0001 C CNN
F 1 "GND" H 8905 4377 50  0000 C CNN
F 2 "" H 8900 4550 50  0001 C CNN
F 3 "" H 8900 4550 50  0001 C CNN
	1    8900 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 4100 8900 4150
Connection ~ 8900 4100
Wire Wire Line
	8900 4450 8900 4550
$Comp
L Device:R R18
U 1 1 64024608
P 9450 4100
F 0 "R18" V 9550 4100 50  0000 C CNN
F 1 "R" V 9650 4100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9380 4100 50  0001 C CNN
F 3 "~" H 9450 4100 50  0001 C CNN
	1    9450 4100
	0    1    1    0   
$EndComp
Wire Wire Line
	9300 4100 9200 4100
Connection ~ 9200 4100
Wire Wire Line
	9600 4100 9650 4100
$Comp
L Device:C C2
U 1 1 64024611
P 9650 4300
F 0 "C2" H 9765 4346 50  0000 L CNN
F 1 "C" H 9765 4255 50  0000 L CNN
F 2 "Capacitor_THT:C_Disc_D5.0mm_W2.5mm_P2.50mm" H 9688 4150 50  0001 C CNN
F 3 "~" H 9650 4300 50  0001 C CNN
	1    9650 4300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 64024617
P 9650 4550
F 0 "#PWR025" H 9650 4300 50  0001 C CNN
F 1 "GND" H 9655 4377 50  0000 C CNN
F 2 "" H 9650 4550 50  0001 C CNN
F 3 "" H 9650 4550 50  0001 C CNN
	1    9650 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	9650 4550 9650 4450
Wire Wire Line
	9650 4150 9650 4100
Connection ~ 9650 4100
Wire Wire Line
	9650 4100 10000 4100
$Comp
L Device:R R20
U 1 1 64024627
P 10000 4700
F 0 "R20" H 10070 4746 50  0000 L CNN
F 1 "R" H 10070 4655 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9930 4700 50  0001 C CNN
F 3 "~" H 10000 4700 50  0001 C CNN
	1    10000 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R22
U 1 1 6402462D
P 10400 4500
F 0 "R22" V 10500 4500 50  0000 C CNN
F 1 "R" V 10600 4500 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 10330 4500 50  0001 C CNN
F 3 "~" H 10400 4500 50  0001 C CNN
	1    10400 4500
	0    1    1    0   
$EndComp
Wire Wire Line
	10000 4300 10000 4500
Wire Wire Line
	10250 4500 10000 4500
Connection ~ 10000 4500
Wire Wire Line
	10000 4500 10000 4550
$Comp
L power:GND #PWR027
U 1 1 64024637
P 10000 4950
F 0 "#PWR027" H 10000 4700 50  0001 C CNN
F 1 "GND" H 10005 4777 50  0000 C CNN
F 2 "" H 10000 4950 50  0001 C CNN
F 3 "" H 10000 4950 50  0001 C CNN
	1    10000 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	10000 4950 10000 4850
Wire Wire Line
	10750 4200 10750 4500
Wire Wire Line
	10750 4500 10550 4500
Wire Wire Line
	10600 4200 10750 4200
Text GLabel 10850 4200 2    50   Output ~ 0
MESI_2
Wire Wire Line
	10850 4200 10750 4200
Connection ~ 10750 4200
Text GLabel 7050 3900 0    50   Input ~ 0
CMD_P2
Wire Wire Line
	7050 3900 7200 3900
$Comp
L Amplifier_Operational:MCP6002-xP U2
U 2 1 640296E3
P 10300 4200
F 0 "U2" H 10300 4567 50  0000 C CNN
F 1 "MCP6002-xP" H 10300 4476 50  0000 C CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 10300 4200 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 10300 4200 50  0001 C CNN
	2    10300 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 1500 2100 3200
Wire Wire Line
	8200 600  7750 600 
Connection ~ 8200 600 
Connection ~ 7750 600 
Wire Wire Line
	7350 600  6100 600 
Connection ~ 7350 600 
$Comp
L Device:D_Schottky D12
U 1 1 64045667
P 9800 1450
F 0 "D12" H 9800 1233 50  0000 C CNN
F 1 "D_Schottky" H 9800 1324 50  0000 C CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 9800 1450 50  0001 C CNN
F 3 "~" H 9800 1450 50  0001 C CNN
	1    9800 1450
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D14
U 1 1 64047BA1
P 10300 1450
F 0 "D14" H 10300 1233 50  0000 C CNN
F 1 "D_Schottky" H 10300 1324 50  0000 C CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 10300 1450 50  0001 C CNN
F 3 "~" H 10300 1450 50  0001 C CNN
	1    10300 1450
	-1   0    0    1   
$EndComp
Text GLabel 10550 1450 2    50   Input ~ 0
Vcc
Wire Wire Line
	9900 1900 9900 1550
Wire Wire Line
	9900 1550 10050 1550
Wire Wire Line
	10050 1550 10050 1450
Wire Wire Line
	10050 1450 9950 1450
Connection ~ 9900 1900
Wire Wire Line
	9900 1900 9950 1900
Wire Wire Line
	10050 1450 10150 1450
Connection ~ 10050 1450
Wire Wire Line
	10450 1450 10550 1450
$Comp
L power:GND #PWR022
U 1 1 64059AD1
P 9550 1450
F 0 "#PWR022" H 9550 1200 50  0001 C CNN
F 1 "GND" V 9555 1322 50  0000 R CNN
F 2 "" H 9550 1450 50  0001 C CNN
F 3 "" H 9550 1450 50  0001 C CNN
	1    9550 1450
	0    1    1    0   
$EndComp
Wire Wire Line
	9650 1450 9550 1450
$Comp
L Device:D_Schottky D13
U 1 1 6405FF59
P 9850 3650
F 0 "D13" H 9850 3433 50  0000 C CNN
F 1 "D_Schottky" H 9850 3524 50  0000 C CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 9850 3650 50  0001 C CNN
F 3 "~" H 9850 3650 50  0001 C CNN
	1    9850 3650
	-1   0    0    1   
$EndComp
$Comp
L Device:D_Schottky D15
U 1 1 6406176D
P 10350 3650
F 0 "D15" H 10350 3433 50  0000 C CNN
F 1 "D_Schottky" H 10350 3524 50  0000 C CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 10350 3650 50  0001 C CNN
F 3 "~" H 10350 3650 50  0001 C CNN
	1    10350 3650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR024
U 1 1 64061BC5
P 9600 3650
F 0 "#PWR024" H 9600 3400 50  0001 C CNN
F 1 "GND" V 9605 3522 50  0000 R CNN
F 2 "" H 9600 3650 50  0001 C CNN
F 3 "" H 9600 3650 50  0001 C CNN
	1    9600 3650
	0    1    1    0   
$EndComp
Text GLabel 10600 3650 2    50   Input ~ 0
Vcc
Wire Wire Line
	10600 3650 10500 3650
Wire Wire Line
	10200 3650 10100 3650
Wire Wire Line
	9700 3650 9600 3650
Wire Wire Line
	10000 4100 10000 3750
Wire Wire Line
	10000 3750 10100 3750
Wire Wire Line
	10100 3750 10100 3650
Connection ~ 10000 4100
Connection ~ 10100 3650
Wire Wire Line
	10100 3650 10000 3650
$Comp
L Amplifier_Operational:MCP6002-xP U2
U 3 1 6407B362
P 8900 5700
F 0 "U2" H 8858 5746 50  0000 L CNN
F 1 "MCP6002-xP" H 8858 5655 50  0000 L CNN
F 2 "Package_DIP:DIP-8_W7.62mm_LongPads" H 8900 5700 50  0001 C CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/21733j.pdf" H 8900 5700 50  0001 C CNN
	3    8900 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR019
U 1 1 640808F0
P 8800 6050
F 0 "#PWR019" H 8800 5800 50  0001 C CNN
F 1 "GND" H 8805 5877 50  0000 C CNN
F 2 "" H 8800 6050 50  0001 C CNN
F 3 "" H 8800 6050 50  0001 C CNN
	1    8800 6050
	1    0    0    -1  
$EndComp
Text GLabel 8800 5250 1    50   Input ~ 0
Vcc
Wire Wire Line
	8800 5400 8800 5250
Wire Wire Line
	8800 6000 8800 6050
Wire Wire Line
	2100 4600 750  4600
Wire Wire Line
	2100 4600 2100 5300
Text GLabel 2850 6400 2    50   Input ~ 0
MESI_2
Text GLabel 2850 6300 2    50   Input ~ 0
MESI_1
Wire Wire Line
	2850 6300 2700 6300
Wire Wire Line
	2700 6400 2850 6400
Text GLabel 1450 6000 0    50   Output ~ 0
CMD_P1
Wire Wire Line
	1450 6000 1700 6000
$Comp
L Connector_Generic:Conn_01x03 J3
U 1 1 640B1E5E
P 4100 7100
F 0 "J3" H 4180 7142 50  0000 L CNN
F 1 "Conn_01x03" H 4180 7051 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-3-2.54_1x03_P2.54mm_Horizontal" H 4100 7100 50  0001 C CNN
F 3 "~" H 4100 7100 50  0001 C CNN
	1    4100 7100
	1    0    0    -1  
$EndComp
Text GLabel 3800 7000 0    50   Output ~ 0
IN_1
Text GLabel 3800 7100 0    50   Output ~ 0
IN_2
$Comp
L power:GND #PWR06
U 1 1 640B2F4D
P 3800 7250
F 0 "#PWR06" H 3800 7000 50  0001 C CNN
F 1 "GND" H 3805 7077 50  0000 C CNN
F 2 "" H 3800 7250 50  0001 C CNN
F 3 "" H 3800 7250 50  0001 C CNN
	1    3800 7250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 7250 3800 7200
Wire Wire Line
	3800 7200 3900 7200
Wire Wire Line
	3900 7100 3800 7100
Wire Wire Line
	3900 7000 3800 7000
$Comp
L Device:R R7
U 1 1 640C9AA2
P 6000 1400
F 0 "R7" H 6070 1446 50  0000 L CNN
F 1 "250" H 6070 1355 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 5930 1400 50  0001 C CNN
F 3 "~" H 6000 1400 50  0001 C CNN
	1    6000 1400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 640C9E81
P 6300 1000
F 0 "J8" H 6380 992 50  0000 L CNN
F 1 "Conn_01x02" H 6380 901 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-2-2.54_1x02_P2.54mm_Horizontal" H 6300 1000 50  0001 C CNN
F 3 "~" H 6300 1000 50  0001 C CNN
	1    6300 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 640CAF07
P 6100 750
F 0 "R8" H 6170 796 50  0000 L CNN
F 1 "0" H 6170 705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 6030 750 50  0001 C CNN
F 3 "~" H 6100 750 50  0001 C CNN
	1    6100 750 
	1    0    0    -1  
$EndComp
Connection ~ 6100 600 
Wire Wire Line
	6100 900  6100 1000
$Comp
L Device:R R6
U 1 1 640D434D
P 5850 1000
F 0 "R6" V 5643 1000 50  0000 C CNN
F 1 "0" V 5734 1000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 5780 1000 50  0001 C CNN
F 3 "~" H 5850 1000 50  0001 C CNN
	1    5850 1000
	0    1    1    0   
$EndComp
Text GLabel 5150 800  0    50   Input ~ 0
Vcc
$Comp
L Device:D_Schottky D4
U 1 1 640D4995
P 5250 1050
F 0 "D4" V 5204 1130 50  0000 L CNN
F 1 "D_Schottky" V 5295 1130 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 5250 1050 50  0001 C CNN
F 3 "~" H 5250 1050 50  0001 C CNN
	1    5250 1050
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D5
U 1 1 640D637F
P 5250 1450
F 0 "D5" V 5204 1530 50  0000 L CNN
F 1 "D_Schottky" V 5295 1530 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 5250 1450 50  0001 C CNN
F 3 "~" H 5250 1450 50  0001 C CNN
	1    5250 1450
	0    1    1    0   
$EndComp
Wire Wire Line
	6100 1000 6000 1000
Connection ~ 6100 1000
Wire Wire Line
	5700 1000 5550 1000
Wire Wire Line
	5550 1000 5550 800 
Wire Wire Line
	5550 800  5250 800 
Wire Wire Line
	5250 800  5250 900 
Wire Wire Line
	6000 1100 6100 1100
Wire Wire Line
	5150 800  5250 800 
Connection ~ 5250 800 
$Comp
L power:GND #PWR013
U 1 1 6410AA34
P 6000 1700
F 0 "#PWR013" H 6000 1450 50  0001 C CNN
F 1 "GND" H 6005 1527 50  0000 C CNN
F 2 "" H 6000 1700 50  0001 C CNN
F 3 "" H 6000 1700 50  0001 C CNN
	1    6000 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR08
U 1 1 6410AE11
P 5250 1700
F 0 "#PWR08" H 5250 1450 50  0001 C CNN
F 1 "GND" H 5255 1527 50  0000 C CNN
F 2 "" H 5250 1700 50  0001 C CNN
F 3 "" H 5250 1700 50  0001 C CNN
	1    5250 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5250 1700 5250 1600
Wire Wire Line
	5250 1200 5250 1250
Wire Wire Line
	6000 1100 6000 1250
Wire Wire Line
	6000 1550 6000 1700
Wire Wire Line
	5250 1250 6000 1250
Connection ~ 5250 1250
Wire Wire Line
	5250 1250 5250 1300
Connection ~ 6000 1250
$Comp
L Device:R R4
U 1 1 6413E1FB
P 4250 1400
F 0 "R4" H 4320 1446 50  0000 L CNN
F 1 "250" H 4320 1355 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 4180 1400 50  0001 C CNN
F 3 "~" H 4250 1400 50  0001 C CNN
	1    4250 1400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J5
U 1 1 6413E201
P 4550 1000
F 0 "J5" H 4630 992 50  0000 L CNN
F 1 "Conn_01x02" H 4630 901 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-2-2.54_1x02_P2.54mm_Horizontal" H 4550 1000 50  0001 C CNN
F 3 "~" H 4550 1000 50  0001 C CNN
	1    4550 1000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R5
U 1 1 6413E207
P 4350 750
F 0 "R5" H 4420 796 50  0000 L CNN
F 1 "0" H 4420 705 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 4280 750 50  0001 C CNN
F 3 "~" H 4350 750 50  0001 C CNN
	1    4350 750 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4350 900  4350 1000
$Comp
L Device:R R3
U 1 1 6413E20E
P 4100 1000
F 0 "R3" V 3893 1000 50  0000 C CNN
F 1 "0" V 3984 1000 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 4030 1000 50  0001 C CNN
F 3 "~" H 4100 1000 50  0001 C CNN
	1    4100 1000
	0    1    1    0   
$EndComp
Text GLabel 3400 800  0    50   Input ~ 0
Vcc
$Comp
L Device:D_Schottky D2
U 1 1 6413E215
P 3500 1050
F 0 "D2" V 3454 1130 50  0000 L CNN
F 1 "D_Schottky" V 3545 1130 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 3500 1050 50  0001 C CNN
F 3 "~" H 3500 1050 50  0001 C CNN
	1    3500 1050
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D3
U 1 1 6413E21B
P 3500 1450
F 0 "D3" V 3454 1530 50  0000 L CNN
F 1 "D_Schottky" V 3545 1530 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 3500 1450 50  0001 C CNN
F 3 "~" H 3500 1450 50  0001 C CNN
	1    3500 1450
	0    1    1    0   
$EndComp
Wire Wire Line
	4350 1000 4250 1000
Connection ~ 4350 1000
Wire Wire Line
	3950 1000 3800 1000
Wire Wire Line
	3800 1000 3800 800 
Wire Wire Line
	3800 800  3500 800 
Wire Wire Line
	3500 800  3500 900 
Wire Wire Line
	4250 1100 4350 1100
Wire Wire Line
	3400 800  3500 800 
Connection ~ 3500 800 
$Comp
L power:GND #PWR07
U 1 1 6413E22A
P 4250 1700
F 0 "#PWR07" H 4250 1450 50  0001 C CNN
F 1 "GND" H 4255 1527 50  0000 C CNN
F 2 "" H 4250 1700 50  0001 C CNN
F 3 "" H 4250 1700 50  0001 C CNN
	1    4250 1700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 6413E230
P 3500 1700
F 0 "#PWR05" H 3500 1450 50  0001 C CNN
F 1 "GND" H 3505 1527 50  0000 C CNN
F 2 "" H 3500 1700 50  0001 C CNN
F 3 "" H 3500 1700 50  0001 C CNN
	1    3500 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3500 1700 3500 1600
Wire Wire Line
	3500 1200 3500 1250
Wire Wire Line
	4250 1100 4250 1250
Wire Wire Line
	4250 1550 4250 1700
Wire Wire Line
	3500 1250 4250 1250
Connection ~ 3500 1250
Wire Wire Line
	3500 1250 3500 1300
Connection ~ 4250 1250
Wire Wire Line
	750  600  750  4600
Connection ~ 4350 600 
Wire Wire Line
	4350 600  6100 600 
Text GLabel 5150 1250 0    50   Output ~ 0
MESI_3
Wire Wire Line
	5250 1250 5150 1250
Text GLabel 3400 1250 0    50   Output ~ 0
MESI_4
Wire Wire Line
	3400 1250 3500 1250
Text GLabel 2850 6500 2    50   Input ~ 0
MESI_3
Text GLabel 2850 6600 2    50   Input ~ 0
MESI_4
Wire Wire Line
	2850 6500 2700 6500
Wire Wire Line
	2850 6600 2700 6600
Wire Wire Line
	750  600  1600 600 
Wire Wire Line
	1600 800  1600 600 
Connection ~ 1600 600 
Wire Wire Line
	1600 600  4350 600 
Wire Wire Line
	1600 1100 1600 1300
Connection ~ 1600 1300
$Comp
L Connector_Generic:Conn_01x03 J6
U 1 1 641C0B88
P 6200 4750
F 0 "J6" H 6280 4792 50  0000 L CNN
F 1 "Conn_01x03" H 6280 4701 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-3-2.54_1x03_P2.54mm_Horizontal" H 6200 4750 50  0001 C CNN
F 3 "~" H 6200 4750 50  0001 C CNN
	1    6200 4750
	1    0    0    -1  
$EndComp
Text GLabel 5850 4300 1    50   Input ~ 0
Vcc
$Comp
L power:GND #PWR09
U 1 1 641C1993
P 5850 5200
F 0 "#PWR09" H 5850 4950 50  0001 C CNN
F 1 "GND" H 5855 5027 50  0000 C CNN
F 2 "" H 5850 5200 50  0001 C CNN
F 3 "" H 5850 5200 50  0001 C CNN
	1    5850 5200
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D6
U 1 1 641C1E59
P 5700 4550
F 0 "D6" V 5650 4350 50  0000 L CNN
F 1 "D_Schottky" V 5750 4000 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 5700 4550 50  0001 C CNN
F 3 "~" H 5700 4550 50  0001 C CNN
	1    5700 4550
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D7
U 1 1 641C2DDF
P 5700 4950
F 0 "D7" V 5650 4700 50  0000 L CNN
F 1 "D_Schottky" V 5750 4400 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 5700 4950 50  0001 C CNN
F 3 "~" H 5700 4950 50  0001 C CNN
	1    5700 4950
	0    1    1    0   
$EndComp
Wire Wire Line
	5700 4700 5700 4750
Wire Wire Line
	5700 4750 6000 4750
Connection ~ 5700 4750
Wire Wire Line
	5700 4750 5700 4800
Wire Wire Line
	5700 5100 5850 5100
Wire Wire Line
	6000 5100 6000 4850
Wire Wire Line
	6000 4650 6000 4400
Wire Wire Line
	6000 4400 5850 4400
Wire Wire Line
	5850 5200 5850 5100
Connection ~ 5850 5100
Wire Wire Line
	5850 5100 6000 5100
Wire Wire Line
	5850 4300 5850 4400
Connection ~ 5850 4400
Wire Wire Line
	5850 4400 5700 4400
Text GLabel 5350 4750 0    50   Output ~ 0
IN_PULSE_1
Wire Wire Line
	5350 4750 5700 4750
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 64229BEC
P 6200 6350
F 0 "J7" H 6280 6392 50  0000 L CNN
F 1 "Conn_01x03" H 6280 6301 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-3-2.54_1x03_P2.54mm_Horizontal" H 6200 6350 50  0001 C CNN
F 3 "~" H 6200 6350 50  0001 C CNN
	1    6200 6350
	1    0    0    -1  
$EndComp
Text GLabel 5850 5900 1    50   Input ~ 0
Vcc
$Comp
L power:GND #PWR010
U 1 1 64229BF3
P 5850 6800
F 0 "#PWR010" H 5850 6550 50  0001 C CNN
F 1 "GND" H 5855 6627 50  0000 C CNN
F 2 "" H 5850 6800 50  0001 C CNN
F 3 "" H 5850 6800 50  0001 C CNN
	1    5850 6800
	1    0    0    -1  
$EndComp
$Comp
L Device:D_Schottky D8
U 1 1 64229BF9
P 5700 6150
F 0 "D8" V 5650 5950 50  0000 L CNN
F 1 "D_Schottky" V 5750 5600 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 5700 6150 50  0001 C CNN
F 3 "~" H 5700 6150 50  0001 C CNN
	1    5700 6150
	0    1    1    0   
$EndComp
$Comp
L Device:D_Schottky D9
U 1 1 64229BFF
P 5700 6550
F 0 "D9" V 5650 6300 50  0000 L CNN
F 1 "D_Schottky" V 5750 6000 50  0000 L CNN
F 2 "Diode_THT:D_DO-15_P2.54mm_Vertical_AnodeUp" H 5700 6550 50  0001 C CNN
F 3 "~" H 5700 6550 50  0001 C CNN
	1    5700 6550
	0    1    1    0   
$EndComp
Wire Wire Line
	5700 6300 5700 6350
Wire Wire Line
	5700 6350 6000 6350
Connection ~ 5700 6350
Wire Wire Line
	5700 6350 5700 6400
Wire Wire Line
	5700 6700 5850 6700
Wire Wire Line
	6000 6700 6000 6450
Wire Wire Line
	6000 6250 6000 6000
Wire Wire Line
	6000 6000 5850 6000
Wire Wire Line
	5850 6800 5850 6700
Connection ~ 5850 6700
Wire Wire Line
	5850 6700 6000 6700
Wire Wire Line
	5850 5900 5850 6000
Connection ~ 5850 6000
Wire Wire Line
	5850 6000 5700 6000
Text GLabel 5350 6350 0    50   Output ~ 0
IN_PULSE_2
Wire Wire Line
	5350 6350 5700 6350
Text GLabel 1550 6700 0    50   Input ~ 0
IN_PULSE_1
Text GLabel 1550 6800 0    50   Input ~ 0
IN_PULSE_2
Wire Wire Line
	1700 6700 1550 6700
Wire Wire Line
	1700 6800 1550 6800
Text GLabel 9000 600  2    50   Output ~ 0
POW_MOS
Wire Wire Line
	9000 600  8850 600 
Connection ~ 8850 600 
Text GLabel 8100 2800 0    50   Input ~ 0
POW_MOS
Wire Wire Line
	8100 2800 8250 2800
Connection ~ 8250 2800
$EndSCHEMATC