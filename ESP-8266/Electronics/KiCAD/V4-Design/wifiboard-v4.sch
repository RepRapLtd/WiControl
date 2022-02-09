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
L Connector:Conn_01x06_Male J5
U 1 1 61F7FBE0
P 5875 3400
F 0 "J5" H 5983 3781 50  0000 C CNN
F 1 "USB-3V3" H 5983 3690 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x06_P2.54mm_Vertical" H 5875 3400 50  0001 C CNN
F 3 "~" H 5875 3400 50  0001 C CNN
	1    5875 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	5675 4500 5775 4500
Wire Wire Line
	5775 4500 5775 3600
$Comp
L power:GND #PWR0101
U 1 1 61F816C9
P 6075 3700
F 0 "#PWR0101" H 6075 3450 50  0001 C CNN
F 1 "GND" H 6080 3527 50  0000 C CNN
F 2 "" H 6075 3700 50  0001 C CNN
F 3 "" H 6075 3700 50  0001 C CNN
	1    6075 3700
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x05_Male J3
U 1 1 61F81C05
P 3250 4900
F 0 "J3" H 3358 5281 50  0000 C CNN
F 1 "Aux1" H 3358 5190 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x05_P2.54mm_Vertical" H 3250 4900 50  0001 C CNN
F 3 "~" H 3250 4900 50  0001 C CNN
	1    3250 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	3875 4700 3450 4700
Wire Wire Line
	3875 4800 3450 4800
Wire Wire Line
	3875 4900 3450 4900
$Comp
L power:GND #PWR0102
U 1 1 61F82EFC
P 3450 5175
F 0 "#PWR0102" H 3450 4925 50  0001 C CNN
F 1 "GND" H 3455 5002 50  0000 C CNN
F 2 "" H 3450 5175 50  0001 C CNN
F 3 "" H 3450 5175 50  0001 C CNN
	1    3450 5175
	1    0    0    -1  
$EndComp
Wire Wire Line
	3450 5100 3450 5175
Wire Wire Line
	3875 5000 3450 5000
$Comp
L Connector:Conn_01x07_Male J4
U 1 1 61F83E9C
P 4725 5900
F 0 "J4" V 4652 5878 50  0000 C CNN
F 1 "Aux2" V 4561 5878 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x07_P2.54mm_Vertical" H 4725 5900 50  0001 C CNN
F 3 "~" H 4725 5900 50  0001 C CNN
	1    4725 5900
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5025 5600 5025 5700
Wire Wire Line
	4925 5600 4925 5700
Wire Wire Line
	4825 5600 4825 5700
Wire Wire Line
	4725 5600 4725 5700
Wire Wire Line
	4625 5600 4625 5700
Wire Wire Line
	4525 5600 4525 5700
$Comp
L power:GND #PWR0103
U 1 1 61F86403
P 4150 5700
F 0 "#PWR0103" H 4150 5450 50  0001 C CNN
F 1 "GND" H 4155 5527 50  0000 C CNN
F 2 "" H 4150 5700 50  0001 C CNN
F 3 "" H 4150 5700 50  0001 C CNN
	1    4150 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	4425 5700 4150 5700
$Comp
L power:+3.3V #PWR0104
U 1 1 61F86FBE
P 3700 5325
F 0 "#PWR0104" H 3700 5175 50  0001 C CNN
F 1 "+3.3V" H 3715 5498 50  0000 C CNN
F 2 "" H 3700 5325 50  0001 C CNN
F 3 "" H 3700 5325 50  0001 C CNN
	1    3700 5325
	1    0    0    -1  
$EndComp
Wire Wire Line
	3875 5100 3875 5325
Wire Wire Line
	3875 5325 3775 5325
$Comp
L Device:C C4
U 1 1 61F87BD1
P 3775 5475
F 0 "C4" H 3890 5521 50  0000 L CNN
F 1 "0.1u" H 3890 5430 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 3813 5325 50  0001 C CNN
F 3 "~" H 3775 5475 50  0001 C CNN
	1    3775 5475
	1    0    0    -1  
$EndComp
Connection ~ 3775 5325
Wire Wire Line
	3775 5325 3700 5325
Wire Wire Line
	4150 5700 3775 5700
Wire Wire Line
	3775 5700 3775 5625
