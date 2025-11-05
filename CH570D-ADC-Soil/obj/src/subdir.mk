################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Main\ copy\ 1.c \
../src/Main\ copy\ 2.c \
../src/Main\ copy.c \
../src/Main.c 

C_DEPS += \
./src/Main\ copy\ 1.d \
./src/Main\ copy\ 2.d \
./src/Main\ copy.d \
./src/Main.d 

OBJS += \
./src/Main\ copy\ 1.o \
./src/Main\ copy\ 2.o \
./src/Main\ copy.o \
./src/Main.o 


EXPANDS += \
./src/Main\ copy\ 1.c.253r.expand \
./src/Main\ copy\ 2.c.253r.expand \
./src/Main\ copy.c.253r.expand \
./src/Main.c.253r.expand 



# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/StdPeriphDriver/inc" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/src" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
src/Main\ copy\ 1.o: ../src/Main\ copy\ 1.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/StdPeriphDriver/inc" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/src" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
src/Main\ copy\ 2.o: ../src/Main\ copy\ 2.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/StdPeriphDriver/inc" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/src" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
src/Main\ copy.o: ../src/Main\ copy.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/StdPeriphDriver/inc" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/src" -I"c:/Users/seanw/mounriver-studio-projects/CH570D-ADC-Soil/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

