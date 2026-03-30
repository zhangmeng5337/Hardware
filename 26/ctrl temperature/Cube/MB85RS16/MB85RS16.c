//Written by Pegasus Yu in 2023

#include "MB85RS16.h"
#include <string.h>

#define SPI2_CS_L HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET)
#define SPI2_CS_H HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET)
extern SPI_HandleTypeDef hspi2;
//extern void PY_Delay_us_t(uint32_t Delay);


uint32_t MB85RS16_ReadID(void)
{
	uint8_t ftd[5];
	uint8_t frd[5];
	uint8_t Manufacturer_ID;
	uint8_t Continuation_Code;
	uint8_t Product_ID_L;
	uint8_t Product_ID_H;

	ftd[0]=RDID;


	SPI2_CS_L;

	HAL_SPI_TransmitReceive(&hspi2, ftd, frd, 5, 0xFFFFFFFF);

	SPI2_CS_H;

	Manufacturer_ID = frd[1];
	Continuation_Code = frd[2];
	Product_ID_L = frd[3];
	Product_ID_H = frd[4];

	return ((Product_ID_H<<24)|(Product_ID_L<<16)|(Continuation_Code<<8)|(Manufacturer_ID));
}

uint8_t MB85RS16_Init(void)
{
	uint8_t st = 0;

	for(uint8_t i=0; i<4; i++)
	{
		if(MB85RS16_ReadID()==MB85RS16_ID)
		{
			st = 1;
			break;
		}

	}

	return st;

}


/*
 * WEL is reset after the following operations which means every write operation must follow once WREN operation MB85RS16_Set_Write_Enable_Latch().
 * After power ON.
 * After WRDI command recognition.
 * At the rising edge of CS after WRSR command recognition.
 * At the rising edge of CS after WRITE command recognition.
 */
void MB85RS16_Set_Write_Enable_Latch(void)
{
    uint8_t cmd = WREN;
	SPI2_CS_L;

	HAL_SPI_Transmit(&hspi2, &cmd, 1, 0xFFFFFFFF);

	SPI2_CS_H;
}


void MB85RS16_Reset_Write_Enable_Latch(void)
{
    uint8_t cmd = WRDI;
	SPI2_CS_L;

	HAL_SPI_Transmit(&hspi2, &cmd, 1, 0xFFFFFFFF);

	SPI2_CS_H;
}


void MB85RS16_Write_Status_Register(uint8_t SRV)
{
    uint8_t data[2];
    data[0] = WRSR;
    data[1] = SRV;

    MB85RS16_Set_Write_Enable_Latch();

   // PY_Delay_us_t(2);

	SPI2_CS_L;

	HAL_SPI_Transmit(&hspi2, data, 2, 0xFFFFFFFF);

	SPI2_CS_H;
}

uint8_t MB85RS16_Read_Status_Register(void)
{
    uint8_t cmd[2];
    uint8_t data[2];
    uint8_t SRV;

    cmd[0] = RDSR;

	SPI2_CS_L;

	HAL_SPI_TransmitReceive(&hspi2, cmd, data, 2, 0xFFFFFFFF);

	SPI2_CS_H;

	SRV = data[1];
	return SRV;

}

/*
 * wd: data buffer pointer
 * addr: address to operate for MB85RS16
 * len: data length to be written
 */

void MB85RS16_Write_Memory(uint8_t * wd, uint16_t addr, uint32_t len)
{
    uint8_t data[len+3];
    data[0] = WRITE;
    data[1] = (uint8_t)(addr>>8);
    data[2] = (uint8_t)addr;

    memcpy(data+3, wd, len);

    MB85RS16_Set_Write_Enable_Latch();

    //PY_Delay_us_t(2);

	SPI2_CS_L;

	HAL_SPI_Transmit(&hspi2, data, len+3, 0xFFFFFFFF);

	SPI2_CS_H;
}


/*
 * rd: data buffer pointer
 * addr: address to operate for MB85RS16
 * len: data length to be written
 */

void MB85RS16_Read_Memory(uint8_t * rd, uint16_t addr, uint32_t len)
{
    uint8_t cmd[len+3];
    uint8_t data[len+3];
    cmd[0] = READ;
    cmd[1] = (uint8_t)(addr>>8);
    cmd[2] = (uint8_t)addr;

	SPI2_CS_L;

	HAL_SPI_TransmitReceive(&hspi2, cmd, data , len+3, 0xFFFFFFFF);

	SPI2_CS_H;

	memcpy(rd, data+3, len);
}





