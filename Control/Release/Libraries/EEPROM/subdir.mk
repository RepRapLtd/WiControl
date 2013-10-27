################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/usr/local/arduino-1.0.5/libraries/EEPROM/EEPROM.cpp 

CPP_DEPS += \
./Libraries/EEPROM/EEPROM.cpp.d 

LINK_OBJ += \
./Libraries/EEPROM/EEPROM.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
Libraries/EEPROM/EEPROM.cpp.o: /usr/local/arduino-1.0.5/libraries/EEPROM/EEPROM.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


