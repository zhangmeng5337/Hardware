#include "uart.h"
tsLpuart1type Lpuart1type;

extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart1;

extern DMA_HandleTypeDef hdma_uart3_rx;
extern DMA_HandleTypeDef hdma_uart2_rx;
extern DMA_HandleTypeDef hdma_usart1_rx;


//rs485_stru lte_str;

rs485_stru rs485A_str;
rs485_stru rs485B_str;

uint8_t Usart1RX = 0;
tsLpuart1type Lpuart1type;



void uart_init()
{

    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, Lpuart1type.Lpuart1DMARecBuff, LPUART1_DMA_REC_SIZE);

    HAL_GPIO_WritePin(485A_EN_GPIO_Port, 485A_EN_Pin, GPIO_PIN_RESET);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart2, (uint8_t *)rs485A_str.recv_buf, RSBUFFER_SIZE);
    rs485A_str.recv_len = 0;

    HAL_GPIO_WritePin(485B_EN_GPIO_Port, 485B_EN_Pin, GPIO_PIN_RESET);
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart3, rs485B_str.recv_buf, RSBUFFER_SIZE);
    rs485A_str.recv_len = 0;

    //__HAL_UART_ENABLE_IT(&huart5, UART_IT_IDLE);
    //  HAL_UART_Receive_DMA(&huart5, (uint8_t*)lcd_str.buff, BUFFER_SIZE);
}

void uart_rs485A()
{
    uint32_t  uart_idle_flag;
    uart_idle_flag =  __HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE);
    // uart_idle_flag =  __HAL_UART_GET_FLAG(&huart4,UART_FLAG_ORE);

    if ((uart_idle_flag != RESET))
    {
        uart_idle_flag = 0;
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);

        uint32_t temp;
        temp = huart2.Instance->SR;
        temp = huart2.Instance->DR;
        temp = hdma_uart2_rx.Instance->NDTR;
        rs485A_str.recv_dma_len = RSBUFFER_SIZE - temp;



        //rs485_str.recv_len  = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
        if (rs485A_str.recv_dma_len > 1)
        {
            rs485A_str.recv_update = 1;
            //memcpy(&rs485_str.recv_buf[rs485_str.recv_len],rs485_str.recv_buf,rs485_str.recv_dma_len);
            rs485A_str.recv_len = rs485A_str.recv_dma_len;
        }
    }

    HAL_UART_DMAStop(&huart2);
    HAL_UART_DMAResume(&huart2);
    //memset((uint8_t*)rs485_str.buff,0,RSBUFFER_SIZE);
    HAL_UART_Receive_DMA(&huart2, (uint8_t *)rs485A_str.recv_buf, RSBUFFER_SIZE);
}
void uart_rs485B()
{
    uint32_t  uart_idle_flag;
    uart_idle_flag =  __HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE);
    // uart_idle_flag =  __HAL_UART_GET_FLAG(&huart4,UART_FLAG_ORE);

    if ((uart_idle_flag != RESET))
    {
        uart_idle_flag = 0;
        __HAL_UART_CLEAR_IDLEFLAG(&huart3);

        uint32_t temp;
        temp = huart3.Instance->SR;
        temp = huart3.Instance->DR;
        temp = hdma_uart3_rx.Instance->NDTR;
        rs485B_str.recv_dma_len = RSBUFFER_SIZE - temp;



        //rs485_str.recv_len  = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);
        if (rs485B_str.recv_dma_len > 1)
        {
            rs485B_str.recv_update = 1;
            //memcpy(&rs485_str.recv_buf[rs485_str.recv_len],rs485_str.recv_buf,rs485_str.recv_dma_len);
            rs485B_str.recv_len = rs485B_str.recv_dma_len;
        }
    }

    HAL_UART_DMAStop(&huart3);
    HAL_UART_DMAResume(&huart3);
    //memset((uint8_t*)rs485_str.buff,0,RSBUFFER_SIZE);
    HAL_UART_Receive_DMA(&huart3, (uint8_t *)rs485B_str.recv_buf, RSBUFFER_SIZE);
}

tsLpuart1type *get_lte_recv()
{
    return &Lpuart1type;
}



void uart_transmit(unsigned char uart_num, uint8_t *pData, uint16_t Size)
{
    switch (uart_num)
    {
        case LTE_No:
            HAL_UART_Transmit(&huart1, pData, Size, 100);
            break;
        case RS485A_No:
        {
            HAL_GPIO_WritePin(485A_EN_GPIO_Port, 485A_EN_Pin, GPIO_PIN_SET);
            HAL_UART_Transmit(&huart2, pData, Size, 100);
            HAL_Delay(3);

            __HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);
            HAL_GPIO_WritePin(485A_EN_GPIO_Port, 485A_EN_Pin, GPIO_PIN_RESET);
            HAL_Delay(1);
            __HAL_UART_CLEAR_IDLEFLAG(&huart2);
            __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
            HAL_UART_DMAStop(&huart2);
            HAL_UART_DMAResume(&huart2);
            HAL_UART_Receive_DMA(&huart2, (uint8_t *)rs485A_str.recv_buf, RSBUFFER_SIZE);
            rs485A_str.recv_len = 0;
        }
        break;
        case RS485B_No:
        {
            HAL_GPIO_WritePin(485B_EN_GPIO_Port, 485B_EN_Pin, GPIO_PIN_SET);
            HAL_UART_Transmit(&huart3, pData, Size, 100);
            HAL_Delay(3);

            __HAL_UART_DISABLE_IT(&huart3, UART_IT_IDLE);
            HAL_GPIO_WritePin(485B_EN_GPIO_Port, 485B_EN_Pin, GPIO_PIN_RESET);
            HAL_Delay(1);
            __HAL_UART_CLEAR_IDLEFLAG(&huart3);
            __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
            HAL_UART_DMAStop(&huart3);
            HAL_UART_DMAResume(&huart3);
            HAL_UART_Receive_DMA(&huart3, (uint8_t *)rs485B_str.recv_buf, RSBUFFER_SIZE);
            rs485B_str.recv_len = 0;
        }
        break;

        default :
            ;
            break;
    }

}

rs485_stru *get_uart_recv(unsigned char uart_num)
{
    switch (uart_num)
    {
        //case LTE_No:	return &lte_str;
        case RS485A_No:
            return &rs485A_str;
        case RS485B_No:
            return &rs485B_str;

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

    memmove(&Lpuart1type.Lpuart1RecBuff[Lpuart1type.Lpuart1RecLen],
            Lpuart1type.Lpuart1DMARecBuff,
            Lpuart1type.Lpuart1DMARecLen);
    Lpuart1type.Lpuart1RecLen += Lpuart1type.Lpuart1DMARecLen;
    last_index = last_index + Lpuart1type.Lpuart1DMARecLen;

    HAL_UART_DMAResume(&huart1);
    HAL_UART_Receive_DMA(&huart1, Lpuart1type.Lpuart1DMARecBuff, LPUART1_DMA_REC_SIZE);
    Lpuart1type.Lpuart1RecFlag = 1;
}

