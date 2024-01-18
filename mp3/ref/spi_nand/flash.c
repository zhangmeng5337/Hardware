#include "flash.h" 
#include "spi.h"
#include "delay.h"   

//W25N01 �������� 

u16 SPI_FLASH_TYPE=W25N01;
//����Ϊ128M�ֽ�,����1024��Block,ÿ��Block��64��Page,ÿ��Page��2KB+64Byte

//��ʼ��SPI FLASH��IO��
void SPI_Flash_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;  //SPI CS
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
	SPI1_Init();		   //��ʼ��SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);	//����Ϊ18Mʱ��,����ģʽ
	SPI_FLASH_TYPE=SPI_FlashReadID();//��ȡFLASH ID.
}  

//��ȡSPI_FLASH��״̬�Ĵ���-1(Protection Register)
//��ַ��Axh
//BIT7  6   5   4   3   2   1   0
//SPR0 BP3 BP2 BP1 BP0 TB WP-E SPR1
//SPR0/1:Ĭ��0,״̬�Ĵ�������λ,���WPʹ��
//TB,BP3,BP2,BP1,BP0:FLASH����д��������
//WP-E:д����ʹ��λ��Ĭ��Ϊ0

//��ȡSPI_FLASH��״̬�Ĵ���-2(Configuration Register)
//��ַ��Bxh
//BIT7    6     5     4    3    2    1    0
//OTP-L OTP-E SR1-L ECC-E BUF  (R)  (R)  (R)
//OTP-L:һ���Ա������λ��Ĭ��0����1��������OTP����
//OTP-E:OTPʹ��λ��Ĭ��0����1����OTPģʽ
//SR1��L:״̬�Ĵ���-1����λ
//ECC-E:ECC(������)ʹ��λ��Ĭ��1
//BUF:Buffer Read Mode (BUF=1) and Continuous Read Mode (BUF=0)
//R: Reserved

//��ȡSPI_FLASH��״̬�Ĵ���-3(Status Only)
//��ַ��Cxh
//BIT7  6     5     4       3      2     1     0
//(R)  LUT-F ECC-1 ECC-0 P-FAIL E-FAIL  WEL   BUSY
//LUT-F:Ĭ��0,��ȫ���λ��20��bad memory blocks�����ú���1 
//ECC-1/0:ECC״̬λ
//P/E-FAIL:Program/Erase Failureλ
//WEL:дʹ������λ
//BUSY:æ���λ��1��æ��0�����У�

void SPI_Flash_ReadSR(u8 sra)   
{     
	u8 sr=0;
	SPI_FLASH_CS=0;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //���Ͷ�ȡ״̬�Ĵ�������    
	SPI1_ReadWriteByte(sra);                  //���ͼĴ�����8bit��ַ
	sr=SPI1_ReadWriteByte(0Xff);             //��ȡһ���ֽ�  
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ        
} 
//дSPI_FLASH״̬�Ĵ���
//SR1�У�SPR1,SPR0,TB,BP3,BP2,BP1,BP0,WP-E(bit 7,6,5,4,3,2,1,0)����д!!!
//SR2�У�OTP-L,OTP-E,SR1-L,ECC-E,BUF(bit 7,6,5,4,3)����д!!!
//SR3�У�Read only.
void SPI_FLASH_WriteSR(u8 sr, u8 sra)   
{   
	SPI_FLASH_CS=0;                            //ʹ������   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //����дȡ״̬�Ĵ������� 
	SPI1_ReadWriteByte(sra);                  //���ͼĴ����ĵ�ַ
	SPI1_ReadWriteByte(sr);               //д��һ���ֽ�  
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
}   
//SPI_FLASHдʹ��	
//��WEL��λ   
void SPI_FLASH_Write_Enable(void)   
{
	SPI_FLASH_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_WriteEnable);      //����дʹ��  
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
} 
//SPI_FLASHд��ֹ	
//��WEL����  
void SPI_FLASH_Write_Disable(void)   
{  
	SPI_FLASH_CS=0;                            //ʹ������   
    SPI1_ReadWriteByte(W25X_WriteDisable);     //����д��ָֹ��    
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
} 			    
//��ȡоƬID W25N01��ID:0XEFAA21
u32 SPI_FlashReadID(void)
{
	u32 Temp = 0;	  
	SPI_FLASH_CS=0;				    
	SPI1_ReadWriteByte(W25X_JedecID);//���Ͷ�ȡID����	    
	SPI1_ReadWriteByte(0xFF); 
	Temp|=SPI1_ReadWriteByte(0xFF)<<16; 	
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	SPI_FLASH_CS=1;				    
	return Temp;
}   		    

//��������(Bad Block Management)
void BadBlockManagement(u16 LBA, u16 PBA)
{
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_SwapBlocks );
	SPI1_ReadWriteByte(LBA);
	SPI1_ReadWriteByte(PBA);
	SPI_FLASH_CS=1;
}

//��ȡBBM���ı�(Read BBM Look Up Table)
void ReadBBMLUT(u16* LBA, u16* PBA)
{
	u16 i;	
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_ReadBBMLUT);
	SPI1_ReadWriteByte(0xFF);
	for(i=0;i<=19;i++)
	{
		SPI1_ReadWriteByte(LBA[i]);
		SPI1_ReadWriteByte(PBA[i]);
	}
	SPI_FLASH_CS=1;
}

