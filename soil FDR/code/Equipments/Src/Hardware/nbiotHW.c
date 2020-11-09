#include "nbiotHW.h"
nbiotUart_stru nbiotUart;


void NbiotUartInit()
{
	UART_HandleTypeDef huart1;
	__HAL_UART_CLEAR_IDLEFLAG(&huart1);
	__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_DMAStop(&huart1);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_Receive_DMA(&huart1,nbiotUart.nbiotRxBuffer,NBIOT_BUFFER_SIZE);
}
void NbiotUart_CLR_Buf(void)                           // 串口缓存清理
{
  memset(nbiotUart.nbiotRxBuffer, 0, NBIOT_BUFFER_SIZE);      //清空                  
}
void NbiotUart_SendStr(u8 *str)
{
  unsigned char len;
	UART_HandleTypeDef huart1;
  len = strlen((const char*)str);
  HAL_UART_Transmit(&huart1, str, len, 1000);
}
void nbiot_HardwareInit(unsigned char flag)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	
	 /* GPIO Ports Clock Enable */
	 __HAL_RCC_GPIOC_CLK_ENABLE();

	

	
	 /*Configure GPIO pin Output Level */
	 HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_SET);
	 GPIO_InitStruct.Pin = SIM_PWR_Pin;
	 GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	 GPIO_InitStruct.Pull = GPIO_NOPULL;
	 GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	 HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  
  if(flag == ON)
  {
	HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_SET);
	HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_RESET);                
	//delay_ms(1500);
	//HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_SET);


  }
  else
  {
	  HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_RESET);
	  HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_SET);
	  HAL_Delay(1000);
	  HAL_GPIO_WritePin(GPIOC, SIM_PWR_Pin, GPIO_PIN_RESET); 

  }
  
}
void Nbiot_RxCpltCallback()
{
    unsigned char temp;
	{
		UART_HandleTypeDef huart1;
		DMA_HandleTypeDef hdma_usart1_rx;

		temp=__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);
			temp=huart1.Instance->SR;
			temp=huart1.Instance->DR;

			nbiotUart.receivedFlag = 1;
			HAL_UART_DMAStop(&huart1);
			HAL_UART_Receive_DMA(&huart1,nbiotUart.nbiotRxBuffer,NBIOT_BUFFER_SIZE);
		}
	}
}
unsigned char *NbiotFrameStatus(void)
{
	return &(nbiotUart.receivedFlag);
}
unsigned char *NbiotFrameBuffer(void)
{
	return nbiotUart.nbiotRxBuffer;

}

void Nbiot_SendData(u8 *buff,u8 len)
{
	UART_HandleTypeDef huart1;
        HAL_UART_Transmit(&huart1,buff,len,100);

}


