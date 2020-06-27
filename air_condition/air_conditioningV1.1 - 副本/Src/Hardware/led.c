#include "led.h"
#include "main.h"

void led_ctrl(unsigned char num,unsigned char on_off)
{
	if(on_off == ON)
	{
			switch(num)
			{
				case WORK_STATUS: HAL_GPIO_WritePin(led_status_GPIO_Port, led_status_Pin,GPIO_PIN_SET);break;
				case HIGH_LEVEL:  HAL_GPIO_WritePin(led_ctrl1_GPIO_Port, led_ctrl1_Pin,GPIO_PIN_SET);break;
				case LOW_LEVEL:   HAL_GPIO_WritePin(led_ctrl2_GPIO_Port, led_ctrl2_Pin,GPIO_PIN_SET);break;		
				case LED_ALL:   
													HAL_GPIO_WritePin(led_status_GPIO_Port, led_status_Pin,GPIO_PIN_SET);
													HAL_GPIO_WritePin(led_ctrl2_GPIO_Port, led_ctrl2_Pin,GPIO_PIN_SET);
													HAL_GPIO_WritePin(led_ctrl1_GPIO_Port, led_ctrl1_Pin,GPIO_PIN_SET);
													break;					
			}
	}
	else
	{
			switch(num)
			{
				case WORK_STATUS:   HAL_GPIO_WritePin(led_status_GPIO_Port, led_status_Pin,GPIO_PIN_RESET);break;
				case HIGH_LEVEL:    HAL_GPIO_WritePin(led_ctrl1_GPIO_Port, led_ctrl1_Pin,GPIO_PIN_SET);break;
				case LOW_LEVEL:     HAL_GPIO_WritePin(led_ctrl2_GPIO_Port, led_ctrl2_Pin,GPIO_PIN_SET);break;		
				case LED_ALL:   
					                  HAL_GPIO_WritePin(led_status_GPIO_Port, led_status_Pin,GPIO_PIN_RESET);
				                    HAL_GPIO_WritePin(led_ctrl2_GPIO_Port, led_ctrl2_Pin,GPIO_PIN_RESET);
				                    HAL_GPIO_WritePin(led_ctrl1_GPIO_Port, led_ctrl1_Pin,GPIO_PIN_RESET);
				                     break;				
			}	
	}
}
