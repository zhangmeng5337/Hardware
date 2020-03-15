#include "io.h"

void device_ctrl(unsigned char devNo,unsigned char on_off)
{

	if(on_off == ON)
	{
		switch(devNo)
		{
			case FAN_HOT:HAL_GPIO_WritePin(CTR_OUT1_GPIO_Port, CTR_OUT1_Pin,GPIO_PIN_SET);break;
			case FAN_COOL:HAL_GPIO_WritePin(CTR_OUT2_GPIO_Port, CTR_OUT2_Pin,GPIO_PIN_SET);break;
			case PUMP:HAL_GPIO_WritePin(CTR_OUT3_GPIO_Port, CTR_OUT3_Pin,GPIO_PIN_SET);break;
		}
	}
	else
	{
		switch(devNo)
		{
			case FAN_HOT:HAL_GPIO_WritePin(CTR_OUT1_GPIO_Port, CTR_OUT1_Pin,GPIO_PIN_RESET);break;
			case FAN_COOL:HAL_GPIO_WritePin(CTR_OUT2_GPIO_Port, CTR_OUT2_Pin,GPIO_PIN_RESET);break;
			case PUMP:HAL_GPIO_WritePin(CTR_OUT3_GPIO_Port, CTR_OUT3_Pin,GPIO_PIN_RESET);break;
		}
	}
}
GPIO_PinState get_io(unsigned char devNo)
{
      if(devNo == 1)
			 return HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin);
			if(devNo == 2)
				return HAL_GPIO_ReadPin(SW2_GPIO_Port, SW2_Pin);
}
