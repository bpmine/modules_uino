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
$Comp
L Connector_Generic:Conn_01x02 J8
U 1 1 63E779B0
P 10450 1400
F 0 "J8" H 10530 1392 50  0000 L CNN
F 1 "Conn_01x02" H 10530 1301 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10450 1400 50  0001 C CNN
F 3 "~" H 10450 1400 50  0001 C CNN
	1    10450 1400
	1    0    0    -1  
$EndComp
$Comp
L Device:Fuse F6
U 1 1 63E7ABCD
P 8850 750
F 0 "F6" V 8653 750 50  0000 C CNN
F 1 "2A" V 8744 750 50  0000 C CNN
F 2 "Fuse:Fuseholder_TR5_Littelfuse_No560_No460" V 8780 750 50  0001 C CNN
F 3 "~" H 8850 750 50  0001 C CNN
	1    8850 750 
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F3
U 1 1 63E7CB5E
P 6250 1300
F 0 "F3" V 6053 1300 50  0000 C CNN
F 1 "2A" V 6144 1300 50  0000 C CNN
F 2 "Fuse:Fuseholder_TR5_Littelfuse_No560_No460" V 6180 1300 50  0001 C CNN
F 3 "~" H 6250 1300 50  0001 C CNN
	1    6250 1300
	0    1    1    0   
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J5
U 1 1 63E7DAF8
P 6800 1400
F 0 "J5" H 6880 1442 50  0000 L CNN
F 1 "Conn_01x03" H 6880 1351 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-3-2.54_1x03_P2.54mm_Horizontal" H 6800 1400 50  0001 C CNN
F 3 "~" H 6800 1400 50  0001 C CNN
	1    6800 1400
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J6
U 1 1 63E7DF8A
P 6800 2200
F 0 "J6" H 6880 2242 50  0000 L CNN
F 1 "Conn_01x03" H 6880 2151 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-3-2.54_1x03_P2.54mm_Horizontal" H 6800 2200 50  0001 C CNN
F 3 "~" H 6800 2200 50  0001 C CNN
	1    6800 2200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x03 J7
U 1 1 63E7E403
P 6800 2950
F 0 "J7" H 6880 2992 50  0000 L CNN
F 1 "Conn_01x03" H 6880 2901 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-3-2.54_1x03_P2.54mm_Horizontal" H 6800 2950 50  0001 C CNN
F 3 "~" H 6800 2950 50  0001 C CNN
	1    6800 2950
	1    0    0    -1  
$EndComp
$Comp
L Device:Fuse F4
U 1 1 63E7ECC6
P 6250 2100
F 0 "F4" V 6053 2100 50  0000 C CNN
F 1 "2A" V 6144 2100 50  0000 C CNN
F 2 "Fuse:Fuseholder_TR5_Littelfuse_No560_No460" V 6180 2100 50  0001 C CNN
F 3 "~" H 6250 2100 50  0001 C CNN
	1    6250 2100
	0    1    1    0   
$EndComp
$Comp
L Device:Fuse F5
U 1 1 63E7F1AD
P 6250 2850
F 0 "F5" V 6053 2850 50  0000 C CNN
F 1 "2A" V 6144 2850 50  0000 C CNN
F 2 "Fuse:Fuseholder_TR5_Littelfuse_No560_No460" V 6180 2850 50  0001 C CNN
F 3 "~" H 6250 2850 50  0001 C CNN
	1    6250 2850
	0    1    1    0   
$EndComp
Wire Wire Line
	6600 1300 6400 1300
Wire Wire Line
	6400 2100 6600 2100
Wire Wire Line
	6600 2850 6400 2850
$Comp
L power:GND #PWR011
U 1 1 63E8160D
P 6600 1650
F 0 "#PWR011" H 6600 1400 50  0001 C CNN
F 1 "GND" H 6605 1477 50  0000 C CNN
F 2 "" H 6600 1650 50  0001 C CNN
F 3 "" H 6600 1650 50  0001 C CNN
	1    6600 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 63E833DC
P 6600 2500
F 0 "#PWR012" H 6600 2250 50  0001 C CNN
F 1 "GND" H 6605 2327 50  0000 C CNN
F 2 "" H 6600 2500 50  0001 C CNN
F 3 "" H 6600 2500 50  0001 C CNN
	1    6600 2500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 63E836E1
P 6600 3250
F 0 "#PWR013" H 6600 3000 50  0001 C CNN
F 1 "GND" H 6605 3077 50  0000 C CNN
F 2 "" H 6600 3250 50  0001 C CNN
F 3 "" H 6600 3250 50  0001 C CNN
	1    6600 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	6600 2500 6600 2300
Wire Wire Line
	6600 1650 6600 1500
$Comp
L Transistor_BJT:2N2222A Q3
U 1 1 63E878A8
P 9350 1400
F 0 "Q3" H 9850 1550 50  0000 L CNN
F 1 "2N2222A" H 9800 1250 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 9600 1325 50  0001 L CIN
F 3 "https://www.arrow.com/en/products/2n2222a/microsemi" H 9350 1400 50  0001 L CNN
F 4 "2N2222A" H 9350 1400 50  0001 C CNN "Arrow Part Number"
F 5 "https://www.arrow.com/en/products/2n2222a/microsemi" H 9350 1400 50  0001 C CNN "Arrow Price/Stock"
F 6 "Trans GP BJT NPN 50V 0.8A 3-Pin TO-18" H 9350 1400 50  0001 C CNN "Description"
F 7 "Microsemi Corporation" H 9350 1400 50  0001 C CNN "Manufacturer_Name"
F 8 "2N2222A" H 9350 1400 50  0001 C CNN "Manufacturer_Part_Number"
F 9 "494-2N2222A" H 9350 1400 50  0001 C CNN "Mouser Part Number"
F 10 "https://www.mouser.co.uk/ProductDetail/Microchip-Microsemi/2N2222A?qs=TXMzd3F6EylR6f6YErRW3Q%3D%3D" H 9350 1400 50  0001 C CNN "Mouser Price/Stock"
	1    9350 1400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 63E8D34A
