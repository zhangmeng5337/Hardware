#include "loraHW.h"
#include "loraHAL.h"
extern loraUart_stru loraUart;


void loraUartInterrupt(LORAHW_stru *p)
{
	if(p->loraNo == 0)
	{
		UART_HandleTypeDef huart2;
		if(p->enable == 0)
		{
			
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);	
			__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
			HAL_UART_DMAStop(&huart2);
			HAL_UART_Receive_DMA(&huart2,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);
			__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断		
		}
		else
		{
			
			__HAL_UART_CLEAR_IDLEFLAG(&huart2);	
			__HAL_UART_DISABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断
			HAL_UART_DMAStop(&huart2);
			//HAL_UART_Receive_DMA(&huart2,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);
		//	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断			
		}

	}
	else
	{
	  UART_HandleTypeDef huart6;
		if(p->enable == 0)
		{
		
			__HAL_UART_CLEAR_IDLEFLAG(&huart6);	
			__HAL_UART_DISABLE_IT(&huart6, UART_IT_IDLE);	//使能空闲中断
			HAL_UART_DMAStop(&huart6);
			HAL_UART_Receive_DMA(&huart6,loraUart.lora2RxBuffer,LORA_BUFFER_SIZE);
			__HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);	//使能空闲中断		
		}
		else
		{
			
			__HAL_UART_CLEAR_IDLEFLAG(&huart6);	
			__HAL_UART_DISABLE_IT(&huart6, UART_IT_IDLE);	//使能空闲中断
			HAL_UART_DMAStop(&huart6);
			//HAL_UART_Receive_DMA(&huart2,loraUart.lora1RxBuffer,LORA_BUFFER_SIZE);
		//	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);	//使能空闲中断			
		}
	}
}
/*******************************************************
功能描述：lora串口设置
返回值：
*********************************************************/
void loraUartSet(LORAHW_stru *p)
{
	if(p->loraNo == 0)
	{
			UART_HandleTypeDef huart2;
			huart2.Instance = USART2;
			huart2.Init.BaudRate = p->baudrate;
			huart2.Init.WordLength = UART_WORDLENGTH_8B;
			huart2.Init.StopBits = UART_STOPBITS_1;
			huart2.Init.Parity = UART_PARITY_NONE;
			huart2.Init.Mode = UART_MODE_TX_RX;
			huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
			huart2.Init.OverSampling = UART_OVERSAMPLING_16;
			switch(p->Parity)
			{
			case 0:
					huart2.Init.Parity=UART_PARITY_NONE;
					break;//无校验
			case 1:
					huart2.Init.Parity=UART_PARITY_ODD;
					break;//奇校验
			case 2:
					huart2.Init.Parity=UART_PARITY_EVEN;
					break;//偶校验
			}
			if (HAL_UART_Init(&huart2) != HAL_OK)
			{
					Error_Handler();
			}
		
	}
	else
	{
				UART_HandleTypeDef huart6;
			huart6.Instance = USART6;
			huart6.Init.BaudRate = p->baudrate;
			huart6.Init.WordLength = UART_WORDLENGTH_8B;
			huart6.Init.StopBits = UART_STOPBITS_1;
			huart6.Init.Parity = UART_PARITY_NONE;
			huart6.Init.Mode = UART_MODE_TX_RX;
			huart6.Init.HwFlowCtl = UART_HWCONTROL_NONE;
			huart6.Init.OverSampling = UART_OVERSAMPLING_16;
			switch(p->Parity)
			{
			case 0:
					huart6.Init.Parity=UART_PARITY_NONE;
					break;//无校验
			case 1:
					huart6.Init.Parity=UART_PARITY_ODD;
					break;//奇校验
			case 2:
					huart6.Init.Parity=UART_PARITY_EVEN;
					break;//偶校验
			}
			if (HAL_UART_Init(&huart6) != HAL_OK)
			{
					Error_Handler();
			}
	}

}
/*******************************************************
功能描述：根据aux引脚判断lora模块是否是空闲状态
返回值：
        0：空闲
				1:非空闲
*********************************************************/
unsigned char loraIdle(LORAHW_stru *p)
{
	uint32_t idleTimeout;
		if(p->loraNo == 0)
	{
		idleTimeout = HAL_GetTick();
		while(HAL_GPIO_ReadPin(GPIOB,LORA_AUX_Pin)==0)
		{
			if((HAL_GetTick()-idleTimeout)>=3000)
				return 1;
		}
		return 0;
	}
	else
	{
		idleTimeout = HAL_GetTick();
		while(HAL_GPIO_ReadPin(GPIOB,LORA_AUX_Pin)==0)
		{
			if((HAL_GetTick()-idleTimeout)>=3000)
				return 1;
		}
		return 0;
	}
}
/*******************************************************
功能描述：lora工作模式切换
参数:
					p->mode:工作模式
          p->loraNo：lora模块选择
返回值：
        0：切换成功
				1:切换不成功
*********************************************************/
unsigned char  loraGpioset(LORAHW_stru *p)
{

	if(p->loraNo == 0)
	{
    if(loraIdle(p) == 0)
		{
			switch(p->mode)
			{
				case TransmitMode:
							HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
							break;
				case WorMOde:
							HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_SET);
							 break;
				case ConfigMOde:
							HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_SET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
							break;
				case SleepMode:
							HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_SET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_SET);
							break;
				default:
							HAL_GPIO_WritePin(GPIOB, LORA_M1_Pin, GPIO_PIN_RESET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M0_Pin, GPIO_PIN_RESET);
							break;
      				
		 }
		 return 0;
		}
    else  
      return 1;			
	}
	else
	{
    if(loraIdle(p) == 0)
		{
			switch(p->mode)
			{
				case TransmitMode:
							HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
							break;
				case WorMOde:
							HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_SET);
							 break;
				case ConfigMOde:
							HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_SET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
							break;
				case SleepMode:
							HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_SET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_SET);
							break;
				default:
							HAL_GPIO_WritePin(GPIOB, LORA_M12_Pin, GPIO_PIN_RESET);	
							HAL_GPIO_WritePin(GPIOB, LORA_M02_Pin, GPIO_PIN_RESET);
							break;			
			}	
			return 0;
		}
	else
		return 1;		
	}
}
/*******************************************************
功能描述：lora发送数据
参数:
					p->mode:工作模式
          p->loraNo：lora模块选择
					buffer:待发送数据
					len：待发送数据长度
返回值：
        0：切换成功
				1:切换不成功
*********************************************************/
unsigned char loraSend(LORAHW_stru *p,unsigned char *buffer,unsigned int len)
{
	UART_HandleTypeDef huart2,huart6;
	
	if(p->loraNo == 0)
	{
		if(loraGpioset(p) == 0)
		{
			HAL_UART_Transmit(&huart2,buffer,len,1000);
		}
		return 0;
	}
	else
	{
			if(loraGpioset(p) == 0)
		{
			HAL_UART_Transmit(&huart6,buffer,len,1000);
		}
		return 0;
	}
	return 1;
}
