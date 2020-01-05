#include "stdio.h"
#include "main.h"
#include "bsp.h"
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
uart_stru uart1;
uart_stru uart2;
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
  
  GPIO_InitStruct.Pin = RM_POWER_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RM_POWER_GPIO_Port, &GPIO_InitStruct);
  
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
	

  /*Configure GPIO pin : SCL_Pin */
  GPIO_InitStruct.Pin = SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SCL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SDA_Pin */
  GPIO_InitStruct.Pin = SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);

	HAL_GPIO_WritePin(I2C_ADD0_GPIO_Port, I2C_ADD0_Pin, GPIO_PIN_RESET);	
	HAL_GPIO_WritePin(I2C_ADD1_GPIO_Port, I2C_ADD1_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(I2C_MODE_GPIO_Port, I2C_MODE_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RM_POWER_GPIO_Port, RM_POWER_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SDA_GPIO_Port, SDA_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SCL_GPIO_Port, SCL_Pin, GPIO_PIN_SET);

	/* ??????????, ??λI2C????????????豸???????? */
	
}

void I2C1_SCL_IN() 					
{  
	GPIO_InitStruct.Pin = SCL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}   /* PB9???ó????? */
void I2C1_SCL_OUT() 				
{
	GPIO_InitStruct.Pin = SCL_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}		/* PB9???ó???? */
void I2C1_SDA_IN() 					
{ 
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}   /* PB9???ó????? */
void I2C1_SDA_OUT() 				
{  
	GPIO_InitStruct.Pin = SDA_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(SDA_GPIO_Port, &GPIO_InitStruct);
}		/* PB9???ó???? */
void led_ctrl(unsigned char flag)
{
	switch(flag)
	{
		case ON:HAL_GPIO_WritePin(GPIOE, LED_Pin, GPIO_PIN_RESET);break;
		case OFF:HAL_GPIO_WritePin(GPIOE, LED_Pin, GPIO_PIN_SET);break;
		case BLINK:HAL_GPIO_TogglePin(GPIOE, LED_Pin);break;
	}
}

uint32_t Get_Battery_Gas_adc(unsigned char count)

{
	uint32_t adc_value_tmp;
	unsigned char i,j;
        uint32_t tmp;

	for(j=0; j<count; j++)
	{
		for(i=0; i<2; i++)
		{

			/*##-3- Start the conversion process ######*/
			if(HAL_ADC_Start(&hadc1) != HAL_OK)
			{ Error_Handler(); }
			/*##-4- Wait for the end of conversion ######*/
			HAL_ADC_PollForConversion(&hadc1, 10);
			if(i==0)
				tmp=HAL_ADC_GetValue(&hadc1)+tmp;
		}


	}
	 tmp=tmp/count;
	 tmp=tmp/4096*33;
	 adc_value_tmp=(uint32_t) tmp;
	HAL_ADC_Stop(&hadc1);
	return adc_value_tmp;
}


//串口接收空闲中断
void UsartReceive_IDLE(unsigned char uart_num)
{
	uint32_t temp;
	if(uart_num==1)
	{
		temp=__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE);
		if((temp!=RESET))
		{
			__HAL_UART_CLEAR_IDLEFLAG(&huart1);

			//HAL_UART_DMAStop(&huart2);
			temp=huart1.Instance->ISR;
			temp=huart1.Instance->RDR;
			//HAL_UART_DMAResume(&huart2);
			HAL_UART_DMAPause(&huart1);
			temp=hdma_usart1_rx.Instance->CNDTR;
			uart1.receive_len=uart1.receive_len+UARTBUFFERSIZE-temp;
			uart1.receive_flag=1;
			uart1.index=uart1.index+UARTBUFFERSIZE-temp;
			if(uart1.index>=UARTBUFFERSIZE)
				uart1.index=uart1.index-UARTBUFFERSIZE;
			HAL_UART_DMAPause(&huart1);
			HAL_UART_Receive_DMA(&huart1,uart1.receive_buffer,UARTBUFFERSIZE);
		}
	}
	if(uart_num==2)
	{
		temp=__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE);
		if((temp!=RESET))
		{led_ctrl(BLINK);
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);

			//HAL_UART_DMAStop(&huart2);
			temp=huart2.Instance->ISR;
			temp=huart2.Instance->RDR;
			//HAL_UART_DMAResume(&huart2);
		//	HAL_UART_DMAPause(&huart2);
			temp=hdma_usart2_rx.Instance->CNDTR;
			uart2.receive_len=uart2.receive_len+UARTBUFFERSIZE-temp;
			
			uart2.receive_flag=1;
			uart2.index=uart2.index+UARTBUFFERSIZE-temp;
			if(uart2.index>=UARTBUFFERSIZE)
				uart2.index=uart2.index-UARTBUFFERSIZE;
			HAL_UART_DMAStop(&huart2);
			HAL_UART_Receive_DMA(&huart2,uart2.receive_buffer,UARTBUFFERSIZE);
		}
	}
	
}
#pragma import(__use_no_semihosting)
void _sys_exit(int x)
{
x = x;
}
struct __FILE
{
int handle;
};
FILE __stdout;

int fputc(int ch, FILE *f)
{
				//发送数据到串口
	unsigned char tmp;
	tmp =(unsigned char )ch;
 HAL_UART_Transmit(&huart1, &tmp, 1, 1);
	return ch;
}

void Hardware_Init()
{
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);	
	__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
	HAL_UART_DMAStop(&huart2);
		HAL_UART_Receive_DMA(&huart2,uart2.receive_buffer,UARTBUFFERSIZE);
		__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
	//bsp_InitI2C();                // PIC configurations + i2c,spi,uart,timers,interruptions inicializations
}
void Hardware_test()
{
	led_ctrl(BLINK);
	HAL_Delay(2000);
	printf("uart1 test\n");
}

