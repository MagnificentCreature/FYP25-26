################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/I2C/subdir.mk
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
EC-reset.hex \

SECONDARY_LIST += \
EC-reset.lst \

SECONDARY_SIZE += \
EC-reset.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: EC-reset.elf secondary-outputs

# Tool invocations
EC-reset.elf: $(OBJS) $(USER_OBJS_ESCAPE)
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -T "c:\Users\seanw\mounriver-studio-projects\EC-reset\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -L"c:\Users\seanw\mounriver-studio-projects\EC-reset" -L"c:\Users\seanw\mounriver-studio-projects\EC-reset\StdPeriphDriver" -Xlinker --print-memory-usage -Wl,-Map,"EC-reset.map" --specs=nano.specs --specs=nosys.specs -o "EC-reset.elf" $(OBJS) $(LIBS)
EC-reset.hex: EC-reset.elf
	@	riscv-wch-elf-objcopy -O ihex "EC-reset.elf" "EC-reset.hex"
EC-reset.lst: EC-reset.elf
	@	riscv-wch-elf-objdump --source --all-headers --demangle -M xw --line-numbers --wide "EC-reset.elf" > "EC-reset.lst"
EC-reset.siz: EC-reset.elf
	riscv-wch-elf-size --format=berkeley "EC-reset.elf"

# Other Targets
clean:
	-$(RM) $(DIR_OBJS) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE) EC-reset.elf
	-$(RM) $(DIR_EXPANDS) $(CALLGRAPH_DOT)
	-$(RM) $(DIR_DEPS)
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets