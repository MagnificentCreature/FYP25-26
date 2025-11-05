################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Main\ copy.c \
../src/Main.c \
../src/Main2.c 

C_DEPS += \
./src/Main\ copy.d \
./src/Main.d \
./src/Main2.d 

OBJS += \
./src/Main\ copy.o \
./src/Main.o \
./src/Main2.o 


EXPANDS += \
./src/Main\ copy.c.253r.expand \
./src/Main.c.253r.expand \
./src/Main2.c.253r.expand 



# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/mounriver-studio-projects/CH570D-power_profiling_dummy_operations/StdPeriphDriver/inc" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-power_profiling_dummy_operations/src" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-power_profiling_dummy_operations/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
src/Main\ copy.o: ../src/Main\ copy.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/mounriver-studio-projects/CH570D-power_profiling_dummy_operations/StdPeriphDriver/inc" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-power_profiling_dummy_operations/src" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-power_profiling_dummy_operations/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

