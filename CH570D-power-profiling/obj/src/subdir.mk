################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Main.c 

C_DEPS += \
./src/Main.d 

OBJS += \
./src/Main.o 


EXPANDS += \
./src/Main.c.253r.expand 



# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