P 9750 1900
F 0 "#PWR017" H 9750 1650 50  0001 C CNN
F 1 "GND" H 9755 1727 50  0000 C CNN
F 2 "" H 9750 1900 50  0001 C CNN
F 3 "" H 9750 1900 50  0001 C CNN
	1    9750 1900
	1    0    0    -1  
$EndComp
Text GLabel 8950 1400 0    50   Input ~ 0
CmdP1
$Comp
L power:GND #PWR02
U 1 1 63E9F5C8
P 1350 950
F 0 "#PWR02" H 1350 700 50  0001 C CNN
F 1 "GND" H 1355 777 50  0000 C CNN
F 2 "" H 1350 950 50  0001 C CNN
F 3 "" H 1350 950 50  0001 C CNN
	1    1350 950 
	1    0    0    -1  
$EndComp
Wire Wire Line
	1350 950  1350 850 
Wire Wire Line
	1350 850  1250 850 
Wire Wire Line
	4450 1400 4650 1400
Wire Wire Line
	4650 1400 4650 1600
$Comp
L power:GND #PWR08
U 1 1 63EA4A35
P 4650 1600
F 0 "#PWR08" H 4650 1350 50  0001 C CNN
F 1 "GND" H 4655 1427 50  0000 C CNN
F 2 "" H 4650 1600 50  0001 C CNN
F 3 "" H 4650 1600 50  0001 C CNN
	1    4650 1600
	1    0    0    -1  
$EndComp
Wire Wire Line
	6100 2850 5850 2850
Wire Wire Line
	5850 2850 5850 2100
Wire Wire Line
	5850 2100 6100 2100
Wire Wire Line
	5850 2100 5850 1300
Wire Wire Line
	5850 1300 6100 1300
Connection ~ 5850 2100
Text GLabel 6450 1400 0    50   Input ~ 0
CmdRGB1
Text GLabel 6450 2200 0    50   Input ~ 0
CmdRGB2
Text GLabel 6450 2950 0    50   Input ~ 0
CmdRGB3
Wire Wire Line
	6600 2950 6450 2950
Wire Wire Line
	6600 2200 6450 2200
Wire Wire Line
	6600 1400 6450 1400
$Comp
L Device:Fuse F2
U 1 1 63EB7648
P 3050 950
F 0 "F2" H 2990 904 50  0000 R CNN
F 1 "5A" H 2990 995 50  0000 R CNN
F 2 "Fuse:Fuseholder_Cylinder-5x20mm_Schurter_0031_8201_Horizontal_Open" V 2980 950 50  0001 C CNN
F 3 "~" H 3050 950 50  0001 C CNN
	1    3050 950 
	-1   0    0    1   
$EndComp
Wire Wire Line
	1250 750  1800 750 
Wire Wire Line
	3050 800  3050 750 
Connection ~ 3050 750 
$Comp
L Device:Fuse F1
U 1 1 63EC2940
P 1800 1050
F 0 "F1" H 1740 1004 50  0000 R CNN
F 1 "500mA" H 1740 1095 50  0000 R CNN
F 2 "Fuse:Fuseholder_TR5_Littelfuse_No560_No460" V 1730 1050 50  0001 C CNN
F 3 "~" H 1800 1050 50  0001 C CNN
	1    1800 1050
	-1   0    0    1   
$EndComp
Wire Wire Line
	1800 900  1800 750 
Connection ~ 1800 750 
Wire Wire Line
	1800 750  3050 750 
Wire Wire Line
	3050 750  8500 750 
$Comp
L Device:R R6
U 1 1 63EC7A87
P 9150 1400
F 0 "R6" V 8943 1400 50  0000 C CNN
F 1 "100" V 9034 1400 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9080 1400 50  0001 C CNN
F 3 "~" H 9150 1400 50  0001 C CNN
	1    9150 1400
	0    1    1    0   
$EndComp
$Comp
L Device:R R9
U 1 1 63EC858D
P 9300 1650
F 0 "R9" H 9230 1604 50  0000 R CNN
F 1 "100k" H 9230 1695 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9230 1650 50  0001 C CNN
F 3 "~" H 9300 1650 50  0001 C CNN
	1    9300 1650
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR014
U 1 1 63EC8BC9
P 9300 1900
F 0 "#PWR014" H 9300 1650 50  0001 C CNN
F 1 "GND" H 9305 1727 50  0000 C CNN
F 2 "" H 9300 1900 50  0001 C CNN
F 3 "" H 9300 1900 50  0001 C CNN
	1    9300 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 1400 9300 1500
Wire Wire Line
	9300 1800 9300 1900
Wire Wire Line
	8950 1400 9000 1400
