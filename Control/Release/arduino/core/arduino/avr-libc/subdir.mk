################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/avr-libc/malloc.c \
/usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/avr-libc/realloc.c 

C_DEPS += \
./arduino/malloc.c.d \
./arduino/realloc.c.d 

AR_OBJ += \
./arduino/malloc.c.o \
./arduino/realloc.c.o 


# Each subdirectory must supply rules for building sources it contributes
arduino/malloc.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/avr-libc/malloc.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '

arduino/realloc.c.o: /usr/local/arduino-1.5.4/hardware/arduino/avr/cores/arduino/avr-libc/realloc.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	-I"/usr/local/arduino-1.5.4/hardware/arduino/avr/cores" -I/usr/local/arduino-1.5.4/hardware/tools/avr/lib/avr/include -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" "$<"  "$@"
	@echo 'Finished building: $<'
	@echo ' '


