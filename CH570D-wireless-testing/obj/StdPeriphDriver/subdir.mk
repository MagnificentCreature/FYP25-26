################################################################################
# MRS Version: 2.2.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_clk.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_cmp.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_flash.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_gpio.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_i2c.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_keyscan.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_pwm.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_pwr.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_spi.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_sys.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_timer.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_uart.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_usbdev.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_usbhostBase.c \
c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_usbhostClass.c 

C_DEPS += \
./StdPeriphDriver/CH57x_clk.d \
./StdPeriphDriver/CH57x_cmp.d \
./StdPeriphDriver/CH57x_flash.d \
./StdPeriphDriver/CH57x_gpio.d \
./StdPeriphDriver/CH57x_i2c.d \
./StdPeriphDriver/CH57x_keyscan.d \
./StdPeriphDriver/CH57x_pwm.d \
./StdPeriphDriver/CH57x_pwr.d \
./StdPeriphDriver/CH57x_spi.d \
./StdPeriphDriver/CH57x_sys.d \
./StdPeriphDriver/CH57x_timer.d \
./StdPeriphDriver/CH57x_uart.d \
./StdPeriphDriver/CH57x_usbdev.d \
./StdPeriphDriver/CH57x_usbhostBase.d \
./StdPeriphDriver/CH57x_usbhostClass.d 

OBJS += \
./StdPeriphDriver/CH57x_clk.o \
./StdPeriphDriver/CH57x_cmp.o \
./StdPeriphDriver/CH57x_flash.o \
./StdPeriphDriver/CH57x_gpio.o \
./StdPeriphDriver/CH57x_i2c.o \
./StdPeriphDriver/CH57x_keyscan.o \
./StdPeriphDriver/CH57x_pwm.o \
./StdPeriphDriver/CH57x_pwr.o \
./StdPeriphDriver/CH57x_spi.o \
./StdPeriphDriver/CH57x_sys.o \
./StdPeriphDriver/CH57x_timer.o \
./StdPeriphDriver/CH57x_uart.o \
./StdPeriphDriver/CH57x_usbdev.o \
./StdPeriphDriver/CH57x_usbhostBase.o \
./StdPeriphDriver/CH57x_usbhostClass.o 


EXPANDS += \
./StdPeriphDriver/CH57x_clk.c.253r.expand \
./StdPeriphDriver/CH57x_cmp.c.253r.expand \
./StdPeriphDriver/CH57x_flash.c.253r.expand \
./StdPeriphDriver/CH57x_gpio.c.253r.expand \
./StdPeriphDriver/CH57x_i2c.c.253r.expand \
./StdPeriphDriver/CH57x_keyscan.c.253r.expand \
./StdPeriphDriver/CH57x_pwm.c.253r.expand \
./StdPeriphDriver/CH57x_pwr.c.253r.expand \
./StdPeriphDriver/CH57x_spi.c.253r.expand \
./StdPeriphDriver/CH57x_sys.c.253r.expand \
./StdPeriphDriver/CH57x_timer.c.253r.expand \
./StdPeriphDriver/CH57x_uart.c.253r.expand \
./StdPeriphDriver/CH57x_usbdev.c.253r.expand \
./StdPeriphDriver/CH57x_usbhostBase.c.253r.expand \
./StdPeriphDriver/CH57x_usbhostClass.c.253r.expand 



# Each subdirectory must supply rules for building sources it contributes
StdPeriphDriver/CH57x_clk.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_clk.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_cmp.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_cmp.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_flash.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_flash.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_gpio.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_gpio.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_i2c.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_i2c.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_keyscan.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_keyscan.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_pwm.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_pwm.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_pwr.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_pwr.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_spi.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_spi.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_sys.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_sys.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_timer.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_timer.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_uart.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_uart.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_usbdev.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_usbdev.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_usbhostBase.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_usbhostBase.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH57x_usbhostClass.o: c:/Users/seanw/Documents/Data\ D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/CH57x_usbhostClass.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=0 -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Startup" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/APP/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/Peripheral/Profile/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/HAL/include" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/Ld" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/BLE/LIB" -I"c:/Users/seanw/Documents/Data D/CP4101/CH572EVT/EVT/EXAM/SRC/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

