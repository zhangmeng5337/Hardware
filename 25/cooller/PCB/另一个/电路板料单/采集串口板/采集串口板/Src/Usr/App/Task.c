/******************** (C) COPYRIGHT 2012 WQS************** ********************
* File Name          : task.c		  
* Author             : wqs
* Version            : V1.01
* Date               : 05/28/2012
* Description        : 
********************************************************************************
* 
*******************************************************************************/

#include "App\task.h"
#include "includes.h"

/*******************************************************************************
* Function Name  : RunTickIntial
* Author         : wqs
* WriteDate      : 01/10/2016
* EditDate       : 01/10/2016
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  RunTickIntial(void)
{   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	  GPIO_InitTypeDef GPIO_InitStructure;
//PA2Pa3 Run-led   Relay PC13-PC14
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC , ENABLE);
	
//...
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);	
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOA,GPIO_Pin_2);
	 GPIO_ResetBits(GPIOA,GPIO_Pin_3);
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC , ENABLE);	
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13 | GPIO_Pin_14;
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOC, &GPIO_InitStructure);	
	 GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	 GPIO_ResetBits(GPIOC,GPIO_Pin_14);
	
//.. 	
    //NVIC_SetPriorityGrouping(7); /* 0 bits for pre-emption priority 4 bits forsubpriority*/
    NVIC_SetPriority(TIM2_IRQn, 0x03); /* 0x01 = 0x0 << 3 | (0x1 & 0x7), prority is 0x01 << 4 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	  
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//.... 72mhz/(71+1) = 1us    1us * 40000 = 40ms
    TIM_TimeBaseStructure.TIM_Period        = 40000-1;//1000:40ms
    TIM_TimeBaseStructure.TIM_Prescaler     = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_ARRPreloadConfig(TIM2, ENABLE);
    TIM_ITConfig(TIM2,TIM_IT_Update, ENABLE);
    /* TIM enable counter */
    //TIM_Cmd(TIM2, ENABLE);
	  
}
/*******************************************************************************
* Function Name  : RunTickIntial
* Author         : wqs
* WriteDate      : 01/10/2016
* EditDate       : 01/10/2016
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned char ReadDataFromSd(unsigned int mReadDataPoint)
{
	 unsigned short mLedNumber,i,j,mRealyNumber;
	 unsigned char  mRelayVaule;
	 unsigned char  *pRelayVlue;
	 unsigned char  *pLedData;
	 unsigned char  *pRelayData;	 
	 unsigned int   ReadNumber;
//..
	 if( (mReadDataPoint&0x01) == 0 ) 
	 {
		   pLedData   = mOneWire.mOneWireDataBuf1;
		   pRelayData = mOneWire.mOneWireRelayBuf1;
		   pRelayVlue = mOneWire.mOneWireRelayValue1;
   }
	 else
   {
		  pLedData    = mOneWire.mOneWireDataBuf2;
		  pRelayData  = mOneWire.mOneWireRelayBuf2;
		  pRelayVlue  = mOneWire.mOneWireRelayValue2;
   }		 
//ReadBufSD	 mOneWire.mOneWireLedData
	f_lseek(&mFileHandle,(mReadDataPoint + 1)<<9 );//移动到 文件头
  f_read(&mFileHandle,mOneWire.mOneWireLedData,512, &ReadNumber);		 
//..
	 for(mLedNumber=0; mLedNumber < ONE_WIRE_LED_NUMBER; mLedNumber++)
	 {
		   for(i=0;i<8;i++) //Read-LED
		      *pLedData++  = Ws2811Tab[mOneWire.mOneWireLedData[mLedNumber]][i];//
   }//for(mLedNumber=0,j=0; mLedNumber < ONE_WIRE_LED_NUMBER; mLedNumber++)
//..  mOneWire.mOneWireLedData[507]......mOneWire.mOneWireLedData[511]                                     
	 for(j=507;j<=511;j++) 
	 {
		  mRelayVaule = mOneWire.mOneWireLedData[j];
		  *pRelayVlue++ = mRelayVaule;
		  for(mRealyNumber=0;mRealyNumber < 8; mRealyNumber++)
		  {//
				  if( (mRelayVaule & 0x01) == 0x00 ) 
					      memset(pRelayData,Ws2811Tab[0x00][0],8);
					else  memset(pRelayData,Ws2811Tab[0xff][0],8);
          pRelayData = pRelayData +8;					
					mRelayVaule = mRelayVaule >>1; 
      }//for(mRealyNumber=0;mRealyNumber < 8; mRealyNumber++)
   }//for(j=507;j<=511;j++) 
	 
	 mSystemData.mSdReadState = 1;
//..
	 return 0;
}
/*******************************************************************************
* Function Name  : SetRelay
* Author         : wqs
* WriteDate      : 01/10/2016
* EditDate       : 01/10/2016
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  SetRelay(unsigned char *pRelayData)
{//PC13 PC14
	 unsigned char mRelayVlue;
	
	 

///...	
	 mRelayVlue = *pRelayData;
	 if( (mRelayVlue & 0x01) == 0x00)
	      GPIO_ResetBits(GPIOC,GPIO_Pin_13);
	 else GPIO_SetBits(GPIOC,GPIO_Pin_13);
	 
	 if( (mRelayVlue & 0x02) == 0x00)
	      GPIO_ResetBits(GPIOC,GPIO_Pin_14);
	 else GPIO_SetBits(GPIOC,GPIO_Pin_14);
}
/*******************************************************************************
* Function Name  : TaskReadFlash
* Author         : wqs
* WriteDate      : 01/10/2016
* EditDate       : 01/10/2016
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TaskReadFlash(void *pdata)
{
    INT8U err;
	  pReadFlashSem     = OSSemCreate(0);
	 while( mSystemData.mSystemInitlFlag == 0 );
//.........................................	
    while(1)
    {
        OSSemPend(pReadFlashSem,0,&err);//
        if(err == OS_NO_ERR)	
				{			          					 
					 if( mOneWire.mOneReadDataPoint < mOneWire.mRunCount )	  
					 {	 
              ReadDataFromSd(mOneWire.mOneReadDataPoint);
					    mOneWire.mOneReadDataPoint++;
					 }//if( mOneWire.mOneReadDataPoint < mOneWire.mRunCount )	  
        }//if(err == OS_NO_ERR)						
    }//while(1)	
}
/*******************************************************************************
* Function Name  : RunTickIntial
* Author         : wqs
* WriteDate      : 01/10/2016
* EditDate       : 01/10/2016
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void PreRunTick(void)
{
//..	
	 mOneWire.mRunPoint = 0;
	 mOneWire.mOneReadDataPoint = 0;
	
//..清零定时2的计数器	
	 TIM_Cmd(TIM2, DISABLE);             //关闭定时器 
	 TIM_ClearFlag(TIM2, TIM_IT_Update);         //清楚定时2的中断标记
   TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清楚定时2的中断标记
   TIM_SetCounter(TIM2, 0x00); 
	 
//..清零定时3的计数器	
	 TIM_Cmd(TIM3, DISABLE);
	 TIM_ClearFlag(TIM3, TIM_IT_Update);         //清楚定时2的中断标记
   TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //清楚定时2的中断标记
   TIM_SetCounter(TIM3, 0x00); 
	              //关闭定时器	
//..	
	 DMA_Cmd(DMA1_Channel2, DISABLE);    //关闭当前LED通信
	 DMA_ClearITPendingBit(DMA1_FLAG_TC2);       //清除
	 TIM3->CCR3 = 0;	
//..	
	 DMA_Cmd(DMA1_Channel7, DISABLE);    //关闭当前Relay通信
	 DMA_ClearITPendingBit(DMA1_FLAG_TC7);       //清除
	 TIM4->CCR4 = 0;
	
	
	 
	
	                 
}
/*******************************************************************************
* Function Name  : RunTick
* Author         : wqs
* WriteDate      : 01/10/2016
* EditDate       : 01/10/2016
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void  ReStartRunDevice(void)
{
	 unsigned char mSdState;
//..	
   PreRunTick();
	 //Read Data,unsign
	 if( mSystemData.mSdMountState != DEVICE_OK )
	 {
		  mSdState = ReadFileFromSD();
		  if( mSdState != DEVICE_OK)
			{//
				 GPIO_SetBits(GPIOA,GPIO_Pin_2);
				 return;
      }//if( mSdState != DEVICE_OK)
   }//if( mSystemData.mSdMountState != DEVICE_OK )	 
		 
	 mSystemData.mSdReadState = 0;
   ReadDataFromSd(mOneWire.mOneReadDataPoint);
	 mOneWire.mOneReadDataPoint++;
	 OSSemPost(pReadFlashSem);//读取第二个
	
		 
//..	
	 
	 
	 TIM_Cmd(TIM2, ENABLE);
	 
	 DMA1_Channel2->CMAR  = mOneWire.mOneWireData1Addr;
	 DMA1_Channel2->CNDTR = ONE_WIRE_DMA_LEN;		       
	 DMA_Cmd(DMA1_Channel2, ENABLE);    //DMA启动	 
	 TIM_DMACmd(TIM3,TIM_DMA_CC3, ENABLE);
   TIM_Cmd(TIM3, ENABLE); 	 
	 // 
	 DMA1_Channel7->CMAR  = (uint32_t)(&mOneWire.mOneWireRelayBuf1);
	 DMA1_Channel7->CNDTR = ONE_WIRE_RELAY_DMA_LEN;		       	 
	 DMA_Cmd(DMA1_Channel7, ENABLE);        //DMA启动
	 TIM_DMACmd(TIM4,TIM_DMA_CC4, ENABLE);	// 
	
	 //GPIOA->ODR = GPIOA->ODR ^ GPIO_Pin_3;
	 GPIO_SetBits(GPIOA,GPIO_Pin_3);
	 mOneWire.mRunFlag = 1;
	 
}

//&
