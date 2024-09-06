#ifndef __LCD__
#define __LCD__

#include "main.h"

#define LCD_CS_H  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET);
#define LCD_CS_L  HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET);

#define LCD_CLK_H  HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_SET);
#define LCD_CLK_L  HAL_GPIO_WritePin(LCD_CLK_GPIO_Port, LCD_CLK_Pin, GPIO_PIN_RESET);

#define LCD_SID_R  HAL_GPIO_ReadPin(LCD_SID_GPIO_Port, LCD_SID_Pin)
#define LCD_SID_H  HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, GPIO_PIN_SET);
#define LCD_SID_L  HAL_GPIO_WritePin(LCD_SID_GPIO_Port, LCD_SID_Pin, GPIO_PIN_RESET);
#endif


