################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/CDC.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/HID.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/IPAddress.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/Print.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/Stream.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/Tone.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/USBCore.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/WMath.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/WString.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/main.cpp \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/new.cpp 

C_SRCS += \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/WInterrupts.c \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/hooks.c \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring.c \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring_analog.c \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring_digital.c \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring_pulse.c \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring_shift.c 

C_DEPS += \
./arduino/WInterrupts.c.d \
./arduino/hooks.c.d \
./arduino/wiring.c.d \
./arduino/wiring_analog.c.d \
./arduino/wiring_digital.c.d \
./arduino/wiring_pulse.c.d \
./arduino/wiring_shift.c.d 

AR_OBJ += \
./arduino/CDC.cpp.o \
./arduino/HID.cpp.o \
./arduino/HardwareSerial.cpp.o \
./arduino/IPAddress.cpp.o \
./arduino/Print.cpp.o \
./arduino/Stream.cpp.o \
./arduino/Tone.cpp.o \
./arduino/USBCore.cpp.o \
./arduino/WInterrupts.c.o \
./arduino/WMath.cpp.o \
./arduino/WString.cpp.o \
./arduino/hooks.c.o \
./arduino/main.cpp.o \
./arduino/new.cpp.o \
./arduino/wiring.c.o \
./arduino/wiring_analog.c.o \
./arduino/wiring_digital.c.o \
./arduino/wiring_pulse.c.o \
./arduino/wiring_shift.c.o 

CPP_DEPS += \
./arduino/CDC.cpp.d \
./arduino/HID.cpp.d \
./arduino/HardwareSerial.cpp.d \
./arduino/IPAddress.cpp.d \
./arduino/Print.cpp.d \
./arduino/Stream.cpp.d \
./arduino/Tone.cpp.d \
./arduino/USBCore.cpp.d \
./arduino/WMath.cpp.d \
./arduino/WString.cpp.d \
./arduino/main.cpp.d \
./arduino/new.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
arduino/CDC.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/CDC.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/HID.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/HID.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/HardwareSerial.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/HardwareSerial.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/IPAddress.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/IPAddress.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/Print.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/Print.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/Stream.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/Stream.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/Tone.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/Tone.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/USBCore.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/USBCore.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/WInterrupts.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/WInterrupts.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/WMath.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/WMath.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/WString.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/WString.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/hooks.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/hooks.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/main.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/main.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/new.cpp.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/new.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring_analog.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring_analog.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring_digital.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring_digital.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring_pulse.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring_pulse.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/wiring_shift.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/wiring_shift.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '


