/*
Uart.c
*/
#include "Uart.h"
#include "includes.h"
/* Private variables ---------------------------------------------------------*/
/*
====================================================================================================
*               定义的局部变量
====================================================================================================
*/
unsigned char Uart1ReciveBuf[UART1RXBUFSIZE];
unsigned char Uart1TxdDataBuf[UART1TXBUFSIZE];
unsigned char Uart1TxdDataLenght;
unsigned char m485LocalAddr;

CModBusRxDataList  mModBusRxDataList;
unsigned int  mRxLenght;

#ifdef OS_VERSION
OS_EVENT    *pModbus_Sem;   //
OS_EVENT    *pTxOkSem;   //
OS_STK      UartTaskStk[UART_TASK_STK_SIZE];
#endif


/*
====================================================================================================
*?Function  ?: Uart1Putchar
*?Write Data?: 2008.09.05
*?Edit Data ?: 2008.09.09
*? Author   ?: Wu Qingsong
*?Parameter ?: c  is the 8bit data that send data
====================================================================================================
*/
int Uart1Putchar (int c)
{  
  ////////////////////////////////////////////////////////////
  USART_SendData(USART1, c);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET );
  return (0); 	                                                
}

/*
====================================================================================================
*?Function  ?: Uart0PutString
*?Write Data?: 2008.09.05
*?Edit Data ?: 2008.09.09
*? Author   ?: Wu Qingsong
*?Parameter ?: s  is the 8bit point  that is the adrese of send data
====================================================================================================
*/
void Uart1PutString(unsigned char *s)
{
    while( *s !='\0')
   {
      Uart1Putchar(*s++);
   } 
}
/*===============================================================================
WriteDate:2010.07.14
EditDate :2010.07.14
Author   :吴青松
Function:
===============================================================================*/
void UartInital(void)
{
   GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	  
//......................................................................
//串口1：
    ////////////////////UART IO inital/////////////////////////////
    //IO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);    
    ///////////////////
    //USART1 时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 , ENABLE);
    /* Enable the USART1 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel            = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
		
    USART_InitStructure.USART_BaudRate            = 9600;
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;
    USART_InitStructure.USART_Parity              = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;
    /* Configure USART1 */
    USART_Init(USART1, &USART_InitStructure);
    /* Enable USART1 Receive and Transmit interrupts */
    USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
    //USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
    /* Enable the USART1 */
    USART_Cmd(USART1, ENABLE);
//......................................................................    
		mModBusRxDataList.mWriteIn = 0;
    mModBusRxDataList.mReadOut = 0;		
//......................................................................
    /* Enable USART1 DMA TX request */
   USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);
	 USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
///..
  //NVIC_SetPriorityGrouping(7); /* 0 bits for pre-emption priority 4 bits forsubpriority*/  	
  NVIC_EnableIRQ(DMA1_Channel4_IRQn); 
	NVIC_SetPriority(DMA1_Channel4_IRQn, 0x02); /* 0x01 = 0x0 << 3 | (0x1 & 0x7), prority is 0x01 << 4 */	
	
   UartDmaIntial();
	 
}
/*===============================================================================
WriteDate:2010.07.14
EditDate :2010.07.14
Author   :吴青松
Function:
===============================================================================*/
void UartDmaIntial(void)
{
  DMA_InitTypeDef   DMA_InitStructure;	
	
	  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//...  USART1-TX -->CH4   
	
    DMA_DeInit(DMA1_Channel4);//DMA1通道4配置  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);         //外设地址  
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)Uart1TxdDataBuf;  //内存地址  
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralDST;      //dma传输方向单向  
    DMA_InitStructure.DMA_BufferSize         = UART1TXBUFSIZE;             //设置DMA在传输时缓冲区的长度  
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;  //设置DMA的外设递增模式，一个外设  
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;       //设置DMA的内存递增模式  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据字长  
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;//内存数据字长  
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Normal;            //设置DMA的传输模式   
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;          //设置DMA的优先级别  
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;            //设置DMA的2个memory中的变量互相访问
    DMA_Init(DMA1_Channel4,&DMA_InitStructure);  
    DMA_ITConfig(DMA1_Channel4,DMA_IT_TC,ENABLE);  
    //DMA_Cmd(DMA1_Channel4, ENABLE);  	//使能通道4  
	 
	
