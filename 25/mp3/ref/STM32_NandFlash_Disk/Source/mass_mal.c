/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : mass_mal.c
* Author             : MCD Application Team
* Version            : V2.2.0
* Date               : 06/13/2008
* Description        : Medium Access Layer interface
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "SD_driver.h"
#include "fsmc_nand.h"
#include "mass_mal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32 Mass_Memory_Size[2];
u32 Mass_Block_Size[2];
u32 Mass_Block_Count[2];

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : MAL_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 MAL_Init(u8 lun)
{
  u16 status = MAL_OK;

  switch (lun)
  {
    case 0:
     SD_Init();
     break;
    case 1:
      FlashInit();
      break;
    default:
      return MAL_FAIL;
  }
  return status;
}
/*******************************************************************************
* Function Name  : MAL_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 MAL_Write(u8 lun, u32 Memory_Offset, u32 *Writebuff, u16 Transfer_Length)
{

  switch (lun)
  {
    case 0:

      SD_WriteSingleBlock(Memory_Offset/512, (u8*)Writebuff);

      break;

    case 1:
      FlashWriteOneSector(Memory_Offset, (u8*)Writebuff, Transfer_Length);
      break;

    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : Buffer pointer
*******************************************************************************/
u16 MAL_Read(u8 lun, u32 Memory_Offset, u32 *Readbuff, u16 Transfer_Length)
{

  switch (lun)
  {
    case 0:

     SD_ReadSingleBlock(Memory_Offset/512, (u8*)Readbuff);

      break;

    case 1:
      FlashReadOneSector(Memory_Offset, (u8*)Readbuff, Transfer_Length);
      break;

    default:
      return MAL_FAIL;
  }
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : MAL_GetStatus
* Description    : Get status   获取磁盘容量
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 MAL_GetStatus (u8 lun)
{
   u8 csd[16];
    u32 Capacity;
    u8 r1;
    u16 i;
	u16 temp;
  if (lun == 0)
  {
  		 
		    if(SD_GetCSD(csd)!=0)
		    {
		        return MAL_FAIL;
		    }
       
				if((csd[0]&0xC0)==0x40)
				{
				Mass_Memory_Size[0]=  (((u32)csd[8])<<8 + (u32)csd[9] +1)*(u32)1024;
				Mass_Block_Count[0] = (((u32)csd[8])<<8 + (u32)csd[9] +1);
				Mass_Block_Size[0]  =1024;
				
				}
					else
					{
					//C_SIZE
					i = csd[6]&0x03;
					i<<=8;
					i += csd[7];
					i<<=2;
					i += ((csd[8]&0xc0)>>6);
					
					//C_SIZE_MULT
					r1 = csd[9]&0x03;
					r1<<=1;
					r1 += ((csd[10]&0x80)>>7);
					
					//BLOCKNR
					r1+=2;
					temp = 1;
					while(r1)
					{
					temp*=2;
					r1--;
					}
					Mass_Memory_Size[0]= ((u32)(i+1))*((u32)temp);
					
					Mass_Block_Count[0] = ((u32)(i+1));
					Mass_Block_Size[0]  =((u32)temp);
					
					// READ_BL_LEN
					i = csd[5]&0x0f;
					//BLOCK_LEN
					temp = 1;
					while(i)
					{
					temp*=2;
					i--;
					}
					//The final result
					Mass_Memory_Size[0] *= (u32)temp; 
					Mass_Block_Count[0] *= (u32)temp;
					//Capacity /= 512;
					}				  
		      return MAL_OK;

  }

  else
  {
   //计算磁盘大小   ＝实际大小－备用块大小
      Mass_Block_Count[1] = FLASH_MAX_SECTOR_ADDR/FLASH_SECTOR_SIZE; //NAND_ZONE_SIZE * NAND_BLOCK_SIZE * NAND_MAX_ZONE ;
      Mass_Block_Size[1]  = FLASH_SECTOR_SIZE;//NAND_PAGE_SIZE;
      Mass_Memory_Size[1] = (Mass_Block_Count[1] * Mass_Block_Size[1]);
      return MAL_OK;
 }

  return MAL_FAIL;
}





/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
