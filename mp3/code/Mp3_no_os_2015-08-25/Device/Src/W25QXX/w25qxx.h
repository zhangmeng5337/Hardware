#ifndef __W25QXX_H
#define __W25QXX_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//W25QXX��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2016/1/16
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//W25Xϵ��/Qϵ��оƬ�б�	   
//W25Q80  ID  0XEF13
//W25Q16  ID  0XEF14
//W25Q32  ID  0XEF15
//W25Q64  ID  0XEF16	
//W25Q128 ID  0XEF17	
//W25Q256 ID  0XEF18
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18
#define W25N01  0XEFAA21
extern u16 W25QXX_TYPE;					//����W25QXXоƬ�ͺ�		   

#define	W25QXX_CS 		PFout(6)  		//W25QXX��Ƭѡ�ź�

////////////////////////////////////////////////////////////////////////////////// 
//ָ���
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadBBMLUT	      0xA5 
#define W25X_SwapBlocks			  0xA1  
#define W25X_LastECCFailPageAddress			0xA9 

#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
//#define W25X_ReadStatusReg1		0x05 
//#define W25X_ReadStatusReg2		0x35 
//#define W25X_ReadStatusReg3		0x15 

#define W25X_WriteStatusReg1    0xa0 
#define W25X_WriteStatusReg2    0xb0 
#define W25X_WriteStatusReg3    0xc0 

#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9
#define W25X_PageDataRead			0x13
#define W25X_ProgramExecute	  0x10 
#define W25X_DieSelect  	  0xc2

#define FLASH_SECTOR_SIZE 	512	
//#define FLASH_SECTOR_COUNT 	512*2*25*2	//W25Q256,ǰ25M�ֽڸ�FATFSռ��	
#define FLASH_BLOCK_SIZE   	64*4     		//ÿ��BLOCK��8������	





//FLASH����ܹ��ﵽ�ĵ�ַ����128M
#define FLASH_MAX_ADDR 0x8000000

//FLASH���С��Ϊ128KB
#define FLASH_BLOCK_SIZE 0x20000

//FLASHҳ��С��Ϊ2KB
#define FLASH_PAGE_SIZE 0x800


//������С
#define FLASH_SECTOR_SIZE 0x200

//�����������Ŀ���
#define FLASH_SWAP_BLOCKS 10

//������������Ӱ��Ŀ���
#define FLASH_BAD_BLOCKS_REMAP 50

//���滵���Ŀ���
#define FLASH_BLOCKS_TABLE 3

//����ܹ��ﵽ��������ַ
#define FLASH_MAX_SECTOR_ADDR (FLASH_MAX_ADDR-(FLASH_BAD_BLOCKS_REMAP+FLASH_SWAP_BLOCKS+FLASH_BLOCKS_TABLE)*FLASH_BLOCK_SIZE)

//����������ʼ��ַ
#define FLASH_SWAP_BLOCK_ADDR (FLASH_MAX_ADDR+1-FLASH_SWAP_BLOCKS*FLASH_BLOCK_SIZE)

//��Ӱ�仵�����ʼ��ַ
#define FLASH_BAD_BLOCK_REMAP_ADDR (FLASH_MAX_ADDR+1-(FLASH_BAD_BLOCKS_REMAP+FLASH_SWAP_BLOCKS)*FLASH_BLOCK_SIZE)

//���屣�滵������ʼ��ַ
#define FLASH_BLOCK_TABLE_ADDR (FLASH_MAX_ADDR+1-(FLASH_BAD_BLOCKS_REMAP+FLASH_SWAP_BLOCKS+FLASH_BLOCKS_TABLE)*FLASH_BLOCK_SIZE)

//Ӱ������״̬��������
#define FLASH_BLOCK_OK    0
#define FLASH_BLOCK_BAD   1
#define FLASH_BLOCK_USED  2

void W25QXX_Init(void);
u32 W25QXX_ReadID(void);  	    		//��ȡFLASH ID

u8 W25QXX_ReadSR(u8 regno);             //��ȡ״̬�Ĵ��� 
void W25QXX_4ByteAddr_Enable(void);     //ʹ��4�ֽڵ�ַģʽ
void W25QXX_Write_SR(u8 regno,u8 sr);   //д״̬�Ĵ���
void W25QXX_Write_Enable(void);  		//дʹ�� 
void W25QXX_Write_Disable(void);		//д����
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //��ȡflash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//д��flash
void W25QXX_Erase_Chip(void);    	  	//��Ƭ����
void W25QXX_Erase_Sector(u32 Dst_Addr);	//��������
void W25QXX_Wait_Busy(void);           	//�ȴ�����
void W25QXX_PowerDown(void);        	//�������ģʽ
void W25QXX_WAKEUP(void);				//����

#endif
