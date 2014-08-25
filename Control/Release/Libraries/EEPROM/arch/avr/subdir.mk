################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/usr/local/arduino-1.5.4/libraries/EEPROM/arch/avr/EEPROM.cpp 

CPP_DEPS += \
./Libraries/EEPROM/arch/avr/EEPROM.cpp.d 

LINK_OBJ += \
./Libraries/EEPROM/arch/avr/EEPROM.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
Libraries/EEPROM/arch/avr/EEPROM.cpp.o: /usr/local/arduino-1.5.4/libraries/EEPROM/arch/avr/EEPROM.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '


