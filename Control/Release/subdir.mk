################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CommandBuffer.cpp \
../Communicator.cpp \
../Control.cpp 

CPP_DEPS += \
./CommandBuffer.cpp.d \
./Communicator.cpp.d \
./Control.cpp.d 

LINK_OBJ += \
./CommandBuffer.cpp.o \
./Communicator.cpp.o \
./Control.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
CommandBuffer.cpp.o: ../CommandBuffer.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Communicator.cpp.o: ../Communicator.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '

Control.cpp.o: ../Control.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-g++" -c -g -Os -w -fno-exceptions -ffunction-sections -fdata-sections -MMD -mmcu=atmega328p -DF_CPU=8000000L -DARDUINO=152    -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/cores/arduino" -I"/usr/local/arduino-1.5.2/hardware/arduino/avr/variants/standard" -I"/usr/local/arduino-1.0.5/libraries/EEPROM" -I"/usr/local/arduino-1.0.5/libraries/panstamp" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


