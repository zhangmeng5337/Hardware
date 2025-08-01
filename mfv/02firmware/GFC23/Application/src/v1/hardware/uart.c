#include "uart.h"
#include "stdio.h"
#include "stdarg.h"


#include "cmd_queue.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;

uart_recv_stru uart_rs485_recv;
uart_recv_stru uart_cdg_recv;
uart_recv_stru uart_debug_recv;

uint8_t push_data;

struct __FILE
{
    int handle;
};
FILE __stdout;
int fputc(int ch, FILE *f) 
{
    // 将字符存储到缓冲区
    static uint8_t buffer[1024];
    static uint16_t index = 0;
 
    buffer[index++] = (uint8_t)ch;
 
    // 如果遇到换行符或缓冲区满，则启动DMA传输
    if (ch == '\n' || index == sizeof(buffer)) {
		 uart_transmit(UART_485, buffer, index);
       
        index = 0; // 重置索引
    }
 
    return ch;
}




//int fputc(int ch, FILE *f)
//{
//    unsigned char tmp;
//    tmp = (unsigned char)ch;
//    if (COM_TYPE == RS_232)
//        HAL_UART_Transmit(&huart3, &tmp, 1, 500);
//    else
//        uart_transmit(UART_485, &tmp, 1);
//    return ch;
//}
//extern   uint32_t tick;
void uart_init()
{
    //rs485 uart init
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&huart2, UART_CLEAR_IDLEF);

    HAL_UART_Receive_DMA(&huart2, uart_rs485_recv.uartDMARecBuff,
                         UART_DMA_REC_SIZE);

    //rs232 uart init for cdg
   
    HAL_UART_Receive_IT(&huart1, &push_data, 1);
 __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);


    //rs232 uart init for debug

    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    __HAL_UART_CLEAR_FLAG(&huart3, UART_CLEAR_IDLEF);
    HAL_UART_Receive_DMA(&huart3, uart_debug_recv.uartDMARecBuff,
                         UART_DMA_REC_SIZE);
   // tick = 0;

}


/********************************************************
function: return uart recv data structure
params:
       uart_num = 0  ---->UART_CDG
       uart_num = 1  ---->UART_485
       uart_num = 2  ---->UART_DEBUG
********************************************************/
uart_recv_stru *get_uart_recv_stru(unsigned char uart_num)
{
    uart_recv_stru *recv_uart_stru;
    switch (uart_num)
    {
        case UART_CDG:
            recv_uart_stru = &uart_cdg_recv;
            break;
        case UART_485:
        {
            recv_uart_stru = &uart_rs485_recv;

        }
        break;
        case UART_DEBUG:
            recv_uart_stru = &uart_debug_recv;
            break;
        default :
            recv_uart_stru = &uart_debug_recv;
            break;
    }
    return  recv_uart_stru;
}