$Comp
L Connector_Generic:Conn_01x02 J9
U 1 1 63ED199A
P 10450 3050
F 0 "J9" H 10530 3042 50  0000 L CNN
F 1 "Conn_01x02" H 10530 2951 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10450 3050 50  0001 C CNN
F 3 "~" H 10450 3050 50  0001 C CNN
	1    10450 3050
	1    0    0    -1  
$EndComp
$Comp
L Device:Fuse F7
U 1 1 63ED19A0
P 8850 3950
F 0 "F7" V 8653 3950 50  0000 C CNN
F 1 "2A" V 8744 3950 50  0000 C CNN
F 2 "Fuse:Fuseholder_TR5_Littelfuse_No560_No460" V 8780 3950 50  0001 C CNN
F 3 "~" H 8850 3950 50  0001 C CNN
	1    8850 3950
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR018
U 1 1 63ED19AF
P 9800 3650
F 0 "#PWR018" H 9800 3400 50  0001 C CNN
F 1 "GND" H 9805 3477 50  0000 C CNN
F 2 "" H 9800 3650 50  0001 C CNN
F 3 "" H 9800 3650 50  0001 C CNN
	1    9800 3650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R7
U 1 1 63ED19B7
P 9150 3150
F 0 "R7" V 8943 3150 50  0000 C CNN
F 1 "100" V 9034 3150 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9080 3150 50  0001 C CNN
F 3 "~" H 9150 3150 50  0001 C CNN
	1    9150 3150
	0    1    1    0   
$EndComp
$Comp
L Device:R R10
U 1 1 63ED19BD
P 9300 3400
F 0 "R10" H 9230 3354 50  0000 R CNN
F 1 "100k" H 9230 3445 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9230 3400 50  0001 C CNN
F 3 "~" H 9300 3400 50  0001 C CNN
	1    9300 3400
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR015
U 1 1 63ED19C3
P 9300 3650
F 0 "#PWR015" H 9300 3400 50  0001 C CNN
F 1 "GND" H 9305 3477 50  0000 C CNN
F 2 "" H 9300 3650 50  0001 C CNN
F 3 "" H 9300 3650 50  0001 C CNN
	1    9300 3650
	1    0    0    -1  
$EndComp
Wire Wire Line
	9300 3550 9300 3650
Wire Wire Line
	8950 3150 9000 3150
$Comp
L Connector_Generic:Conn_01x02 J10
U 1 1 63ED7DF2
P 10450 4950
F 0 "J10" H 10530 4942 50  0000 L CNN
F 1 "Conn_01x02" H 10530 4851 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 10450 4950 50  0001 C CNN
F 3 "~" H 10450 4950 50  0001 C CNN
	1    10450 4950
	1    0    0    -1  
$EndComp
$Comp
L Device:Fuse F8
U 1 1 63ED7DF8
P 8900 2350
F 0 "F8" V 8703 2350 50  0000 C CNN
F 1 "2A" V 8794 2350 50  0000 C CNN
F 2 "Fuse:Fuseholder_TR5_Littelfuse_No560_No460" V 8830 2350 50  0001 C CNN
F 3 "~" H 8900 2350 50  0001 C CNN
	1    8900 2350
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR019
U 1 1 63ED7E07
P 9850 5600
F 0 "#PWR019" H 9850 5350 50  0001 C CNN
F 1 "GND" H 9855 5427 50  0000 C CNN
F 2 "" H 9850 5600 50  0001 C CNN
F 3 "" H 9850 5600 50  0001 C CNN
	1    9850 5600
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 63ED7E0E
P 9250 5100
F 0 "R8" V 9043 5100 50  0000 C CNN
F 1 "100" V 9134 5100 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9180 5100 50  0001 C CNN
F 3 "~" H 9250 5100 50  0001 C CNN
	1    9250 5100
	0    1    1    0   
$EndComp
$Comp
L Device:R R11
U 1 1 63ED7E14
P 9400 5350
F 0 "R11" H 9330 5304 50  0000 R CNN
F 1 "100k" H 9330 5395 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9330 5350 50  0001 C CNN
F 3 "~" H 9400 5350 50  0001 C CNN
	1    9400 5350
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR016
U 1 1 63ED7E1A
P 9400 5600
F 0 "#PWR016" H 9400 5350 50  0001 C CNN
F 1 "GND" H 9405 5427 50  0000 C CNN
F 2 "" H 9400 5600 50  0001 C CNN
F 3 "" H 9400 5600 50  0001 C CNN
	1    9400 5600
	1    0    0    -1  
$EndComp
Wire Wire Line
	9400 5100 9400 5200
Wire Wire Line
	9400 5500 9400 5600
Wire Wire Line
	9050 5100 9100 5100
Text GLabel 1650 1900 0    50   Input ~ 0
CmdPWR5
Wire Wire Line
	1800 1200 1800 1300
Text GLabel 2600 4400 2    50   Output ~ 0
CmdRGB1
Text GLabel 2600 4500 2    50   Output ~ 0
CmdRGB2
Text GLabel 2600 4600 2    50   Output ~ 0
CmdRGB3
Text GLabel 2600 4800 2    50   Output ~ 0
CmdP1
Text GLabel 2600 5000 2    50   Output ~ 0
CmdP3
Text GLabel 2600 4200 2    50   Output ~ 0
CmdPWR5
Text GLabel 2600 4100 2    50   Input ~ 0
SELECT
$Comp
L power:GND #PWR03
U 1 1 63EE5035
P 1750 5900
F 0 "#PWR03" H 1750 5650 50  0001 C CNN
F 1 "GND" H 1755 5727 50  0000 C CNN
F 2 "" H 1750 5900 50  0001 C CNN
F 3 "" H 1750 5900 50  0001 C CNN
	1    1750 5900
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 5500 1700 5700
Wire Wire Line
	1700 5700 1750 5700
