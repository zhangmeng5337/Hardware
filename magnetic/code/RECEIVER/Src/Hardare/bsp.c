
#include "main.h"
#include "bsp.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
uart_stru uart1;
uart_stru uart2;
uart_stru uart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;

//串口接收空闲中断
void UsartReceive_IDLE(unsigned char uart_num)
{
	uint32_t temp;
	if(uart_num==1)
	{
		temp=__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);

			//HAL_UART_DMAStop(&huart2);
			temp=huart1.Instance->ISR;
			temp=huart1.Instance->RDR;
			//HAL_UART_DMAResume(&huart2);
			HAL_UART_DMAPause(&huart1);
			temp=hdma_usart1_rx.Instance->CNDTR;
			uart1.receive_len=uart1.receive_len+UARTBUFFERSIZE-temp;
			uart1.receive_flag=1;
			uart1.index=uart1.index+UARTBUFFERSIZE-temp;
			if(uart1.index>=UARTBUFFERSIZE)
				uart1.index=uart1.index-UARTBUFFERSIZE;
			HAL_UART_DMAPause(&huart1);
			HAL_UART_Receive_DMA(&huart1,uart1.receive_buffer,UARTBUFFERSIZE);
		}
	}
	if(uart_num==2)
	{
		temp=__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);

			//HAL_UART_DMAStop(&huart2);
			temp=huart2.Instance->ISR;
			temp=huart2.Instance->RDR;
			//HAL_UART_DMAResume(&huart2);
			HAL_UART_DMAPause(&huart2);
			temp=hdma_usart2_rx.Instance->CNDTR;
			uart2.receive_len=uart2.receive_len+UARTBUFFERSIZE-temp;
			uart2.receive_flag=1;
			uart2.index=uart2.index+UARTBUFFERSIZE-temp;
			if(uart2.index>=UARTBUFFERSIZE)
				uart2.index=uart2.index-UARTBUFFERSIZE;
			HAL_UART_DMAPause(&huart2);
			HAL_UART_Receive_DMA(&huart2,uart2.receive_buffer,UARTBUFFERSIZE);
		}
	}
	if(uart_num==3)
	{
		temp=__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart3);

			//HAL_UART_DMAStop(&huart2);
			temp=huart3.Instance->ISR;
			temp=huart3.Instance->RDR;
			//HAL_UART_DMAResume(&huart2);
			HAL_UART_DMAPause(&huart3);
			temp=hdma_usart3_rx.Instance->CNDTR;
			uart3.receive_len=uart3.receive_len+UARTBUFFERSIZE-temp;
			uart3.receive_flag=1;
			uart3.index=uart3.index+UARTBUFFERSIZE-temp;
			if(uart3.index>=UARTBUFFERSIZE)
				uart3.index=uart3.index-UARTBUFFERSIZE;
			HAL_UART_DMAPause(&huart3);
			HAL_UART_Receive_DMA(&huart3,uart3.receive_buffer,UARTBUFFERSIZE);
		}
	}	
}

