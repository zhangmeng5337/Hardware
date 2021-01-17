#ifndef __FM_H_
#define __FM_H_

#include "main.h"

#define FM_ON()		HAL_GPIO_WritePin(FM_GPIO_Port, FM_Pin, GPIO_PIN_SET);;
#define FM_OFF()	HAL_GPIO_WritePin(FM_GPIO_Port, FM_Pin, GPIO_PIN_RESET);;

void FM_GPIO_Config(void);		//≥ı ºªØGPIO

#endif /*__FM_H_*/
