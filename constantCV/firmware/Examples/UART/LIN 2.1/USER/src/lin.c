#include "lin.h"

void UART_SendBreak(UART_TypeDef* UARTx, uint8_t BreakLength);
uint8_t Lin_CheckPID(	uint8_t id);
void Lin_SendHead(uint8_t id);

uint8_t Lin_Send_EN,Lin_LBD_Flag;
uint8_t Lin_Sending_Flag;
uint8_t Lin_Reception_of_data;

LinTxMsg linTxMsg;
LinRxMsg linRxMsg;

uint8_t Lin_Mode				=	LIN_Mode_MasterRead;
uint8_t Lin_CheckSum_Mode	=	CheckSum_Classic;
uint16_t Lin_Speed			=	19200;
uint8_t Lin_Checksum(uint8_t id , uint8_t data[]);
uint8_t lin_data[8];



void UART1_IRQHandler()
{
	if(UART_GetFlagStatus(CW_UART1,UART_FLAG_RXBRK)!=RESET)//LIN中断检测标志
	{
		if(Lin_Sending_Flag==DISABLE)//没有发送数据
		{
			Lin_LBD_Flag=ENABLE;
		}
		UART_ClearFlag(CW_UART1,UART_FLAG_RXBRK);//LIN断开
	}
	if(UART_GetFlagStatus(CW_UART1,UART_FLAG_RC)!=RESET)//接收数据寄存器不为空标志
	{
		if(Lin_LBD_Flag==ENABLE)//lin中断使能可接收数据
		{
			uint8_t data=UART_ReceiveData(CW_UART1);//接收数据
			
			if(linRxMsg.Index==1)
			{
				linRxMsg.Protected_ID=data;//接收PID
				linRxMsg.ID=linRxMsg.Protected_ID&0x3f;
			}
 		if(	linRxMsg.ID==0x23)	
{				
				for (uint8_t i = 0; i < 8; i++) 
	{   
		lin_data[i]=1;
		while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
		UART_SendData(CW_UART1,lin_data[i]);       //发送数据
	}
		while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
		uint16_t checkSums=Lin_Checksum(linRxMsg.ID,lin_data);

	 UART_SendData(CW_UART1,checkSums);
			linRxMsg.ID=0;
}	
		else if(linRxMsg.Index>1 && linRxMsg.Index<linRxMsg.Length+2)
			{
				linRxMsg.Data[linRxMsg.Index-2]=data;//接收数据				
			}
			else if(linRxMsg.Index==linRxMsg.Length+2)//接收校验场
			{
				linRxMsg.CheckSum=data;
			}
			linRxMsg.Index++;
			if(linRxMsg.Index>linRxMsg.Length+2)//数据接收完毕
			{
				linRxMsg.Receive_Complete=ENABLE;//接收数据使能
				Lin_LBD_Flag=DISABLE;//接收数据失能
				linRxMsg.Index=0;
			}
		}
		UART_ClearFlag(CW_UART1,UART_FLAG_RC);//接收完毕，清除接收数据寄存器不为空标志
	}
}

uint8_t lin_Check_ProtectedID(uint16_t id,uint8_t len)//输入ID返回PID
{
	uint8_t checkSum=id&0x3f;
	checkSum|=(((checkSum&0x01)^((checkSum&0x02)>>1)^((checkSum&0x04)>>2)^((checkSum&0x10)>>4))<<6)&0x40;
	checkSum|=(~((((checkSum&0x02)>>1)^((checkSum&0x08)>>3)^((checkSum&0x10)>>4)^((checkSum&0x20)>>5))<<7))&0x80;
	return checkSum;
}

