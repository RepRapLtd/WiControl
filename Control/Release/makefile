################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include arduino/core/subdir.mk
-include Libraries/panstamp/subdir.mk
-include Libraries/EEPROM/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
/home/ensab/Panstamp/WiControl/Control/Release/arduino.ar \

ELF += \
Control.elf \

EEP += \
Control.eep \

FLASH_IMAGE += \
Release/Control.hex \


# All Target
all: Control

# Tool invocations
Control: $(FLASH_IMAGE)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-size" -A "/home/ensab/Panstamp/WiControl/Control/Release/Control.hex"
	@echo 'Finished building target: $@'
	@echo ' '

/home/ensab/Panstamp/WiControl/Control/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-ar" rcs "/home/ensab/Panstamp/WiControl/Control/Release/arduino.ar"   $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

Control.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-gcc" -Os -Wl,--gc-sections -mmcu=atmega328p -o "/home/ensab/Panstamp/WiControl/Control/Release/Control.elf"   $(LINK_OBJ) $(AR)  "/home/ensab/Panstamp/WiControl/Control/Release/arduino.ar" "-L/home/ensab/Panstamp/WiControl/Control/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

Control.eep: $(ELF)
	@echo 'Create eeprom image'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-objcopy" -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 "/home/ensab/Panstamp/WiControl/Control/Release/Control.elf" "/home/ensab/Panstamp/WiControl/Control/Release/Control.eep"
	@echo 'Finished building: $@'
	@echo ' '

Release/Control.hex: $(EEP)
	@echo 'Create Flash image (ihex format)'
	"/usr/local/arduino-1.5.2/hardware/tools/avr/bin/avr-objcopy" -O ihex -R .eeprom "/home/ensab/Panstamp/WiControl/Control/Release/Control.elf" "/home/ensab/Panstamp/WiControl/Control/Release/Control.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_DEPS)$(AR)$(FLASH_IMAGE)$(EEP)$(CXX_DEPS)$(C++_DEPS)$(ELF)$(CC_DEPS)$(AR_OBJ)$(CPP_DEPS)$(LINK_OBJ)$(C_UPPER_DEPS)$(SIZEDUMMY) Control
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
