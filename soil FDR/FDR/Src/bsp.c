#include "stdio.h"
#include "main.h"
#include "bsp.h"
extern UART_HandleTypeDef huart1;
#pragma import(__use_no_semihosting)
void _sys_exit(int x)
{
x = x;
}
struct __FILE
{
int handle;
};
FILE __stdout;

int fputc(int ch, FILE *f)
{
				//发送数据到串口
	unsigned char tmp;
	tmp =(unsigned char )ch;
 HAL_UART_Transmit(&huart1, &tmp, 1, 1);
	return ch;
}

void led_ctrl(uint32_t time)
{ 
	uint32_t ums;
	
  if(time>2)
	{
		if((ums-HAL_GetTick())>=time)
		{
			ums= HAL_GetTick();
			HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);	
		}
		else
			;	
	}
	else if(time == 0)
	{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);	
	}
	else if(time == 1)
	{
				HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);	
	}
	else if(time == 2)
	{
				HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);	
	}
}

