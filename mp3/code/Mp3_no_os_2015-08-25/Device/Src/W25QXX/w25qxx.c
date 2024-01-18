#include "w25qxx.h"
#include "spi2.h"
#include "delay.h"
//#include "usart.h"
#include "stm32f10x.h"
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

u16 W25QXX_TYPE=W25Q256;	//默认是W25Q256
u8 W25QXX_BUFFER[256];	

//4Kbytes为一个Sector
//16个扇区为1个Block
//W25Q256
//容量为32M字节,共有512个Block,8192个Sector 
													 
//初始化SPI FLASH的IO口
void W25QXX_Init(void)
{ 
    u8 temp;
  /*  GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOF_CLK_ENABLE();           //使能GPIOF时钟
    
    //PF6
    GPIO_Initure.Pin=GPIO_PIN_6;            //PF6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //推挽输出
    GPIO_Initure.Pull=GPIO_PULLUP;          //上拉
    GPIO_Initure.Speed=GPIO_SPEED_FAST;     //快速         
    HAL_GPIO_Init(GPIOF,&GPIO_Initure);     //初始化
    
	W25QXX_CS=1;			                //SPI FLASH不选中
	SPI5_Init();		   			        //初始化SPI
	SPI5_SetSpeed(SPI_BAUDRATEPRESCALER_2); //设置为45M时钟,高速模式*/

	SPI2_Enable();
	W25QXX_TYPE=W25QXX_ReadID();	        //读取FLASH ID.
    if(W25QXX_TYPE==W25N01)                //SPI FLASH为W25Q256
    {
        temp=W25QXX_ReadSR(3);              //读取状态寄存器3，判断地址模式
        if((temp&0X01)==0)			        //如果不是4字节地址模式,则进入4字节地址模式
		{
			W25QXX_CS=0; 			        //选中
			SPI2_ReadWriteByte(W25X_Enable4ByteAddr);//发送进入4字节地址模式指令   
			W25QXX_CS=1;       		        //取消片选   
		}
    }
}  

//读取W25QXX的状态寄存器，W25QXX一共有3个状态寄存器
//状态寄存器1：
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
//状态寄存器2：
//BIT7  6   5   4   3   2   1   0
//SUS   CMP LB3 LB2 LB1 (R) QE  SRP1
//状态寄存器3：
//BIT7      6    5    4   3   2   1   0
//HOLD/RST  DRV1 DRV0 (R) (R) WPS ADP ADS
//regno:状态寄存器号，范:1~3
//返回值:状态寄存器值
u8 W25QXX_ReadSR(u8 regno)   
{  
	u8 byte=0,command=0; 
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //读状态寄存器1指令
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //读状态寄存器2指令
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //读状态寄存器3指令
            break;
        case 4:
            command=0xC0;    //读状态寄存器3指令
            break;

        default:
            command=W25X_WriteStatusReg1;    
            break;
    }    
	W25QXX_CS=0;                            //使能器件  
	SPI2_ReadWriteByte(W25X_WriteStatusReg);    //发送读取状态寄存器命令    
	SPI2_ReadWriteByte(command);            //发送读取状态寄存器命令    
	byte=SPI2_ReadWriteByte(0Xff);          //读取一个字节  
	W25QXX_CS=1;                            //取消片选     
	return byte;   
	
} 



//写W25QXX状态寄存器
void W25QXX_Write_SR(u8 regno,u8 sr)   
{   
    u8 command=0;
    switch(regno)
    {
        case 1:
            command=W25X_WriteStatusReg1;    //写状态寄存器1指令
            break;
        case 2:
            command=W25X_WriteStatusReg2;    //写状态寄存器2指令
            break;
        case 3:
            command=W25X_WriteStatusReg3;    //写状态寄存器3指令
            break;
        default:
            command=W25X_WriteStatusReg1;    
            break;
    }   
	W25QXX_CS=0;                            //使能器件   
	SPI2_ReadWriteByte(W25X_WriteStatusReg);   //发送写取状态寄存器命令
	SPI2_ReadWriteByte(command);            //发送写取状态寄存器命令    
	SPI2_ReadWriteByte(sr);                 //写入一个字节  
	W25QXX_CS=1;                            //取消片选     	      
}   
//W25QXX写使能	
//将WEL置位   
void W25QXX_Write_Enable(void)   
{
	W25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(W25X_WriteEnable);   //发送写使能  
	W25QXX_CS=1;                            //取消片选     	      
} 
//W25QXX写禁止	
//将WEL清零  
void W25QXX_Write_Disable(void)   
{  
	W25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(W25X_WriteDisable);  //发送写禁止指令    
	W25QXX_CS=1;                            //取消片选     	      
} 

