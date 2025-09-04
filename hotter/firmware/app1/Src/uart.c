#include "uart.h"
tsLpuart1type Lpuart1type;

extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;

extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;


//rs485_stru lte_str;
rs485_stru lcd_str;
rs485_stru rs485_str;

uint8_t Usart1RX = 0;
tsLpuart1type Lpuart1type;


void uart_init()
{
    HAL_GPIO_WritePin(Mb_rxen1_GPIO_Port, Mb_rxen1_Pin, GPIO_PIN_RESET);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
        HAL_UART_Receive_DMA(&huart1,Lpuart1type.Lpuart1DMARecBuff, LPUART1_DMA_REC_SIZE);
		
    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart4, (uint8_t*)rs485_str.recv_buf, RSBUFFER_SIZE);
	rs485_str.recv_len = 0;
	

    //__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
  //  HAL_UART_Receive_DMA(&huart5, (uint8_t*)lcd_str.buff, BUFFER_SIZE);
}
/*void uart_lte()
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

}*/

void uart_rs485()
{
    uint32_t  uart_idle_flag;
    uart_idle_flag =  __HAL_UART_GET_FLAG(&huart4,UART_FLAG_IDLE);
   // uart_idle_flag =  __HAL_UART_GET_FLAG(&huart4,UART_FLAG_ORE);

    if((uart_idle_flag != RESET))
    {
        uart_idle_flag=0;
        __HAL_UART_CLEAR_IDLEFLAG(&huart4);

        uint32_t temp;
        temp = huart4.Instance->SR;
        temp = huart4.Instance->DR;
        temp = hdma_uart4_rx.Instance->NDTR;
        rs485_str.recv_dma_len = RSBUFFER_SIZE - temp;



        //rs485_str.recv_len  = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
        if(rs485_str.recv_dma_len>1)
        {
            rs485_str.recv_update = 1;
        	//memcpy(&rs485_str.recv_buf[rs485_str.recv_len],rs485_str.recv_buf,rs485_str.recv_dma_len);
        	rs485_str.recv_len = rs485_str.recv_dma_len;
        }
    }
	
	HAL_UART_DMAStop(&huart4);
	HAL_UART_DMAResume(&huart4);
	//memset((uint8_t*)rs485_str.buff,0,RSBUFFER_SIZE);
    HAL_UART_Receive_DMA(&huart4, (uint8_t*)rs485_str.recv_buf, RSBUFFER_SIZE);
}
tsLpuart1type *get_lte_recv()
{
    return &Lpuart1type;
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

void uart_transmit(unsigned char uart_num,uint8_t * pData,uint16_t Size)
{
    switch(uart_num)
    {
    case LTE_No:
        HAL_UART_Transmit(&huart1, pData, Size, 100);
        break;
    case RS485_No:
    {
        HAL_GPIO_WritePin(Mb_rxen1_GPIO_Port, Mb_rxen1_Pin, GPIO_PIN_SET);
        HAL_UART_Transmit(&huart4, pData, Size, 100);
        HAL_Delay(3);
		
		__HAL_UART_DISABLE_IT(&huart4, UART_IT_IDLE);	 
        HAL_GPIO_WritePin(Mb_rxen1_GPIO_Port, Mb_rxen1_Pin, GPIO_PIN_RESET);	
		HAL_Delay(1);
      __HAL_UART_CLEAR_IDLEFLAG(&huart4);
	   __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
	   HAL_UART_DMAStop(&huart4);
	   HAL_UART_DMAResume(&huart4);
	   HAL_UART_Receive_DMA(&huart4, (uint8_t*)rs485_str.recv_buf, RSBUFFER_SIZE);


			
	rs485_str.recv_len = 0;
	

    }
    break;
    case LCD_No:
        HAL_UART_Transmit(&huart5, pData, Size, 100);
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
    case LCD_No:
        return &lcd_str;
    default :
        return 0;
    }
}
/*使能LPuart1串口中断*/
/*void EnableUartIT(void)
{
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
    __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    HAL_UART_Receive_DMA(&huart1,Lpuart1type.Lpuart1DMARecBuff,LPUART1_DMA_REC_SIZE);
}
*/
/*lpuart1回调函数*/
 uint32_t last_index = 0;

void uart_lte()

{
	
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);
		
	        uint32_t temp;
			
        __HAL_UART_CLEAR_IDLEFLAG(&huart1);
        HAL_UART_DMAStop(&huart1);
        temp = huart1.Instance->SR;
        temp = huart1.Instance->DR;
        temp = hdma_usart1_rx.Instance->NDTR;
        Lpuart1type.Lpuart1DMARecLen = LPUART1_DMA_REC_SIZE - temp;
       // HAL_UART_RxCpltCallback(&huart1);		
	
		


     memmove(&Lpuart1type.Lpuart1RecBuff[Lpuart1type.Lpuart1RecLen],
              Lpuart1type.Lpuart1DMARecBuff,
              Lpuart1type.Lpuart1DMARecLen);
            Lpuart1type.Lpuart1RecLen += Lpuart1type.Lpuart1DMARecLen;
			last_index = last_index + Lpuart1type.Lpuart1DMARecLen;

	HAL_UART_DMAResume(&huart1);
    HAL_UART_Receive_DMA(&huart1,Lpuart1type.Lpuart1DMARecBuff, LPUART1_DMA_REC_SIZE);
    Lpuart1type.Lpuart1RecFlag = 1;
}

