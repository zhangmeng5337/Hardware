################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/common/blt_common.c \
../vendor/common/blt_fw_sign.c \
../vendor/common/blt_led.c \
../vendor/common/blt_soft_timer.c \
../vendor/common/common_dbg.c \
../vendor/common/custom_pair.c \
../vendor/common/device_manage.c \
../vendor/common/flash_fw_check.c \
../vendor/common/simple_sdp.c \
../vendor/common/tl_audio.c 

OBJS += \
./vendor/common/blt_common.o \
./vendor/common/blt_fw_sign.o \
./vendor/common/blt_led.o \
./vendor/common/blt_soft_timer.o \
./vendor/common/common_dbg.o \
./vendor/common/custom_pair.o \
./vendor/common/device_manage.o \
./vendor/common/flash_fw_check.o \
./vendor/common/simple_sdp.o \
./vendor/common/tl_audio.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/common/%.o: ../vendor/common/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222" -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\vendor\common" -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\drivers\8258" -D__PROJECT_8258_DEMO__=1 -DCHIP_TYPE=CHIP_TYPE_825x -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