Connection ~ 4150 5700
$Comp
L Connector:Conn_01x02_Male J2
U 1 1 61F889EB
P 2000 4500
F 0 "J2" H 2108 4681 50  0000 C CNN
F 1 "thermistor" H 2108 4590 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 2000 4500 50  0001 C CNN
F 3 "~" H 2000 4500 50  0001 C CNN
	1    2000 4500
	1    0    0    -1  
$EndComp
Wire Wire Line
	3875 4500 2675 4500
$Comp
L Device:C C2
U 1 1 61F897EB
P 2475 4650
F 0 "C2" H 2590 4696 50  0000 L CNN
F 1 "1u" H 2590 4605 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 2513 4500 50  0001 C CNN
F 3 "~" H 2475 4650 50  0001 C CNN
	1    2475 4650
	1    0    0    -1  
$EndComp
Connection ~ 2475 4500
Wire Wire Line
	2475 4500 2200 4500
$Comp
L power:GND #PWR0105
U 1 1 61F89CC2
P 2475 4900
F 0 "#PWR0105" H 2475 4650 50  0001 C CNN
F 1 "GND" H 2480 4727 50  0000 C CNN
F 2 "" H 2475 4900 50  0001 C CNN
F 3 "" H 2475 4900 50  0001 C CNN
	1    2475 4900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2475 4800 2475 4850
Wire Wire Line
	2200 4600 2200 4850
Wire Wire Line
	2200 4850 2475 4850
Connection ~ 2475 4850
Wire Wire Line
	2475 4850 2475 4900
$Comp
L Device:R R2
U 1 1 61F8B094
P 2675 4350
F 0 "R2" H 2745 4396 50  0000 L CNN
F 1 "10K" H 2745 4305 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 2605 4350 50  0001 C CNN
F 3 "~" H 2675 4350 50  0001 C CNN
	1    2675 4350
	1    0    0    -1  
$EndComp
Connection ~ 2675 4500
Wire Wire Line
	2675 4500 2475 4500
$Comp
L power:+3.3V #PWR0106
U 1 1 61F8B72F
P 2675 4125
F 0 "#PWR0106" H 2675 3975 50  0001 C CNN
F 1 "+3.3V" H 2690 4298 50  0000 C CNN
F 2 "" H 2675 4125 50  0001 C CNN
F 3 "" H 2675 4125 50  0001 C CNN
	1    2675 4125
	1    0    0    -1  
$EndComp
Wire Wire Line
	2675 4125 2675 4200
$Comp
L Device:R R3
U 1 1 61F8C73D
P 3475 4050
F 0 "R3" H 3545 4096 50  0000 L CNN
F 1 "10K" H 3545 4005 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3405 4050 50  0001 C CNN
F 3 "~" H 3475 4050 50  0001 C CNN
	1    3475 4050
	1    0    0    -1  
$EndComp
Wire Wire Line
	3875 4600 3475 4600
Wire Wire Line
	3475 4600 3475 4200
$Comp
L power:GND #PWR0107
U 1 1 61F8E52B
P 4000 4025
F 0 "#PWR0107" H 4000 3775 50  0001 C CNN
F 1 "GND" H 4005 3852 50  0000 C CNN
F 2 "" H 4000 4025 50  0001 C CNN
F 3 "" H 4000 4025 50  0001 C CNN
	1    4000 4025
	1    0    0    -1  
$EndComp
$Comp
L Device:C C5
U 1 1 61F8E953
P 4000 3750
F 0 "C5" H 4115 3796 50  0000 L CNN
F 1 "0.1u" H 4115 3705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0805_2012Metric" H 4038 3600 50  0001 C CNN
F 3 "~" H 4000 3750 50  0001 C CNN
	1    4000 3750
	1    0    0    -1  
$EndComp
$Comp
L Device:R R4
U 1 1 61F8ED61
P 4000 3300
F 0 "R4" H 4070 3346 50  0000 L CNN
F 1 "10K" H 4070 3255 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 3930 3300 50  0001 C CNN
F 3 "~" H 4000 3300 50  0001 C CNN
	1    4000 3300
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0108
U 1 1 61F8F107
P 4000 3025
F 0 "#PWR0108" H 4000 2875 50  0001 C CNN
F 1 "+3.3V" H 4015 3198 50  0000 C CNN
F 2 "" H 4000 3025 50  0001 C CNN
F 3 "" H 4000 3025 50  0001 C CNN
	1    4000 3025
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 3025 4000 3125
Wire Wire Line
	4000 3450 4000 3525