//读取芯片ID
//返回值如下:				   
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
//0XEF18,表示芯片型号为W25Q256
//读取芯片ID W25N01的ID:0XEFAA21
u32 W25QXX_ReadID(void)
{
	
	u32 Temp = 0;	  
	W25QXX_CS=0;				    
	SPI1_ReadWriteByte(W25X_JedecDeviceID);//发送读取ID命令	    
	SPI1_ReadWriteByte(0xFF); 
	Temp|=SPI1_ReadWriteByte(0xFF)<<16; 	
	Temp|=SPI1_ReadWriteByte(0xFF)<<8;  
	Temp|=SPI1_ReadWriteByte(0xFF);	 
	W25QXX_CS=1;				    
	return Temp;
}


//擦除整个芯片		  
//等待时间超长...
void W25QXX_Erase_Chip(void)   
{                                   
    W25QXX_Write_Enable();                  //SET WEL 
    W25QXX_Wait_Busy();   
  	W25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(W25X_ChipErase);        //发送片擦除命令  
	W25QXX_CS=1;                            //取消片选     	      
	W25QXX_Wait_Busy();   				   //等待芯片擦除结束
}   
//擦除一个扇区
//Dst_Addr:扇区地址 根据实际容量设置
//擦除一个扇区的最少时间:150ms
void W25QXX_Erase_Sector(u32 Dst_Addr)   
{  
	//监视falsh擦除情况,测试用   
 	//printf("fe:%x\r\n",Dst_Addr);	  
 	Dst_Addr*=4096*16*2;
    W25QXX_Write_Enable();                  //SET WEL 	
    W25QXX_CS=0;  
	SPI2_ReadWriteByte(W25X_SectorErase);   //发送扇区擦除指令 
	SPI2_ReadWriteByte(0xFF);   //发送扇区擦除指令 
    if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        SPI2_ReadWriteByte((u8)((Dst_Addr)>>24)); 
    }
	SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));  //发送24bit地址    
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI2_ReadWriteByte((u8)Dst_Addr);  	
	W25QXX_CS=1;                            //取消片选     	      
    W25QXX_Wait_Busy();   				    //等待擦除完成

} 

//死区管理(Bad Block Management)
void BadBlockManagement(u16 LBA, u16 PBA)
{
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_SwapBlocks );
	SPI2_ReadWriteByte(LBA);
	SPI2_ReadWriteByte(PBA);
	W25QXX_CS=1;
}

//读取BBM查阅表(Read BBM Look Up Table)
void ReadBBMLUT(u16* LBA, u16* PBA)
{
	u16 i;	
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_ReadBBMLUT);
	SPI2_ReadWriteByte(0xFF);
	for(i=0;i<=19;i++)
	{
		SPI2_ReadWriteByte(LBA[i]);
		SPI2_ReadWriteByte(PBA[i]);
	}
	W25QXX_CS=1;
}

//A9h
//上电后默认ECC-E=1，此时is enabled for all Program and Read operation
void LastECCFailuraPA(u16 PA_Addr)
{
	PA_Addr*=65536;
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_LastECCFailPageAddress);
	SPI2_ReadWriteByte(0xFF);
	SPI2_ReadWriteByte(PA_Addr);
	W25QXX_CS=1;
}

//块擦除
//使用擦除指令前，必须先执行WriteEnable指令。WEL=1
//PA_Addr 页地址  0000h-FFFFh  PA[15:0];CA[11:0]
void BlockErase(u32 nSector)
{
	W25QXX_Write_Enable();             //SET WEL
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_BlockErase);
	SPI2_ReadWriteByte(0xFF);
    SPI2_ReadWriteByte((nSector>>16)&0xFF);
	SPI2_ReadWriteByte((nSector>>8)&0xFF);
	SPI2_ReadWriteByte((nSector)&0xFF);  //发送16bit页地址       
	W25QXX_CS=1;
	W25QXX_Wait_Busy();                //等待擦除结束
}

//die选择00,0x01
void DieSelect(u8 dieNo)
{
	//W25QXX_Write_Enable();             //SET WEL
	W25QXX_CS=0;
	SPI2_ReadWriteByte(W25X_DieSelect);
    SPI2_ReadWriteByte(dieNo);      
	W25QXX_CS=1;
	
}

//等待空闲
void W25QXX_Wait_Busy(void)   
{   
	while((W25QXX_ReadSR(4)&0x01)==0x01);   // 等待BUSY位清空
}  
//读取SPI FLASH  
//pBuffer:数据存储区
//NumByteToRead:要读取的字节数(最大65535)
//1.页数据读取（13h）：将指定内存页中的内容转换到Data Buffer中
//2.数据读取（03h）：从Data Buffer中读取内容


