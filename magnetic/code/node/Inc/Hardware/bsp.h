
#ifndef BSP_H
#define BSP_H
#include "main.h"
#define DEBUG_LOG		1
#define ON	0
#define OFF    1
#define BLINK	2
#define UARTBUFFERSIZE		64
/* ?????��SCL??SDA??? */
#define I2C1_SCL_1()   HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET)				/* SCL = 1 */
#define I2C1_SCL_0()   HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_RESET)				/* SCL = 0 */

#define I2C1_SDA_1()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);				/* SDA = 1 */
#define I2C1_SDA_0()   HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_RESET);				/* SDA = 0 */

#define I2C1_SDA_READ()  HAL_GPIO_ReadPin(SDA_GPIO_Port, SDA_Pin)	/* ??SDA?????? */
#define I2C1_SCL_READ()  HAL_GPIO_ReadPin(SCL_GPIO_Port, SCL_Pin)		/* ??SCL?????? */

typedef struct{
unsigned char receive_flag;
unsigned int receive_len;
unsigned int read_len;
unsigned int index;
unsigned int read_index;
unsigned char receive_buffer[UARTBUFFERSIZE];
}uart_stru;
void led_ctrl(unsigned char flag);
void I2C1_SCL_IN(void);
void I2C1_SCL_OUT(void);
void I2C1_SDA_IN(void);
void I2C1_SDA_OUT(void);
void bsp_InitI2C(void);
void Hardware_Init(void);
void UsartReceive_IDLE(unsigned char uart_num);
uint32_t Get_Battery_Gas_adc(unsigned char count);

#endif

