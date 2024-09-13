/*
*/

#include "APT\APT.h"

#include "includes.h"


#define IVC102_INTEGRATOR         0
#define IVC102_CLEAR              1

#define   DATA_NORMAL             0x06
#define   DATA_OVER               0x05
#define   DATA_ERRO               0x09
#define   DATA_TIME               0x04


#define CHANNEL0        0x00
#define CHANNEL1        0x01



/*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 2014/2/6
* EditDate       : 2014/2/6
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef OS_VERSION
OS_EVENT      *pAptSem;   //
OS_STK        AptTaskStk[APT_TASK_STK_SIZE];
#endif

void TaskApt(void *pdata)
{
#ifdef OS_VERSION
    INT8U err;
	  pAptSem        = OSSemCreate(0);//
	  while( mSystemData.mSystemInitlFlag ==0 );
#endif
//......APT Test...............................
    //AptMeasure(  &err);
//..............................................	
    while(1)
    {
//..............................................
#ifdef OS_VERSION
        OSSemPend(pAptSem,200,&err);//
        if(err == OS_ERR_NONE)
        {				
					   //AptAdjustMeasure(&err);
					   
					   if( mSystemData.mAptMeasureFlag ==1)        AptMeasure(&err);	       //开始数据采集
						 else if( mSystemData.mAptMeasureFlag ==0)   AptAdjustMeasure(&err);	 //校准
             OSSemSet(	pAptSem,0,&err);				
        }
#endif
//..............................................
    }

}
/*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 2014/2/6
* EditDate       : 2014/2/6
* Description    :
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef OS_VERSION
void TaskAptCreate(void)
{
    INT8U os_err;
//..
    os_err = OSTaskCreateExt( (void (*)(void *))TaskApt,
                              (void          * ) 0,
                              (OS_STK        * )&AptTaskStk[APT_TASK_STK_SIZE - 1],
                              (INT8U           ) APT_TASK_START_PRIO,
                              (INT16U          ) APT_TASK_START_PRIO,
                              (OS_STK        * )&AptTaskStk[0],
                              (INT32U          ) APT_TASK_STK_SIZE,
                              (void          * )0,
                              (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(APT_TASK_START_PRIO, (CPU_INT08U *)"TaskApt", &os_err);
#endif
//..............................................................
}
#endif
/*******************************************************************************
* Function Name  : 
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AptMeasureTimeCountIntial(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
//.. 	
    //NVIC_SetPriorityGrouping(7); /* 0 bits for pre-emption priority 4 bits forsubpriority*/
    NVIC_SetPriority(TIM2_IRQn, 0x03); /* 0x01 = 0x0 << 3 | (0x1 & 0x7), prority is 0x01 << 4 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);	  
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
//....  72mhz/(71+1) = 1us   1ms定时
    TIM_TimeBaseStructure.TIM_Period        = 1000;//60,000:60ms
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
* Function Name  : 
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void AptMeasureTimeCountStart(void)
{
//..清零定时2的计数器	
	 TIM_Cmd(TIM2, DISABLE);             //关闭定时器
	 TIM_ClearFlag(TIM2, TIM_IT_Update);         //清楚定时2的中断标记
   TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //清楚定时2的中断标记
   TIM_SetCounter(TIM2, 0x00); 
   //TIM_GetCounter
	 mSystemData.mAptMeasureTime = 0;

   TIM_Cmd(TIM2, ENABLE);	
}
/*******************************************************************************
* Function Name  : 
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM2_IRQHandler(void)
{
	
//..定时1ms	
	 if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
   {
		   mSystemData.mAptMeasureTime++;
		   TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
	 }
 }

 /*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 01/27/2015
* EditDate       : 01/27/2015
* Description    : 
									 S2:清零 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetIVC102State(unsigned char mState)
{
   if( mState == IVC102_INTEGRATOR)	
	 {//S1 = 1;S2=1 
		  //GPIO_SetBits(GPIOB,   GPIO_Pin_3);
		 GPIO_SetBits(GPIOB,   GPIO_Pin_7);
   }//if( mState == IVC102_INTEGRATOR)
	 else  if( mState == IVC102_CLEAR)	
	 {//S1= 1;S2=0
		  //GPIO_ResetBits(GPIOB, GPIO_Pin_3);
		  GPIO_ResetBits(GPIOB, GPIO_Pin_7);
   }//else  if( mState == IVC102_CLEAR)		 
}
/*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 01/27/2015
* EditDate       : 01/27/2015
* Description    : 
                   S1:积分 PC2
									 S2:清零 PA0 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void IVC102Inital(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;

   //PB7:积分及输出清零  PB6保持高电平
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE);
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); 
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);
			   	
	 GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
   GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	 GPIO_SetBits(GPIOB,   GPIO_Pin_6);
	
	 SetIVC102State(IVC102_CLEAR);
//..	
}
 /*******************************************************************************
* Function Name  : Apt
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
unsigned short AptCollection(unsigned char *erro)
{
	  unsigned short  mAdvalue;
	  mAdvalue =  ADSPI3collectionADSignal(CHANNEL0,erro);	 	 			
		return mAdvalue;
}
 /*******************************************************************************
* Function Name  : AptMeasure
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    :
* Input          : None
* Output         : None
* Return         : None
S2:低电平Reset  高电平正常积分
*******************************************************************************/
//0x4100     0x84308   0x84618    0x8e111    0x10A511  
//0x10A311   0x19A318  0x18E31A   0x212922   0x224929
extern CUartCom       mAptUartComTx; 
unsigned short AptMeasure(unsigned char *pErro)
{
//......................................................................	
	  unsigned int   mTimeBuf,i,mTimeBuf2;
	  unsigned short mAdValue,mAdValueBuf,mAdValueStart,mTxLength; 
	  
	  unsigned char  os_err;
//......................................................................	
    mAptUartComTx.Cmd              = 0x3A;	
	  mAptUartComTx.CharacterWord[0] = 0x54;
	  mAptUartComTx.CharacterWord[1] = 0x43;
	  mAptUartComTx.CharacterWord[2] = 0x4e;
//..   
	  mAptUartComTx.DataLength        = 7;
	  mAptUartComTx.DataBuf[0]        = 0xff;	
	  mAptUartComTx.DataBuf[1]        = 0; 
	  mAptUartComTx.DataBuf[2]        = 0; 
	  mAptUartComTx.DataBuf[3]        = 0; 
	  mAptUartComTx.DataBuf[4]        = 0; 
	  mAptUartComTx.DataBuf[5]        = 0; 
	  mAptUartComTx.DataBuf[6]        = 0; 
	
	  mAptUartComTx.DataBuf[7]        = PdcCheck( (unsigned char*)(&mAptUartComTx), mAptUartComTx.DataLength + 5);	
	  mTxLength = mAptUartComTx.DataLength + 6;
//...........................................................................	 	 
	  SetIVC102State(IVC102_CLEAR);
	  OSTimeDly(10);	  
//采集0信号
	  mAdValueStart = AptCollection(pErro);
    if( *pErro != DATA_NORMAL )
		{		
			 //-------------------发送过程数据---------------------------------------------------------------
				mAptUartComTx.DataBuf[0]        = 2;	//0:最终结果   1:过程数据  其它:错误
			  mAptUartComTx.DataBuf[1]        = (unsigned char)( (mTimeBuf>>24)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[2]        = (unsigned char)( (mTimeBuf>>16)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[3]        = (unsigned char)( (mTimeBuf>>8)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[4]        = (unsigned char)(mTimeBuf &0xff);    //时间ms低位			
	      mAptUartComTx.DataBuf[5]        = (unsigned char)(mAdValueBuf>>8);    //AD高位
	      mAptUartComTx.DataBuf[6]        = (unsigned char)(mAdValueBuf &0xff); //AD低位
			
	      mAptUartComTx.DataBuf[7]        = PdcCheck( (unsigned char*)(&mAptUartComTx), mAptUartComTx.DataLength + 5);	
//				OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
//	      memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
//	      Uart1TxdDataLenght = mTxLength;
//	     // OSSemPost(pComTxSem);//发送数据 
			  return 0;
		}
//积分	 			
	  AptMeasureTimeCountStart();                 //采集数据开始   mAds1253.mConut2 = OSTimeGet();
	  SetIVC102State(IVC102_INTEGRATOR);          
	  mTimeBuf2 = OSTimeGet();
	  for(i=0;i<16;i++)
	  {//测量15S
			  if( OSTimeGet() <  (mTimeBuf2 + 1000) )
				{//mTimeBuf2:上次时间  则:  (mSystemData.mAptMeasureTime - mTimeBuf2) <100
					  OSTimeDly(1000 + mTimeBuf2 - OSTimeGet() );
				}
			  mTimeBuf2   = OSTimeGet();
				
        mTimeBuf    = mSystemData.mAptMeasureTime; 			
			  mAdValueBuf = AptCollection(pErro);				
				mSystemData.mAptMeasureTimeList[i] = mTimeBuf;  
				mSystemData.mAptMeasureAdList[i]   = mAdValueBuf;
				mSystemData.mAptMeasureLable       = i; 
			  if( *pErro != DATA_NORMAL ) break;				
			  if( mAdValueBuf >= 50000 )
				{//信号Ok,返回
					  TIM_Cmd(TIM2, DISABLE);	
					  break;
				}	
//-------------------发送过程数据---------------------------------------------------------------
				mAptUartComTx.DataBuf[0]        = 1;	//0:最终结果   1:过程数据  其它:错误
			  mAptUartComTx.DataBuf[1]        = (unsigned char)( (mTimeBuf>>24)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[2]        = (unsigned char)( (mTimeBuf>>16)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[3]        = (unsigned char)( (mTimeBuf>>8)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[4]        = (unsigned char)(mTimeBuf &0xff);    //时间ms低位			
	      mAptUartComTx.DataBuf[5]        = (unsigned char)(mAdValueBuf>>8);    //AD高位
	      mAptUartComTx.DataBuf[6]        = (unsigned char)(mAdValueBuf &0xff); //AD低位
				
	      mAptUartComTx.DataBuf[7]        = PdcCheck( (unsigned char*)(&mAptUartComTx), mAptUartComTx.DataLength + 5);	
//				OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
//	      memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
//	      Uart1TxdDataLenght = mTxLength;
//	     // OSSemPost(pComTxSem);//发送数据 
//-----------------------------------------------------------------------------------------------				
		}//for(i=0;i<150;i++)
		if( i>=15 )
		{//15S:没有超限
			  mTimeBuf    = mSystemData.mAptMeasureTime; 			
			  mAdValueBuf = AptCollection(pErro);
		}
	  
		SetIVC102State(IVC102_CLEAR); 
		TIM_Cmd(TIM2, DISABLE);	
		
    if( *pErro != DATA_NORMAL ) 
		{		
			  mAptUartComTx.DataBuf[0]        = *pErro;
		}
		else
		{
			  mAptUartComTx.DataBuf[0]        = 0;	//0:最终结果   1:过程数据  其它:错误
		}
//..返回结果
    if( mAdValueBuf > mAdValueStart )		
			    mAdValue         =  mAdValueBuf - mAdValueStart;		 
    else 	mAdValue=0 ;	
//..	
//-------------------发送过程数据---------------------------------------------------------------
				mAptUartComTx.DataBuf[0]        = 0;	//0:最终结果   1:过程数据  其它:错误	      
			  mAptUartComTx.DataBuf[1]        = (unsigned char)( (mTimeBuf>>24)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[2]        = (unsigned char)( (mTimeBuf>>16)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[3]        = (unsigned char)( (mTimeBuf>>8)&0xff );//时间ms高位  
	      mAptUartComTx.DataBuf[4]        = (unsigned char)(mTimeBuf &0xff);    //时间ms低位			
	      mAptUartComTx.DataBuf[5]        = (unsigned char)(mAdValueBuf>>8);    //AD高位
	      mAptUartComTx.DataBuf[6]        = (unsigned char)(mAdValueBuf &0xff); //AD低位
		
	      mAptUartComTx.DataBuf[7]        = PdcCheck( (unsigned char*)(&mAptUartComTx), mAptUartComTx.DataLength + 5);	
//				OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
//	      memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
//	      Uart1TxdDataLenght = mTxLength;
//	     // OSSemPost(pComTxSem);//发送数据 
//-----------------------------------------------------------------------------------------------				
	  return mAdValue;
}

unsigned short AptAdjustMeasure(unsigned char *pErro)
{
//......................................................................	
	  unsigned int   mTimeBuf,i,mTimeBuf2;
	  unsigned short mAdValue,mAdValueBuf,mAdValueStart,mTxLength; 
	  CUartCom       mUartComTx; 
	  unsigned char  os_err;
//......................................................................	
    mUartComTx.Cmd              = 0x31;	
	  mUartComTx.CharacterWord[0] = 0x54;
	  mUartComTx.CharacterWord[1] = 0x43;
	  mUartComTx.CharacterWord[2] = 0x4e;
//..   
	 mUartComTx.DataLength        = 7;
	 mUartComTx.DataBuf[0]        = 0;	
	 mUartComTx.DataBuf[1]        = 0; 
	 mUartComTx.DataBuf[2]        = 0; 
	 mUartComTx.DataBuf[3]        = 0; 
	 mUartComTx.DataBuf[4]        = 0; 
	 mUartComTx.DataBuf[5]        = 0; 
	 mUartComTx.DataBuf[6]        = 0; 
	
	 mUartComTx.DataBuf[7]        = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 5);	
	 mTxLength = mUartComTx.DataLength + 6;
//...........................................................................	 	 
	  SetIVC102State(IVC102_CLEAR);
	  OSTimeDly(10);	  
//采集0信号
	  mAdValueStart = AptCollection(pErro);
    if( *pErro != DATA_NORMAL )
		{		
			 //-------------------发送过程数据---------------------------------------------------------------
				mUartComTx.DataBuf[0]        = 0xff;	//0:最终结果   1:过程数据  其它:错误
			  mUartComTx.DataBuf[1]        = (unsigned char)( (mTimeBuf>>24)&0xff );//时间ms高位  
	      mUartComTx.DataBuf[2]        = (unsigned char)( (mTimeBuf>>16)&0xff );//时间ms高位  
	      mUartComTx.DataBuf[3]        = (unsigned char)( (mTimeBuf>>8)&0xff );//时间ms高位  
	      mUartComTx.DataBuf[4]        = (unsigned char)(mTimeBuf &0xff);    //时间ms低位			
	      mUartComTx.DataBuf[5]        = (unsigned char)(mAdValueBuf>>8);    //AD高位
	      mUartComTx.DataBuf[6]        = (unsigned char)(mAdValueBuf &0xff); //AD低位
			
	      mUartComTx.DataBuf[7]        = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 5);	
				OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
	      memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
	      Uart1TxdDataLenght = mTxLength;
	      OSSemPost(pComTxSem);//发送数据 
			  return 0;
		}
//积分	 			
	  AptMeasureTimeCountStart();                 //采集数据开始   mAds1253.mConut2 = OSTimeGet();
	  SetIVC102State(IVC102_INTEGRATOR);          
	  mTimeBuf2 = OSTimeGet();
	  for(i=0;i<17;i++)
	  {//测量15S
			  if( OSTimeGet() <  (mTimeBuf2 + 1000) )
				{//mTimeBuf2:上次时间  则:  (mSystemData.mAptMeasureTime - mTimeBuf2) <100
					  OSTimeDly(1000 + mTimeBuf2 - OSTimeGet() );
				}
			  mTimeBuf2   = OSTimeGet();
				
        mTimeBuf    = mSystemData.mAptMeasureTime; 			
			  mAdValueBuf = AptCollection(pErro);				
				mSystemData.mAptMeasureTimeList[i] = mTimeBuf;  
				mSystemData.mAptMeasureAdList[i]   = mAdValueBuf;
				mSystemData.mAptMeasureLable       = i; 
			  if( *pErro != DATA_NORMAL ) break;							  
//-------------------发送过程数据---------------------------------------------------------------
				mUartComTx.DataBuf[0]        = i;	//0:最终结果   1:过程数据  其它:错误
			  mUartComTx.DataBuf[1]        = (unsigned char)( (mTimeBuf>>24)&0xff );//时间ms高位  
	      mUartComTx.DataBuf[2]        = (unsigned char)( (mTimeBuf>>16)&0xff );//时间ms高位  
	      mUartComTx.DataBuf[3]        = (unsigned char)( (mTimeBuf>>8)&0xff );//时间ms高位  
	      mUartComTx.DataBuf[4]        = (unsigned char)(mTimeBuf &0xff);    //时间ms低位			
	      mUartComTx.DataBuf[5]        = (unsigned char)(mAdValueBuf>>8);    //AD高位
	      mUartComTx.DataBuf[6]        = (unsigned char)(mAdValueBuf &0xff); //AD低位
				
	      mUartComTx.DataBuf[7]        = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 5);	
				OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
	      memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
	      Uart1TxdDataLenght = mTxLength;
	      OSSemPost(pComTxSem);//发送数据 
//-----------------------------------------------------------------------------------------------				
		}//for(i=0;i<150;i++)
//-----------------------------------------------------------------------------------------------				
		SetIVC102State(IVC102_CLEAR); 
		TIM_Cmd(TIM2, DISABLE);	
		
	  return mAdValue;
}
//$