void W25QXX_Read(u8* pBuffer,u32 nSector,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(W25X_PageDataRead);      //发送读取命令  
    if(W25QXX_TYPE==W25Q256)                //如果是W25Q256的话地址为4字节的，要发送最高8位
    {
        SPI2_ReadWriteByte((u8)((nSector)>>24));    
    }
    SPI2_ReadWriteByte((u8)((nSector)>>16));   //发送24bit地址    
    SPI2_ReadWriteByte((u8)((nSector)>>8));   
    SPI2_ReadWriteByte((u8)nSector);  
	W25QXX_CS=1;  
	W25QXX_Wait_Busy();  
	W25QXX_CS=0; 
	SPI2_ReadWriteByte(W25X_ReadData);
	SPI2_ReadWriteByte(0xFF);

    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI2_ReadWriteByte(0XFF);    //循环读数  
    }
	W25QXX_CS=1;  						 //取消片选  
}  





//写SPI_Flash
//Program operation包括Load Program Data和Program Execute
//1.加载程序数据:将program data加载到DataBuffer中
//2.程序执行：将Data Buffer中的内容编程到指定的物理内存页中

//SPI在一页(0~65535)内写入少于256个字节的数据
//在指定地址开始写入最大256字节的数据
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大256),该数不应该超过该页的剩余字节数!!!	 
u8 W25QXX_Write_Page(u8* pBuffer,u32 nSector,u16 NumByteToWrite)
{
 	u16 i;  
    W25QXX_Write_Enable();                  //SET WEL 
	W25QXX_CS=0;                            //使能器件   
    SPI2_ReadWriteByte(W25X_PageProgram);   //Load Program Data   
    SPI2_ReadWriteByte(0);
	SPI2_ReadWriteByte(0);    
	for(i=0;i<2112;i++)
		SPI2_ReadWriteByte(pBuffer[i]);//循环写数  
	W25QXX_CS=1;                            //取消片选 
		W25QXX_Wait_Busy();					   //等待写入结束
	W25QXX_CS=0; 	
	SPI2_ReadWriteByte(W25X_ProgramExecute);   
	SPI2_ReadWriteByte(0xff); 

    SPI2_ReadWriteByte((u8)((nSector)>>16)); //发送24bit地址    
    SPI2_ReadWriteByte((u8)((nSector)>>8));   
    SPI2_ReadWriteByte((u8)nSector);   
	W25QXX_CS=1;

	W25QXX_Wait_Busy();					   //等待写入结束
	W25QXX_Write_Disable() ;
    return W25QXX_ReadSR(3);//2020.3.13
	
} 