Wire Wire Line
	1800 5700 1800 5500
Wire Wire Line
	1750 5700 1750 5900
Connection ~ 1750 5700
Wire Wire Line
	1750 5700 1800 5700
Wire Wire Line
	2600 4600 2300 4600
Wire Wire Line
	2600 4500 2300 4500
Wire Wire Line
	2600 4400 2300 4400
Wire Wire Line
	2600 4100 2300 4100
Wire Wire Line
	2600 4200 2300 4200
Wire Wire Line
	2600 4800 2300 4800
Wire Wire Line
	2300 5000 2600 5000
Wire Wire Line
	2600 4900 2300 4900
Text GLabel 2600 4900 2    50   Output ~ 0
CmdP2
Text GLabel 8950 3150 0    50   Input ~ 0
CmdP2
Text GLabel 9050 5100 0    50   Input ~ 0
CmdP3
$Comp
L Connector_Generic:Conn_01x02 J2
U 1 1 63ED5CAF
P 1500 7050
F 0 "J2" H 1580 7042 50  0000 L CNN
F 1 "Conn_01x02" H 1580 6951 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1500 7050 50  0001 C CNN
F 3 "~" H 1500 7050 50  0001 C CNN
	1    1500 7050
	1    0    0    -1  
$EndComp
Text GLabel 1200 7050 0    50   Output ~ 0
SELECT
$Comp
L power:GND #PWR01
U 1 1 63ED7046
P 1200 7300
F 0 "#PWR01" H 1200 7050 50  0001 C CNN
F 1 "GND" H 1205 7127 50  0000 C CNN
F 2 "" H 1200 7300 50  0001 C CNN
F 3 "" H 1200 7300 50  0001 C CNN
	1    1200 7300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1200 7300 1200 7150
Wire Wire Line
	1200 7150 1300 7150
Wire Wire Line
	1300 7050 1200 7050
Wire Wire Line
	6600 3050 6600 3250
$Comp
L bpc:RtcArduino U1
U 1 1 63FDB714
P 4100 6850
F 0 "U1" H 4678 6546 50  0000 L CNN
F 1 "RtcArduino" H 4678 6455 50  0000 L CNN
F 2 "bpc:BPC_RtcArduino" H 4450 7150 50  0001 C CNN
F 3 "" H 4100 6750 50  0001 C CNN
	1    4100 6850
	1    0    0    -1  
$EndComp
$Comp
L bpc:BPC_DHT22 U3
U 1 1 63FDC37A
P 6050 7000
F 0 "U3" H 6378 6901 50  0000 L CNN
F 1 "BPC_DHT22" H 6378 6810 50  0000 L CNN
F 2 "bpc:BPC_DHT22" H 6200 7200 50  0001 C CNN
F 3 "" H 6050 7100 50  0001 C CNN
	1    6050 7000
	1    0    0    -1  
$EndComp
$Comp
L bpc:Arduino_Nano_v2.x A1
U 1 1 63FDEB19
P 1800 4500
F 0 "A1" H 1600 3400 50  0000 C CNN
F 1 "Arduino_Nano_v2.x" H 1300 3500 50  0000 C CNN
F 2 "bpc:BPC_Arduino_Nano" H 1800 4500 50  0001 C CIN
F 3 "https://www.arduino.cc/en/uploads/Main/ArduinoNanoManual23.pdf" H 1800 4500 50  0001 C CNN
	1    1800 4500
	-1   0    0    -1  
$EndComp
Text GLabel 1200 5000 0    50   Output ~ 0
SCL
Wire Wire Line
	1300 5000 1200 5000
Text GLabel 1200 4900 0    50   BiDi ~ 0
SDA
Wire Wire Line
	1300 4900 1200 4900
Text GLabel 2850 7150 0    50   BiDi ~ 0
SDA
Text GLabel 2850 7250 0    50   Input ~ 0
SCL
$Comp
L power:GND #PWR07
U 1 1 640042AF
P 4350 6400
F 0 "#PWR07" H 4350 6150 50  0001 C CNN
F 1 "GND" H 4355 6227 50  0000 C CNN
F 2 "" H 4350 6400 50  0001 C CNN
F 3 "" H 4350 6400 50  0001 C CNN
	1    4350 6400
	1    0    0    -1  
$EndComp
Text GLabel 1600 3350 1    50   Output ~ 0
Vcc
Wire Wire Line
	1600 3350 1600 3500
Text GLabel 3250 6400 1    50   Input ~ 0
Vcc
Wire Wire Line
	4000 6950 3850 6950
Wire Wire Line
	3850 6950 3850 6250
Wire Wire Line
	3850 6250 4350 6250
Wire Wire Line
	4350 6250 4350 6400
Wire Wire Line
	4000 7050 3750 7050
Wire Wire Line
	3750 7050 3750 6500
Wire Wire Line
	3750 6500 3450 6500
Wire Wire Line
	3250 6500 3250 6400
$Comp
L power:GND #PWR010
U 1 1 6400D88E
P 5800 7350
F 0 "#PWR010" H 5800 7100 50  0001 C CNN
F 1 "GND" H 5805 7177 50  0000 C CNN
F 2 "" H 5800 7350 50  0001 C CNN
F 3 "" H 5800 7350 50  0001 C CNN
	1    5800 7350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5950 7300 5800 7300
