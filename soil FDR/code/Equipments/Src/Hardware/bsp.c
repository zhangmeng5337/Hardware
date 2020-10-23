#include "bsp.h"

extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;

//uart_set_stru uart_set;

/***********************************************************
串口参数设置
***********************************************************/
void uart_init(uart_set_stru uartx)
{
	UART_HandleTypeDef uart_No;
	switch(uartx.uart_Num)
	{
		case 1: uart_No = huart1;break;
		case 2: uart_No = huart2;break;		
		case 3: uart_No = huart3;break;
		case 4: uart_No = huart5;break;
		case 5: uart_No = huart6;break;		
		default: break;
	}  
	
	switch(uartx.uart_Num)
	{
		case 1: uart_No.Instance = USART1;break;
		case 2: uart_No.Instance = USART2;break;		
		case 3: uart_No.Instance = USART3;break;
		case 4: uart_No.Instance = UART5;break;
		case 5: uart_No.Instance = USART6;break;		
		default: break;
	}
	//huart1.Instance = USART1;
  uart_No.Init.BaudRate = uartx.uart_rate ;
  uart_No.Init.WordLength = UART_WORDLENGTH_8B;
  uart_No.Init.StopBits = UART_STOPBITS_1;
  uart_No.Init.Parity = UART_PARITY_NONE;
  uart_No.Init.Mode = UART_MODE_TX_RX;
  uart_No.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  uart_No.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&uart_No) != HAL_OK)
  {
    Error_Handler();
  }
}

void loraGpioSet(unsigned char p,unsigned char q)
{

	  uint32_t timeout;
	  timeout = HAL_GetTick();
	  if(p == 0)
		{
			while(HAL_GPIO_ReadPin(GPIOB,LORA_AUX_Pin)==0)
			{
				if((HAL_GetTick()-timeout)>=10000)	
					break;			
			}
			HAL_Delay(2);
			switch(q)
			{
				case 0: 	 
								HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
								break;	
				case 1: 	 
								HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_SET);
								break;	
				case 2: 	 
								HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
								break;	
				case 3: 	 
								HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_SET);
								break;	
				default:	
								HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
                break;				
			}
			
		}
    else
		{
			while(HAL_GPIO_ReadPin(GPIOB,LORA_AUX2_Pin)==0)
			{
				if((HAL_GetTick()-timeout)>=10000)	
					break;			
			}
			HAL_Delay(2);
			switch(q)
			{
				case 0: 	 
								HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);
								break;	
				case 1: 	 
								HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_SET);
								break;	
				case 2: 	 
								HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);
								break;	
				case 3: 	 
								HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_SET);
								HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_SET);
								break;	
				default:	
								HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
								HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);
                break;				
			}
			
		}
	
}
void loratxdata(unsigned char p,unsigned char *q,unsigned char len)
{
	  uint32_t timeout;
	  timeout = HAL_GetTick();
	  if(p == 0)
		{
			while(HAL_GPIO_ReadPin(GPIOB,LORA_AUX_Pin)==0)
			{
				if((HAL_GetTick()-timeout)>=10000)	
					break;			
			}
			HAL_Delay(2);
			HAL_UART_Transmit(&huart2, q, len, 1000);
		}
    else
		{
			while(HAL_GPIO_ReadPin(GPIOB,LORA_AUX2_Pin)==0)
			{
				if((HAL_GetTick()-timeout)>=10000)	
					break;	
			}
			HAL_Delay(2);
			HAL_UART_Transmit(&huart6, q, len, 1000);
		}
}