Wire Wire Line
	4000 3900 4000 3975
Wire Wire Line
	3875 4400 3775 4400
Wire Wire Line
	3775 4400 3775 3525
Wire Wire Line
	3775 3525 4000 3525
Connection ~ 4000 3525
Wire Wire Line
	4000 3525 4000 3575
$Comp
L Switch:SW_Push SW1
U 1 1 61F92018
P 4375 3775
F 0 "SW1" V 4329 3923 50  0000 L CNN
F 1 "reset" V 4420 3923 50  0000 L CNN
F 2 "Button_Switch_SMD:SW_SPST_TL3342" H 4375 3975 50  0001 C CNN
F 3 "~" H 4375 3975 50  0001 C CNN
	1    4375 3775
	0    1    1    0   
$EndComp
Wire Wire Line
	4375 3975 4000 3975
Connection ~ 4000 3975
Wire Wire Line
	4000 3975 4000 4025
Wire Wire Line
	4375 3575 4000 3575
Connection ~ 4000 3575
Wire Wire Line
	4000 3575 4000 3600
$Comp
L Device:R R1
U 1 1 61F855E5
P 1900 3125
F 0 "R1" H 1970 3171 50  0000 L CNN
F 1 "6R8" H 1970 3080 50  0000 L CNN
F 2 "Resistor_THT:R_Axial_DIN0414_L11.9mm_D4.5mm_P15.24mm_Horizontal" V 1830 3125 50  0001 C CNN
F 3 "~" H 1900 3125 50  0001 C CNN
	1    1900 3125
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3300 1900 3275
$Comp
L power:GND #PWR0109
U 1 1 61F86DA4
P 2500 3675
F 0 "#PWR0109" H 2500 3425 50  0001 C CNN
F 1 "GND" H 2505 3502 50  0000 C CNN
F 2 "" H 2500 3675 50  0001 C CNN
F 3 "" H 2500 3675 50  0001 C CNN
	1    2500 3675
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR0110
U 1 1 61F88262
P 1900 2875
F 0 "#PWR0110" H 1900 2725 50  0001 C CNN
F 1 "+12V" H 1915 3048 50  0000 C CNN
F 2 "" H 1900 2875 50  0001 C CNN
F 3 "" H 1900 2875 50  0001 C CNN
	1    1900 2875
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 2875 1900 2925
$Comp
L Connector:Conn_01x02_Male J1
U 1 1 61F89C4D
P 1100 2925
F 0 "J1" H 1208 3106 50  0000 C CNN
F 1 "PWR" H 1208 3015 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 1100 2925 50  0001 C CNN
F 3 "~" H 1100 2925 50  0001 C CNN
	1    1100 2925
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 2925 1300 2925
Connection ~ 1900 2925
Wire Wire Line
	1900 2925 1900 2975
Wire Wire Line
	1300 3025 1300 3625
Wire Wire Line
	1300 3625 1900 3625
$Comp
L power:+3.3V #PWR0111
U 1 1 61F8DBBE
P 3000 3300
F 0 "#PWR0111" H 3000 3150 50  0001 C CNN
F 1 "+3.3V" H 3015 3473 50  0000 C CNN
F 2 "" H 3000 3300 50  0001 C CNN
F 3 "" H 3000 3300 50  0001 C CNN
	1    3000 3300
	1    0    0    -1  
$EndComp
$Comp
L Device:CP C1
U 1 1 61F9006A
P 1900 3475
F 0 "C1" H 2018 3521 50  0000 L CNN
F 1 "1000u" H 2018 3430 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 1938 3325 50  0001 C CNN
F 3 "~" H 1900 3475 50  0001 C CNN
	1    1900 3475
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3300 1900 3325
Connection ~ 1900 3300
Wire Wire Line
	3000 3350 3000 3300
Connection ~ 3000 3300
$Comp
L Device:CP C3
U 1 1 61F94E9D
P 3000 3500
F 0 "C3" H 3118 3546 50  0000 L CNN
F 1 "1000u" H 3118 3455 50  0000 L CNN
F 2 "Capacitor_THT:CP_Radial_D10.0mm_P5.00mm" H 3038 3350 50  0001 C CNN
F 3 "~" H 3000 3500 50  0001 C CNN
	1    3000 3500
	1    0    0    -1  
