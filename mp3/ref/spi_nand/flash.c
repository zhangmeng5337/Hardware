#include "flash.h" 
#include "spi.h"
#include "delay.h"   

//W25N01 驱动函数 

u16 SPI_FLASH_TYPE=W25N01;
//容量为128M字节,共有1024个Block,每个Block有64个Page,每个Page有2KB+64Byte

//初始化SPI FLASH的IO口
void SPI_Flash_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;

  RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE );

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;  //SPI CS
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_SetBits(GPIOA,GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4);
	SPI1_Init();		   //初始化SPI
	SPI1_SetSpeed(SPI_BaudRatePrescaler_4);	//设置为18M时钟,高速模式
	SPI_FLASH_TYPE=SPI_FlashReadID();//读取FLASH ID.
}  

//读取SPI_FLASH的状态寄存器-1(Protection Register)
//地址：Axh
//BIT7  6   5   4   3   2   1   0
//SPR0 BP3 BP2 BP1 BP0 TB WP-E SPR1
//SPR0/1:默认0,状态寄存器保护位,配合WP使用
//TB,BP3,BP2,BP1,BP0:FLASH区域写保护设置
//WP-E:写保护使能位，默认为0

//读取SPI_FLASH的状态寄存器-2(Configuration Register)
//地址：Bxh
//BIT7    6     5     4    3    2    1    0
//OTP-L OTP-E SR1-L ECC-E BUF  (R)  (R)  (R)
//OTP-L:一次性编程锁定位，默认0，置1锁定整个OTP区域
//OTP-E:OTP使能位，默认0，置1进入OTP模式
//SR1—L:状态寄存器-1锁定位
//ECC-E:ECC(纠错码)使能位，默认1
//BUF:Buffer Read Mode (BUF=1) and Continuous Read Mode (BUF=0)
//R: Reserved

//读取SPI_FLASH的状态寄存器-3(Status Only)
//地址：Cxh
//BIT7  6     5     4       3      2     1     0
//(R)  LUT-F ECC-1 ECC-0 P-FAIL E-FAIL  WEL   BUSY
//LUT-F:默认0,完全查表位。20个bad memory blocks被利用后置1 
//ECC-1/0:ECC状态位
//P/E-FAIL:Program/Erase Failure位
//WEL:写使能锁定位
//BUSY:忙标记位（1，忙；0，空闲）

void SPI_Flash_ReadSR(u8 sra)   
{     
	u8 sr=0;
	SPI_FLASH_CS=0;                            //使能器件   
	SPI1_ReadWriteByte(W25X_ReadStatusReg);    //发送读取状态寄存器命令    
	SPI1_ReadWriteByte(sra);                  //发送寄存器的8bit地址
	sr=SPI1_ReadWriteByte(0Xff);             //读取一个字节  
	SPI_FLASH_CS=1;                            //取消片选        
} 
//写SPI_FLASH状态寄存器
//SR1中：SPR1,SPR0,TB,BP3,BP2,BP1,BP0,WP-E(bit 7,6,5,4,3,2,1,0)可以写!!!
//SR2中：OTP-L,OTP-E,SR1-L,ECC-E,BUF(bit 7,6,5,4,3)可以写!!!
//SR3中：Read only.
void SPI_FLASH_WriteSR(u8 sr, u8 sra)   
{   
	SPI_FLASH_CS=0;                            //使能器件   
	SPI1_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令 
	SPI1_ReadWriteByte(sra);                  //发送寄存器的地址
	SPI1_ReadWriteByte(sr);               //写入一个字节  
	SPI_FLASH_CS=1;                            //取消片选     	      
}   
//SPI_FLASH写使能	
//将WEL置位   
void SPI_FLASH_Write_Enable(void)   
{
	SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_WriteEnable);      //发送写使能  
	SPI_FLASH_CS=1;                            //取消片选     	      
} 
//SPI_FLASH写禁止	
//将WEL清零  
void SPI_FLASH_Write_Disable(void)   
{  
	SPI_FLASH_CS=0;                            //使能器件   
    SPI1_ReadWriteByte(W25X_WriteDisable);     //发送写禁止指令    
	SPI_FLASH_CS=1;                            //取消片选     	      
} 			    
//读取芯片ID W25N01的ID:0XEFAA21
u32 SPI_FlashReadID(void)
{
	u32 Temp = 0;	  
	SPI_FLASH_CS=0;				    
	SPI1_ReadWriteByte(W25X_JedecID);//发送读取ID命令	    
	SPI1_ReadWriteByte(0xFF); 
	Temp|=SPI1_ReadWriteByte(0xFF)<<16; 	
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	SPI_FLASH_CS=1;				    
	return Temp;
}   		    

