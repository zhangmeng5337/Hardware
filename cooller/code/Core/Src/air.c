#include "air.h"

void air_init()
{
	HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_RESET);
}
void air_pwr_ctrl(unsigned char flag)
{
    if(flag == ON)
		HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_SET);

	else 
	HAL_GPIO_WritePin(air_pwr_GPIO_Port, air_pwr_Pin, GPIO_PIN_RESET);
}