$EndComp
Wire Wire Line
	4000 3125 3625 3125
Wire Wire Line
	3625 3125 3625 3300
Wire Wire Line
	3625 3300 3475 3300
Connection ~ 4000 3125
Wire Wire Line
	4000 3125 4000 3150
Wire Wire Line
	3475 3300 3475 3900
Connection ~ 3475 3300
Wire Wire Line
	3475 3300 3000 3300
$Comp
L Connector:Conn_01x02_Male J6
U 1 1 61FAB057
P 6550 5100
F 0 "J6" H 6658 5281 50  0000 C CNN
F 1 "PROG" H 6658 5190 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 6550 5100 50  0001 C CNN
F 3 "~" H 6550 5100 50  0001 C CNN
	1    6550 5100
	-1   0    0    1   
$EndComp
Wire Wire Line
	6350 5100 6225 5100
Wire Wire Line
	6350 4800 6350 5000
$Comp
L Device:R R8
U 1 1 61FB129D
P 6350 4575
F 0 "R8" H 6420 4621 50  0000 L CNN
F 1 "10K" H 6420 4530 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6280 4575 50  0001 C CNN
F 3 "~" H 6350 4575 50  0001 C CNN
	1    6350 4575
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 4725 6350 4800
Connection ~ 6350 4800
$Comp
L power:+3.3V #PWR0112
U 1 1 61FB3335
P 6350 4350
F 0 "#PWR0112" H 6350 4200 50  0001 C CNN
F 1 "+3.3V" H 6365 4523 50  0000 C CNN
F 2 "" H 6350 4350 50  0001 C CNN
F 3 "" H 6350 4350 50  0001 C CNN
	1    6350 4350
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 4350 6350 4375
$Comp
L Device:R R6
U 1 1 61FB552F
P 6075 5000
F 0 "R6" H 6145 5046 50  0000 L CNN
F 1 "10K" H 6145 4955 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6005 5000 50  0001 C CNN
F 3 "~" H 6075 5000 50  0001 C CNN
	1    6075 5000
	0    1    1    0   
$EndComp
Wire Wire Line
	6225 5000 6225 5100
Connection ~ 6225 5100
$Comp
L Device:R R7
U 1 1 61FB954C
P 6200 4575
F 0 "R7" H 6270 4621 50  0000 L CNN
F 1 "10K" H 6270 4530 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6130 4575 50  0001 C CNN
F 3 "~" H 6200 4575 50  0001 C CNN
	1    6200 4575
	1    0    0    -1  
$EndComp
Wire Wire Line
	6200 4900 6200 4725
Wire Wire Line
	6200 4425 6200 4375
Wire Wire Line
	6200 4375 6350 4375
Connection ~ 6350 4375
Wire Wire Line
	6350 4375 6350 4425
$Comp
L power:GND #PWR0113
U 1 1 61FBE193
P 5675 5200
F 0 "#PWR0113" H 5675 4950 50  0001 C CNN
F 1 "GND" H 5680 5027 50  0000 C CNN
F 2 "" H 5675 5200 50  0001 C CNN
F 3 "" H 5675 5200 50  0001 C CNN
	1    5675 5200
	1    0    0    -1  
$EndComp
Wire Wire Line
	5675 4800 6350 4800
Wire Wire Line
	5675 4900 6200 4900
Wire Wire Line
	5675 5000 5925 5000
Wire Wire Line
	5675 5100 5675 5200
Wire Wire Line
	5675 5100 6225 5100
Wire Wire Line
	5675 4700 5875 4700
Wire Wire Line
	5875 4700 5875 5375
$Comp
L power:GND #PWR0114
U 1 1 61FDB9EF
P 5875 6125
F 0 "#PWR0114" H 5875 5875 50  0001 C CNN
F 1 "GND" H 5880 5952 50  0000 C CNN
F 2 "" H 5875 6125 50  0001 C CNN
F 3 "" H 5875 6125 50  0001 C CNN
	1    5875 6125
	1    0    0    -1  
$EndComp
Wire Wire Line
	5875 6025 5875 6125
