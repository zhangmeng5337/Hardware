/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : fsmc_nand.h
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Header for fsmc_nand.c file.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FSMC_NAND_H
#define __FSMC_NAND_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_lib.h"

//my type
#define uint8    unsigned char
#define uint16   unsigned short int
#define uint32   unsigned long int
#define int8     signed char
#define int16    signed short int
#define int32    signed long int
#define uint64   unsigned long long int
#define int64    signed long long int

#define uint unsigned int

/* Exported types ------------------------------------------------------------*/

/* Exported constants --------------------------------------------------------*/
/* NAND Area definition  for STM3210E-EVAL Board RevD */
#define CMD_AREA                   (u32)(1<<16)  /* A16 = CLE  high */
#define ADDR_AREA                  (u32)(1<<17)  /* A17 = ALE high */

#define DATA_AREA                  ((u32)0x00000000) 


/* FSMC NAND memory parameters */
#define NAND_PAGE_SIZE             ((u16)0x0200) /* 512 bytes per page w/o Spare Area */
#define NAND_BLOCK_SIZE            ((u16)0x0020) /* 32x512 bytes pages per block */
#define NAND_ZONE_SIZE             ((u16)0x0400) /* 1024 Block per zone */
#define NAND_SPARE_AREA_SIZE       ((u16)0x0010) /* last 16 bytes as spare area */
//#define NAND_MAX_ZONE              ((u16)0x0004) /* 4 zones of 1024 block */
#define NAND_MAX_ZONE              ((u16)0x0001) /* 1 zones of 1024 block */

/* FSMC NAND memory address computation */
#define ADDR_1st_CYCLE(ADDR)       (u8)((ADDR)& 0xFF)               /* 1st addressing cycle */
#define ADDR_2nd_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF00) >> 8)      /* 2nd addressing cycle */
#define ADDR_3rd_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF0000) >> 16)   /* 3rd addressing cycle */
#define ADDR_4th_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF000000) >> 24) /* 4th addressing cycle */

//FLASH����ܹ��ﵽ�ĵ�ַ����128M
#define FLASH_MAX_ADDR 0x7FFFFFF

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

void FSMC_NAND_Init(void);
void FlashReset(void);
void FlashLoadBadBlockTable(void);
void FlashInit(void);
uint8 FlashEraseBlock(uint32 Addr);
uint32 FlashManageSwapBlock(uint32 Op);
uint32 FlashWriteOneSector(uint32 Addr, uint8 * pBuf, uint32 Remain);
uint32 FlashReadOneSector(uint32 Addr, uint8 * pBuf, uint32 Remain);
void FlashReadId(uint8 *Buf);
void FlashSaveBadBlockTable(void);


/********************************************************************
�������ܣ���ȡ��һ�����õĽ������ַ��
��ڲ������ޡ�
��    �أ���һ�����õĽ�����ĵ�ַ��
��    ע���ޡ�
********************************************************************/
#define FlashSwapBlockInit()  FlashManageSwapBlock(0)
/////////////////////////End of function/////////////////////////////

/********************************************************************
�������ܣ���ȡ��һ�����õĽ������ַ��
��ڲ������ޡ�
��    �أ���һ�����õĽ�����ĵ�ַ��
��    ע���ޡ�
********************************************************************/
#define FlashGetNextSwapBlock()  FlashManageSwapBlock(1)
/////////////////////////End of function/////////////////////////////

/********************************************************************
�������ܣ���ȡ��ǰ����ʹ�õĽ������ַ��
��ڲ������ޡ�
��    �أ���һ�����õĽ�����ĵ�ַ��
��    ע���ޡ�
********************************************************************/
#define FlashGetCurrentSwapBlock()  FlashManageSwapBlock(2)
/////////////////////////End of function/////////////////////////////

/********************************************************************
�������ܣ���־��ǰ������Ϊ���顣
��ڲ������ޡ�
��    �أ���һ�����õĽ�����ĵ�ַ��
��    ע���ޡ�
********************************************************************/
#define FlashMarkBadCurrentSwapBlock()  FlashManageSwapBlock(3)
/////////////////////////End of function/////////////////////////////

extern uint8 FlashSectorBuf[FLASH_SECTOR_SIZE];


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __FSMC_NAND_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