Wire Wire Line
	5800 7300 5800 7350
Text GLabel 5800 6900 1    50   Input ~ 0
Vcc
Wire Wire Line
	5950 7000 5800 7000
Wire Wire Line
	5800 7000 5800 6900
Wire Wire Line
	5950 7100 5800 7100
Text GLabel 5800 7100 0    50   Input ~ 0
Temp
Text GLabel 2600 5100 2    50   Input ~ 0
Temp
Wire Wire Line
	2600 5100 2300 5100
$Comp
L Device:R R5
U 1 1 6401AD87
P 3450 6750
F 0 "R5" H 3380 6704 50  0000 R CNN
F 1 "100k" H 3380 6795 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 3380 6750 50  0001 C CNN
F 3 "~" H 3450 6750 50  0001 C CNN
	1    3450 6750
	-1   0    0    1   
$EndComp
$Comp
L Device:R R4
U 1 1 6401B981
P 3100 6750
F 0 "R4" H 3030 6704 50  0000 R CNN
F 1 "100k" H 3030 6795 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 3030 6750 50  0001 C CNN
F 3 "~" H 3100 6750 50  0001 C CNN
	1    3100 6750
	-1   0    0    1   
$EndComp
Wire Wire Line
	3250 6500 3100 6500
Wire Wire Line
	3100 6500 3100 6600
Connection ~ 3250 6500
Wire Wire Line
	3450 6600 3450 6500
Connection ~ 3450 6500
Wire Wire Line
	3450 6500 3250 6500
Wire Wire Line
	3100 6900 3100 7250
Connection ~ 3100 7250
Wire Wire Line
	3100 7250 2850 7250
Wire Wire Line
	3450 6900 3450 7150
Connection ~ 3450 7150
Wire Wire Line
	3450 7150 2850 7150
Wire Wire Line
	3450 7150 4000 7150
Wire Wire Line
	3100 7250 4000 7250
NoConn ~ 3900 7350
NoConn ~ 3900 7450
Wire Wire Line
	4000 7350 3900 7350
Wire Wire Line
	4000 7450 3900 7450
NoConn ~ 1150 3900
NoConn ~ 1150 4000
NoConn ~ 1150 4300
NoConn ~ 1150 4600
NoConn ~ 1150 4700
NoConn ~ 1150 4800
NoConn ~ 1150 5100
NoConn ~ 1150 5200
Wire Wire Line
	1300 5200 1150 5200
Wire Wire Line
	1300 5100 1150 5100
Wire Wire Line
	1300 4800 1150 4800
Wire Wire Line
	1300 4700 1150 4700
Wire Wire Line
	1300 4600 1150 4600
Wire Wire Line
	1300 4300 1150 4300
Wire Wire Line
	1300 4000 1150 4000
Wire Wire Line
	1300 3900 1150 3900
NoConn ~ 2400 5200
Wire Wire Line
	2300 5200 2400 5200
NoConn ~ 2400 4300
NoConn ~ 2400 4700
Wire Wire Line
	2300 4300 2400 4300
Wire Wire Line
	2300 4700 2400 4700
Text GLabel 1900 1300 2    50   Output ~ 0
Vin
Wire Wire Line
	1900 1300 1800 1300
$Comp
L bpc:P03P4L-04 Q2
U 1 1 640C33FB
P 2950 1600
F 0 "Q2" H 3154 1646 50  0000 L CNN
F 1 "P03P4L-04" H 3154 1555 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 3050 2100 50  0001 C CNN
F 3 "~" H 2950 1600 50  0001 C CNN
	1    2950 1600
	1    0    0    -1  
$EndComp
Text GLabel 4650 1300 2    50   Output ~ 0
PWR_5V
Wire Wire Line
	4450 1300 4650 1300
Text GLabel 5650 1300 0    50   Input ~ 0
PWR_5V
Wire Wire Line
	5650 1300 5850 1300
Connection ~ 5850 1300
$Comp
L bpc:P03P4L-04 Q6
U 1 1 640D705A
P 10150 1100
F 0 "Q6" H 10354 1146 50  0000 L CNN
F 1 "P03P4L-04" H 10354 1055 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 10250 1600 50  0001 C CNN
F 3 "~" H 10150 1100 50  0001 C CNN
	1    10150 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 750  10250 900 
Wire Wire Line
	10250 1300 10250 1400
$Comp
L power:GND #PWR020
U 1 1 640E2862
P 10250 1650
F 0 "#PWR020" H 10250 1400 50  0001 C CNN
F 1 "GND" H 10255 1477 50  0000 C CNN
F 2 "" H 10250 1650 50  0001 C CNN
F 3 "" H 10250 1650 50  0001 C CNN
	1    10250 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 1650 10250 1500
Wire Wire Line
	9000 750  9650 750 
Wire Wire Line
	8700 750  8500 750 
Connection ~ 8500 750 
$Comp
L Device:R R12
U 1 1 640F167B
P 9650 950
F 0 "R12" V 9443 950 50  0000 C CNN
F 1 "100" V 9534 950 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9580 950 50  0001 C CNN
F 3 "~" H 9650 950 50  0001 C CNN
	1    9650 950 
	-1   0    0    1   
$EndComp
Wire Wire Line
	9950 1100 9750 1100
