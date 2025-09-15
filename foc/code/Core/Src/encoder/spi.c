//spi.c
/* USER CODE BEGIN 1 */
#include <stdio.h>
#include "motor_runtime_param.h"
#include "foc.h"	
#include "arm_math.h"
#include "crc.h"

extern SPI_HandleTypeDef hspi1;
uint8_t mt6701_rx_data[3];

void encoder_init()
{
	HAL_GPIO_WritePin(spi1_cs_GPIO_Port, spi1_cs_Pin, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive_DMA(&hspi1, mt6701_rx_data, mt6701_rx_data, 3);
	HAL_Delay(100);//延时一会，让角度变量被赋值，不然角度会是0

}
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
  if (hspi->Instance == SPI1)
  {
    HAL_GPIO_WritePin(spi1_cs_GPIO_Port, spi1_cs_Pin, GPIO_PIN_SET);
	if(crc6_itu(mt6701_rx_data,3) == (mt6701_rx_data[2]&0x3f))
	{
    get_motor_runtime()->angle_raw = (mt6701_rx_data[1] >> 2) | (mt6701_rx_data[0] << 6);
	
    get_motor_runtime()->encoder_angle = 2 * 3.1415926 * get_motor_runtime()->angle_raw / ((1 << 14) - 1);

	}
	static float encoder_angle_last = 0;
	  /****encoder_angle_last默认值是0，不能用于计算，要先赋值一次****/
	  static int once = 1;
	  if (once)
	  {
		once = !once;
		encoder_angle_last = get_motor_runtime()->encoder_angle;
	  }
	  /*************/
	  float _encoder_angle = get_motor_runtime()->encoder_angle;
	  // 角度差值，用于累计多圈逻辑角度
	  float diff_angle = cycle_diff(_encoder_angle - encoder_angle_last, 2 * PI);
	  encoder_angle_last = _encoder_angle;
	  // 实现周期操作，将motor_logic_angle转到周期内
	  get_motor_runtime()->motor_logic_angle = cycle_diff(get_motor_runtime()->motor_logic_angle + diff_angle, position_cycle);

	
    HAL_GPIO_WritePin(spi1_cs_GPIO_Port, spi1_cs_Pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive_DMA(&hspi1, mt6701_rx_data, mt6701_rx_data, 3);
    return;
  }
}

void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
  printf("HAL_SPI_ErrorCallback:%d\n", hspi->ErrorCode);
  if (hspi->Instance == SPI1)
  {
    HAL_GPIO_WritePin(spi1_cs_GPIO_Port, spi1_cs_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(spi1_cs_GPIO_Port, spi1_cs_Pin, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive_DMA(&hspi1, mt6701_rx_data, mt6701_rx_data, 3);
  }
  return;
}
/* USER CODE END 1 */

