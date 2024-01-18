#ifndef __FLASH_H
#define __FLASH_H			    
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//Mini STM32������
//W25X16 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2010/6/13 
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
////////////////////////////////////////////////////////////////////////////////// 	 
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16

#define W25N01  0XEFAA21
 

extern u16 SPI_FLASH_TYPE;		//��������ʹ�õ�flashоƬ�ͺ�		

#define	SPI_FLASH_CS PAout(2)  //ѡ��FLASH					 
////////////////////////////////////////////////////////////////////////////

extern u8 SPI_FLASH_BUF[4096];
////W25X16��д
//#define FLASH_ID 0XEF14

//W25n01��д
#define FLASH_ID 0XAA21
//ָ���
//#define W25X_WriteEnable		0x06 
//#define W25X_WriteDisable		0x04 
//#define W25X_ReadStatusReg		0x05 
//#define W25X_WriteStatusReg		0x01 
//#define W25X_ReadData			0x03 
//#define W25X_FastReadData		0x0B 
//#define W25X_FastReadDual		0x3B 
//#define W25X_PageProgram		0x02 
//#define W25X_BlockErase			0xD8 
//#define W25X_SectorErase		0x20 
//#define W25X_ChipErase			0xC7 
//#define W25X_PowerDown			0xB9 
//#define W25X_ReleasePowerDown	0xAB 
//#define W25X_DeviceID			0xAB 
//#define W25X_ManufactDeviceID	0x90 
//#define W25X_JedecDeviceID		0x9F 

#define W25X_WriteEnable		  0x06 
#define W25X_WriteDisable		  0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			    0x03 
#define W25X_FastReadData		  0x0B 
#define W25X_FastReadwith4ByteAddress			0x0C
//#define W25X_FastReadDualOut		0x3B
//#define W25X_FastReadDualOutwith4ByteAddress		0x3C 
//#define W25X_FastReadQuadOut		0x6B
//#define W25X_FastReadQuadOutwith4ByteAddress		0x6C
//#define W25X_FastReadDualIO		0xBB 
//#define W25X_FastReadDualIOwith4ByteAddress		0xBC
//#define W25X_FastReadQuadIO		0xEB
//#define W25X_FastReadQuadIOwith4ByteAddress			0xEC
#define W25X_ProgramDataLoad	0x02 
#define W25X_BlockErase			  0xD8 
//#define W25X_QuadProgramDataLoad		0x32 
#define W25X_RandomProgramDataLoad			0x84 
#define W25X_LastECCFailPageAddress			0xA9 
#define W25X_ReadBBMLUT	      0xA5 
#define W25X_SwapBlocks			  0xA1   
//#define W25X_RandomQuadProgramDataLoad			0x34 
#define W25X_ProgramExecute	  0x10 
#define W25X_PageDataRead			0x13
#define W25X_DeviceRESET	    0xFF
#define W25X_JedecID		      0x9F 

void SPI_Flash_Init(void);
u32 SPI_FlashReadID(void);  	    //��ȡFLASH ID
void SPI_Flash_ReadSR(u8 sra);        //��ȡ״̬�Ĵ��� 
void SPI_FLASH_WriteSR(u8 sr, u8 sra);  	//д״̬�Ĵ���
void SPI_FLASH_Write_Enable(void);  //дʹ�� 
void SPI_FLASH_Write_Disable(void);	//д����
void BadBlockManagement(u16 LBA, u16 PBA);  //��������
void SPI_Flash_Read(u8* pBuffer,u32 ReadCA,u16 NumByteToRead);   //��ȡflash
void SPI_Flash_Write(u32 nSector,u8* pBuffer);//д��flash
void SPI_Flash_Erase_Chip(void);    	  //��Ƭ����
void SPI_Flash_Erase_Sector(u32 Dst_Addr);//��������
void SPI_Flash_Wait_Busy(void);           //�ȴ�����
void SPI_Flash_PowerDown(void);           //�������ģʽ
void SPI_Flash_WAKEUP(void);			  //����
#endif
















