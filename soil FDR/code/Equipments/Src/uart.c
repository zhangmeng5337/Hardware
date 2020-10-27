#include "uart.h"
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
uart_usr_stru lora_master_uart;
uart_usr_stru lora_slave_uart;

unsigned char nb_buffer[NB_SIZE];
unsigned char gps_buffer[NB_SIZE];
unsigned char Uart1_Receive_flag;
//其他 MCU 请移植。
void LORA_Receive_command(unsigned char revdat,unsigned char num)
{
	unsigned char temp,inByte;
	static unsigned int Receive_item;
        inByte = revdat;
        if(inByte==0xc0||inByte==0xc1)
        {
            Receive_item=0;
					  
        }
				if(num == 2)
				{
					lora_master_uart.len++;
					lora_master_uart.buffer[Receive_item++]=inByte;				
				}

				else
				{
					lora_slave_uart.len++;
					lora_slave_uart.buffer[Receive_item++]=inByte;				
				}

        
        if(Receive_item>=15)
        {
          ;//  Receive_item=0;
        }
        if(lora_master_uart.buffer[15]==0x55)
        {
            temp=0; //对接受的数据进行累计校验
            for(int i=0; i<(LORA_BUFFER_SIZE-2); i++)
            {
                temp += lora_master_uart.buffer[i];
            }
            if(temp==lora_master_uart.buffer[LORA_BUFFER_SIZE-2])
            {
							lora_master_uart.receivedFlag=1;
            }
           
        }
}
//串口接收空闲中断
void UsartReceive_IDLE(unsigned char uart_num)
{
	uint32_t temp;

	if(uart_num==1)
	{
		{
			temp=huart1.Instance->SR;
			temp=huart1.Instance->DR;
			LORA_Receive_command(temp,uart_num);
		}
	}
	if(uart_num==2)
	{
		{
			temp=huart2.Instance->SR;
			temp=huart2.Instance->DR;
			LORA_Receive_command(temp,uart_num);
		}
	}	
}