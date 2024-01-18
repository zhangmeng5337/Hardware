/*
 * ICM20948.c
 *
 *  Created on: Oct 26, 2018
 *      Author: cory
 */

#include "main.h"
//#include "gpio.h"
//#include "spi.h"
//#include "usart.h"
//#include "dma.h"
#include "ICM20948.h"
#include <string.h>
extern I2C_HandleTypeDef hi2c2;
mag_acc_gyro_stru mag_acc_gyro;
char uart_buffer[200];
/*
 *
 * SPI abstraction
 * Creates a layer between STM32 HAL and the ICM library that will allow for easy platform swap
 *
 */
void ICM_ReadBytes(uint8_t reg, uint8_t *pData, uint16_t Size) // ***
{
	reg = reg | 0x80;
	HAL_I2C_Mem_Read(I2C_BUS, DEV_ADDR, reg, 1, pData,Size, 10);
	/*HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(SPI_BUS, &reg, 1);
	HAL_SPI_Receive_DMA(SPI_BUS, pData, Size);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);*/
}

void ICM_WriteBytes(uint8_t reg, uint8_t *pData, uint16_t Size) // ***
{
	reg = reg & 0x7F;
	HAL_I2C_Mem_Write(I2C_BUS, DEV_ADDR, reg, 1, pData, Size, 5);
	/*HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(SPI_BUS, &reg, 1);
	HAL_SPI_Transmit_DMA(SPI_BUS, pData, Size);
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);*/

}

void ICM_ReadOneByte(uint8_t reg, uint8_t* pData) // ***
{
	reg = reg | 0x80;
	HAL_I2C_Mem_Read(I2C_BUS, DEV_ADDR, reg, 1, pData,1, 10);
	/*HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	HAL_SPI_Transmit_DMA(SPI_BUS, &reg, 1);
	while (HAL_SPI_GetState(SPI_BUS) != HAL_SPI_STATE_READY)
		;
	HAL_SPI_Receive_DMA(SPI_BUS, pData, 1);
	while (HAL_SPI_GetState(SPI_BUS) != HAL_SPI_STATE_READY)
		;
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);*/
}

void ICM_WriteOneByte(uint8_t reg, uint8_t Data) // ***
{
	reg = reg & 0x7F;
	HAL_I2C_Mem_Write(I2C_BUS, DEV_ADDR, reg, 1, &Data, 1, 5);
	
	//HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_RESET);
	//HAL_SPI_Transmit_DMA(SPI_BUS, &reg, 1);
	//HAL_SPI_Transmit_DMA(SPI_BUS, &Data, 1);
	//HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, GPIO_PIN_SET);
}

/*
 *
 * AUX I2C abstraction for magnetometer
 * Creates a layer between STM32 HAL and the ICM library that will allow for easy platform swap
 *
 */
void ICM_MagWrite(uint8_t reg, uint8_t value) {
	ICM_WriteOneByte(0x7F, 0x30);
	HAL_Delay(1);
	ICM_WriteOneByte(0x03, 0x0C);
	HAL_Delay(1);
	ICM_WriteOneByte(0x04, reg);
	HAL_Delay(1);
	ICM_WriteOneByte(0x06, value);
	HAL_Delay(1);
}

static uint8_t ICM_MagRead(uint8_t reg) {
	uint8_t Data;
	ICM_WriteOneByte(0x7F, 0x30);
	HAL_Delay(1);
	ICM_WriteOneByte(0x03, 0x0C | 0x80);
	HAL_Delay(1);
	ICM_WriteOneByte(0x04, reg);
	HAL_Delay(1);
	ICM_WriteOneByte(0x06, 0xff);
	HAL_Delay(1);
	ICM_WriteOneByte(0x7F, 0x00);
	ICM_ReadOneByte(0x3B, &Data);
	HAL_Delay(1);
	return Data;
}
signed char ICM_TmpRead(float *tmp)
{
	unsigned char buffer[2];
	unsigned short int tmper;
	ICM_ReadBytes(0x3B, buffer, 2);
	tmper = (signed short int)(buffer[1] << 8) | buffer[0];
	*tmp = (((float)tmper - _tempOffset)/_tempScale) + _tempOffset;
	return 1;
}