//死区管理(Bad Block Management)
void BadBlockManagement(u16 LBA, u16 PBA)
{
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_SwapBlocks );
	SPI1_ReadWriteByte(LBA);
	SPI1_ReadWriteByte(PBA);
	SPI_FLASH_CS=1;
}

//读取BBM查阅表(Read BBM Look Up Table)
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
//上电后默认ECC-E=1，此时is enabled for all Program and Read operation
void LastECCFailuraPA(u16 PA_Addr)
{
	PA_Addr*=65536;
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_LastECCFailPageAddress);
	SPI1_ReadWriteByte(0xFF);
	SPI1_ReadWriteByte(PA_Addr);
	SPI_FLASH_CS=1;
}

//块擦除
//使用擦除指令前，必须先执行WriteEnable指令。WEL=1
//PA_Addr 页地址  0000h-FFFFh  PA[15:0];CA[11:0]
void BlockErase(u32 nSector)
{
	SPI_FLASH_Write_Enable();             //SET WEL
	SPI_FLASH_CS=0;
	SPI1_ReadWriteByte(W25X_BlockErase);
	SPI1_ReadWriteByte(0xFF);
  SPI1_ReadWriteByte((nSector>>16)&0xFF);
	SPI1_ReadWriteByte((nSector>>8)&0xFF);
	SPI1_ReadWriteByte((nSector)&0xFF);  //发送16bit页地址       
	SPI_FLASH_CS=1;
	SPI_Flash_Wait_Busy();                //等待擦除结束
}


//读取SPI FLASH  
//pBuffer:数据存储区
//NumByteToRead:要读取的字节数(最大65535)
//1.页数据读取（13h）：将指定内存页中的内容转换到Data Buffer中
//2.数据读取（03h）：从Data Buffer中读取内容

void SPI_Flash_Read(u8* pBuffer,u32 nSector,u16 NumByteToRead)   
{ 
 	u16 i;    												    
	SPI_FLASH_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_PageDataRead);     
  SPI1_ReadWriteByte((nSector>>16)&0xFF);
	SPI1_ReadWriteByte((nSector>>8)&0xFF);
	SPI1_ReadWriteByte((nSector)&0xFF);	
	SPI_FLASH_CS=1;	
	SPI_Flash_Wait_Busy();

	SPI_FLASH_CS=0;                            //使能器件   
  SPI1_ReadWriteByte(W25X_ReadData);
  SPI1_ReadWriteByte(0xFF);
	for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI1_ReadWriteByte(0XFF);   //循环读数  
   }
	SPI_FLASH_CS=1;                            //取消片选     	      
}  


//写SPI_Flash
//Program operation包括Load Program Data和Program Execute
//1.加载程序数据:将program data加载到DataBuffer中
//2.程序执行：将Data Buffer中的内容编程到指定的物理内存页中
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

//等待空闲
void SPI_Flash_Wait_Busy(void)   
{   
	while ((SPI_Flash_ReadSR(0xC0)&0x01)==0x01);   // 等待BUSY位清空
}  

////Program operation包括Load Program Data和Program Execute
////1.加载程序数据:将program data加载到DataBuffer中
////CA_Addr 列地址 CA[11:0]
//void LoadProgramData(u16 CA_Addr,u8* data)
//{
//	CA_Addr*=2048;
//	SPI_FLASH_Write_Enable();
//	SPI_FLASH_CS=0;
//	SPI1_ReadWriteByte(W25X_ProgramDataLoad);
//	SPI1_ReadWriteByte(0xFF);
//	SPI1_ReadWriteByte(CA_Addr);   //发送16bit地址 但只有[11:0]有效
//	u16 i;	
//	for(i=0;i<=2111;i++)
//		{
//			SPI1_ReadWriteByte(data[i]);
//			if(data[i]==0x00)
//				break;
//		}
//	SPI_FLASH_CS=1;
//}

////2.程序执行：将Data Buffer中的内容编程到指定的物理内存页中
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

////页数据读取（13h）：将指定内存页中的内容转换到Data Buffer中
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
























