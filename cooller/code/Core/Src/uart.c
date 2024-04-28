#include "uart.h"
#include "stdio.h"
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_uart4_rx;


uart_recv_stru uart_lcd_recv;
uart_recv_stru uart_rf_recv;
uart_recv_stru uart_air_recv;


struct __FILE
{
    int handle;
};
FILE __stdout;

int fputc(int ch, FILE *f)
{
    unsigned char tmp;
    tmp =(unsigned char )ch;
    HAL_UART_Transmit(&huart4, &tmp, 1, 500);
    return ch;
}

void uart_init()
{
    //rs485 uart init
    //HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart2, uart_air_recv.uartDMARecBuff,
                         UART_DMA_REC_SIZE);

    //rs232 uart init for cdg
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); 
    HAL_UART_Receive_DMA(&huart1, uart_lcd_recv.uartDMARecBuff, UART_CDG_DMA_REC_SIZE);
 
   
    //rs232 uart init for debug 

    __HAL_UART_ENABLE_IT(&huart4, UART_IT_IDLE);
	HAL_UART_Receive_DMA(&huart4, uart_rf_recv.uartDMARecBuff,
                         UART_DMA_REC_SIZE);

}



/********************************************************
function: return uart recv data structure
params:
       uart_num = 0  ---->UART_AIR
       uart_num = 1  ---->UART_LCD
       uart_num = 2  ---->UART_RF
********************************************************/
uart_recv_stru *get_uart_recv_stru(unsigned char uart_num)
{
    uart_recv_stru *recv_uart_stru;
    switch (uart_num)
    {
        case UART_AIR:
            recv_uart_stru = &uart_air_recv;
            break;
        case UART_RF:
        {
            recv_uart_stru = &uart_rf_recv;

        }
        break;
        case UART_LCD:
            recv_uart_stru = &uart_lcd_recv;
            break;
        default :
            recv_uart_stru = &uart_lcd_recv;
            break;
    }
    return  recv_uart_stru;
}

void uart_transmit(unsigned char uart_num, uint8_t *pData, uint16_t Size)
{
    switch (uart_num)
    {
        case UART_LCD:
            HAL_UART_Transmit(&huart1, pData, Size, 100);
            break;
        case UART_AIR:
        {
            HAL_UART_Transmit(&huart2, pData, Size, 100);

        }
        break;
        case UART_RF:
            HAL_UART_Transmit(&huart5, pData, Size, 100);
            break;
        default :
            ;
            break;
    }
}
//void uart_debug_recv_proc()
//{
//
//    if(__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) != 0)
//    { 
//			uart_debug_recv.uartRecLen  = UART_DMA_REC_SIZE - __HAL_DMA_GET_COUNTER(
//                                      &hdma_usart3_rx);
//        __HAL_UART_CLEAR_FLAG(&huart3, UART_CLEAR_IDLEF);
//        memcpy(uart_debug_recv.uartRecBuff,
//               uart_debug_recv.uartDMARecBuff, uart_debug_recv.uartRecLen);
//        uart_debug_recv.uartRecFlag = 1;
//        uart_debug_recv.uartRecBuff_index = uart_debug_recv.uartRecLen +
//                                            uart_debug_recv.uartRecBuff_index;
//        if (uart_debug_recv.uartRecBuff_index >= (UART_REC_SIZE - 8))
//            uart_debug_recv.uartRecBuff_index = 0;
//		memset(uart_debug_recv.uartDMARecBuff,0,UART_DMA_REC_SIZE);
//    }
//
//	  HAL_UART_DMAStop(&huart3);
//    HAL_UART_DMAResume(&huart3);
//    HAL_UART_Receive_DMA(&huart3, (uint8_t *)uart_debug_recv.uartDMARecBuff,
//                         UART_DMA_REC_SIZE);
//}
void uart_lcd_recv_proc()
{
    
     if(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
    {
			uart_lcd_recv.uartRecLen  = UART_CDG_DMA_REC_SIZE - __HAL_DMA_GET_COUNTER(
                                    &hdma_usart1_rx);
    //__HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_IDLEF);
        memcpy(uart_lcd_recv.uartRecBuff,
               uart_lcd_recv.uartDMARecBuff, UART_CDG_DMA_REC_SIZE);
        uart_lcd_recv.uartRecFlag = 1;
        //uart_cdg_recv.uartRecBuff_index = uart_cdg_recv.uartRecLen +
         //                                 uart_cdg_recv.uartRecBuff_index;
        //if (uart_cdg_recv.uartRecBuff_index >= (UART_REC_SIZE - 8))
        //    uart_cdg_recv.uartRecBuff_index = 0;
	  HAL_UART_DMAStop(&huart1);
    HAL_UART_DMAResume(&huart1);
    HAL_UART_Receive_DMA(&huart1, (uint8_t *)uart_lcd_recv.uartDMARecBuff,
                         UART_CDG_DMA_REC_SIZE);    
		}

}
void uart_air_recv_proc()
{

     if(__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != 0)
    {
			uart_air_recv.uartRecLen  = UART_DMA_REC_SIZE - __HAL_DMA_GET_COUNTER(
                                      &hdma_usart2_rx);
        memcpy(uart_air_recv.uartRecBuff,
               uart_air_recv.uartDMARecBuff, uart_air_recv.uartRecLen);
        uart_air_recv.uartRecFlag = 1;
        uart_air_recv.uartRecBuff_index = uart_air_recv.uartRecLen +
                                            uart_air_recv.uartRecBuff_index;
        if (uart_air_recv.uartRecBuff_index >= (UART_REC_SIZE - 8))
            uart_air_recv.uartRecBuff_index = 0;
    }
    HAL_UART_DMAStop(&huart2);
    HAL_UART_DMAResume(&huart2);
    HAL_UART_Receive_DMA(&huart2, (uint8_t *)uart_air_recv.uartDMARecBuff,
                         UART_DMA_REC_SIZE);
}
void uart_rf_recv_proc()
{

     if(__HAL_UART_GET_FLAG(&huart4, UART_FLAG_IDLE) != 0)
    {
			uart_rf_recv.uartRecLen  = UART_DMA_REC_SIZE - __HAL_DMA_GET_COUNTER(
                                      &hdma_uart5_rx);
        memcpy(uart_rf_recv.uartRecBuff,
               uart_rf_recv.uartDMARecBuff, uart_rf_recv.uartRecLen);
        uart_rf_recv.uartRecFlag = 1;
        uart_rf_recv.uartRecBuff_index = uart_rf_recv.uartRecLen +
                                            uart_rf_recv.uartRecBuff_index;
        if (uart_rf_recv.uartRecBuff_index >= (UART_REC_SIZE - 8))
            uart_rf_recv.uartRecBuff_index = 0;
    }
    HAL_UART_DMAStop(&huart5);
    HAL_UART_DMAResume(&huart5);
    HAL_UART_Receive_DMA(&huart5, (uint8_t *)uart_rf_recv.uartDMARecBuff,
                         UART_DMA_REC_SIZE);
}

