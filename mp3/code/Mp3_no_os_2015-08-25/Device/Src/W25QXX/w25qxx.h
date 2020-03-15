#ifndef __W25QXX_H
#define __W25QXX_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F429开发板
//W25QXX驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/16
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//W25X系列/Q系列芯片列表	   
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
extern u16 W25QXX_TYPE;					//定义W25QXX芯片型号		   

#define	W25QXX_CS 		PFout(6)  		//W25QXX的片选信号

////////////////////////////////////////////////////////////////////////////////// 
//指令表
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
//#define FLASH_SECTOR_COUNT 	512*2*25*2	//W25Q256,前25M字节给FATFS占用	
#define FLASH_BLOCK_SIZE   	64*4     		//每个BLOCK有8个扇区	





//FLASH最大能够达到的地址，是128M
#define FLASH_MAX_ADDR 0x8000000

//FLASH块大小，为128KB
#define FLASH_BLOCK_SIZE 0x20000

//FLASH页大小，为2KB
#define FLASH_PAGE_SIZE 0x800


//扇区大小
#define FLASH_SECTOR_SIZE 0x200

//用做交换区的块数
#define FLASH_SWAP_BLOCKS 10

//用做坏块重新影射的块数
#define FLASH_BAD_BLOCKS_REMAP 50

//保存坏块表的块数
#define FLASH_BLOCKS_TABLE 3

//最大能够达到的扇区地址
#define FLASH_MAX_SECTOR_ADDR (FLASH_MAX_ADDR-(FLASH_BAD_BLOCKS_REMAP+FLASH_SWAP_BLOCKS+FLASH_BLOCKS_TABLE)*FLASH_BLOCK_SIZE)

//交换区的起始地址
#define FLASH_SWAP_BLOCK_ADDR (FLASH_MAX_ADDR+1-FLASH_SWAP_BLOCKS*FLASH_BLOCK_SIZE)

//重影射坏块的起始地址
#define FLASH_BAD_BLOCK_REMAP_ADDR (FLASH_MAX_ADDR+1-(FLASH_BAD_BLOCKS_REMAP+FLASH_SWAP_BLOCKS)*FLASH_BLOCK_SIZE)

//定义保存坏块表的起始地址
#define FLASH_BLOCK_TABLE_ADDR (FLASH_MAX_ADDR+1-(FLASH_BAD_BLOCKS_REMAP+FLASH_SWAP_BLOCKS+FLASH_BLOCKS_TABLE)*FLASH_BLOCK_SIZE)

//影射区的状态常量定义
#define FLASH_BLOCK_OK    0
#define FLASH_BLOCK_BAD   1
#define FLASH_BLOCK_USED  2

void W25QXX_Init(void);
u32 W25QXX_ReadID(void);  	    		//读取FLASH ID

u8 W25QXX_ReadSR(u8 regno);             //读取状态寄存器 
void W25QXX_4ByteAddr_Enable(void);     //使能4字节地址模式
void W25QXX_Write_SR(u8 regno,u8 sr);   //写状态寄存器
void W25QXX_Write_Enable(void);  		//写使能 
void W25QXX_Write_Disable(void);		//写保护
void W25QXX_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25QXX_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void W25QXX_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void W25QXX_Erase_Chip(void);    	  	//整片擦除
void W25QXX_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void W25QXX_Wait_Busy(void);           	//等待空闲
void W25QXX_PowerDown(void);        	//进入掉电模式
void W25QXX_WAKEUP(void);				//唤醒

#endif
