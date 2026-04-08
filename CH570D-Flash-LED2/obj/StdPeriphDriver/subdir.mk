################################################################################
# MRS Version: 2.3.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StdPeriphDriver/CH57x_clk.c \
../StdPeriphDriver/CH57x_cmp.c \
../StdPeriphDriver/CH57x_flash.c \
../StdPeriphDriver/CH57x_gpio.c \
../StdPeriphDriver/CH57x_i2c.c \
../StdPeriphDriver/CH57x_keyscan.c \
../StdPeriphDriver/CH57x_pwm.c \
../StdPeriphDriver/CH57x_pwr.c \
../StdPeriphDriver/CH57x_spi.c \
../StdPeriphDriver/CH57x_sys.c \
../StdPeriphDriver/CH57x_timer.c \
../StdPeriphDriver/CH57x_uart.c \
../StdPeriphDriver/CH57x_usbdev.c \
../StdPeriphDriver/CH57x_usbhostBase.c \
../StdPeriphDriver/CH57x_usbhostClass.c 

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

DIR_OBJS += \
./StdPeriphDriver/*.o \

DIR_DEPS += \
./StdPeriphDriver/*.d \

DIR_EXPANDS += \
./StdPeriphDriver/*.253r.expand \


# Each subdirectory must supply rules for building sources it contributes
StdPeriphDriver/%.o: ../StdPeriphDriver/%.c
	@	riscv-wch-elf-gcc -march=rv32imc_zba_zbb_zbc_zbs_xw -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common --param=highcode-gen-section-name=1 -g -DDEBUG=1 -I"c:/Users/seanw/Documents/Data D/CP4101/Git Repo/CH570D-Flash-LED2/StdPeriphDriver/inc" -I"c:/Users/seanw/Documents/Data D/CP4101/Git Repo/CH570D-Flash-LED2/src" -I"c:/Users/seanw/Documents/Data D/CP4101/Git Repo/CH570D-Flash-LED2/RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

