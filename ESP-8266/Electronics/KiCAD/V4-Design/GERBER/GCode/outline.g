; pcb2gcode 2.0.0 
; Software-independent Gcode 

G94 ; Millimeters per minute feed rate. 
G21 ; Units == Millimeters. 

G90 ; Absolute coordinates. 
;G1 F3000 S10000 ; RPM spindle speed. 
G1 F300 F600.00000 ; Feedrate. 


G1 F3000 Z3.00000 ;Retract to tool change height
;T0
;M5      ;Spindle stop.
;G04 P1.00000 ;Wait for spindle to stop
;MSG, Change tool bit to cutter diameter 0.12000mm
;M6      ;Tool change.
;M0      ;Temporary machine stop.
;M3 ; Spindle on clockwise. 
;G04 P1.00000 ;Wait for spindle to get up to speed
;G04 P0 ; dwell for no time -- G64 should not smooth over this point 
G1 F3000 Z3.00000 ; retract 

G1 F3000 X-0.08497 Y0.14502 ; rapid move to begin. 
G1 F300 Z-0.07000 F300.00000 ; plunge. 
;G04 P0 ; dwell for no time -- G64 should not smooth over this point 
G1 F300 F600.00000
G1 F300 X-0.08497 Y75.14500
G1 F300 X-0.09508 Y75.17835
G1 F300 X-0.14499 Y75.20502
G1 F300 X-50.14499 Y75.20502
G1 F300 X-50.17832 Y75.19491
G1 F300 X-50.20499 Y75.14500
G1 F300 X-50.20499 Y0.14502
G1 F300 X-50.19488 Y0.11170
G1 F300 X-50.14499 Y0.08503
G1 F300 X-0.14499 Y0.08503
G1 F300 X-0.10255 Y0.10261
G1 F300 X-0.08497 Y0.14502

;G04 P0 ; dwell for no time -- G64 should not smooth over this point 
G1 F3000 Z3.000 ; retract 

;M5 ; Spindle off. 
;G04 P1.000000
;M9 ; Coolant off. 
M0 ; Program end. 

