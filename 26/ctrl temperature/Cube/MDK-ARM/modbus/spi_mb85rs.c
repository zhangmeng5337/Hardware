
#include "spi.h"
#include "spi_mb85rs.h"
//#include "sys.h"

extern SPI_HandleTypeDef hspi2;

static uint8_t SPI_ReadWriteByte(uint8_t data)
{
	uint8_t Rx_data = 0;
	HAL_SPI_TransmitReceive(&hspi2, &data, &Rx_data, 1, 0xff);
	return Rx_data;
}

void MB85Ctrl_Write_one(uint8_t data,uint16_t addr)//写入一个字节到特定地址空间
{
    uint8_t addr_H, addr_L;
    addr_H = (uint8_t)((addr & 0xff00) >> 8);
		addr_L = (uint8_t)(addr & 0x00ff);

    MB85_CS_CLR;
    SPI_ReadWriteByte(MB85RS_CMD_WREN);
		MB85_CS_SET;
	
    HAL_Delay(1);
		MB85_CS_CLR;
	
    SPI_ReadWriteByte(MB85RS_CMD_WRITE);
		SPI_ReadWriteByte(addr_H);
		SPI_ReadWriteByte(addr_L);
	  SPI_ReadWriteByte(data);
	   
    MB85_CS_SET;

}
uint8_t MB85Ctrl_Read_one(uint16_t addr)//
{
    uint8_t dat,addr_H, addr_L;
    addr_H = (uint8_t)((addr & 0xff00) >> 8);
		addr_L = (uint8_t)(addr & 0x00ff);

    MB85_CS_CLR;
		SPI_ReadWriteByte(MB85RS_CMD_READ);
		SPI_ReadWriteByte(addr_H);
		SPI_ReadWriteByte(addr_L);
    dat = SPI_ReadWriteByte(0x00);
    MB85_CS_SET;
    return(dat);
    

}

void MB85Ctrl_Write(uint16_t addr, uint8_t *buf, uint16_t len)
{
	uint8_t addr_H, addr_L;
	addr_H = (uint8_t)((addr & 0xff00) >> 8);
	addr_L = (uint8_t)(addr & 0x00ff);
	
	MB85_CS_CLR;
  SPI_ReadWriteByte(MB85RS_CMD_WREN);
	MB85_CS_SET;
	
	HAL_Delay(1);
	MB85_CS_CLR;
  SPI_ReadWriteByte(MB85RS_CMD_WRITE);
	SPI_ReadWriteByte(addr_H);
	SPI_ReadWriteByte(addr_L);

	for(; len > 0; len--)
	{
		 SPI_ReadWriteByte(*buf++);
		
	}
	MB85_CS_SET;
	
}

void MB85Ctrl_Read(uint16_t addr, uint8_t *buf, uint16_t len)
{
	uint8_t addr_H, addr_L;
	addr_H = (uint8_t)((addr & 0xff00) >> 8);
	addr_L = (uint8_t)(addr & 0x00ff);

	MB85_CS_CLR;
	SPI_ReadWriteByte(MB85RS_CMD_READ);
	SPI_ReadWriteByte(addr_H);
	SPI_ReadWriteByte(addr_L);
  
	for(; len > 0; len--)
	{
		*buf++ = SPI_ReadWriteByte(0x00);
	}
	MB85_CS_SET;
}



void MB85Ctrl_Init()
{



}