Wire Wire Line
	5675 4600 6050 4600
$Comp
L Device:R R9
U 1 1 61FE2D1A
P 6925 4075
F 0 "R9" H 6995 4121 50  0000 L CNN
F 1 "10K" H 6995 4030 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6855 4075 50  0001 C CNN
F 3 "~" H 6925 4075 50  0001 C CNN
	1    6925 4075
	0    1    1    0   
$EndComp
Wire Wire Line
	7175 4075 7075 4075
$Comp
L power:GND #PWR0115
U 1 1 61FE9A03
P 7475 4400
F 0 "#PWR0115" H 7475 4150 50  0001 C CNN
F 1 "GND" H 7480 4227 50  0000 C CNN
F 2 "" H 7475 4400 50  0001 C CNN
F 3 "" H 7475 4400 50  0001 C CNN
	1    7475 4400
	1    0    0    -1  
$EndComp
Connection ~ 5675 5100
Wire Wire Line
	6050 4600 6050 4075
Wire Wire Line
	5875 5675 5875 5725
$Comp
L Device:LED D1
U 1 1 61FDAFF4
P 5875 5875
F 0 "D1" V 5914 5757 50  0000 R CNN
F 1 "LED" V 5823 5757 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 5875 5875 50  0001 C CNN
F 3 "~" H 5875 5875 50  0001 C CNN
	1    5875 5875
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R5
U 1 1 61FDA492
P 5875 5525
F 0 "R5" H 5945 5571 50  0000 L CNN
F 1 "180R" H 5945 5480 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 5805 5525 50  0001 C CNN
F 3 "~" H 5875 5525 50  0001 C CNN
	1    5875 5525
	-1   0    0    1   
$EndComp
$Comp
L wifiboard-v4-rescue:ESP-12E-esp-8266 U1
U 1 1 61F7D88E
P 4775 4700
F 0 "U1" H 4775 5465 50  0000 C CNN
F 1 "ESP-12E" H 4775 5374 50  0000 C CNN
F 2 "esp-8266:ESP-12E" H 4775 4700 50  0001 C CNN
F 3 "http://l0l.org.uk/2014/12/esp8266-modules-hardware-guide-gotta-catch-em-all/" H 4775 4700 50  0001 C CNN
	1    4775 4700
	1    0    0    -1  
$EndComp
$Comp
L Device:LED D2
U 1 1 6200681E
P 7025 5550
F 0 "D2" V 7064 5432 50  0000 R CNN
F 1 "LED" V 6973 5432 50  0000 R CNN
F 2 "LED_THT:LED_D3.0mm" H 7025 5550 50  0001 C CNN
F 3 "~" H 7025 5550 50  0001 C CNN
	1    7025 5550
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R10
U 1 1 6200728C
P 7025 5150
F 0 "R10" H 7095 5196 50  0000 L CNN
F 1 "180R" H 7095 5105 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 6955 5150 50  0001 C CNN
F 3 "~" H 7025 5150 50  0001 C CNN
	1    7025 5150
	1    0    0    -1  
$EndComp
$Comp
L power:+12V #PWR0116
U 1 1 6200CEA4
P 8000 3025
F 0 "#PWR0116" H 8000 2875 50  0001 C CNN
F 1 "+12V" H 8015 3198 50  0000 C CNN
F 2 "" H 8000 3025 50  0001 C CNN
F 3 "" H 8000 3025 50  0001 C CNN
	1    8000 3025
	1    0    0    -1  
$EndComp
$Comp
L Connector:Conn_01x03_Male J7
U 1 1 620106EE
P 8200 3850
F 0 "J7" H 8172 3782 50  0000 R CNN
F 1 "OUT" H 8172 3873 50  0000 R CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x03_P2.54mm_Vertical" H 8200 3850 50  0001 C CNN
F 3 "~" H 8200 3850 50  0001 C CNN
	1    8200 3850
	-1   0    0    1   
$EndComp
Wire Wire Line
	8000 4325 8000 3950
Wire Wire Line
	6075 3600 6075 3700
Wire Wire Line
	5675 3600 5675 4400
Wire Wire Line
	8000 3025 8000 3475
Wire Wire Line
	7475 4275 7475 4325
Wire Wire Line
	8000 3850 7775 3850
Wire Wire Line
	7475 3850 7475 3875
