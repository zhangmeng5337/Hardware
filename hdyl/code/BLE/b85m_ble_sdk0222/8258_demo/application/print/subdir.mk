################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../application/print/putchar.c \
../application/print/u_printf.c 

OBJS += \
./application/print/putchar.o \
./application/print/u_printf.o 


# Each subdirectory must supply rules for building sources it contributes
application/print/%.o: ../application/print/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: TC32 Compiler'
	tc32-elf-gcc -ffunction-sections -fdata-sections -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222" -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\vendor\common" -I"F:\projects\git\Hardware\hdyl\code\BLE\b85m_ble_sdk0222\drivers\8258" -D__PROJECT_8258_DEMO__=1 -DCHIP_TYPE=CHIP_TYPE_825x -Wall -O2 -fpack-struct -fshort-enums -finline-small-functions -std=gnu99 -fshort-wchar -fms-extensions -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


