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
* ������ɺ�������??* �人оԴ�뵼�����޹�˾������ʹ�����б�̴���ʾ���ķ�ר���İ�Ȩ��ɣ��������ɴ�
* ���ɸ��������ض���Ҫ�����Ƶ����ƹ��ܡ����ݲ��ܱ��ų����κη�����֤���人оԴ??* �������޹�˾������򿪷��̺͹�Ӧ�̶Գ������֧�֣�����У����ṩ�κ���ʾ��??* ���ı�֤�������������������ڰ������й������ԡ�������ĳ���ض���;�ͷ���Ȩ�ı�֤
* ������??* ���ۺ������Σ��人оԴ�뵼�����޹�˾������򿪷��̻�Ӧ�̾��������и����
* ��ʹ����֪�䷢���Ŀ�����ʱ��Ҳ����ˣ����ݵĶ�ʧ���𻵣�ֱ�ӵġ��ر�ġ�������
* ���ӵ��𺦣����κκ���Ծ����𺦣�������ҵ�����롢������Ԥ�ڿɽ�ʡ����
* ��ʧ??* ĳЩ˾��Ͻ���������ֱ�ӵġ������Ļ����Ե������κε��ų������ƣ����ĳЩ??* ȫ�������ų������ƿ��ܲ�����������??*
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

	erase:FLASH_UnlockPages(START_ADDR, END_ADDR);  //����127ҳ��
    Flag = FLASH_ErasePages(START_ADDR, END_ADDR); //����

	    if (Flag)
    {
        goto erase; //����ʧ��
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


