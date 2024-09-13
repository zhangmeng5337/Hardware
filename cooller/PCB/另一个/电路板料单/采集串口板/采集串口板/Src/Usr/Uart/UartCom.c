

#include "UartCom.h"
#include "includes.h"

#ifdef OS_VERSION
OS_EVENT      *pComTxSem;   //
OS_STK        UartTaskComTxStk[UART_TASK_COM_TX_STK_SIZE];
OS_FLAG_GRP   *UartIdleFlag;   //串口空闲
#endif



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
void TaskUartComTx(void *pdata)
{
#ifdef OS_VERSION
    INT8U err;
	  pComTxSem     = OSSemCreate(0);//创建 Uart 接收信号量
	  UartIdleFlag  = OSFlagCreate(0,&err);
	  while( mSystemData.mSystemInitlFlag ==0 );	  	    
#endif

    while(1)
    {
//..............................................
#ifdef OS_VERSION
        OSSemPend(pComTxSem,200,&err);//等待CAN接收数据的信号量
        if(err == OS_ERR_NONE)
        {
					  
					  //OSFlagPost(UartIdleFlag,(OS_FLAGS)UART_TX_IDLE,OS_FLAG_SET,&err);//
            //UartDmaTxNString(Uart1TxdDataBuf,Uart1TxdDataLenght);
					  //OSFlagPost(UartIdleFlag,(OS_FLAGS)UART_TX_IDLE,OS_FLAG_CLR,&err);//
        }
#endif
//..............................................
    }

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
void TaskUartComTxCreate(void)
{
    INT8U os_err;
//..
    os_err = OSTaskCreateExt( (void (*)(void *))TaskUartComTx,
                              (void          * ) 0,
                              (OS_STK        * )&UartTaskComTxStk[UART_TASK_COM_TX_STK_SIZE - 1],
                              (INT8U           ) UART_TASK_COM_TX_START_PRIO,
                              (INT16U          ) UART_TASK_COM_TX_START_PRIO,
                              (OS_STK        * )&UartTaskComTxStk[0],
                              (INT32U          ) UART_TASK_COM_TX_STK_SIZE,
                              (void          * )0,
                              (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(UART_TASK_COM_TX_START_PRIO, (CPU_INT08U *)"UartCan", &os_err);
#endif
//..............................................................	
}	
/*******************************************************************************
* Function Name  : 
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : 4c 53 44
* Input          : None
* Output         : None
* Return         : None	 
*******************************************************************************/
unsigned char StartApt(CUartCom *pUartCom);
CUartCom       mAptUartComTx; 
void  UartComProccese(CUartCom *pUartCom)
{
	 unsigned char mPdc,mCheckPdc; 
	 CUartCom       mUartComTx; 
	 unsigned char  os_err;
//..	
   if( (pUartCom->CharacterWord[0] == 0x4c) && 
       (pUartCom->CharacterWord[1] == 0x53) &&
       (pUartCom->CharacterWord[2] == 0x44)	 )	
	 {//满足特征
		   mPdc      =  pUartCom->DataBuf[pUartCom->DataLength];
		   mCheckPdc = PdcCheck(  (unsigned char*)pUartCom,pUartCom->DataLength+5);
		   if( mPdc != mCheckPdc ) return;
		   else if( pUartCom->Cmd == 0x18 )  QueryState(pUartCom);
		   else if( pUartCom->Cmd == 0x19 )  PowerLed(pUartCom);
		   else if( pUartCom->Cmd == 0x1A )  ReadAD(pUartCom);
		   else if( pUartCom->Cmd == 0x1B )  ReadMultipleAD(pUartCom);
		   else if( pUartCom->Cmd == 0x3A )  
			 {
				   mSystemData.mAptMeasureFlag = 1; StartApt(pUartCom); 				 
				   mUartComTx.Cmd              = 0x3A;	
	         mUartComTx.CharacterWord[0] = 0x54;
	         mUartComTx.CharacterWord[1] = 0x43;
	         mUartComTx.CharacterWord[2] = 0x4e;
				   mUartComTx.DataLength        = 0;
	         mUartComTx.DataBuf[0]        = PdcCheck( (unsigned char*)(&mUartComTx), 5);	
				 	 OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
	         memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),6);
	         Uart1TxdDataLenght           = 6;
	         OSSemPost(pComTxSem);//发送数据 
			 } //开始数据采集
		   else if( pUartCom->Cmd == 0x31 )  {mSystemData.mAptMeasureFlag = 0; StartApt(pUartCom); } //校准
			 else if( pUartCom->Cmd == 0x30 ) 
			 {//读取atp 采集值
				   mAptUartComTx.Cmd              = 0x30;	
	         mAptUartComTx.CharacterWord[0] = 0x54;
	         mAptUartComTx.CharacterWord[1] = 0x43;
	         mAptUartComTx.CharacterWord[2] = 0x4e;
				   OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
	         memcpy(Uart1TxdDataBuf, (unsigned char*)(&mAptUartComTx),mAptUartComTx.DataLength + 6);
	         Uart1TxdDataLenght = mAptUartComTx.DataLength + 6;
	         OSSemPost(pComTxSem);//发送数据 
			 }
	 }//
	 else return;
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
unsigned char PdcCheck(unsigned char *pData,unsigned char mDataLength)
{
	 unsigned short mPdcSum = 0;
	 unsigned char i;
///..
   for(i=0;i<mDataLength;i++)	
	 {
		  mPdcSum += *pData++;
	 } 
	 return  ( (unsigned char)(mPdcSum &0xff) );
}

/*******************************************************************************
* Function Name  : 查询命令
* Author         : wqs
* WriteDate      : 05/28/2012
* EditDate       : 05/28/2012
* Description    : 18 4c 53 44 00 
* Input          : None
* Output         : None
* Return         : None	 
*******************************************************************************/
unsigned char QueryState(CUartCom *pUartCom)
{
	 CUartCom mUartComTx;
	 unsigned char mTxLength;
	 INT8U os_err;
//..
   mUartComTx.Cmd              = pUartCom->Cmd;	
	 mUartComTx.CharacterWord[0] = 0x54;
	 mUartComTx.CharacterWord[1] = 0x43;
	 mUartComTx.CharacterWord[2] = 0x4e;
	 mUartComTx.DataLength       = 5;
	 mUartComTx.DataBuf[0]       = mSystemData.mSPIFlag;//状态0:正常
	 mUartComTx.DataBuf[1]       = mSystemData.mChipIDStr[0];
	 mUartComTx.DataBuf[2]       = mSystemData.mChipIDStr[1];
	 mUartComTx.DataBuf[3]       = mSystemData.mChipIDStr[2];
	 mUartComTx.DataBuf[4]       = mSystemData.mChipIDStr[3];
	 mUartComTx.DataBuf[5]       = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 5);
	
	 mTxLength = mUartComTx.DataLength + 6;
	 
	 OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
	 memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
	 Uart1TxdDataLenght = mTxLength;
	 OSSemPost(pComTxSem);//UartDmaTxNString(Uart1TxdDataBuf,Uart1TxdDataLenght);
	 return 0;
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
unsigned char PowerLed(CUartCom *pUartCom)
{
	 CUartCom mUartComTx;
	 unsigned char mTxLength;
	 INT8U os_err;
//..
	 SetLedValue(pUartCom->DataBuf[0],pUartCom->DataBuf[1]);//   
//..
   mUartComTx.Cmd              = pUartCom->Cmd;	
	 mUartComTx.CharacterWord[0] = 0x54;
	 mUartComTx.CharacterWord[1] = 0x43;
	 mUartComTx.CharacterWord[2] = 0x4e;
	 mUartComTx.DataLength       = 1;
	 mUartComTx.DataBuf[0]       = 0;	 //状态0:正常
	 mUartComTx.DataBuf[1]       = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 5);
	
	 mTxLength = mUartComTx.DataLength + 6;	 
	 OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&os_err);	
	 memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
	 Uart1TxdDataLenght = mTxLength;
	 OSSemPost(pComTxSem);//UartDmaTxNString(Uart1TxdDataBuf,Uart1TxdDataLenght);
	 return 0;
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
unsigned char ReadAD(CUartCom *pUartCom)
{
//..
   mFoodData.mChannel =  pUartCom->DataBuf[0];//通道AD值
	 #ifdef FOOD_NUMBER_SPI1
	 if(  (mFoodData.mChannel >= 1) &&  (mFoodData.mChannel <=8 ) )
	 {//SPI1
		   mFoodData.mADReadFlag[0] =  1 << (mFoodData.mChannel - 1);
		   OSSemPost(pFood8Sem);
	 } 
	 #endif
	 #ifdef FOOD_NUMBER_SPI2
	 if(  (mFoodData.mChannel >= 9) &&  (mFoodData.mChannel <= 16 ) )
	 {//SPI1
		   mFoodData.mADReadFlag[1] =  1 << (mFoodData.mChannel - 9);
		   OSSemPost(pFood16Sem);
	 } 
	 #endif
	 #ifdef FOOD_NUMBER_SPI3
	 if(  (mFoodData.mChannel >= 17) &&  (mFoodData.mChannel <= 24 ) )
	 {//SPI1
		   mFoodData.mADReadFlag[2] =  1 << (mFoodData.mChannel - 17);
		   OSSemPost(pFood24Sem);
	 } 
	 #endif
//..
	 return 0;
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
#define CHANNEL0        0x00
#define CHANNEL1        0x01

#define   DATA_NORMAL             0x06
#define   DATA_OVER               0x05
#define   DATA_ERRO               0x09
#define   DATA_TIME               0x04
unsigned char ReadMultipleAD(CUartCom *pUartCom)
{

	 unsigned char mStartChannel,mStopChannel;
	 unsigned int  mReadAdFlag = 0xffffff,mReadAdFlag2;
	 unsigned char mChannelNumber,i,mByteBits,mByteLable;
	 CUartCom       mUartComTx;	
	 unsigned short mADValue,mTxLength;
	 INT8U          err;
//..
   //pUartCom->DataBuf[0];//通道40-47	
	 //pUartCom->DataBuf[1];//通道32-39	
	 //pUartCom->DataBuf[2];//通道24-31	
	 //pUartCom->DataBuf[3];//通道16-23	
	 //pUartCom->DataBuf[4];//通道 8-15	
	 //pUartCom->DataBuf[5];//通道 0-7	
//..	 
	 mUartComTx.Cmd              = 0x1A; 
   mUartComTx.CharacterWord[0] = 0x54;
	 mUartComTx.CharacterWord[1] = 0x43;
	 mUartComTx.CharacterWord[2] = 0x4e;	 	 	 
	 mUartComTx.DataBuf[0]       = 0; 
	 mByteLable=1;
//..	 
	 for(mByteBits=0x01,mChannelNumber=0,i=0;i<8;i++)
	 {
		  if( (pUartCom->DataBuf[5] & mByteBits) == mByteBits )  
			{
				 SetSpiOneADChannel(i);
				 mADValue = ADSPI1collectionADSignal(CHANNEL0,&err);
				 if(err != DATA_NORMAL )     mUartComTx.DataBuf[0]  = err;
				 mUartComTx.DataBuf[mByteLable++]       = i+1;//通道值
	       mUartComTx.DataBuf[mByteLable++]       = (unsigned char)(mADValue>>8);	 //高8位
	       mUartComTx.DataBuf[mByteLable++]       = (unsigned char)(mADValue&0xff);	 //低8位 
				
				 mChannelNumber++;
			}//if( (pUartCom->DataBuf[4] & mByteBits) == mByteBits )  
		  if( (pUartCom->DataBuf[4] & mByteBits) == mByteBits )  
			{
				 SetSpiTwoADChannel(i);
				 mADValue = ADSPI2collectionADSignal(CHANNEL0,&err);
				 if(err != DATA_NORMAL )     mUartComTx.DataBuf[0]  = err;
				 mUartComTx.DataBuf[mByteLable++]       = i+9;//通道值
	       mUartComTx.DataBuf[mByteLable++]       = (unsigned char)(mADValue>>8);	 //高8位
	       mUartComTx.DataBuf[mByteLable++]       = (unsigned char)(mADValue&0xff);	 //低8位 				
				 mChannelNumber++;
			}//if( (pUartCom->DataBuf[5] & mByteBits) == mByteBits )  
			mByteBits =  mByteBits <<1;
	 }//for(mByteBits=0x01,mChannelNumber=0,i=0;i<8;i++)
//..............................................
	 mUartComTx.DataLength       = mChannelNumber*3;
   mUartComTx.DataBuf[mByteLable++] = PdcCheck( (unsigned char*)(&mUartComTx), mUartComTx.DataLength + 6);
	
	 mTxLength = mUartComTx.DataLength + 7;	 
	 OSFlagPend(UartIdleFlag, (OS_FLAGS)UART_TX_IDLE,OS_FLAG_WAIT_CLR_ALL,0,&err);	
	 memcpy(Uart1TxdDataBuf, (unsigned char*)(&mUartComTx),mTxLength);
	 Uart1TxdDataLenght = mTxLength;
	 OSSemPost(pComTxSem);//	 
	 
	 return 0;
//..
   mStartChannel = pUartCom->DataBuf[0];//通道0
	 mStopChannel  = pUartCom->DataBuf[1];//结束通道
	 if( mStartChannel  == 0  ) return 0;
	 if( mStopChannel   >24   ) return 0;
	 mStartChannel  = mStartChannel - 1;	
	 mReadAdFlag    = 0xffffff << mStartChannel;
	 mReadAdFlag2   = 0xffffff >> (24-mStopChannel); 	 
	 mReadAdFlag    = mReadAdFlag & mReadAdFlag2;
	 #ifdef FOOD_NUMBER_SPI1	 	 
	  mFoodData.mADReadFlag[0] =  mReadAdFlag & 0xff;
		OSSemPost(pFood8Sem);	 
	 #endif
	 #ifdef FOOD_NUMBER_SPI2
	  mFoodData.mADReadFlag[1] =  (mReadAdFlag>>8) & 0xff;
	  OSSemPost(pFood16Sem);
	 #endif
	 #ifdef FOOD_NUMBER_SPI3	 
	  mFoodData.mADReadFlag[2] =  (mReadAdFlag>>16) & 0xff; 
	  OSSemPost(pFood24Sem);
	 #endif
//..
	 return 0;
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
unsigned char StartApt(CUartCom *pUartCom)
{
	  OSSemPost(pAptSem);
	  return 0;
}


//$