Wire Wire Line
	9650 800  9650 750 
Connection ~ 9650 750 
Wire Wire Line
	9650 750  10250 750 
Wire Wire Line
	8500 750  8500 2350
$Comp
L bpc:P03P4L-04 Q7
U 1 1 6410F13F
P 10150 2750
F 0 "Q7" H 10354 2796 50  0000 L CNN
F 1 "P03P4L-04" H 10354 2705 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 10250 3250 50  0001 C CNN
F 3 "~" H 10150 2750 50  0001 C CNN
	1    10150 2750
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 2350 9650 2350
Wire Wire Line
	10250 2350 10250 2550
Wire Wire Line
	8750 2350 8500 2350
Connection ~ 8500 2350
Wire Wire Line
	8500 2350 8500 3950
$Comp
L Device:R R13
U 1 1 6411F665
P 9650 2550
F 0 "R13" H 9580 2504 50  0000 R CNN
F 1 "100k" H 9580 2595 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9580 2550 50  0001 C CNN
F 3 "~" H 9650 2550 50  0001 C CNN
	1    9650 2550
	-1   0    0    1   
$EndComp
Wire Wire Line
	9650 2400 9650 2350
Connection ~ 9650 2350
Wire Wire Line
	9650 2350 9050 2350
Wire Wire Line
	9950 2750 9800 2750
Wire Wire Line
	9650 2750 9650 2700
Wire Wire Line
	10250 2950 10250 3050
$Comp
L power:GND #PWR021
U 1 1 64133443
P 10250 3450
F 0 "#PWR021" H 10250 3200 50  0001 C CNN
F 1 "GND" H 10255 3277 50  0000 C CNN
F 2 "" H 10250 3450 50  0001 C CNN
F 3 "" H 10250 3450 50  0001 C CNN
	1    10250 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 3450 10250 3150
$Comp
L bpc:P03P4L-04 Q8
U 1 1 64144756
P 10150 4550
F 0 "Q8" H 10354 4596 50  0000 L CNN
F 1 "P03P4L-04" H 10354 4505 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-220-3_Vertical" H 10250 5050 50  0001 C CNN
F 3 "~" H 10150 4550 50  0001 C CNN
	1    10150 4550
	1    0    0    -1  
$EndComp
Wire Wire Line
	8500 3950 8700 3950
$Comp
L power:GND #PWR022
U 1 1 6414A57B
P 10250 5350
F 0 "#PWR022" H 10250 5100 50  0001 C CNN
F 1 "GND" H 10255 5177 50  0000 C CNN
F 2 "" H 10250 5350 50  0001 C CNN
F 3 "" H 10250 5350 50  0001 C CNN
	1    10250 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10250 5350 10250 5050
Wire Wire Line
	10250 4950 10250 4750
Wire Wire Line
	9000 3950 9750 3950
Wire Wire Line
	10250 3950 10250 4350
$Comp
L Device:R R14
U 1 1 641592C1
P 9750 4250
F 0 "R14" H 9680 4204 50  0000 R CNN
F 1 "100k" H 9680 4295 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 9680 4250 50  0001 C CNN
F 3 "~" H 9750 4250 50  0001 C CNN
	1    9750 4250
	-1   0    0    1   
$EndComp
Wire Wire Line
	9950 4550 9850 4550
Wire Wire Line
	9750 4550 9750 4400
Wire Wire Line
	9750 4100 9750 3950
Connection ~ 9750 3950
Wire Wire Line
	9750 3950 10250 3950
Text GLabel 1900 3350 1    50   Input ~ 0
Vin
Wire Wire Line
	1900 3350 1900 3500
Wire Wire Line
	3050 1100 3050 1250
$Comp
L Device:R R3
U 1 1 641B5435
P 2750 1250
F 0 "R3" V 2950 1350 50  0000 R CNN
F 1 "100k" V 2850 1350 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 2680 1250 50  0001 C CNN
F 3 "~" H 2750 1250 50  0001 C CNN
	1    2750 1250
	0    -1   -1   0   
$EndComp
Wire Wire Line
	2900 1250 3050 1250
Connection ~ 3050 1250
Wire Wire Line
	3050 1250 3050 1400
Wire Wire Line
	2750 1600 2550 1600
Wire Wire Line
	2450 1600 2450 1250
Wire Wire Line
	2450 1250 2600 1250
Wire Wire Line
	3200 1950 3050 1950
Wire Wire Line
	3050 1950 3050 1800
$Comp
L power:GND #PWR06
U 1 1 641E0128
P 3050 2350
F 0 "#PWR06" H 3050 2100 50  0001 C CNN
F 1 "GND" H 3055 2177 50  0000 C CNN
F 2 "" H 3050 2350 50  0001 C CNN
F 3 "" H 3050 2350 50  0001 C CNN
	1    3050 2350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR05
U 1 1 641E6350
P 2550 2400
F 0 "#PWR05" H 2550 2150 50  0001 C CNN
F 1 "GND" H 2555 2227 50  0000 C CNN
F 2 "" H 2550 2400 50  0001 C CNN
F 3 "" H 2550 2400 50  0001 C CNN
	1    2550 2400
	1    0    0    -1  
$EndComp
$Comp
L Device:R R1
U 1 1 641E6356
P 1950 1900
F 0 "R1" V 1743 1900 50  0000 C CNN
F 1 "100" V 1834 1900 50  0000 C CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 1880 1900 50  0001 C CNN
F 3 "~" H 1950 1900 50  0001 C CNN
	1    1950 1900
	0    1    1    0   