/********************************************************************
函数功能：往FLASH中写一个扇区（FLASH_SECTOR_SIZE字节）。
入口参数：Addr: 字节地址；pBuf：保存数据的缓冲区；Remain：预计接下来还需要写多少扇区
返    回：写入的状态。0：成功。非0：失败。
备    注：当Remain不为0时，当前页以及该块内剩余部分将不会回写！
          如果数据传输结束，应该将Remain置0，将数据写回。
********************************************************************/
/*uint32 FlashWriteOneSector(uint32 Addr, uint8 * pBuf, uint32 Remain)
{
 uint32 i;
 uint32 SwapPageAddr;
 
 //FlashClrCe(); //选中芯片
 if(Addr>FLASH_MAX_SECTOR_ADDR)return 1; //如果地址超出范围，则返回失败代码1，越界
 Addr=FlashAddrRemap(Addr); //重新影射地址
 if((Addr&(~(FLASH_PAGE_SIZE-1)))!=(FlashCurrentWriteSectorAddr&(~(FLASH_PAGE_SIZE-1)))) //如果跨page
 {
	 if(FlashNeedWriteBack) //如果前面写了数据，则需要将当前读出的page写回
	  {
	    if(W25QXX_ReadSR(3)&0x08) //写入失败
	   {
	    Addr=FlashDealBadBlock(Addr-FLASH_PAGE_SIZE,3)+FLASH_PAGE_SIZE;  //坏块处理
	   }
	  }
	  if((Addr&(~(FLASH_BLOCK_SIZE-1)))!=(FlashCurrentWriteSectorAddr&(~(FLASH_BLOCK_SIZE-1))))  //如果跨block，则需要擦除新的块，
	  {
	   //在擦除之前，要先将原来的块复制到交换区，并且将该块前面部分数据写回
	   //该函数除了将整块数据复制到交换区以外，并且还将擦除掉原来的块，然后将前面部分复制回原来的块
	   Addr=FlashCopyBlockToSwap(Addr);
	  }
	  //从交换区中读出对应的一页,复制命令
	  W25QXX_Read(W25QXX_BUFFER, FlashGetCurrentSwapBlock()+(Addr&(FLASH_BLOCK_SIZE-1)),2112);  
	  W25QXX_Write_Page(W25QXX_BUFFER,Addr,2112);
	  
	  W25QXX_Write_Page(pBuf,Addr,2112);
	  FlashNeedWriteBack=1; //需要写回
 }
 else  //没有超过一页地址，则直接写数据
 {
  //随机写
  FlashWriteCommand(0x85);
  FlashWriteAddr2Byte(Addr);

  for(i=0;i<FLASH_SECTOR_SIZE;i++)
  {
   FlashWriteByte(pBuf[i]);
  }

  FlashNeedWriteBack=1; //需要写回
 }
 FlashCurrentWriteSectorAddr=Addr; //保存本次地址 
 if(Remain==0) //剩余扇区数为0，不会再写了，需要写回
 {
  if(FlashNeedWriteBack) //如果前面写了数据，则需要将当前读出的page写回
  {
   if(W25QXX_ReadSR(3)&0x08) //写入失败
   {
    Addr=FlashDealBadBlock(Addr,3);  //坏块处理
   }
  }
  //计算剩余页数
  Remain=(((Addr+FLASH_BLOCK_SIZE)&(~(FLASH_BLOCK_SIZE-1)))-(Addr&(~(FLASH_PAGE_SIZE-1))))/FLASH_PAGE_SIZE-1;
  //计算在交换块中的起始页地址
  SwapPageAddr=FlashGetCurrentSwapBlock()+(Addr&(FLASH_BLOCK_SIZE-1));
  
  for(i=0;i<Remain;i++)  //将该块内保存在交换块中剩余部分页的数据复制回该块
  {
   Addr+=FLASH_PAGE_SIZE;   //从下一页开始写
   SwapPageAddr+=FLASH_PAGE_SIZE;   
   if(0x01==(FlashCopyPage(SwapPageAddr,Addr)&0x01)) //如果复制失败
   {
    Addr=FlashDealBadBlock(Addr,2);  //处理坏块
   }
  }
  FlashNeedWriteBack=0; //清除需要写回标志
  FlashCurrentWriteSectorAddr=-1;
 }
 FlashSetCe(); //释放FLASH芯片
 return 0;
}
*/





//无检验写SPI FLASH 
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能 
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25QXX_Write_NoCheck(u8* pBuffer,u32 nSector,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=2112-nSector%2112; //单页剩余的字节数		 	    
	if(NumByteToWrite<=pageremain)
		pageremain=NumByteToWrite;//不大于2112字节
	while(1)
	{	   
		W25QXX_Write_Page(pBuffer,nSector,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			nSector+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入了的字节数
			if(NumByteToWrite>2112)pageremain=2112; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	};	    
} 
//写SPI FLASH  
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(24bit)						
//NumByteToWrite:要写入的字节数(最大65535)   
	 
void W25QXX_Write(u8* pBuffer,u32 nSector,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25QXX_BUFFER;	     
 	secpos=nSector/(4096*16*2);//扇区地址  
	secoff=nSector%(4096*16*2);//在扇区内的偏移
	secremain=4096*16*2-secoff;//扇区剩余空间大小   
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)
		secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25QXX_Read(W25QXX_BUF,secpos*4096*16*2,4096*16*2);//读出整个扇区的内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除  	  
		}
		if(i<secremain)//需要擦除
		{
			W25QXX_Erase_Sector(secpos);//擦除这个扇区
			for(i=0;i<secremain;i++)	   //复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25QXX_Write_NoCheck(W25QXX_BUF,secpos*4096*16*2,4096*16*2);//写入整个扇区  

		}else W25QXX_Write_NoCheck(pBuffer,nSector,secremain);//写已经擦除了的,直接写入扇区剩余区间. 				   
		if(NumByteToWrite==secremain)break;//写入结束了
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 	 

		   	pBuffer+=secremain;  //指针偏移
			nSector+=secremain;//写地址偏移	   
		   	NumByteToWrite-=secremain;				//字节数递减
			if(NumByteToWrite>4096*16*2)secremain=4096*16*2;	//下一个扇区还是写不完
			else secremain=NumByteToWrite;			//下一个扇区可以写完了
		}	 
	};	 
}

