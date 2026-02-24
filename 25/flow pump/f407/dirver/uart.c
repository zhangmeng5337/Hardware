#include "uart.h"
tsLpuart1type Lpuart1type;


extern UART_HandleTypeDef huart1;
extern DMA_HandleTypeDef hdma_usart1_rx;
rs485_stru rs485_str;

uint8_t Usart1RX = 0;
tsLpuart1type Lpuart1type;


void uart_init()
{
  //  HAL_GPIO_WritePin(Mb_rxen1_GPIO_Port, Mb_rxen1_Pin, GPIO_PIN_RESET);
		
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)rs485_str.recv_buf, RSBUFFER_SIZE);
	rs485_str.recv_len = 0;
}


void uart_rs485()
{
    uint32_t  uart_idle_flag;
    uart_idle_flag =  __HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE);
   // uart_idle_flag =  __HAL_UART_GET_FLAG(&huart1,UART_FLAG_ORE);

    if((uart_idle_flag != RESET))
    {
        uart_idle_flag=0;
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);

        uint32_t temp;
        temp = huart1.Instance->SR;
        temp = huart1.Instance->DR;
        temp = hdma_usart1_rx.Instance->NDTR;
        rs485_str.recv_dma_len = RSBUFFER_SIZE - temp;



        //rs485_str.recv_len  = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
        if(rs485_str.recv_dma_len>1)
        {
            rs485_str.recv_update = 1;
        	//memcpy(&rs485_str.recv_buf[rs485_str.recv_len],rs485_str.recv_buf,rs485_str.recv_dma_len);
        	rs485_str.recv_len = rs485_str.recv_dma_len;
        }
    }
	
	HAL_UART_DMAStop(&huart1);
	HAL_UART_DMAResume(&huart1);
	//memset((uint8_t*)rs485_str.buff,0,RSBUFFER_SIZE);
    HAL_UART_Receive_DMA(&huart1, (uint8_t*)rs485_str.recv_buf, RSBUFFER_SIZE);
}



void uart_transmit(unsigned char uart_num,uint8_t * pData,uint16_t Size)
{
    switch(uart_num)
    {
    case RS485_No:
    {
        __HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);
		HAL_Delay(5);
        HAL_UART_Transmit(&huart1, pData, Size, 200);
		while(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_TC)==0)
			;
        HAL_Delay(1);
      __HAL_UART_CLEAR_IDLEFLAG(&huart1);
	   __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
	   HAL_UART_DMAStop(&huart1);
	   HAL_UART_DMAResume(&huart1);
	   HAL_UART_Receive_DMA(&huart1, (uint8_t*)rs485_str.recv_buf, RSBUFFER_SIZE);
	rs485_str.recv_len = 0;
    }
    break;

    default :
        ;
        break;
    }

}

rs485_stru *get_uart_recv(unsigned char uart_num)
{
    switch(uart_num)
    {
    //case LTE_No:	return &lte_str;
    case RS485_No:
        return &rs485_str;

    default :
        return 0;
    }
}