Wire Wire Line
	8000 4325 7475 4325
Connection ~ 7475 4325
Wire Wire Line
	7475 4325 7475 4400
$Comp
L wifiboard-v4-rescue:CPC1966Y-cpc1966y K1
U 1 1 61FAC409
P 8275 3075
F 0 "K1" V 8813 2647 50  0000 R CNN
F 1 "CPC1966Y" V 8722 2647 50  0000 R CNN
F 2 "reprapltd-kicad:CPC1966Y" H 9325 3175 50  0001 L CNN
F 3 "https://datasheet.datasheetarchive.com/originals/distributors/Datasheets_SAMA/4bd38bcd53ac06cbf93d43ee226943fd.pdf" H 9325 3075 50  0001 L CNN
F 4 "IXYS 3 A rms SPNO Solid State Relay, Zero Crossing, PCB Mount SCR, 240 V ac Maximum Load" H 9325 2975 50  0001 L CNN "Description"
F 5 "10" H 9325 2875 50  0001 L CNN "Height"
F 6 "LITTELFUSE" H 9325 2775 50  0001 L CNN "Manufacturer_Name"
F 7 "CPC1966Y" H 9325 2675 50  0001 L CNN "Manufacturer_Part_Number"
F 8 "849-CPC1966Y" H 9325 2575 50  0001 L CNN "Mouser Part Number"
F 9 "https://www.mouser.co.uk/ProductDetail/IXYS-Integrated-Circuits/CPC1966Y?qs=8uBHJDVwVqxNfAOiNv29Iw%3D%3D" H 9325 2475 50  0001 L CNN "Mouser Price/Stock"
F 10 "CPC1966Y" H 9325 2375 50  0001 L CNN "Arrow Part Number"
F 11 "https://www.arrow.com/en/products/cpc1966y/ixys" H 9325 2275 50  0001 L CNN "Arrow Price/Stock"
	1    8275 3075
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R11
U 1 1 61FAD78C
P 8375 3325
F 0 "R11" H 8445 3371 50  0000 L CNN
F 1 "1K" H 8445 3280 50  0000 L CNN
F 2 "Resistor_SMD:R_0805_2012Metric" V 8305 3325 50  0001 C CNN
F 3 "~" H 8375 3325 50  0001 C CNN
	1    8375 3325
	1    0    0    -1  
$EndComp
Wire Wire Line
	8375 3075 8375 3175
Wire Wire Line
	8375 3475 8000 3475
Connection ~ 8000 3475
Wire Wire Line
	8000 3475 8000 3750
Wire Wire Line
	8275 3075 8275 3250
Wire Wire Line
	8275 3250 7775 3250
Wire Wire Line
	7775 3250 7775 3850
Connection ~ 7775 3850
Wire Wire Line
	7775 3850 7475 3850
$Comp
L Connector:Screw_Terminal_01x02 J8
U 1 1 61FBF554
P 9275 3125
F 0 "J8" H 9355 3117 50  0000 L CNN
F 1 "mains" H 9355 3026 50  0000 L CNN
F 2 "TerminalBlock:TerminalBlock_bornier-2_P5.08mm" H 9275 3125 50  0001 C CNN
F 3 "~" H 9275 3125 50  0001 C CNN
	1    9275 3125
	1    0    0    -1  
$EndComp
Wire Wire Line
	9075 3125 8575 3125
Wire Wire Line
	8575 3125 8575 3075
Wire Wire Line
	8475 3075 8475 3225
Wire Wire Line
	8475 3225 9075 3225
$Comp
L Transistor_FET:QM6006D Q1
U 1 1 61F9F639
P 7375 4075
F 0 "Q1" H 7580 4121 50  0000 L CNN
F 1 "FDD8870" H 7580 4030 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 7575 4000 50  0001 L CIN
F 3 "http://www.jaolen.com/images/pdf/QM6006D.pdf" H 7375 4075 50  0001 L CNN
	1    7375 4075
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H4
U 1 1 61FAD79F
P 3950 6500
F 0 "H4" H 4050 6546 50  0000 L CNN
F 1 "MountingHole" H 4050 6455 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm" H 3950 6500 50  0001 C CNN
F 3 "~" H 3950 6500 50  0001 C CNN
	1    3950 6500
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H3
U 1 1 61FAE0AC
P 3675 6500
F 0 "H3" H 3775 6546 50  0000 L CNN
F 1 "MountingHole" H 3775 6455 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm" H 3675 6500 50  0001 C CNN
F 3 "~" H 3675 6500 50  0001 C CNN
	1    3675 6500
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H2
U 1 1 61FAE4A9
P 3400 6500
F 0 "H2" H 3500 6546 50  0000 L CNN
F 1 "MountingHole" H 3500 6455 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm" H 3400 6500 50  0001 C CNN
F 3 "~" H 3400 6500 50  0001 C CNN
	1    3400 6500
	1    0    0    -1  
