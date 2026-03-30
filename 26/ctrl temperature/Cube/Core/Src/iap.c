////#include "sys.h"
//#include "delay.h"
//#include "usart.h"
#include "stmflash.h"
#include "iap.h" 
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//IAP 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2016/1/18
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 

iapfun jump2app; 
uint32_t iapbuf[512]; 	//2K字节缓存  
uint32_t Startup_Update[2];
//appxaddr:应用程序的起始地址
//appbuf:应用程序CODE.
//appsize:应用程序大小(字节).

__asm void MSR_MSP(uint32_t addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}


void iap_write_appbin(uint32_t appxaddr,uint8_t *appbuf,uint32_t appsize)
{
	uint32_t t;
	uint16_t i=0;
	uint32_t temp;
	uint32_t fwaddr=appxaddr;//当前写入的地址
	uint8_t *dfu=appbuf;
	for(t=0;t<appsize;t+=4)
	{						   
		temp=(uint32_t)dfu[3]<<24;   
		temp|=(uint32_t)dfu[2]<<16;    
		temp|=(uint32_t)dfu[1]<<8;
		temp|=(uint32_t)dfu[0];	  
		dfu+=4;//偏移4个字节
		iapbuf[i++]=temp;	    
		if(i==512)
		{
			i=0; 
			STMFLASH_Write(fwaddr,iapbuf,512);
			fwaddr+=2048;//偏移2048  512*4=2048
		} 
	} 
	if(i)STMFLASH_Write(fwaddr,iapbuf,i);//将最后的一些内容字节写进去.  
} //0x08010FF0

//跳转到应用程序段
//appxaddr:用户代码起始地址.
void iap_load_app(uint32_t appxaddr)
{ 
	if(((*(__IO uint32_t*)appxaddr)&0x2FFE0000)==0x20000000)	//检查栈顶地址是否合法.
	{ 
		jump2app=(iapfun)*(__IO uint32_t*)(appxaddr+4);		//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(__IO uint32_t*)appxaddr);					//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									//跳转到APP.
	}
}		 




void SoftReset(void)

{ 
	__set_FAULTMASK(1); // 关闭所有中端

	NVIC_SystemReset(); // 复位

}
/* USER CODE END 1 */
//void getSTM32SeriesID(void)
//{
//	uint32_t UIDw[3];
////  uint32_t HalVersion  = HAL_GetHalVersion();
////  uint32_t REVID       = HAL_GetREVID();
////  uint32_t DEVID       = HAL_GetDEVID();
//  UIDw[0]              = HAL_GetUIDw0();
//  UIDw[1]              = HAL_GetUIDw1();
//  UIDw[2]              = HAL_GetUIDw2();
////	printf("\r\n芯片的HAL库版本号为: %d\r\n",HalVersion);
////	printf("\r\n芯片修订标识符为: %d\r\n",REVID);
////	printf("\r\n芯片标识符为: %d\r\n",DEVID);
//	printf("\r\n芯片的唯一ID为: %08X-%08X-%08X\r\n",UIDw[0],UIDw[1],UIDw[2]);
//}



