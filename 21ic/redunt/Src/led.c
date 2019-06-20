	#include "led.h"
#include "stm32f0xx_hal.h"
#include "main.h"

uint32_t sys_tick_counter1,sys_tick_counter2;
/*
	blink :
	        0,led always on
	        1,led always off
	        2,led toggle status at 3hz
*/
void led_ctrl(unsigned char blink,unsigned char ledNo)
{
       unsigned char block_flag1,block_flag2;
	if(ledNo ==LED_ADC_STATUS)
	{
		switch(blink)
		{
			case 0:
				{
					HAL_GPIO_WritePin(led_adc_status_GPIO_Port,led_adc_status_Pin, GPIO_PIN_RESET);
					block_flag1 = 0;
			        }break;
			case 1:
				{
					HAL_GPIO_WritePin(led_adc_status_GPIO_Port,led_adc_status_Pin, GPIO_PIN_SET);
					block_flag1 = 0;
			        }break;
			case 2:
				{     
					
					
					if(block_flag1 == 0)
					{     
					       block_flag1 = 1;
						HAL_GPIO_TogglePin(led_adc_status_GPIO_Port,led_adc_status_Pin);
						sys_tick_counter1 = HAL_GetTick();
					}
					else
					{
						if((HAL_GetTick() - sys_tick_counter1) >= DELAY_TIME)
							block_flag1 = 0;
					}
					
			        }break;
		}
	}
if(ledNo ==LED_MASTER_STATUS)
	{
		switch(blink)
		{
			case 0:
				{
					HAL_GPIO_WritePin(led_master_GPIO_Port,led_master_Pin, GPIO_PIN_RESET);
					block_flag2 = 0;
			        }break;
			case 1:
				{
					HAL_GPIO_WritePin(led_master_GPIO_Port,led_master_Pin, GPIO_PIN_SET);
					block_flag2 = 0;
			        }break;
			case 2:
				{     
					
					
					if(block_flag2 == 0)
					{     
					       block_flag2 = 1;
						HAL_GPIO_TogglePin(led_master_GPIO_Port,led_master_Pin);
						sys_tick_counter2 = HAL_GetTick();
					}
					else
					{
						if((HAL_GetTick() - sys_tick_counter2) >= (DELAY_TIME*3))
							block_flag2 = 0;
					}
					
			        }break;
		}
	}
}