$EndComp
$Comp
L Device:R R2
U 1 1 641E635C
P 2100 2150
F 0 "R2" H 2030 2104 50  0000 R CNN
F 1 "100k" H 2030 2195 50  0000 R CNN
F 2 "Resistor_THT:R_Axial_DIN0309_L9.0mm_D3.2mm_P2.54mm_Vertical" V 2030 2150 50  0001 C CNN
F 3 "~" H 2100 2150 50  0001 C CNN
	1    2100 2150
	-1   0    0    1   
$EndComp
$Comp
L power:GND #PWR04
U 1 1 641E6362
P 2100 2400
F 0 "#PWR04" H 2100 2150 50  0001 C CNN
F 1 "GND" H 2105 2227 50  0000 C CNN
F 2 "" H 2100 2400 50  0001 C CNN
F 3 "" H 2100 2400 50  0001 C CNN
	1    2100 2400
	1    0    0    -1  
$EndComp
Wire Wire Line
	2100 2300 2100 2400
Wire Wire Line
	1800 1900 1650 1900
Connection ~ 9750 1100
Wire Wire Line
	9750 1100 9650 1100
Wire Wire Line
	9750 1700 9750 1900
Wire Wire Line
	9350 1400 9300 1400
Connection ~ 9300 1400
$Comp
L Transistor_BJT:2N2222A Q4
U 1 1 642593B4
P 9400 3150
F 0 "Q4" H 9900 3300 50  0000 L CNN
F 1 "2N2222A" H 9850 3000 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 9650 3075 50  0001 L CIN
F 3 "https://www.arrow.com/en/products/2n2222a/microsemi" H 9400 3150 50  0001 L CNN
F 4 "2N2222A" H 9400 3150 50  0001 C CNN "Arrow Part Number"
F 5 "https://www.arrow.com/en/products/2n2222a/microsemi" H 9400 3150 50  0001 C CNN "Arrow Price/Stock"
F 6 "Trans GP BJT NPN 50V 0.8A 3-Pin TO-18" H 9400 3150 50  0001 C CNN "Description"
F 7 "Microsemi Corporation" H 9400 3150 50  0001 C CNN "Manufacturer_Name"
F 8 "2N2222A" H 9400 3150 50  0001 C CNN "Manufacturer_Part_Number"
F 9 "494-2N2222A" H 9400 3150 50  0001 C CNN "Mouser Part Number"
F 10 "https://www.mouser.co.uk/ProductDetail/Microchip-Microsemi/2N2222A?qs=TXMzd3F6EylR6f6YErRW3Q%3D%3D" H 9400 3150 50  0001 C CNN "Mouser Price/Stock"
	1    9400 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	9800 3650 9800 3450
Wire Wire Line
	9400 3150 9300 3150
Wire Wire Line
	9300 3150 9300 3250
Connection ~ 9300 3150
Wire Wire Line
	9800 2850 9800 2750
Connection ~ 9800 2750
Wire Wire Line
	9800 2750 9650 2750
$Comp
L Transistor_BJT:2N2222A Q5
U 1 1 642705ED
P 9450 5100
F 0 "Q5" H 9950 5250 50  0000 L CNN
F 1 "2N2222A" H 9900 4950 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 9700 5025 50  0001 L CIN
F 3 "https://www.arrow.com/en/products/2n2222a/microsemi" H 9450 5100 50  0001 L CNN
F 4 "2N2222A" H 9450 5100 50  0001 C CNN "Arrow Part Number"
F 5 "https://www.arrow.com/en/products/2n2222a/microsemi" H 9450 5100 50  0001 C CNN "Arrow Price/Stock"
F 6 "Trans GP BJT NPN 50V 0.8A 3-Pin TO-18" H 9450 5100 50  0001 C CNN "Description"
F 7 "Microsemi Corporation" H 9450 5100 50  0001 C CNN "Manufacturer_Name"
F 8 "2N2222A" H 9450 5100 50  0001 C CNN "Manufacturer_Part_Number"
F 9 "494-2N2222A" H 9450 5100 50  0001 C CNN "Mouser Part Number"
F 10 "https://www.mouser.co.uk/ProductDetail/Microchip-Microsemi/2N2222A?qs=TXMzd3F6EylR6f6YErRW3Q%3D%3D" H 9450 5100 50  0001 C CNN "Mouser Price/Stock"
	1    9450 5100
	1    0    0    -1  
$EndComp
Wire Wire Line
	9850 5600 9850 5400
Wire Wire Line
	9450 5100 9400 5100
Connection ~ 9400 5100
Wire Wire Line
	9850 4800 9850 4550
Connection ~ 9850 4550
Wire Wire Line
	9850 4550 9750 4550