//A9h
//�ϵ��Ĭ��ECC-E=1����ʱis enabled for all Program and Read operation
void LastECCFailuraPA(u16 PA_Addr)
{
	PA_Addr*=65536;
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_LastECCFailPageAddress);
	SPI1_ReadWriteByte(0xFF);
	SPI1_ReadWriteByte(PA_Addr);
	SPI_FLASH_CS=1;
}

//�����
//ʹ�ò���ָ��ǰ��������ִ��WriteEnableָ�WEL=1
//PA_Addr ҳ��ַ  0000h-FFFFh  PA[15:0];CA[11:0]
void BlockErase(u32 nSector)
{
	SPI_FLASH_Write_Enable();             //SET WEL
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_BlockErase);
	SPI1_ReadWriteByte(0xFF);
  SPI1_ReadWriteByte((nSector>>16)&0xFF);
	SPI1_ReadWriteByte((nSector>>8)&0xFF);
	SPI1_ReadWriteByte((nSector)&0xFF);  //����16bitҳ��ַ       
	SPI_FLASH_CS=1;
	SPI_Flash_Wait_Busy();                //�ȴ���������
}


//��ȡSPI FLASH  
//pBuffer:���ݴ洢��
//NumByteToRead:Ҫ��ȡ���ֽ���(���65535)
//1.ҳ���ݶ�ȡ��13h������ָ���ڴ�ҳ�е�����ת����Data Buffer��
//2.���ݶ�ȡ��03h������Data Buffer�ж�ȡ����

void SPI_Flash_Read(u8* pBuffer,u32 nSector,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	SPI_FLASH_CS=0;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_PageDataRead);     
  SPI1_ReadWriteByte((nSector>>16)&0xFF);
	SPI1_ReadWriteByte((nSector>>8)&0xFF);
	SPI1_ReadWriteByte((nSector)&0xFF);	
	SPI_FLASH_CS=1;	
	SPI_Flash_Wait_Busy();

	SPI_FLASH_CS=0;                            //ʹ������   
  SPI1_ReadWriteByte(W25X_ReadData);
  SPI1_ReadWriteByte(0xFF);
	for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //ѭ������  
   }
	SPI_FLASH_CS=1;                            //ȡ��Ƭѡ     	      
}  


//дSPI_Flash
//Program operation����Load Program Data��Program Execute
//1.���س�������:��program data���ص�DataBuffer��
//2.����ִ�У���Data Buffer�е����ݱ�̵�ָ���������ڴ�ҳ��
void SPI_Flash_Write(u32 nSector,u8* pBuffer)
{
	u32 j;
	SPI_FLASH_Write_Enable();
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_ProgramDataLoad);
	SPI1_ReadWriteByte(0);
	SPI1_ReadWriteByte(0);   
	for(j=0;j<=2111;j++)
		{
			SPI1_ReadWriteByte(pBuffer[j]);
		}
	SPI_FLASH_CS=1;
	SPI_Flash_Wait_Busy();
		
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_ProgramExecute);
	SPI1_ReadWriteByte(0xFF);
  SPI1_ReadWriteByte((nSector>>16)&0xFF);
	SPI1_ReadWriteByte((nSector>>8)&0xFF);
	SPI1_ReadWriteByte((nSector)&0xFF);	
	SPI_FLASH_CS=1;
	SPI_Flash_Wait_Busy();
	SPI_FLASH_Write_Disable();
}

//�ȴ�����
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR(0xC0)&0x01)==0x01);   // �ȴ�BUSYλ���
}  

////Program operation����Load Program Data��Program Execute
////1.���س�������:��program data���ص�DataBuffer��
////CA_Addr �е�ַ CA[11:0]
//void LoadProgramData(u16 CA_Addr,u8* data)
//{
//	CA_Addr*=2048;
//	SPI_FLASH_Write_Enable();
//	SPI_FLASH_CS=0;
//	SPI1_ReadWriteByte(W25X_ProgramDataLoad);
//	SPI1_ReadWriteByte(0xFF);
//	SPI1_ReadWriteByte(CA_Addr);   //����16bit��ַ ��ֻ��[11:0]��Ч
//	u16 i;	
//	for(i=0;i<=2111;i++)
//		{
//			SPI1_ReadWriteByte(data[i]);
//			if(data[i]==0x00)
//				break;
//		}
//	SPI_FLASH_CS=1;
//}

////2.����ִ�У���Data Buffer�е����ݱ�̵�ָ���������ڴ�ҳ��
//void ProgramExecute(u16 PA_Addr)
//{
//	PA_Addr*=65536;
//	SPI_FLASH_CS=0;
//	SPI1_ReadWriteByte(W25X_ProgramExecute);
//	SPI1_ReadWriteByte(0xFF);
//	SPI1_ReadWriteByte(PA_Addr);
//	SPI_FLASH_CS=1;
//	SPI_Flash_Wait_Busy();
//	SPI_FLASH_Write_Disable();
//}

////ҳ���ݶ�ȡ��13h������ָ���ڴ�ҳ�е�����ת����Data Buffer��
//void PageDataRead(u16 PA_Addr)
//{
//	PA_Addr*=65536;
//	SPI_FLASH_CS=0;
//	SPI1_ReadWriteByte(W25X_PageDataRead);
//	SPI1_ReadWriteByte(0xFF);
//	SPI1_ReadWriteByte(PA_Addr);
//	SPI_FLASH_CS=1;
//	SPI_Flash_Wait_Busy();
//}
