//..	 USART1-RX -->CH5
	  DMA_DeInit(DMA1_Channel5);//DMA1通道5配置  
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)(&USART1->DR);         //外设地址  
    DMA_InitStructure.DMA_MemoryBaseAddr     = (uint32_t)Uart1ReciveBuf;   //内存地址  
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralSRC;      //置外设为数据源，外设寄存器 -> 内存缓冲区 
    DMA_InitStructure.DMA_BufferSize         = UART1RXBUFSIZE;             //设置DMA在传输时缓冲区的长度  
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;  //设置DMA的外设递增模式，一个外设  
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Enable;       //设置DMA的内存递增模式  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据字长  
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_PeripheralDataSize_Byte;//内存数据字长  
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;          //设置DMA的传输模式   
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;          //设置DMA的优先级别  
    DMA_InitStructure.DMA_M2M                = DMA_M2M_Disable;            //设置DMA的2个memory中的变量互相访问
    DMA_Init(DMA1_Channel5,&DMA_InitStructure);  
    //DMA_ITConfig(DMA1_Channel5,DMA_IT_TC,ENABLE);  
		DMA_Cmd(DMA1_Channel5, ENABLE);  	//使能通道5				
		
		//sprintf(Uart1TxdDataBuf,"test is ok");
		//UartDmaTxNString(Uart1TxdDataBuf,10); 
}

/*===============================================================================
WriteDate:2010.07.14
EditDate :2010.07.14
Author   :吴青松
Function :将需要发送的数据放在 Uart1TxdDataBuf 
===============================================================================*/
void UartDmaTxNString(unsigned char* pData,unsigned char mSendSize)
{
	 INT8U err;
	
	 if( mSendSize > UART1TXBUFSIZE )  mSendSize = UART1TXBUFSIZE;
	 DMA_Cmd(DMA1_Channel4, DISABLE); 
	 DMA_ClearITPendingBit(DMA1_FLAG_TC4);     //清除
	 DMA1_Channel4->CMAR  = (unsigned int)Uart1TxdDataBuf; //
	 DMA1_Channel4->CNDTR = mSendSize;		     //发送长度  
	 DMA_Cmd(DMA1_Channel4, ENABLE);           //DMA启动	
	 
	 //OSTimeDly(mSendSize) ; 
	 //mTest = DMA1_Channel4->CNDTR;
	 OSSemPend(pTxOkSem,mSendSize*2,&err);//等待发送完毕
   if(err == OS_ERR_NONE)
	 {//
	 }
	 OSTimeDly(2) ; 
	 
}
void DMA1_Channel4_IRQHandler(void)  
{  
	 if( DMA_GetITStatus(DMA1_FLAG_TC4)!= RESET)
	 {//DMA1通道4传输完成
		   DMA_Cmd(DMA1_Channel4, DISABLE);         //DMA停止		 		  		 
		   DMA_ClearITPendingBit(DMA1_FLAG_TC4);    //清除	
       OSSemPost(pTxOkSem);		 
   }//if( DMA_GetITStatus(DMA1_FLAG_TC4)!= RESET)
	 
}
/**
  * @brief  This function handles USART1 global interrupt request.
  * @param  None
  * @retval : None
  * WriteDate      : 2010/07/14
  * EditDate       : 2010/07/14
  */