$Comp
L Transistor_BJT:2N2222A Q1
U 1 1 642836BB
P 2150 1900
F 0 "Q1" H 2650 2050 50  0000 L CNN
F 1 "2N2222A" H 2600 1750 50  0000 L CNN
F 2 "Package_TO_SOT_THT:TO-18-3" H 2400 1825 50  0001 L CIN
F 3 "https://www.arrow.com/en/products/2n2222a/microsemi" H 2150 1900 50  0001 L CNN
F 4 "2N2222A" H 2150 1900 50  0001 C CNN "Arrow Part Number"
F 5 "https://www.arrow.com/en/products/2n2222a/microsemi" H 2150 1900 50  0001 C CNN "Arrow Price/Stock"
F 6 "Trans GP BJT NPN 50V 0.8A 3-Pin TO-18" H 2150 1900 50  0001 C CNN "Description"
F 7 "Microsemi Corporation" H 2150 1900 50  0001 C CNN "Manufacturer_Name"
F 8 "2N2222A" H 2150 1900 50  0001 C CNN "Manufacturer_Part_Number"
F 9 "494-2N2222A" H 2150 1900 50  0001 C CNN "Mouser Part Number"
F 10 "https://www.mouser.co.uk/ProductDetail/Microchip-Microsemi/2N2222A?qs=TXMzd3F6EylR6f6YErRW3Q%3D%3D" H 2150 1900 50  0001 C CNN "Mouser Price/Stock"
	1    2150 1900
	1    0    0    -1  
$EndComp
Connection ~ 2550 1600
Wire Wire Line
	2550 1600 2450 1600
Wire Wire Line
	2100 1900 2150 1900
Wire Wire Line
	2100 2000 2100 1900
Connection ~ 2100 1900
Wire Wire Line
	2550 2400 2550 2200
$Comp
L bpc:BPC_ESP8266-01_ESP-01 U2
U 1 1 64003E7B
P 4950 4500
F 0 "U2" H 4950 5067 50  0000 C CNN
F 1 "BPC_ESP8266-01_ESP-01" H 4950 4976 50  0000 C CNN
F 2 "bpc:BPC_XCVR_ESP8266-01_ESP-01" H 4950 4500 50  0001 L BNN
F 3 "" H 4950 4500 50  0001 L BNN
F 4 "11.2 mm" H 4950 4500 50  0001 L BNN "MAXIMUM_PACKAGE_HEIGHT"
F 5 "V1.2" H 4950 4500 50  0001 L BNN "PARTREV"
F 6 "Manufacturer recommendations or IPC 7351B" H 4950 4500 50  0001 L BNN "STANDARD"
F 7 "AI-Thinker" H 4950 4500 50  0001 L BNN "MANUFACTURER"
	1    4950 4500
	1    0    0    -1  
$EndComp
Text GLabel 1700 3350 1    50   Output ~ 0
3.3V
Wire Wire Line
	1700 3350 1700 3500
Wire Wire Line
	5800 4200 5650 4200
$Comp
L power:GND #PWR09
U 1 1 64018899
P 5750 4950
F 0 "#PWR09" H 5750 4700 50  0001 C CNN
F 1 "GND" H 5755 4777 50  0000 C CNN
F 2 "" H 5750 4950 50  0001 C CNN
F 3 "" H 5750 4950 50  0001 C CNN
	1    5750 4950
	1    0    0    -1  
$EndComp
Wire Wire Line
	5650 4800 5750 4800
Wire Wire Line
	5750 4800 5750 4950
Text GLabel 2600 4000 2    50   Output ~ 0
NanoTx
Text GLabel 2600 3900 2    50   Input ~ 0
NanoRX
Wire Wire Line
	2600 3900 2300 3900
Wire Wire Line
	2300 4000 2600 4000
Text GLabel 4100 4600 0    50   Input ~ 0
NanoRX
Text GLabel 4100 4700 0    50   Output ~ 0
NanoTx
Wire Wire Line
	4250 4600 4100 4600
Wire Wire Line
	4100 4700 4250 4700
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 64051E46
P 1050 750
F 0 "J1" H 1130 742 50  0000 L CNN
F 1 "Conn_01x02" H 1130 651 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 1050 750 50  0001 C CNN
F 3 "~" H 1050 750 50  0001 C CNN
	1    1050 750 
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J3
U 1 1 6405B3E0
P 3400 1950
F 0 "J3" H 3480 1942 50  0000 L CNN
F 1 "Conn_01x02" H 3480 1851 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MKDS-1,5-2_1x02_P5.00mm_Horizontal" H 3400 1950 50  0001 C CNN
F 3 "~" H 3400 1950 50  0001 C CNN
	1    3400 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 2050 3050 2050
Wire Wire Line
	3050 2050 3050 2350
$Comp
L Connector_Generic:Conn_01x02 J4
U 1 1 6406B4DC
P 4250 1300
F 0 "J4" H 4330 1292 50  0000 L CNN
F 1 "Conn_01x02" H 4330 1201 50  0000 L CNN
F 2 "TerminalBlock_Phoenix:TerminalBlock_Phoenix_MPT-0,5-2-2.54_1x02_P2.54mm_Horizontal" H 4250 1300 50  0001 C CNN
F 3 "~" H 4250 1300 50  0001 C CNN
	1    4250 1300
	-1   0    0    -1  
$EndComp
NoConn ~ 1150 4500
Wire Wire Line
	1300 4500 1150 4500
Text GLabel 5800 3700 1    50   Input ~ 0
3.3V
$Comp
L Connector_Generic:Conn_01x02 J11
U 1 1 64031DCB
P 6100 3900
F 0 "J11" H 6180 3892 50  0000 L CNN
F 1 "Conn_01x02" H 6180 3801 50  0000 L CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6100 3900 50  0001 C CNN
F 3 "~" H 6100 3900 50  0001 C CNN
	1    6100 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 3700 5800 3900
Wire Wire Line
	5800 3900 5900 3900
Wire Wire Line
	5800 4200 5800 4000
Wire Wire Line
	5800 4000 5900 4000
$EndSCHEMATC
