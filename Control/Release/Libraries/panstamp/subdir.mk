################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/usr/local/arduino-1.0.5/libraries/panstamp/calibration.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/cc1101.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/panstamp.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/register.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/repeater.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/spi.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/swcommand.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/swpacket.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/swquery.cpp \
/usr/local/arduino-1.0.5/libraries/panstamp/swstatus.cpp 

CPP_DEPS += \
./Libraries/panstamp/calibration.cpp.d \
./Libraries/panstamp/cc1101.cpp.d \
./Libraries/panstamp/panstamp.cpp.d \
./Libraries/panstamp/register.cpp.d \
./Libraries/panstamp/repeater.cpp.d \
./Libraries/panstamp/spi.cpp.d \
./Libraries/panstamp/swcommand.cpp.d \
./Libraries/panstamp/swpacket.cpp.d \
./Libraries/panstamp/swquery.cpp.d \
./Libraries/panstamp/swstatus.cpp.d 

LINK_OBJ += \
./Libraries/panstamp/calibration.cpp.o \
./Libraries/panstamp/cc1101.cpp.o \
./Libraries/panstamp/panstamp.cpp.o \
./Libraries/panstamp/register.cpp.o \
./Libraries/panstamp/repeater.cpp.o \
./Libraries/panstamp/spi.cpp.o \
./Libraries/panstamp/swcommand.cpp.o \
./Libraries/panstamp/swpacket.cpp.o \
./Libraries/panstamp/swquery.cpp.o \
./Libraries/panstamp/swstatus.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
Libraries/panstamp/calibration.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/calibration.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/cc1101.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/cc1101.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/panstamp.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/panstamp.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/register.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/register.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/repeater.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/repeater.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/spi.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/spi.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/swcommand.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/swcommand.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/swpacket.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/swpacket.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/swquery.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/swquery.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Libraries/panstamp/swstatus.cpp.o: /usr/local/arduino-1.0.5/libraries/panstamp/swstatus.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


