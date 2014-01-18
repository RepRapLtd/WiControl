################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/HeatProfile.cpp \
../src/Heating.cpp \
../src/Line.cpp \
../src/Wireless.cpp 

OBJS += \
./src/HeatProfile.o \
./src/Heating.o \
./src/Line.o \
./src/Wireless.o 

CPP_DEPS += \
./src/HeatProfile.d \
./src/Heating.d \
./src/Line.d \
./src/Wireless.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/lib -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


