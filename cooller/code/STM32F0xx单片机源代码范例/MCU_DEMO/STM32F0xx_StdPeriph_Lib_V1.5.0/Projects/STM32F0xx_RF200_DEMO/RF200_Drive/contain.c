#include "stm32f0xx.h"
#include "contain.h"
#include "delay.h"
#include "usart.h"
#include "RF200_DRV.h"
#include "stdio.h"
#include "time.h"

/*************Single Inventory example *****************/
void contain_test(void)
{
	RCC_HSICmd(ENABLE);
	delay_init();
	TIM3_INIT(100-1,480-1);  //1000us 
	USART1_INIT(115200);	//PA9;PA10; 
	
	delay_ms(200);

	while(1)
	{  
		if(flag1s == 1)
		{
			flag1s = 0; //1s;
			SendCmdFrame(SingleInventCMD);
		}
		USART_RXD_Data_Process();
		RF200_FRAME_RX_HANDLE();		
	}
}
/*************Multi Inventory example *****************/
void contain_test1(void)
{
	RCC_HSICmd(ENABLE);
	delay_init();
	TIM3_INIT(100-1,480-1);  //1000us 
	USART1_INIT(115200);	//PA9;PA10; 
	
	delay_ms(200);
	
	SendCmdFrame(MultiInventCMD);
	
	while(1)
	{  
		USART_RXD_Data_Process();
		RF200_FRAME_RX_HANDLE();		
	}
}
/*************Read/Write User bank example*****************/
uint8_t TagAccessPassword[4]={0x00,0x00,0x00,0x00};
uint8_t TagWriteDATA[8]={0x12,0x34,0x56,0x78,0x78,0x56,0x34,0x16};
void contain_test2(void)
{
	RCC_HSICmd(ENABLE);
	delay_init();
	TIM3_INIT(100-1,480-1);  //1000us 
	USART1_INIT(115200);	//PA9;PA10; 
	
	delay_ms(100);
	SendCmdFrame(StopMultiInventCMD);
	delay_ms(100);
	//set select mode
	SendCmdFrame(SetSelectModeCMD);
	while(1)
	{  
		//first step inventory a tag EPC;
		if((flag1s == 1)&&(RF200Status.InventATagFlag == 0))// no tag
		{
			flag1s = 0; //1s;
			SendCmdFrame(SingleInventCMD);
		}
		//second step write TagWriteDATA array data to the first tag;
		if((RF200Status.InventATagFlag == 1)&&(RF200Status.WriteDataFlag == 0)&&(flag1s == 1))
		{
			TAG_WR_PARA.AccessPW[0]=TagAccessPassword[0]; //ACCESS PASSWORD
			TAG_WR_PARA.AccessPW[1]=TagAccessPassword[1];
			TAG_WR_PARA.AccessPW[2]=TagAccessPassword[2];
			TAG_WR_PARA.AccessPW[3]=TagAccessPassword[3];
			TAG_WR_PARA.MemBank=Membank_USER;
			TAG_WR_PARA.Offset=0;// OFFSET 0 WORD;(2BYTE=1WORD)
			TAG_WR_PARA.len=4;//LENGTH 4 WORDS=8 BYTES;
			TAG_WR_PARA.dat=TagWriteDATA;
			TagWriteProcess(TAG[0].EPC,TAG_WR_PARA);
			flag1s = 0; //1s;
		}
		//third step read the first tag data to TagReadDATA array;
		if((RF200Status.InventATagFlag == 1)&&(RF200Status.WriteDataFlag == 1)&&(flag1s == 1))
		{
			TAG_WR_PARA.AccessPW[0]=TagAccessPassword[0]; //ACCESS PASSWORD
			TAG_WR_PARA.AccessPW[1]=TagAccessPassword[1];
			TAG_WR_PARA.AccessPW[2]=TagAccessPassword[2];
			TAG_WR_PARA.AccessPW[3]=TagAccessPassword[3];
			TAG_WR_PARA.MemBank=Membank_USER;
			TAG_WR_PARA.Offset=0;// OFFSET 0 WORD;(2BYTE=1WORD)
			TAG_WR_PARA.len=4;//LENGTH 4 WORDS=8 BYTES;
			TAG_WR_PARA.dat=TAG_READ_DATA.dat;// data buffer
			TagReadProcess(TAG[0].EPC,TAG_WR_PARA);
			flag1s = 0; //1s;
		}
		//all done and stop running; user can check TAG_READ_DATA struct 
		if((RF200Status.InventATagFlag == 1)&&(EPC_Compare(TAG[0].EPC,TAG_READ_DATA.EPC) == 1)&&(flag1s == 1))
		{
			//check the data in TAG_WR_PARA.dat or TAG_READ_DATA.dat;
			while(1);
		}
		USART_RXD_Data_Process();
		RF200_FRAME_RX_HANDLE();		
	}
}

/*********************************************END OF FILE**********************/


