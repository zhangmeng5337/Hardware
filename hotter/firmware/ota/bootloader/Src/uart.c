#include "uart.h"
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;

extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;


rs485_stru lte_str;
rs485_stru lcd_str;
rs485_stru rs485_str;


void uart_init()
{
    HAL_GPIO_WritePin(Mb_rxen1_GPIO_Port, Mb_rxen1_Pin, GPIO_PIN_RESET);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart1, (uint8_t*)lte_str.buff, BUFFER_SIZE);  

	__HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart4, (uint8_t*)rs485_str.buff, BUFFER_SIZE); 

	__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart5, (uint8_t*)lcd_str.buff, BUFFER_SIZE); 
}
void uart_lte()
{
	//停止本次DMA传输
		HAL_UART_DMAStop(&huart1);	
														   
		//计算接收到的数据长度
		lte_str.recv_len  = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);  
		if(lte_str.recv_update == 0)
		{
			memcpy(lte_str.recv_buf,lte_str.buff,lte_str.recv_len);
			lte_str.recv_update = 1;
		}
		
		//重启开始DMA传输 每次BUFFER_SIZE字节数据
		HAL_UART_Receive_DMA(&huart1, (uint8_t*)lte_str.buff, BUFFER_SIZE);  

}

void uart_rs485()
{
	//停止本次DMA传输
		HAL_UART_DMAStop(&huart4);	
														   
		//计算接收到的数据长度
		rs485_str.recv_len  = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);  
		if(rs485_str.recv_update == 0)
		{
			memcpy(rs485_str.recv_buf,rs485_str.buff,rs485_str.recv_len);
			rs485_str.recv_update = 1;
		}

		//重启开始DMA传输 每次BUFFER_SIZE字节数据
		HAL_UART_Receive_DMA(&huart4, (uint8_t*)rs485_str.buff, BUFFER_SIZE);  

}

void uart_lcd()
{
	//停止本次DMA传输
		HAL_UART_DMAStop(&huart5);	
														   
		//计算接收到的数据长度
		lcd_str.recv_len  = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart5_rx);  
		if(lcd_str.recv_update == 0)
		{
			memcpy(lcd_str.recv_buf,lcd_str.buff,lcd_str.recv_len);
			lcd_str.recv_update = 1;
		}

		//重启开始DMA传输 每次BUFFER_SIZE字节数据
		HAL_UART_Receive_DMA(&huart5, (uint8_t*)lcd_str.buff, BUFFER_SIZE);  

}

void uart_transmit(unsigned char uart_num,uint8_t * pData ,uint16_t Size)
{
	switch(uart_num)
	{
		case LTE_No:	HAL_UART_Transmit(&huart1, pData, Size, 100);break;
		case RS485_No:	
		{
		    HAL_GPIO_WritePin(Mb_rxen1_GPIO_Port, Mb_rxen1_Pin, GPIO_PIN_SET);
			HAL_UART_Transmit(&huart4, pData, Size, 100);
			HAL_Delay(1);
			HAL_GPIO_WritePin(Mb_rxen1_GPIO_Port, Mb_rxen1_Pin, GPIO_PIN_RESET);

		}break;
		case LCD_No:	HAL_UART_Transmit(&huart5, pData, Size, 100);break; 
		default :		;break;		
	}

}

rs485_stru *get_uart_recv(unsigned char uart_num)
{
	switch(uart_num)
	{
		case LTE_No:	return &lte_str;
		case RS485_No:	return &rs485_str;
		case LCD_No:	return &lcd_str; 
		default :		return 0;		
	}
}


