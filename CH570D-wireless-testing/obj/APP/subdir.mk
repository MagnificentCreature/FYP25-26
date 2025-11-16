################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APP/peripheral.c \
../APP/peripheral_main.c 

C_DEPS += \
./APP/peripheral.d \
./APP/peripheral_main.d 

OBJS += \
./APP/peripheral.o \
./APP/peripheral_main.o 


EXPANDS += \
./APP/peripheral.c.253r.expand \
./APP/peripheral_main.c.253r.expand 



# Each subdirectory must supply rules for building sources it contributes
APP/%.o: ../APP/%.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