void USART1_IRQHandler(void)
{
	  unsigned short mDataLength,mDateLable;
    unsigned int   mRxBufLenght;
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {        
			  USART_ReceiveData(USART1);//清除中断标记		
			  mRxLenght = mDataLength = UART1RXBUFSIZE - DMA1_Channel5->CNDTR;  //接收的数据长度
			  if( mRxLenght == 0) return;			 	  					
					
			  mRxBufLenght = mModBusRxDataList.mWriteIn - mModBusRxDataList.mReadOut ;
        mRxBufLenght = mRxBufLenght  & (~(MODBUS_RX_DATA_LIST_NUMBER-1));
        if( mRxBufLenght == 0 )
        {// 表示 已经接受到缓冲区的数据 小于等于CAN_RX_DATA_LIST_NUMBER,即可继续向缓冲区写数据
           mDateLable    = mModBusRxDataList.mWriteIn & (MODBUS_RX_DATA_LIST_NUMBER -1);
           mModBusRxDataList.mModBusRecive[mDateLable].ReciveLength = mDataLength & (UART1RXBUFSIZE-1);
					 memcpy(mModBusRxDataList.mModBusRecive[mDateLable].ReciveBuf,Uart1ReciveBuf,
					        mModBusRxDataList.mModBusRecive[mDateLable].ReciveLength);
           mModBusRxDataList.mWriteIn++;					 					 
        }				
		    OSSemPost(pModbus_Sem);
			  DMA_Cmd(DMA1_Channel5, DISABLE);           //DMA启动	 
			  DMA_ClearITPendingBit(DMA1_FLAG_TC5);     //清除
			  DMA1_Channel5->CMAR  = (unsigned int)Uart1ReciveBuf;   //乒乓缓冲区地址切换
	      DMA1_Channel5->CNDTR = UART1RXBUFSIZE;		//重新赋值接收缓冲区长度  
	      DMA_Cmd(DMA1_Channel5, ENABLE);           //DMA启动	 
			  				
    }//if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
}
/*******************************************************************************
* Function Name  : MoubusProccese
* Author         : wqs
* WriteDate      : 2014/12/12
* EditDate       : 2014/12/12
* Description    : 处理Modbus接收的数据
* Input          : None
* Output         : None
* Return         : None
------------------------------------------------------------
|子节点地址 | 功能代码 | 数据        | CRC                  |
|  1字节    |  1字节   | 0到 252字节 | 2字节(CRC 低 CRC 高) |
-------------------------------------------------------------
*******************************************************************************/
void MoubusTask(void)
{
	  unsigned char  mFrameNumer,mRxBufLenght,mDateLable,mAddr;//
	  unsigned short mCrcRevice,mCrcCal,mReciveLength;	
    unsigned char  *pData;  
//	
    mRxBufLenght = mModBusRxDataList.mWriteIn - mModBusRxDataList.mReadOut;	
	  if( mRxBufLenght == 0 )  return;
//..
    for(mFrameNumer=0; mFrameNumer < mRxBufLenght ; mFrameNumer++)
	  {
			  mDateLable    = mModBusRxDataList.mReadOut & (MODBUS_RX_DATA_LIST_NUMBER -1);
			  mModBusRxDataList.mReadOut++;
			  //校验
			  mReciveLength = mModBusRxDataList.mModBusRecive[mDateLable].ReciveLength;
			  pData         = mModBusRxDataList.mModBusRecive[mDateLable].ReciveBuf;
			  
			  UartComProccese((CUartCom *)pData);
			  
			  
    }//for(mFrameNumer=0; mFrameNumer < mRxBufLenght ; mFrameNumer++)
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
void TaskModbus(void *pdata)
{
#ifdef OS_VERSION
    INT8U err;
	  pModbus_Sem   = OSSemCreate(0);//创建 Uart 接收信号量
	  pTxOkSem      = OSSemCreate(0);
	  while( mSystemData.mSystemInitlFlag ==0 );	  	    
#endif

    while(1)
    {
//..............................................
#ifdef OS_VERSION
        OSSemPend(pModbus_Sem,0,&err);//
        if(err == OS_ERR_NONE)
        {
            MoubusTask();
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
void TaskUartCreate(void)
{
    INT8U os_err;
//..
    os_err = OSTaskCreateExt( (void (*)(void *))TaskModbus,
                              (void          * ) 0,
                              (OS_STK        * )&UartTaskStk[UART_TASK_STK_SIZE - 1],
                              (INT8U           ) UART_TASK_START_PRIO,
                              (INT16U          ) UART_TASK_START_PRIO,
                              (OS_STK        * )&UartTaskStk[0],
                              (INT32U          ) UART_TASK_STK_SIZE,
                              (void          * )0,
                              (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
#if (OS_TASK_NAME_SIZE >= 11)
    OSTaskNameSet(UART_TASK_START_PRIO, (CPU_INT08U *)"UartCan", &os_err);
#endif
//..............................................................
    TaskUartComTxCreate(); 															
}
#endif
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
