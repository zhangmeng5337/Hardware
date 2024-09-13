

#include "Food.h"
#include "includes.h"


#define CHANNEL0        0x00
#define CHANNEL1        0x01

#define   DATA_NORMAL             0x06
#define   DATA_OVER               0x05
#define   DATA_ERRO               0x09
#define   DATA_TIME               0x04

#ifdef OS_VERSION
OS_EVENT      *pFood8Sem;   //
OS_EVENT      *pFood16Sem;   //
OS_EVENT      *pFood24Sem;   //

OS_STK        Food8TaskStk[FOOD8_TASK_STK_SIZE];
OS_STK        Food16TaskStk[FOOD16_TASK_STK_SIZE];
OS_STK        Food24TaskStk[FOOD24_TASK_STK_SIZE];
#endif


CFoodData   mFoodData;
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
void TaskFoodCH8(void *pdata)
{
	  unsigned char i,mFlagBits;
	  unsigned short mADValue,mTxLength;
	  CUartCom       mUartComTx;
#ifdef OS_VERSION
    INT8U err;
	  pFood8Sem     = OSSemCreate(0);//
	  while( mSystemData.mSystemInitlFlag ==0 );	
#endif    
    while(1)
    {
//..............................................
#ifdef OS_VERSION
        OSSemPend(pFood8Sem,200,&err);//等待CAN接收数据的信号量
        if(err == OS_ERR_NONE)
        {			
            for(i=0,mFlagBits=1;i<8;i++)					
					  {
							  if(  (mFoodData.mADReadFlag[0] & mFlagBits) ==  mFlagBits )
								{
									  mFoodData.mADReadFlag[0] = mFoodData.mADReadFlag[0] & (~mFlagBits);
									  SetSpiOneADChannel(i);
									  mADValue = ADSPI1collectionADSignal(CHANNEL0,&err);
									
									  mUartComTx.Cmd              = 0x1A; 
	                  mUartComTx.CharacterWord[0] = 0x54;
	                  mUartComTx.CharacterWord[1] = 0x43;
	                  mUartComTx.CharacterWord[2] = 0x4e;
	                  mUartComTx.DataLength       = 4;
									  if(err != DATA_NORMAL )     mUartComTx.DataBuf[0]  = err;
										else                        mUartComTx.DataBuf[0]  = 0;
									  mUartComTx.DataBuf[1]       = i+1;//通道值
	                  mUartComTx.DataBuf[2]       = (unsigned char)(mADValue>>8);	 //高8位
	                  mUartComTx.DataBuf[3]       = (unsigned char)(mADValue&0xff);	 //低8位 
	                  mUartComTx.DataBuf[4]       = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 5);
	
	                  mTxLength = mUartComTx.DataLength + 6;	 
	                  OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&err);	
	                  memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
	                  Uart1TxdDataLenght = mTxLength;
	                  OSSemPost(pComTxSem);//
								}//if(  (mFoodData.mADReadFlag[0] & mFlagBits) ==  mFlagBits )	
								mFlagBits = mFlagBits <<1;
								OSTimeDly(100) ; 
						}//for(i=0,mFlagBits=1;i<8;i++)	
        }//if(err == OS_ERR_NONE)
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
void TaskFoodCH16(void *pdata)
{
	  unsigned char i,mFlagBits;
	  unsigned short mADValue,mTxLength;
	  CUartCom       mUartComTx;	
#ifdef OS_VERSION
    INT8U err;
	  pFood16Sem     = OSSemCreate(0);//
	  while( mSystemData.mSystemInitlFlag ==0 );	  	    
#endif
    //mFoodData.mADReadFlag[1] = 0xff;
    while(1)
    {
//..............................................
#ifdef OS_VERSION
        OSSemPend(pFood16Sem,200,&err);//
        if(err == OS_ERR_NONE)
        {		
            for(i=0,mFlagBits=1;i<8;i++)					
					  {
							  if(  (mFoodData.mADReadFlag[1] & mFlagBits) ==  mFlagBits )
								{
									  mFoodData.mADReadFlag[1] = mFoodData.mADReadFlag[1] & (~mFlagBits);
									  SetSpiTwoADChannel(i);
									  mADValue = ADSPI2collectionADSignal(CHANNEL0,&err);
									
									  mUartComTx.Cmd              = 0x1A; 
	                  mUartComTx.CharacterWord[0] = 0x54;
	                  mUartComTx.CharacterWord[1] = 0x43;
	                  mUartComTx.CharacterWord[2] = 0x4e;
	                  mUartComTx.DataLength       = 4;
									  if(err != DATA_NORMAL )     mUartComTx.DataBuf[0]  = err;
										else                        mUartComTx.DataBuf[0]  = 0;
									  mUartComTx.DataBuf[1]       = i+9;//通道值
	                  mUartComTx.DataBuf[2]       = (unsigned char)(mADValue>>8);	 //高8位
	                  mUartComTx.DataBuf[3]       = (unsigned char)(mADValue&0xff);	 //低8位 
	                  mUartComTx.DataBuf[4]       = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 5);
	
	                  mTxLength = mUartComTx.DataLength + 6;	 
	                  OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&err);	
	                  memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
	                  Uart1TxdDataLenght = mTxLength;
	                  OSSemPost(pComTxSem);//
								}//if(  (mFoodData.mADReadFlag[0] & mFlagBits) ==  mFlagBits )	
								mFlagBits = mFlagBits <<1;
								OSTimeDly(100) ;
						}//for(i=0,mFlagBits=1;i<8;i++)						
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
void TaskFoodCH24(void *pdata)
{
	  unsigned char i,mFlagBits;
	  unsigned short mADValue,mTxLength;
	  CUartCom       mUartComTx;	
#ifdef OS_VERSION
    INT8U err;
	  pFood24Sem     = OSSemCreate(0);//
	  while( mSystemData.mSystemInitlFlag ==0 );	  	    
#endif

    while(1)
    {
//..............................................
#ifdef OS_VERSION
        OSSemPend(pFood24Sem,200,&err);//
        if(err == OS_ERR_NONE)
        {					  					 
            for(i=0,mFlagBits=1;i<8;i++)					
					  {
							  if(  (mFoodData.mADReadFlag[2] & mFlagBits) ==  mFlagBits )
								{
									  mFoodData.mADReadFlag[2] = mFoodData.mADReadFlag[2] & (~mFlagBits);
									  
									  #ifdef FOOD_NUMBER_SPI3
                    SetSpiThreeADChannel(i);
                    #endif
									  mADValue = ADSPI3collectionADSignal(CHANNEL0,&err);
									
									  mUartComTx.Cmd              = 0x1A; 
	                  mUartComTx.CharacterWord[0] = 0x54;
	                  mUartComTx.CharacterWord[1] = 0x43;
	                  mUartComTx.CharacterWord[2] = 0x4e;
	                  mUartComTx.DataLength       = 4;
									  if(err != DATA_NORMAL )     mUartComTx.DataBuf[0]  = err;
										else                        mUartComTx.DataBuf[0]  = 0;
									  mUartComTx.DataBuf[1]       = i+17;//通道值
	                  mUartComTx.DataBuf[2]       = (unsigned char)(mADValue>>8);	 //高8位
	                  mUartComTx.DataBuf[3]       = (unsigned char)(mADValue&0xff);	 //低8位 
	                  mUartComTx.DataBuf[4]       = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 5);
	
	                  mTxLength = mUartComTx.DataLength + 6;	 
	                  OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&err);	
	                  memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
	                  Uart1TxdDataLenght = mTxLength;
	                  OSSemPost(pComTxSem);//
								}//if(  (mFoodData.mADReadFlag[0] & mFlagBits) ==  mFlagBits )	
								mFlagBits = mFlagBits <<1;
						}//for(i=0,mFlagBits=1;i<8;i++)						
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
void TaskFoodCreate(void)
{
    INT8U os_err;
//..
    os_err = OSTaskCreateExt( (void (*)(void *))TaskFoodCH8,
                              (void          * ) 0,
                              (OS_STK        * )&Food8TaskStk[FOOD8_TASK_STK_SIZE - 1],
                              (INT8U           ) FOOD8_TASK_START_PRIO,
                              (INT16U          ) FOOD8_TASK_START_PRIO,
                              (OS_STK        * )&Food8TaskStk[0],
                              (INT32U          ) FOOD8_TASK_STK_SIZE,
                              (void          * )0,
                              (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(FOOD8_TASK_START_PRIO, (CPU_INT08U *)"TaskFoodCH8", &os_err);
#endif
//..............................................................
//..
    os_err = OSTaskCreateExt( (void (*)(void *))TaskFoodCH16,
                              (void          * ) 0,
                              (OS_STK        * )&Food16TaskStk[FOOD16_TASK_STK_SIZE - 1],
                              (INT8U           ) FOOD16_TASK_START_PRIO,
                              (INT16U          ) FOOD16_TASK_START_PRIO,
                              (OS_STK        * )&Food16TaskStk[0],
                              (INT32U          ) FOOD16_TASK_STK_SIZE,
                              (void          * )0,
                              (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(FOOD16_TASK_START_PRIO, (CPU_INT08U *)"TaskFoodCH16", &os_err);
#endif
//..............................................................
//..
    os_err = OSTaskCreateExt( (void (*)(void *))TaskFoodCH24,
                              (void          * ) 0,
                              (OS_STK        * )&Food24TaskStk[FOOD24_TASK_STK_SIZE - 1],
                              (INT8U           ) FOOD24_TASK_START_PRIO,
                              (INT16U          ) FOOD24_TASK_START_PRIO,
                              (OS_STK        * )&Food24TaskStk[0],
                              (INT32U          ) FOOD24_TASK_STK_SIZE,
                              (void          * )0,
                              (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(FOOD24_TASK_START_PRIO, (CPU_INT08U *)"TaskFoodCH24", &os_err);
#endif
//..............................................................															
    													
}
#endif

/*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 01/27/2015
* EditDate       : 01/27/2015
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void FoodIntal(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
//Power1 2 3 PB9 PB1 PC0	
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	  GPIO_Init(GPIOC, &GPIO_InitStructure);
	  SetLedValue(0,0);//0-7关闭
	  SetLedValue(8,0);//8-15关闭
	  SetLedValue(16,0);//16-23关闭
//................................................................................   
#ifdef FOOD_NUMBER_SPI1
//PC4 PC5 PB0 	 POwer
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0;
	  GPIO_Init(GPIOB, &GPIO_InitStructure);	 
#endif
//................................................................................	
#ifdef FOOD_NUMBER_SPI2
//PC6 PC7 PC8
	  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);	
    SetSpiTwoADChannel(7);	  
#endif
//................................................................................	
#ifdef FOOD_NUMBER_SPI3
//PB6 PB7 PB8
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB  | RCC_APB2Periph_GPIOD, ENABLE);
	 	
	  GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
#endif	
//................................................................................	
}
/*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 01/27/2015
* EditDate       : 01/27/2015
* Description    : C[PB0] B[PC5] A[PC4]
                      
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef FOOD_NUMBER_SPI1
void SetSpiOneADChannel(unsigned char mChannel)
{// C B A
//.............CBA:000	 0
	 if( mChannel == 6 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOB, GPIO_Pin_0);//
		   GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		   GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	 }//if( mChannel ==  )
//.............CBA:001	1
	 else if( mChannel == 5 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOB, GPIO_Pin_0);//
		   GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		   GPIO_SetBits(GPIOC, GPIO_Pin_4);
	 }//if( mChannel ==  )	 
//.............CBA:010	2
	 else if( mChannel == 4 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOB, GPIO_Pin_0);//
		   GPIO_SetBits(GPIOC, GPIO_Pin_5);
		   GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	 }//if( mChannel ==  )		
//.............CBA:011	3
	 else if( mChannel == 7 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOB, GPIO_Pin_0);//
		   GPIO_SetBits(GPIOC, GPIO_Pin_5);
		   GPIO_SetBits(GPIOC, GPIO_Pin_4);
	 }//if( mChannel ==  )	
//.............CBA:100	4
	 else if( mChannel == 3 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOB, GPIO_Pin_0);//
		   GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		   GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	 }//if( mChannel ==  )	
//.............CBA:101	5
	 else if( mChannel == 0 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOB, GPIO_Pin_0);//
		   GPIO_ResetBits(GPIOC, GPIO_Pin_5);
		   GPIO_SetBits(GPIOC, GPIO_Pin_4);
	 }//if( mChannel ==  )	
//.............CBA:110	6
	 else if( mChannel == 2 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOB, GPIO_Pin_0);//
		   GPIO_SetBits(GPIOC, GPIO_Pin_5);
		   GPIO_ResetBits(GPIOC, GPIO_Pin_4);
	 }//if( mChannel ==  )	 
//.............CBA:111	6
	 else if( mChannel == 1 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOB, GPIO_Pin_0);//
		   GPIO_SetBits(GPIOC, GPIO_Pin_5);
		   GPIO_SetBits(GPIOC, GPIO_Pin_4);
	 }//if( mChannel ==  )	 	 
}	
#endif

/*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 01/27/2015
* EditDate       : 01/27/2015
* Description    : C[PB4] B[PB3] A[PD2]
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef FOOD_NUMBER_SPI2
void SetSpiTwoADChannel(unsigned char mChannel)
{// C B A
//.............CBA:000	 0
	 if( mChannel == 6 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOC, GPIO_Pin_8);//
		   GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		   GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	 }//if( mChannel ==  )
//.............CBA:001	1
	 else if( mChannel == 5 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOC, GPIO_Pin_8);//
		   GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		   GPIO_SetBits(GPIOC, GPIO_Pin_6);
	 }//if( mChannel ==  )	 
//.............CBA:010	2
	 else if( mChannel == 4 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOC, GPIO_Pin_8);//
		   GPIO_SetBits(GPIOC, GPIO_Pin_7);
		   GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	 }//if( mChannel ==  )		
//.............CBA:011	3
	 else if( mChannel == 7 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOC, GPIO_Pin_8);//
		   GPIO_SetBits(GPIOC, GPIO_Pin_7);
		   GPIO_SetBits(GPIOC, GPIO_Pin_6);
	 }//if( mChannel ==  )	
//.............CBA:100	4
	 else if( mChannel == 3 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOC, GPIO_Pin_8);//
		   GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		   GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	 }//if( mChannel ==  )	
//.............CBA:101	5
	 else if( mChannel == 0 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOC, GPIO_Pin_8);//
		   GPIO_ResetBits(GPIOC, GPIO_Pin_7);
		   GPIO_SetBits(GPIOC, GPIO_Pin_6);
	 }//if( mChannel ==  )	
//.............CBA:110	6
	 else if( mChannel == 2 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOC, GPIO_Pin_8);//
		   GPIO_SetBits(GPIOC, GPIO_Pin_7);
		   GPIO_ResetBits(GPIOC, GPIO_Pin_6);
	 }//if( mChannel ==  )	 
//.............CBA:111	6
	 else if( mChannel == 1 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOC, GPIO_Pin_8);//
		   GPIO_SetBits(GPIOC, GPIO_Pin_7);
		   GPIO_SetBits(GPIOC, GPIO_Pin_6);
	 }//if( mChannel ==  )	 
}	
#endif

/*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 01/27/2015
* EditDate       : 01/27/2015
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef FOOD_NUMBER_SPI3
void SetSpiThreeADChannel(unsigned char mChannel)
{// C B A
//.............CBA:000	 0
	 if( mChannel == 6 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOB, GPIO_Pin_8);//
		   GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		   GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	 }//if( mChannel ==  )
//.............CBA:001	1
	 else if( mChannel == 5 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOB, GPIO_Pin_8);//
		   GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		   GPIO_SetBits(GPIOB, GPIO_Pin_6);
	 }//if( mChannel ==  )	 
//.............CBA:010	2
	 else if( mChannel == 4 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOB, GPIO_Pin_8);//
		   GPIO_SetBits(GPIOB, GPIO_Pin_7);
		   GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	 }//if( mChannel ==  )		
//.............CBA:011	3
	 else if( mChannel == 7 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_ResetBits(GPIOB, GPIO_Pin_8);//
		   GPIO_SetBits(GPIOB, GPIO_Pin_7);
		   GPIO_SetBits(GPIOB, GPIO_Pin_6);
	 }//if( mChannel ==  )	
//.............CBA:100	4
	 else if( mChannel == 3 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOB, GPIO_Pin_8);//
		   GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		   GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	 }//if( mChannel ==  )	
//.............CBA:101	5
	 else if( mChannel == 0 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOB, GPIO_Pin_8);//
		   GPIO_ResetBits(GPIOB, GPIO_Pin_7);
		   GPIO_SetBits(GPIOB, GPIO_Pin_6);
	 }//if( mChannel ==  )	
//.............CBA:110	6
	 else if( mChannel == 2 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOB, GPIO_Pin_8);//
		   GPIO_SetBits(GPIOB, GPIO_Pin_7);
		   GPIO_ResetBits(GPIOB, GPIO_Pin_6);
	 }//if( mChannel ==  )	 
//.............CBA:111	6
	 else if( mChannel == 1 )
	 {//GPIO_ResetBits  GPIO_SetBits
		   GPIO_SetBits(GPIOB, GPIO_Pin_8);//
		   GPIO_SetBits(GPIOB, GPIO_Pin_7);
		   GPIO_SetBits(GPIOB, GPIO_Pin_6);
	 }//if( mChannel ==  )
}	
#endif
/*******************************************************************************
* Function Name  :
* Author         : wqs
* WriteDate      : 01/27/2015
* EditDate       : 01/27/2015
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SetLedValue(unsigned char mChannel,unsigned char mState)
{//PB9 PB1 PC0	
	  if( mState == 1) 
		{//开灯  GPIO_SetBits
			  if(mChannel <= 0x07 )       GPIO_SetBits(GPIOC, GPIO_Pin_0);
			  else if(mChannel <= 0x0f )  GPIO_SetBits(GPIOB, GPIO_Pin_1); 
			  else if(mChannel <= 0x10 )  GPIO_SetBits(GPIOB, GPIO_Pin_9);
		}
		else if( mState == 0) 
		{//关灯
			  if(mChannel <= 0x07 )       GPIO_ResetBits(GPIOC, GPIO_Pin_0);
			  else if(mChannel <= 0x0f )  GPIO_ResetBits(GPIOB, GPIO_Pin_1);
			  else if(mChannel <= 0x10 )  GPIO_ResetBits(GPIOB, GPIO_Pin_9);
		}
}
//$
