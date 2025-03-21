/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2021
 *
 */
/*******************************************************************************
*
* 代码许可和免责信??* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源??* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或??* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件??* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失??* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些??* 全部上述排除或限制可能并不适用于您??*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "cw32l010_flash.h"
#include "flash.h"

void floatTouint32(float dat,unsigned char *pb)
{
	uint32_t b;
	b = *(uint32_t*)&dat;
	pb[0] = b>>0;
	pb[1] = b>>8;
	pb[2] = b>>16;
	pb[3] = b>>24;
}
void floatTouint32_m(float dat,unsigned char *pb)
{
	uint32_t b;
	b = *((volatile  uint32_t*)&dat);
	pb[3] = b>>0;
	pb[2] = b>>8;
	pb[1] = b>>16;
	pb[0] = b>>24;
}

void floatTouint32_pos(float dat,unsigned char *pb,unsigned char pos)
{
	uint32_t b;
	b = *(uint32_t*)&dat;
	if(pos == 1) //0x3f8ccd
	{
		
	pb[0] = b>>8;
	pb[1] = b>>0;	
	}
	if(pos == 0)
	{
		
	pb[0] = b>>24;
	pb[1] = b>>16;	
	}
}

//float uint32Tofloat(unsigned char *buf)
//{
//	uint32_t tmp;
//	tmp = (buf[3]<<0)|(buf[2]<<8)|(buf[1]<<16)|(buf[0]<<24);
//	val_u32ToFloat
//	return *(float *)&tmp;
//}
uint32_t uint32Tofloat(unsigned char *buf)
{
	uint32_t tmp;
	tmp = (buf[3]<<0)|(buf[2]<<8)|(buf[1]<<16)|(buf[0]<<24);
	
	return tmp;
}
float uint32TofloatR(unsigned char *buf)
{
	uint32_t tmp;
	tmp = (buf[3]<<0)|(buf[2]<<8)|(buf[1]<<16)|(buf[0]<<24);
	
	return *((float *)&tmp);
}

uint32_t  uint32Tofloat_pos(unsigned char *pb,unsigned char pos)
{
	uint32_t tmp;

	if(pos == 1)
	{
		
	tmp = (pb[1]<<0)|(pb[0]<<8);
	}
	if(pos == 0)
	{	
	tmp = (pb[1]<<16)|(pb[0]<<24);	
	}
	return tmp;
}


void flash_write_byte(uint32_t WriteAddr, uint8_t *pWrBuf, uint16_t count)
{
    uint8_t buf[256];
    uint32_t i;
    uint16_t  j;
	uint8_t  Flag;
    //read
    j = 0;
    for (i = START_ADDR; i <= (READ_SIZE + START_ADDR); i++)
    {
        buf[j] =  *((volatile uint8_t *)(i));
        j++;
    }
	//update data
    j = 0;

	erase:FLASH_UnlockPages(START_ADDR, END_ADDR);  //解锁127页面
    Flag = FLASH_ErasePages(START_ADDR, END_ADDR); //擦除

	    if (Flag)
    {
        goto erase; //擦除失败
    }
		j = 0;
    for (i = START_ADDR; i < (READ_SIZE + START_ADDR); i++)
    {
           		 buf[j] =  pWrBuf[j];
				 j++;
				 

    }
	 FLASH_UnlockPages(START_ADDR, END_ADDR);  //
     FLASH_WriteBytes(START_ADDR +WriteAddr, buf,READ_SIZE);

}
void flash_read_byte(uint32_t ReadAddr, uint8_t *pWrBuf, uint16_t count)
{
    uint32_t i;
	uint16_t j;
    j = 0;
    for (i = ReadAddr+START_ADDR; i < (count + START_ADDR); i++)
    {
        pWrBuf[j] =  *((volatile uint8_t *)(i));
        j++;
			if(i == 0xfdff)
				;
    }
}


