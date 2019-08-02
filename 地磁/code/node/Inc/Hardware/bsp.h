#ifndef BSP_H
#define BSP_H
#include "main.h"
/* ?????§ÕSCL??SDA??? */
#define I2C1_SCL_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SCL_Pin, GPIO_PIN_SET)				/* SCL = 1 */
#define I2C1_SCL_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)				/* SCL = 0 */

#define I2C1_SDA_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);				/* SDA = 1 */
#define I2C1_SDA_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);				/* SDA = 0 */

#define I2C1_SDA_READ()  HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)	/* ??SDA?????? */
#define I2C1_SCL_READ()  HAL_GPIO_ReadPin(SCL_GPIO_Port, SCL_Pin)		/* ??SCL?????? */



void I2C1_SCL_IN(void);
void I2C1_SCL_OUT(void);
void I2C1_SDA_IN(void);
void I2C1_SDA_OUT(void);
void bsp_InitI2C(void);
void Hardware_Init(void);


#endif
