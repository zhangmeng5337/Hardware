################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../drivers/8258/flash/flash_mid011460c8.c \
../drivers/8258/flash/flash_mid1060c8.c \
../drivers/8258/flash/flash_mid13325e.c \
../drivers/8258/flash/flash_mid134051.c \
../drivers/8258/flash/flash_mid136085.c \
../drivers/8258/flash/flash_mid1360c8.c \
../drivers/8258/flash/flash_mid1360eb.c \
../drivers/8258/flash/flash_mid14325e.c \
../drivers/8258/flash/flash_mid1460c8.c 

OBJS += \
./drivers/8258/flash/flash_mid011460c8.o \
./drivers/8258/flash/flash_mid1060c8.o \
./drivers/8258/flash/flash_mid13325e.o \
./drivers/8258/flash/flash_mid134051.o \
./drivers/8258/flash/flash_mid136085.o \
./drivers/8258/flash/flash_mid1360c8.o \
./drivers/8258/flash/flash_mid1360eb.o \
./drivers/8258/flash/flash_mid14325e.o \
./drivers/8258/flash/flash_mid1460c8.o 


# Each subdirectory must supply rules for building sources it contributes
drivers/8258/flash/%.o: ../drivers/8258/flash/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222" -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\vendor\common" -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\drivers\8258" -D__PROJECT_8258_DEMO__=1 -DCHIP_TYPE=CHIP_TYPE_825x -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


