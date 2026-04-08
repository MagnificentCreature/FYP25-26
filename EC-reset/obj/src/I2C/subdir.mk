################################################################################
# MRS Version: 2.4.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/I2C/app_i2c.c \
../src/I2C/mcp9808.c 

C_DEPS += \
./src/I2C/app_i2c.d \
./src/I2C/mcp9808.d 

OBJS += \
./src/I2C/app_i2c.o \
./src/I2C/mcp9808.o 

DIR_OBJS += \
./src/I2C/*.o \

DIR_DEPS += \
./src/I2C/*.d \

DIR_EXPANDS += \
./src/I2C/*.253r.expand \


# Each subdirectory must supply rules for building sources it contributes
src/I2C/%.o: ../src/I2C/%.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/mounriver-studio-projects/EC-reset/StdPeriphDriver/inc" -I"c:/Users/seanw/mounriver-studio-projects/EC-reset/src" -I"c:/Users/seanw/mounriver-studio-projects/EC-reset/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

