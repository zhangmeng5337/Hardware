################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../vendor/b85m_demo/app.c \
../vendor/b85m_demo/app_att.c \
../vendor/b85m_demo/app_buffer.c \
../vendor/b85m_demo/app_ui.c \
../vendor/b85m_demo/main.c 

OBJS += \
./vendor/b85m_demo/app.o \
./vendor/b85m_demo/app_att.o \
./vendor/b85m_demo/app_buffer.o \
./vendor/b85m_demo/app_ui.o \
./vendor/b85m_demo/main.o 


# Each subdirectory must supply rules for building sources it contributes
vendor/b85m_demo/%.o: ../vendor/b85m_demo/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222" -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\vendor\common" -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\drivers\8258" -D__PROJECT_8258_DEMO__=1 -DCHIP_TYPE=CHIP_TYPE_825x -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