void ICM_ReadMagData(int16_t heading[3]) {
	uint8_t mag_buffer[10];
	mag_buffer[0] = ICM_MagRead(0x01);
	mag_buffer[1] = ICM_MagRead(0x11);
	mag_buffer[2] = ICM_MagRead(0x12);
	heading[0] = mag_buffer[1] | mag_buffer[2] << 8;
	mag_buffer[3] = ICM_MagRead(0x13);
	mag_buffer[4] = ICM_MagRead(0x14);
	heading[1] = mag_buffer[3] | mag_buffer[4] << 8;
	mag_buffer[5] = ICM_MagRead(0x15);
	mag_buffer[6] = ICM_MagRead(0x16);
	heading[2] = mag_buffer[5] | mag_buffer[6] << 8;
	ICM_MagWrite(0x31, 0x01);
}

void ICM_SelectBank(uint8_t bank) {
	ICM_WriteOneByte(USER_BANK_SEL, bank);
}
void ICM_Disable_I2C(void) {
	ICM_WriteOneByte(0x03, 0x78);
}
void ICM_Enable_I2C(void) {
	ICM_WriteOneByte(0x03, 0x78);
}
/*
 *
 * Sequence to setup ICM290948 as early as possible after power on
 *
 */



void ICM_ReadAccelGyroData(void) {
	uint8_t raw_data[12];
	ICM_ReadBytes(0x2D, raw_data, 12);

	mag_acc_gyro.accel_data[0] = (raw_data[0] << 8) | raw_data[1];
	mag_acc_gyro.accel_data[1] = (raw_data[2] << 8) | raw_data[3];
	mag_acc_gyro.accel_data[2] = (raw_data[4] << 8) | raw_data[5];

	mag_acc_gyro.gyro_data[0] = (raw_data[6] << 8) | raw_data[7];
	mag_acc_gyro.gyro_data[1] = (raw_data[8] << 8) | raw_data[9];
	mag_acc_gyro.gyro_data[2] = (raw_data[10] << 8) | raw_data[11];

	mag_acc_gyro.accel_data[0] = mag_acc_gyro.accel_data[0] / 8;
	mag_acc_gyro.accel_data[1] = mag_acc_gyro.accel_data[1] / 8;
	mag_acc_gyro.accel_data[2] = mag_acc_gyro.accel_data[2] / 8;

	mag_acc_gyro.gyro_data[0] = mag_acc_gyro.gyro_data[0] / 250;
	mag_acc_gyro.gyro_data[1] = mag_acc_gyro.gyro_data[1] / 250;
	mag_acc_gyro.gyro_data[2] = mag_acc_gyro.gyro_data[2] / 250;
}


/*void ICM_CSHigh(void) {
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, SET);
}
void ICM_CSLow(void) {
	HAL_GPIO_WritePin(ICM_CS_GPIO_Port, ICM_CS_Pin, RESET);
}*/
void ICM_SetClock(uint8_t clk) {
	ICM_WriteOneByte(PWR_MGMT_1, clk);
}
void ICM_AccelGyroOff(void) {
	ICM_WriteOneByte(PWR_MGMT_2, (0x38 | 0x07));
}
void ICM_AccelGyroOn(void) {
	ICM_WriteOneByte(0x07, (0x00 | 0x00));
}
uint8_t ICM_WHOAMI(void) {
	uint8_t spiData = 0x01;
	ICM_ReadOneByte(0x00, &spiData);
	return spiData;
}
void ICM_SetGyroRateLPF(uint8_t rate, uint8_t lpf) {
	ICM_WriteOneByte(GYRO_CONFIG_1, (rate | lpf));
}

