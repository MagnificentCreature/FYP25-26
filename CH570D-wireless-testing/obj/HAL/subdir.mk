################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/MCU.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/RTC.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/SLEEP.c 

C_DEPS += \
./HAL/MCU.d \
./HAL/RTC.d \
./HAL/SLEEP.d 

OBJS += \
./HAL/MCU.o \
./HAL/RTC.o \
./HAL/SLEEP.o 


EXPANDS += \
./HAL/MCU.c.253r.expand \
./HAL/RTC.c.253r.expand \
./HAL/SLEEP.c.253r.expand 



# Each subdirectory must supply rules for building sources it contributes
HAL/MCU.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/MCU.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
HAL/RTC.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/RTC.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
HAL/SLEEP.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/SLEEP.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

