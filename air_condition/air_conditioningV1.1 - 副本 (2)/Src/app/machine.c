#include "machine.h"
extern UART_HandleTypeDef huart1;
unsigned char Receive_data[UART1_BUFFER_SIZE],Receive_data_temp[UART1_BUFFER_SIZE];
unsigned char  Uart1_Receive_flag;
unsigned char inByte,temp,Receive_item;

//本程序是 Arduino 的示例程序，其他 MCU 请移植。
void Receive_command(unsigned char revdat)
{
	//unsigned char Receive_data_temp[UART1_BUFFER_SIZE];

   
	
    //if(Serial.available() > 0)
    {
        inByte = revdat;
        if(inByte==0xAA)
        {
            Receive_item=0;
        }
        Receive_data_temp[Receive_item++]=inByte;
        ;
        if(Receive_item>=15)
        {
          ;//  Receive_item=0;
        }
        if(Receive_data_temp[15]==0x55)
        {
            temp=0; //对接受的数据进行累计校验
            for(int i=0; i<(UART1_BUFFER_SIZE-2); i++)
            {
                temp += Receive_data_temp[i];
            }
            if(temp==Receive_data_temp[UART1_BUFFER_SIZE-2])
            {
                for(int j=0; j<UART1_BUFFER_SIZE; j++)
                {
                    Receive_data[j]=Receive_data_temp[j];
                } //如果接受到的数据正确则 copy 到 Receive_data 数组中
            }
            Uart1_Receive_flag=1;
        }
    }
	//	HAL_UART_DMAPause(&huart1);
		//	HAL_UART_Receive_DMA(&huart1,Receive_data_temp,UART1_BUFFER_SIZE);
}
uint32_t tx_count;
void TransmitCommand(unsigned int pspeed,unsigned char sw)
{
   unsigned char p[16],i;
   unsigned char tmp,tmp2;
   
   tmp = 0xaa;
	i=0;
   p[i++] =tmp;
   
   //memcpy((p+i),&tmp,1);i++;
   tmp = 0x00;
   p[i++] =tmp;
  // memcpy((p+i),&tmp,1);i++;
	tmp = sw;
  p[i++] =tmp;
   //memcpy((p+i),&tmp);i++;
   tmp = (unsigned char)(pspeed&0x00ff);
   tmp2 = tmp;
   p[i++] =tmp;
  // memcpy((p+i),&tmp,1);i++;
   tmp = (unsigned char)(pspeed>>8);
  p[i++] =tmp;
  // memcpy((p+i),&tmp,1);i++;
   tmp = p[0] +p[1] + p[2] + p[3]+ p[4];
   tmp2 = 0;
	p[i++] =tmp2;
	p[i++] =tmp2;
	p[i++] =tmp2;
	p[i++] =tmp2;
	p[i++] =tmp2;
	p[i++] =tmp2;
	p[i++] =tmp2;
	p[i++] =tmp2;
	p[i++] =tmp2;
	p[i++] =tmp;

   //memcpy((p+i),&tmp2,1);i++;
/*	memcpy((p+i),&tmp2,1);i++;
	memcpy((p+i),&tmp2,1);i++;
	memcpy((p+i),&tmp2,1);i++;
	memcpy((p+i),&tmp2,1);i++;
	memcpy((p+i),&tmp2,1);i++;
	memcpy((p+i),&tmp2,1);i++;
	memcpy((p+i),&tmp2,1);i++;
	memcpy((p+i),&tmp2,1);i++;
   memcpy((p+i),&tmp,1); i++;*/
   tmp = 0x55;
   	p[i++] =tmp;
		tx_count++;
   //memcpy((p+i),&tmp,1);  i++;
   HAL_UART_Transmit(&huart1, p, 16, 200);
}
//串口接收空闲中断
void UsartReceive_IDLE(unsigned char uart_num)
{
	uint32_t temp;

	if(uart_num==1)
	{
	//	temp=__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE);
	//	if((temp!=RESET))
		{
		//	 __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_IDLE);

			//HAL_UART_DMAStop(&huart2);
			temp=huart1.Instance->SR;
			temp=huart1.Instance->DR;
			Receive_command(temp);
		}
	}
	if(uart_num==3)
	{
		/*temp=__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart3);


			temp=huart3.Instance->SR;
			temp=huart3.Instance->DR;
			HAL_UART_DMAPause(&huart3);
			temp=hdma_usart3_rx.Instance->NDTR;
			UsartType3.rx_len=buffer_size-temp;
			UsartType3.receive_flag=1;
			UsartType3.real_index=UsartType3.real_index+UsartType3.rx_len_var;
			if(UsartType3.real_index>=buffer_size)
				UsartType3.real_index=UsartType3.real_index-buffer_size;
			UsartType3.rx_len_var=UsartType3.rx_len;
			UsartType3.rx_len=buffer_size-temp;
			if(UsartType3.rx_len<UsartType3.rx_len_var)
			{
				UsartType3.rx_len_var=UsartType3.rx_len+buffer_size-UsartType3.rx_len_var;
			}
			else
				UsartType3.rx_len_var=UsartType3.rx_len-UsartType3.rx_len_var;

			HAL_UART_DMAResume(&huart3);
			//   UsartType3.real_index=0;
			//UsartType3.rx_len=0;
			//UsartType3.rx_len_var=0;
			//UsartType3.loop_index=0;
			HAL_UART_Receive_DMA(&huart3,UsartType3.usartDMA_rxBuf,buffer_size);

		}*/
	}
}


void machine_speed(uint32_t pspeed)
{

}