$EndComp
$Comp
L Mechanical:MountingHole H1
U 1 1 61FAE960
P 3125 6500
F 0 "H1" H 3225 6546 50  0000 L CNN
F 1 "MountingHole" H 3225 6455 50  0000 L CNN
F 2 "MountingHole:MountingHole_2.5mm" H 3125 6500 50  0001 C CNN
F 3 "~" H 3125 6500 50  0001 C CNN
	1    3125 6500
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0117
U 1 1 62033A97
P 7025 5825
F 0 "#PWR0117" H 7025 5575 50  0001 C CNN
F 1 "GND" H 7030 5652 50  0000 C CNN
F 2 "" H 7025 5825 50  0001 C CNN
F 3 "" H 7025 5825 50  0001 C CNN
	1    7025 5825
	1    0    0    -1  
$EndComp
Wire Wire Line
	7025 5300 7025 5400
Wire Wire Line
	7025 5700 7025 5825
Wire Wire Line
	7025 4925 7025 5000
$Comp
L Connector:Conn_01x02_Male J9
U 1 1 61FC195B
P 7100 4550
F 0 "J9" H 7208 4731 50  0000 C CNN
F 1 "led-select" H 7208 4640 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical" H 7100 4550 50  0001 C CNN
F 3 "~" H 7100 4550 50  0001 C CNN
	1    7100 4550
	-1   0    0    1   
$EndComp
Wire Wire Line
	6050 4075 6675 4075
Wire Wire Line
	6675 4075 6675 4450
Wire Wire Line
	6675 4450 6725 4450
Connection ~ 6675 4075
Wire Wire Line
	6675 4075 6775 4075
Wire Wire Line
	7025 4925 6900 4925
Wire Wire Line
	6900 4925 6900 4750
$Comp
L Regulator_Linear:L7805 U2
U 1 1 61FDBAF2
P 2500 3300
F 0 "U2" H 2500 3542 50  0000 C CNN
F 1 "78033" H 2500 3451 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:TO-252-2" H 2525 3150 50  0001 L CIN
F 3 "http://www.st.com/content/ccc/resource/technical/document/datasheet/41/4f/b3/b0/12/d4/47/88/CD00000444.pdf/files/CD00000444.pdf/jcr:content/translations/en.CD00000444.pdf" H 2500 3250 50  0001 C CNN
	1    2500 3300
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 3300 2200 3300
Wire Wire Line
	2800 3300 3000 3300
Wire Wire Line
	2500 3600 2500 3625
Wire Wire Line
	3000 3650 2500 3650
Connection ~ 2500 3650
Wire Wire Line
	2500 3650 2500 3675
Wire Wire Line
	1900 3625 2500 3625
Connection ~ 1900 3625
Connection ~ 2500 3625
Wire Wire Line
	2500 3625 2500 3650
$Comp
L Jumper:SolderJumper_2_Bridged JP1
U 1 1 61FD9520
P 6725 4600
F 0 "JP1" V 6679 4668 50  0000 L CNN
F 1 "SolderJumper_2_Bridged" V 6770 4668 50  0000 L CNN
F 2 "Jumper:SolderJumper-2_P1.3mm_Bridged_RoundedPad1.0x1.5mm" H 6725 4600 50  0001 C CNN
F 3 "~" H 6725 4600 50  0001 C CNN
	1    6725 4600
	0    1    1    0   
$EndComp
Connection ~ 6725 4450
Wire Wire Line
	6725 4450 6900 4450
Wire Wire Line
	6725 4750 6900 4750
Connection ~ 6900 4750
Wire Wire Line
	6900 4750 6900 4550
$EndSCHEMATC
