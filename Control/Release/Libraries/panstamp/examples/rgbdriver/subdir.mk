################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/usr/local/arduino-1.5.4/libraries/panstamp/examples/rgbdriver/rgbled.cpp 

CPP_DEPS += \
./Libraries/panstamp/examples/rgbdriver/rgbled.cpp.d 

LINK_OBJ += \
./Libraries/panstamp/examples/rgbdriver/rgbled.cpp.o 


# Each subdirectory must supply rules for building sources it contributes
Libraries/panstamp/examples/rgbdriver/rgbled.cpp.o: /usr/local/arduino-1.5.4/libraries/panstamp/examples/rgbdriver/rgbled.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -x c++ "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '


