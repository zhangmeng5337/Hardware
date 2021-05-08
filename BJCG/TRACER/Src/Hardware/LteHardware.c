#include "LteHardware.h"
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;


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

void LteUartConfig()
{
    __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    __HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_DMAStop(&huart1);
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	//使能空闲中断
    HAL_UART_Receive_DMA(&huart1,nbiotUart.nbiotRxBuffer,NBIOT_BUFFER_SIZE);

	}

