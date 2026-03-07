################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Main.c \
../src/packet_gen.c \
../src/utility.c 

C_DEPS += \
./src/Main.d \
./src/packet_gen.d \
./src/utility.d 

OBJS += \
./src/Main.o \
./src/packet_gen.o \
./src/utility.o 

DIR_OBJS += \
./src/*.o \

DIR_DEPS += \
./src/*.d \

DIR_EXPANDS += \
./src/*.253r.expand \


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/Downloads/CH570D-Backscatter-V3/CH570D-Backscatter-V3/StdPeriphDriver/inc" -I"c:/Users/seanw/Downloads/CH570D-Backscatter-V3/CH570D-Backscatter-V3/src" -I"c:/Users/seanw/Downloads/CH570D-Backscatter-V3/CH570D-Backscatter-V3/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

