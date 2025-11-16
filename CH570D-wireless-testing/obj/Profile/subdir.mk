################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Profile/gattprofile.c 

C_DEPS += \
./Profile/gattprofile.d 

OBJS += \
./Profile/gattprofile.o 


EXPANDS += \
./Profile/gattprofile.c.253r.expand 



# Each subdirectory must supply rules for building sources it contributes
Profile/%.o: ../Profile/%.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