uint16_t ICM_Initialize(void) {
	ICM_SelectBank(USER_BANK_2);
	HAL_Delay(20);
	ICM_SetGyroRateLPF(GYRO_RATE_250, GYRO_LPF_17HZ);
	HAL_Delay(10);

	// Set gyroscope sample rate to 100hz (0x0A) in GYRO_SMPLRT_DIV register (0x00)
	ICM_WriteOneByte(0x00, 0x0A);
	HAL_Delay(10);

	// Set accelerometer low pass filter to 136hz (0x11) and the rate to 8G (0x04) in register ACCEL_CONFIG (0x14)
	ICM_WriteOneByte(0x14, (0x04 | 0x11));

	// Set accelerometer sample rate to 225hz (0x00) in ACCEL_SMPLRT_DIV_1 register (0x10)
	ICM_WriteOneByte(0x10, 0x00);
	HAL_Delay(10);

	// Set accelerometer sample rate to 100 hz (0x0A) in ACCEL_SMPLRT_DIV_2 register (0x11)
	ICM_WriteOneByte(0x11, 0x0A);
	HAL_Delay(10);

	ICM_SelectBank(USER_BANK_2);
	HAL_Delay(20);

	// Configure AUX_I2C Magnetometer (onboard ICM-20948)
	ICM_WriteOneByte(0x7F, 0x00);
	ICM_WriteOneByte(0x0F, 0x30);
	ICM_WriteOneByte(0x03, 0x20);
	ICM_WriteOneByte(0x7F, 0x30);
	ICM_WriteOneByte(0x01, 0x4D);
	ICM_WriteOneByte(0x02, 0x01);
	ICM_WriteOneByte(0x05, 0x81);
	ICM_MagWrite(0x32, 0x01);
	HAL_Delay(1000);
	ICM_MagWrite(0x31, 0x02);
	return 1337;
}
void ICM_PowerOn(void) {
//	char uart_buffer[200];
	uint8_t whoami = 0xEA;
	while(whoami != ICM_WHOAMI())
		
//	ICM_CSHigh();
	HAL_Delay(10);
	ICM_SelectBank(USER_BANK_0);
	HAL_Delay(10);
	ICM_Enable_I2C();
	HAL_Delay(10);
	ICM_SetClock((uint8_t) CLK_BEST_AVAIL);
	HAL_Delay(10);
	ICM_AccelGyroOff();
	HAL_Delay(20);
	ICM_AccelGyroOn();
	HAL_Delay(10);
	ICM_Initialize();
}
extern UART_HandleTypeDef huart1;
void Chip_Init(void)
{
 
	ICM_SelectBank(USER_BANK_0);
	HAL_Delay(10);
	ICM_PowerOn();
	HAL_Delay(10);
  /* USER CODE END 2 */
}
void ICM_Process()
{
		// Select User Bank 0
		ICM_SelectBank(USER_BANK_0);
		HAL_Delay(10);

		// Obtain accelerometer and gyro data
		ICM_ReadAccelGyroData();

		// Obtain magnetometer data
	//	int16_t mag_data[3];
		ICM_ReadMagData(mag_acc_gyro.mag_data);

		ICM_TmpRead(mag_acc_gyro.temp);

		// Print raw, but joined, axis data values to screen
		sprintf(uart_buffer,
				"(Ax: %u | Ay: %u | Az: %u)   "
				"(Gx: %u | Gy: %u | Gz: %u)   "
				"(Mx: %i | My: %i | Mz: %i)"
				" \r\n",
				mag_acc_gyro.accel_data[0], mag_acc_gyro.accel_data[1], mag_acc_gyro.accel_data[2],
				mag_acc_gyro.gyro_data[0], mag_acc_gyro.gyro_data[1], mag_acc_gyro.gyro_data[2],
				mag_acc_gyro.mag_data[0], mag_acc_gyro.mag_data[1], mag_acc_gyro.mag_data[2]);
		HAL_UART_Transmit(&huart1, (uint8_t*) uart_buffer, strlen(uart_buffer), 1000);
		HAL_Delay(5);
}