void uart_transmit(unsigned char uart_num, uint8_t *pData, uint16_t Size)
{
    switch (uart_num)
    {
        case UART_CDG:
            HAL_UART_Transmit(&huart1, pData, Size, 100);
            break;
        case UART_485:
        {
           // while(!__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC));
            HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);
//            HAL_UART_Transmit(&huart2, pData, Size, 500);
//            //HAL_Delay(50);
			HAL_UART_Transmit_DMA(&huart2, pData, Size);
//            while(!__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC));
//            HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);

        }
        break;
        case UART_DEBUG:
            HAL_UART_Transmit(&huart3, pData, Size, 100);
            break;
        default :
            ;
            break;
    }
}
void rs485_state_conv()
{
	if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_TC))
				HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_RESET);

}
void uart_debug_recv_proc()
{

    if (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) != 0)
    {
        uart_debug_recv.uartRecLen  = UART_DMA_REC_SIZE - __HAL_DMA_GET_COUNTER(
                                          &hdma_usart3_rx);
        __HAL_UART_CLEAR_FLAG(&huart3, UART_CLEAR_IDLEF);
        memcpy(uart_debug_recv.uartRecBuff,
               uart_debug_recv.uartDMARecBuff, uart_debug_recv.uartRecLen);
        uart_debug_recv.uartRecFlag = 1;
        uart_debug_recv.uartRecBuff_index = uart_debug_recv.uartRecLen +
                                            uart_debug_recv.uartRecBuff_index;
        if (uart_debug_recv.uartRecBuff_index >= (UART_REC_SIZE))
            uart_debug_recv.uartRecBuff_index = 0;
        memset(uart_debug_recv.uartDMARecBuff, 0, UART_DMA_REC_SIZE);
    }

    HAL_UART_DMAStop(&huart3);
    HAL_UART_DMAResume(&huart3);
    HAL_UART_Receive_DMA(&huart3, (uint8_t *)uart_debug_recv.uartDMARecBuff,
                         UART_DMA_REC_SIZE);
}
void uart_cdg_recv_proc()
{

//    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE))
//    {            uart_cdg_recv.uartRecLen  = UART_CDG_DMA_REC_SIZE - __HAL_DMA_GET_COUNTER(
//                                            &hdma_usart1_rx);
//            __HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_IDLEF);
//       // if (uart_cdg_recv.uartRecFlag == 0)
//        {
//                      if(uart_cdg_recv.uartDMARecBuff[0]==0x07)
//            memcpy(uart_cdg_recv.uartRecBuff,
//                   uart_cdg_recv.uartDMARecBuff, UART_CDG_DMA_REC_SIZE);
//            uart_cdg_recv.uartRecFlag = 1;
//        }

//        //uart_cdg_recv.uartRecBuff_index = uart_cdg_recv.uartRecLen +
//        //                                 uart_cdg_recv.uartRecBuff_index;
//        //if (uart_cdg_recv.uartRecBuff_index >= (UART_REC_SIZE - 8))
//        //    uart_cdg_recv.uartRecBuff_index = 0;
//        HAL_UART_DMAStop(&huart1);
//        HAL_UART_DMAResume(&huart1);
//        HAL_UART_Receive_DMA(&huart1, (uint8_t *)uart_cdg_recv.uartDMARecBuff,
//                             UART_CDG_DMA_REC_SIZE);
//    }


//if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE))
    {
        //  HAL_UART_Receive(&huart1, &push_data, 1, 10);

        //HAL_UART_Receive(&huart1, &push_data, 1, 1);
        queue_push(push_data);

        uart_cdg_recv.uartRecFlag = 1;
        HAL_UART_Receive_IT(&huart1, &push_data, 1);
        //__HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);

    }
//HAL_UART_Receive_IT(&huart1, &push_data, 1);
}
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // 处理接收到的数据
        // push_data = *(uint8_t *)huart->pRxBuffPtr;
        queue_push(push_data);

        uart_cdg_recv.uartRecFlag = 1;


        // 重新开启中断以继续接收数据
        HAL_UART_Receive_IT(&huart1, &push_data, 1);
    }
}


void uart_485_recv_proc()
{

    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != 0)
    {
        uart_rs485_recv.uartRecLen  = UART_DMA_REC_SIZE - __HAL_DMA_GET_COUNTER(
                                          &hdma_usart2_rx);
        __HAL_UART_CLEAR_FLAG(&huart2, UART_CLEAR_IDLEF);
        memcpy(uart_rs485_recv.uartRecBuff,
               uart_rs485_recv.uartDMARecBuff, uart_rs485_recv.uartRecLen);
        uart_rs485_recv.uartRecFlag = 1;
        uart_rs485_recv.uartRecBuff_index = uart_rs485_recv.uartRecLen +
                                            uart_rs485_recv.uartRecBuff_index;
        if (uart_rs485_recv.uartRecBuff_index >= (UART_REC_SIZE - 8))
            uart_rs485_recv.uartRecBuff_index = 0;
    }
    HAL_UART_DMAStop(&huart2);
    HAL_UART_DMAResume(&huart2);
    HAL_UART_Receive_DMA(&huart2, (uint8_t *)uart_rs485_recv.uartDMARecBuff,
                         UART_DMA_REC_SIZE);
}
