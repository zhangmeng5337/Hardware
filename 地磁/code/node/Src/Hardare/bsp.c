#include "main.h"
#include "bsp.h"

/*
*********************************************************************************************************
*	?? ?? ??: bsp_InitI2C
*	???????: ????I2C?????GPIO?????????IO???????
*	??    ??:  ??
*	?? ?? ?: ??
*********************************************************************************************************
*/

GPIO_InitTypeDef GPIO_InitStruct;
void bsp_InitI2C(void)
{
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
	
	  /*Configure GPIO pin : SCL_ADDR Pin */
  GPIO_InitStruct.Pin = I2C_ADD1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(I2C_ADD1_GPIO_Port, &GPIO_InitStruct);
	
  GPIO_InitStruct.Pin = I2C_ADD0_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(I2C_ADD0_GPIO_Port, &GPIO_InitStruct);	
	  /*Configure GPIO pin : I2C_MODE_Pin */	
  GPIO_InitStruct.Pin = I2C_MODE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(I2C_MODE_GPIO_Port, &GPIO_InitStruct);	
	
  HAL_GPIO_WritePin(I2C_ADD0_GPIO_Port, I2C_ADD0_Pin, GPIO_PIN_RESET);	
  HAL_GPIO_WritePin(I2C_ADD1_GPIO_Port, I2C_ADD1_Pin, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(I2C_MODE_GPIO_Port, I2C_MODE_Pin, GPIO_PIN_RESET);
 
  /*Configure GPIO pin : SCL_Pin */
  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SCL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SDA_Pin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);

	/* ??????????, ??竹I2C????????????蘟???????? */
	
}

void I2C1_SCL_IN() 					
{  
	GPIO_InitStruct.Pin = SCL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}   /* PB9???車????? */
void I2C1_SCL_OUT() 				
{
	GPIO_InitStruct.Pin = SCL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}		/* PB9???車???? */
void I2C1_SDA_IN() 					
{ 
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}   /* PB9???車????? */
void I2C1_SDA_OUT() 				
{  
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}		/* PB9???車???? */

void Hardware_Init()
{
	bsp_InitI2C();                // PIC configurations + i2c,spi,uart,timers,interruptions inicializations
}
