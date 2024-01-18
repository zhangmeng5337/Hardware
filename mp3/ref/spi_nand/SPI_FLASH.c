#include "SPI_FLASH.h"

UINT8 FLASH_INIT(void){
  DDRP_DDRP5 = 1;
  PTP_PTP5 = 1;
}

UINT8 SPI1_Flash_Send(UINT8 val)
{
	while (!SPI1SR_SPTEF);//发送
	SPI1DR = val;
	while (!(SPI1SR_SPIF));//接收
	return SPI1DR;
}
/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  unsigned char FLASH_Status = 0;

  /* Select the FLASH: Chip Select low */
  FLASH_CS_L();

  /* Send "Read Status Register" instruction */
  SPI1_Flash_Send(W25N_ReadStatusReg);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI1_Flash_Send(Dummy_Byte);
    //printf("FLASH_Status=%x\r\n",FLASH_Status);	 
  }while ((FLASH_Status & WIP_Flag) != 1); /* Write in progress */

  /* Deselect the FLASH: Chip Select high */
  FLASH_CS_H();
}

/*******************************************************************************
* Function Name  : PageUnlock
* Description    : flash page unlock
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PageUnlock(void)
{
	 FLASH_CS_L();
	 
	 SPI1_Flash_Send(0x01); 
	 SPI1_Flash_Send(0xA0);
	 SPI1_Flash_Send(0x00); 
	 
	 FLASH_CS_H();
	 SPI_FLASH_WaitForWriteEnd();		
}

void ChangeSR2(void)
{
	 FLASH_CS_L();
	 
	 SPI1_Flash_Send(0x01); 
	 SPI1_Flash_Send(0xB0);
	 SPI1_Flash_Send(0x08); 
	 
	 FLASH_CS_H();

	 SPI_FLASH_WaitForWriteEnd();		
}

UINT16 W25NXX_ReadID(void)
{
	UINT16 Temp = 0;
	
	SPI1CR2 = 0x00;
	SPI1CR1 = 0x5E;
		  
	FLASH_CS_L();				    
	SPI1_Flash_Send(0x9F);//读取ID	    
	SPI1_Flash_Send(Dummy_Byte); 	    
	SPI1_Flash_Send(Dummy_Byte); 	    
 	 			     
	Temp|=SPI1_Flash_Send(Dummy_Byte)<<8;
	Temp|=SPI1_Flash_Send(Dummy_Byte);	 
	FLASH_CS_H();				    
	return Temp;
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  /* Select the FLASH: Chip Select low */
  FLASH_CS_L();

  /* Send "Write Enable" instruction */
  SPI1_Flash_Send(W25N_WriteEnable);

  /* Deselect the FLASH: Chip Select high */
  FLASH_CS_H();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(UINT16 BlockAddr)
{
  //UINT8 i = 0;
  SPI1CR2 = 0x00;
	SPI1CR1 = 0x5E;
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();
  //SPI_FLASH_WaitForWriteEnd();
  
  /* Select the FLASH: Chip Select low */
  FLASH_CS_L();
  /* Sector Erase */   
    /* Send Sector Erase instruction */
    SPI1_Flash_Send(W25N_BlockErase);
    /* Send SectorAddr high nibble address byte */
    SPI1_Flash_Send(Dummy_Byte);
    /* Send SectorAddr medium nibble address byte */
    SPI1_Flash_Send((BlockAddr & 0xFF00) >> 8);
    /* Send SectorAddr low nibble address byte */
    SPI1_Flash_Send(BlockAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
    FLASH_CS_H();
  /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd();
  
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageErase
* Description    : Erases the specified FLASH Page.
* Input          : SectorAddr: address of the Page to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageErase(UINT16 BlockAddr){

  UINT8 i = 0;
  
   for(i = 0; i < 8; i++){  //0x0000~0x007F  256KB
    
    SPI_FLASH_SectorErase(BlockAddr+(128*i));
   }
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(UINT8* pBuffer, UINT16 WriteAddr, UINT16 NumByteToWrite)
{
  SPI1CR2 = 0x00;
	SPI1CR1 = 0x5E;
  /* Enable the write access to the FLASH */
  SPI_FLASH_WriteEnable();

  /* Select the FLASH: Chip Select low */
  FLASH_CS_L();
  /* Send "Write to Memory " instruction */
  SPI1_Flash_Send(W25N_PageProgram);
  /* Send WriteAddr high nibble address byte to write to */
  
  /* Send WriteAddr medium nibble address byte to write to */
  SPI1_Flash_Send((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI1_Flash_Send(WriteAddr & 0xFF);

  if(NumByteToWrite > SPI_FLASH_PerWritePageSize)
  {
     NumByteToWrite = SPI_FLASH_PerWritePageSize;
     //printf("\n\r Err: SPI_FLASH_PageWrite too large!");
  }

  /* while there is data to be written on the FLASH */
  while (NumByteToWrite--)
  {
    /* Send the current byte */
    SPI1_Flash_Send(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  FLASH_CS_H();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(UINT8* pBuffer, UINT16 ReadAddr, UINT16 NumByteToRead)
{

  SPI1CR2 = 0x00;
	SPI1CR1 = 0x5E;
  /* Select the FLASH: Chip Select low */
  FLASH_CS_L();

  /* Send "Read from Memory " instruction */
  SPI1_Flash_Send(W25N_ReadData);

  /* Send ReadAddr high nibble address byte to read from */
  
  /* Send ReadAddr medium nibble address byte to read from */
  SPI1_Flash_Send((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI1_Flash_Send(ReadAddr & 0xFF);
  
  SPI1_Flash_Send(Dummy_Byte);
  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI1_Flash_Send(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  FLASH_CS_H();
}
