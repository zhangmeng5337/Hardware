#include "LteHardware.h"
#include "LteHal.h"

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;

extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart3_rx;
void LtePowerManage(unsigned char moduleType,unsigned char powerCtrl)
{
    if(moduleType == LTE_4G)
    {
        if(powerCtrl == ON)
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_SET);//lte电源使能

            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_RESET);//4g开机
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_SET);
            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_SET);//开gps
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_RESET);//lte电源禁止
            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_RESET);//gps禁止
        }
    }
    else if((moduleType == LTE_NBIOT))
    {
        if(powerCtrl == ON)
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_SET);//lte电源使能

            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_RESET);//4g开机
            HAL_GPIO_WritePin(GPIOA, SIM_PWR_Pin, GPIO_PIN_SET);
            HAL_Delay(1000);
            HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_RESET);

            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_RESET);//
        }
        else
        {
            HAL_GPIO_WritePin(GPIOB,EN_3V8_Pin, GPIO_PIN_RESET);//lte电源禁止
            HAL_GPIO_WritePin(GPIOC, POWER_CTRL_GPS_Pin, GPIO_PIN_RESET);//
        }

    }
    else
    {

    }
}

void LteUart_SendStr(unsigned char moduleType,u8 *str)
{
    unsigned char len;
    len = strlen((const char*)str);


    if(moduleType == LTE_4G)
    {

        HAL_UART_Transmit(&huart2, str, len, 1000);

    }
    else if((moduleType == LTE_NBIOT))
    {

        HAL_UART_Transmit(&huart2, str, len, 1000);
    }
    else
    {

    }

}

void LteUartConfig()
{
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_DMAStop(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_Receive_DMA(&huart1,GetLteStru()->lterxbuffer,BUFFERSIZE);

    __HAL_UART_CLEAR_IDLEFLAG(&huart2);
    __HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断

    HAL_UART_DMAStop(&huart2);
    __HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_Receive_DMA(&huart2,GetLteStru()->gpsrxbuffer,BUFFERSIZE);
}
void Lte_RxCpltCallback(unsigned char flag)
{
    unsigned char temp;

    temp=__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE);
    if(flag == 1)
    {
        if((temp!=RESET))
        {
            __HAL_UART_CLEAR_IDLEFLAG(&huart1);
            temp=huart1.Instance->ISR;
            temp=huart1.Instance->RDR;

            GetLteStru()->LteReceivedFlag = 1;
            HAL_UART_DMAStop(&huart1);
            GetLteStru()->rxSize = BUFFERSIZE -hdma_usart1_rx.Instance->CNDTR;
            HAL_UART_Receive_DMA(&huart1,GetLteStru()->lterxbuffer,BUFFERSIZE);
        }
    }
    else
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);
        temp=huart2.Instance->ISR;
        temp=huart2.Instance->RDR;
        GetLteStru()->rxSize = BUFFERSIZE -hdma_usart2_rx.Instance->CNDTR;
        GetLteStru()->GpsReceivedFlag = 1;
        HAL_UART_DMAStop(&huart2);
        HAL_UART_Receive_DMA(&huart2,GetLteStru()->gpsrxbuffer,BUFFERSIZE);
    }

}