void Lin_Send_Msg()                             //主机发送帧头和数据  从机接收
{
		UART_ClearFlag(CW_UART1,UART_FLAG_TC);
	Lin_Sending_Flag=ENABLE;
	linTxMsg.Protected_ID = lin_Check_ProtectedID(linTxMsg.ID,linTxMsg.Length);//校验和PID
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
		UART_SendBreak(CW_UART1,15);//同步时间间隔
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
	UART_SendData(CW_UART1,0x55);//同步段
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
	UART_SendData(CW_UART1,linTxMsg.Protected_ID);//发送PID
	for (uint8_t i = 0; i < linTxMsg.Length; i++) 
	{
		while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
		linTxMsg.Data[i]=1;                             //数据全为0x01
		UART_SendData(CW_UART1,linTxMsg.Data[i]);       //发送数据
	}

		linTxMsg.CheckSum= Lin_Checksum(linTxMsg.ID,linTxMsg.Data);  //校验和
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);//发送校验和
	UART_SendData(CW_UART1,linTxMsg.CheckSum);
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);//等待发送结束
	linTxMsg.CheckSum=0;
	Lin_Sending_Flag=DISABLE;
}

void Lin_Send_Wharf(void)                         //主机只发帧头，对特定的ID从机发送数据和校验
{
	
	UART_ClearFlag(CW_UART1,UART_FLAG_TC);
	Lin_Sending_Flag=ENABLE;
	linTxMsg.Protected_ID = lin_Check_ProtectedID(linTxMsg.ID,linTxMsg.Length);//校验和PID
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
	UART_SendBreak(CW_UART1,15);//同步时间间隔
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
	UART_SendData(CW_UART1,0x55);//同步段
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
	UART_SendData(CW_UART1,linTxMsg.Protected_ID);//发送PID
	while(UART_GetFlagStatus(CW_UART1,UART_FLAG_TXE)==RESET);
	linTxMsg.CheckSum=0;
	Lin_Sending_Flag=DISABLE;
}

void Lin_Data_receiving_callback(void)//Lin接收回传函数
{
	if(Lin_Reception_of_data==1)
	{
		Lin_Reception_of_data=0;
		switch(linRxMsg.ID)                              //由ID执行对应任务
		{
			case 0x07:
		PB02_TOG();            
				break;	
		}
	}
}

void Lin_SendData_Process(void)//接收数据的过程
{
	if(linRxMsg.Receive_Complete)
	{
		uint16_t checkSum=0;
		checkSum=Lin_Checksum(linRxMsg.ID,linRxMsg.Data);
		if(linRxMsg.CheckSum==checkSum)
		{
			Lin_Reception_of_data=1;
			Lin_Data_receiving_callback();
		}
		linRxMsg.Receive_Complete=DISABLE;
	}
}

void UART_SendBreak(UART_TypeDef* UARTx, uint8_t BreakLength)
{
  assert_param(IS_UART_ALL_PERIPH(UARTx));
  assert_param(IS_UART_DATA_8BIT(BreakLength));

  UARTx->TDR =  (1<<10) | BreakLength;
  

}


uint8_t Lin_Checksum(uint8_t id , uint8_t data[])
{
	uint8_t t ;
	uint16_t sum ;

	sum = data[0];
	if(id == 0x3c)			// 如果是诊断帧，用经典校验
	{
		
		for(t=1;t<8;t++)
		{
			sum += data[t];
			if(sum&0xff00)
			{
				sum&=0x00ff;
				sum+=1;
			}
		}
		sum = ~sum;	
		
		return (uint8_t)sum ;
	}
	
	for(t=1;t<8;t++)
	{
		sum += data[t];
		if(sum&0xff00)
		{
			sum&=0x00ff;
			sum+=1;
		}
	}
	sum+=Lin_CheckPID(id);
	if(sum&0xff00)
	{
		sum&=0x00ff;
		sum+=1;
	}
	sum = ~sum;	
	return (uint8_t)sum ;
}

uint8_t Lin_CheckPID(uint8_t id)
{
	uint8_t returnpid ;
	uint8_t P0 ;
	uint8_t P1 ;
	
	P0 = (((id)^(id>>1)^(id>>2)^(id>>4))&0x01)<<6 ;
	P1 = ((~((id>>1)^(id>>3)^(id>>4)^(id>>5)))&0x01)<<7 ;
	
	returnpid = id|P0|P1 ;
	
	return returnpid ;
	
}

