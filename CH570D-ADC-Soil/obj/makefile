################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include StdPeriphDriver/subdir.mk
-include Startup/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
CH570D-ADC-Soil.hex \

SECONDARY_LIST += \
CH570D-ADC-Soil.lst \

SECONDARY_SIZE += \
CH570D-ADC-Soil.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: CH570D-ADC-Soil.elf secondary-outputs

# Tool invocations
CH570D-ADC-Soil.elf: $(OBJS) $(USER_OBJS)
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -T "c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -L"c:\Users\seanw\mounriver-studio-projects\CH570D-ADC-Soil" -L"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/StdPeriphDriver" -Xlinker --print-memory-usage -Xlinker --print-memory-usage -Wl,-Map,"CH570D-ADC-Soil.map" --specs=nano.specs --specs=nosys.specs -o "CH570D-ADC-Soil.elf" $(OBJS)  $(LIBS)
CH570D-ADC-Soil.hex: CH570D-ADC-Soil.elf
	@	riscv-wch-elf-objcopy -O ihex "CH570D-ADC-Soil.elf" "CH570D-ADC-Soil.hex"
CH570D-ADC-Soil.lst: CH570D-ADC-Soil.elf
	@	riscv-wch-elf-objdump --source --all-headers --demangle -M xw --line-numbers --wide "CH570D-ADC-Soil.elf" > "CH570D-ADC-Soil.lst"
CH570D-ADC-Soil.siz: CH570D-ADC-Soil.elf
	riscv-wch-elf-size --format=berkeley "CH570D-ADC-Soil.elf"

# Other Targets
clean:
	-$(RM) $(OBJS) $(EXPANDS) $(CALLGRAPH_DOT) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(S_DEPS)$(S_UPPER_DEPS)$(ASM_DEPS)$(ASM_UPPER_DEPS)$(C_DEPS) CH570D-ADC-Soil.elf

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets