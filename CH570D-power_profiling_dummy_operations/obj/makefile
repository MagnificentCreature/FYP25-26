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
CH570D-power_profiling_dummy_operations.hex \

SECONDARY_LIST += \
CH570D-power_profiling_dummy_operations.lst \

SECONDARY_SIZE += \
CH570D-power_profiling_dummy_operations.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: CH570D-power_profiling_dummy_operations.elf secondary-outputs

# Tool invocations
CH570D-power_profiling_dummy_operations.elf: $(OBJS) $(USER_OBJS)
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -T "c:/Users/seanw/mounriver-studio-projects/CH570D-power_profiling_dummy_operations/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -L"c:\Users\seanw\mounriver-studio-projects\CH570D-power_profiling_dummy_operations" -L"c:/Users/seanw/mounriver-studio-projects/CH570D-power_profiling_dummy_operations/StdPeriphDriver" -Xlinker --print-memory-usage -Xlinker --print-memory-usage -Wl,-Map,"CH570D-power_profiling_dummy_operations.map" --specs=nano.specs --specs=nosys.specs -o "CH570D-power_profiling_dummy_operations.elf" $(OBJS)  $(LIBS)
CH570D-power_profiling_dummy_operations.hex: CH570D-power_profiling_dummy_operations.elf
	@	riscv-wch-elf-objcopy -O ihex "CH570D-power_profiling_dummy_operations.elf" "CH570D-power_profiling_dummy_operations.hex"
CH570D-power_profiling_dummy_operations.lst: CH570D-power_profiling_dummy_operations.elf
	@	riscv-wch-elf-objdump --source --all-headers --demangle -M xw --line-numbers --wide "CH570D-power_profiling_dummy_operations.elf" > "CH570D-power_profiling_dummy_operations.lst"
CH570D-power_profiling_dummy_operations.siz: CH570D-power_profiling_dummy_operations.elf
	riscv-wch-elf-size --format=berkeley "CH570D-power_profiling_dummy_operations.elf"

# Other Targets
clean:
	-$(RM) $(OBJS) $(EXPANDS) $(CALLGRAPH_DOT) $(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(S_DEPS)$(S_UPPER_DEPS)$(ASM_DEPS)$(ASM_UPPER_DEPS)$(C_DEPS) CH570D-power_profiling_dummy_operations.elf

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets