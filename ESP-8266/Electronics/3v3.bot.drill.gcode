;(.../Electronics/Eagle/pcb-gcode/pcb-gcode.ulp)
;(Copyright 2005 - 2012 by John Johnson)
;(See readme.txt for licensing terms.)
;(This file generated from the board:)
;(.../WiControl/ESP-8266/Electronics/3v3.brd)
;(Current profile is .../pcb-gcode/profiles/generic.pp  )
;(This file generated 15/01/2019 15:39)
;(Settings from pcb-machine.h)
;(spindle on time = 1000.0000)
;(spindle speed = 20000.0000)
;(tool change at 0.0000 0.0000 5.0000 )
;(feed rate xy = F0.00  )
;(feed rate z  = F200.00 )
;(Z Axis Settings)
;(  High     Up        Down     Drill)
;(5.0000 	2.0000 	-0.2000 	-2.0000 )
;(Settings from pcb-defaults.h)
;(isolate min = 0.0250)
;(isolate max = 0.5000)
;(isolate step = 0.0800)
;(Generated bottom outlines, bottom drill, )
;(Unit of measure: mm)
;( Tool|       Size       |  Min Sub |  Max Sub |   Count )
;( T01  1.016mm 0.0400in 0.0000in 0.0000in )
;(Metric Mode)
G21
;(Absolute Coordinates)
G90
;S20000
G00 Z5.0000  F3000
G00 X0.0000 Y0.0000  F3000
;M03 S1
G04 P1000.000000
;M03 S0
;M06 T01  ; 1.0160 
G01 Z0.0000 F200.00 
;M06 
G00 Z2.0000  F3000
;M03 S1
G04 P1000.000000
G00 X-2.0100 Y2.6100  F3000
G01 Z-2.0000 F200.00 
G00 Z2.0000  F3000
G00 X-2.0100 Y7.6900  F3000
G01 Z-2.0000 F200.00 
G00 Z2.0000  F3000
G00 X-14.7100 Y3.8800  F3000
G01 Z-2.0000 F200.00 
G00 Z2.0000  F3000
;T01 
G00 Z5.0000  F3000
;M03 S0
M0
