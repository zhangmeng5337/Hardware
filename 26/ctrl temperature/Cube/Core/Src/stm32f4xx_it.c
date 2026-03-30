/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f4xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f4xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "cmsis_os.h"
#include "temp_pid.h"
#include "modbus_rtu.h"
#include "app_fifo.h"
#include "max31856.h"
#include "math.h"
int ru_cnt = 0;
int flag1 = 0;
int V_cnt=0;
//int sendbegin_flag=0;


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint32_t pw1=0;
uint32_t pw2=0;
uint32_t pw3=0;


/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;

extern DMA_HandleTypeDef hdma_uart4_rx;
extern DMA_HandleTypeDef hdma_uart4_tx;
extern DMA_HandleTypeDef hdma_uart5_rx;
extern DMA_HandleTypeDef hdma_uart5_tx;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern DMA_HandleTypeDef hdma_usart1_tx;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern DMA_HandleTypeDef hdma_usart2_tx;
extern DMA_HandleTypeDef hdma_usart3_rx;
extern DMA_HandleTypeDef hdma_usart3_tx;
extern DMA_HandleTypeDef hdma_usart6_rx;
extern DMA_HandleTypeDef hdma_usart6_tx;
extern UART_HandleTypeDef huart4;
extern UART_HandleTypeDef huart5;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern UART_HandleTypeDef huart3;
extern UART_HandleTypeDef huart6;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
/* USER CODE BEGIN EV */
extern osSemaphoreId uart1_SemHandle;
extern osSemaphoreId uart2_SemHandle;
extern osSemaphoreId TIM3_SemHandle;
extern osSemaphoreId uart4_SemHandle;
extern osSemaphoreId uart3_SemHandle;
extern osSemaphoreId uart5_SemHandle;
/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M4 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Pre-fetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/******************************************************************************/
/* STM32F4xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f4xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 stream0 global interrupt.
  */
void DMA1_Stream0_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream0_IRQn 0 */

  /* USER CODE END DMA1_Stream0_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart5_rx);
  /* USER CODE BEGIN DMA1_Stream0_IRQn 1 */

  /* USER CODE END DMA1_Stream0_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream1 global interrupt.
  */
void DMA1_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream1_IRQn 0 */

  /* USER CODE END DMA1_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_rx);
  /* USER CODE BEGIN DMA1_Stream1_IRQn 1 */

  /* USER CODE END DMA1_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream2 global interrupt.
  */
void DMA1_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream2_IRQn 0 */

  /* USER CODE END DMA1_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_rx);
  /* USER CODE BEGIN DMA1_Stream2_IRQn 1 */

  /* USER CODE END DMA1_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream3 global interrupt.
  */
void DMA1_Stream3_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream3_IRQn 0 */

  /* USER CODE END DMA1_Stream3_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart3_tx);
  /* USER CODE BEGIN DMA1_Stream3_IRQn 1 */

  /* USER CODE END DMA1_Stream3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream4 global interrupt.
  */
void DMA1_Stream4_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream4_IRQn 0 */

  /* USER CODE END DMA1_Stream4_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart4_tx);
  /* USER CODE BEGIN DMA1_Stream4_IRQn 1 */

  /* USER CODE END DMA1_Stream4_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream5 global interrupt.
  */
void DMA1_Stream5_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream5_IRQn 0 */

  /* USER CODE END DMA1_Stream5_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_rx);
  /* USER CODE BEGIN DMA1_Stream5_IRQn 1 */

  /* USER CODE END DMA1_Stream5_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream6 global interrupt.
  */
void DMA1_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream6_IRQn 0 */
  /* USER CODE END DMA1_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart2_tx);
  /* USER CODE BEGIN DMA1_Stream6_IRQn 1 */

  /* USER CODE END DMA1_Stream6_IRQn 1 */
}

/**
  * @brief This function handles TIM1 update interrupt and TIM10 global interrupt.
  */
void TIM1_UP_TIM10_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */
	if(ru_cnt<120&&flag1 ==0)
		ru_cnt++;
	else if(ru_cnt>=120&&tF>-1300&&tM>-1300)  //&&flag1 ==0
	{
		flag1 = 1;
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14,GPIO_PIN_SET);
	}
  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

/**
  * @brief This function handles TIM2 global interrupt.
  */
void TIM2_IRQHandler(void)
{
  /* USER CODE BEGIN TIM2_IRQn 0 */
	#if 0
	pw1++;
	pw2++;
	pw3++;

  if(pw1>=pid1.pwmcycle)  //
  {
	  pw1=0;
		//printf("aaaaa\n");
  }
	
	if(pw1<pid1.OUT-1)
		//if(pw1<250)
	 {
	   pwmout_0;//加热
	 }
	 else
	 {
	   pwmout_1;//停止加热
	 }
	 
	 
	 
	 
	 
	 if(pw2>=pid2.pwmcycle)  //
  {
	  pw2=0;
		//printf("aaaaa\n");
  }
	if(pw2<pid2.OUT -1)
	 {
			pwmout2_0;//加
	 }
	 else
	 {
			pwmout2_1;//加
	 }
	 
	 
	 
	 
	 if(pw3>=pid3.pwmcycle)  //
  {
	  pw3=0;
		//printf("aaaaa\n");
  } 
	if(pw3<pid3.OUT-1&&pw3<250)
	 {
			pwmout3_0;//加
	 }
	 else
	 {
			pwmout3_1;//加
	 }
	
	 #endif
	 
	
	
  /* USER CODE END TIM2_IRQn 0 */
  HAL_TIM_IRQHandler(&htim2);
  /* USER CODE BEGIN TIM2_IRQn 1 */

  /* USER CODE END TIM2_IRQn 1 */
}

/**
  * @brief This function handles TIM3 global interrupt.
  */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
	//定时1s
	//BaseType_t xHigherPriorityTaskWoken;
//	if(V_cnt<3){
//			V_cnt++;
//	}else{
//		sendbegin_flag=1;
//	}
	//xSemaphoreGiveFromISR(TIM3_SemHandle,&xHigherPriorityTaskWoken);//每1s释放信号量 定时测量体积
	//osSemaphoreRelease(TIM3_SemHandle);
	
	if(Vfid_flag_warning == 1)
	{
		Vfid_flag_cnt++;
	}
	else 
	{
		Vfid_flag_cnt = 0;
	}
	
	if(Vms_flag_warning  == 1)
	{
		Vms_flag_cnt++;
	}
	else 
	{
		Vms_flag_cnt = 0;
	}
	
	if(Vheat_flag_warning  == 1)
	{
		Vheat_flag_cnt++;
	}
	else 
	{
		Vheat_flag_cnt = 0;
	}
	
	if(Tfid_flag_warning  == 1)
	{
		Tfid_flag_cnt++;
	}
	else 
	{
		Tfid_flag_cnt = 0;
	}
	
	if(Tms_flag_warning  == 1)
	{
		Tms_flag_cnt++;
	}
	else 
	{
		Tms_flag_cnt = 0;
	}
	
	if(Tcap_flag_warning  == 1)
	{
		Tcap_flag_cnt++;
	}
	else
	{		
		Tcap_flag_cnt = 0;
	}
	
	
	if(Vfid_flag_cnt == 3)
	{
		Vfid_flag_cnt = 0;
		Vfid_flag_W = 1;
	}
	if(Vms_flag_cnt == 3)
	{
		Vms_flag_cnt = 0;
		Vms_flag_W = 1;
	}
	if(Vheat_flag_cnt == 3)
	{
		Vheat_flag_cnt = 0;
		Vheat_flag_W = 1;
	}
	if(Tfid_flag_cnt == 3)
	{
		Tfid_flag_cnt = 0;
		Tfid_flag_W = 1;
	}
	if(Tms_flag_cnt == 3)
	{
		Tms_flag_cnt =  0;
		Tms_flag_W = 1;
	}
	if(Tcap_flag_cnt == 3)
	{
		Tcap_flag_cnt = 0;
		Tcap_flag_W = 1;
	}
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}

/**
  * @brief This function handles TIM4 global interrupt.
  */
//int32_t duty = 0;
void TIM4_IRQHandler(void)
{
//  /* USER CODE BEGIN TIM4_IRQn 0 */
//	__IO  int32_t raw_temp = 0;
//		__IO  int32_t raw_temp1 = 0;
//		 uint8_t txData[3]={0x00};
//		__IO uint8_t rxData[3]={0x00};
//		__IO uint32_t FID,MS,HEAT;
//		union int_bit32_data tem32;
//		
//		//读第一路温度
//		TEM1.fault = Max31856_READ_Fault(cs1);
//		osDelay(1);
//		if(TEM1.fault != 0)
//		{
//			pid1.Sv   = -200;
//      //pid2.Sv = -200;
//      //pid3.Sv = -200;
//			Tfid_flag_W = 1;
//      //HAL_TIM_Base_Stop_IT(&htim2);//关闭PID中断 停止加热
//			infoS.FIDT_12.byte0=0x3c;
//			infoS.FIDT_12.byte1=0xf6;
//			infoS.FIDT_12.byte2=0xff;
//			infoS.FIDT_12.byte3=0xff;
//		}
//		else
//		{
//			Max31856_READ_MODE(cs1,TEM1.mode);
//			if(TEM1.mode[0] != 0xB0)
//			{
//				//printf("MODE1,ERROR\r\n");
//				Max31856_SET_MODE(cs1,0xB0);
//			}
//			if(TEM1.mode[1] != 0x07)
//			{
//				//printf("MODE2,ERROR\r\n");
//				Max31856_SET_TYPE(cs1,0x07);
//			}
//			
//			TEM1.temp_tc = Get_Max31856_TC(cs1);
//			osDelay(1);
//			//FID = round(10 * TEM1.temp_tc);
//			tem32.data = round(10 * TEM1.temp_tc);
//			infoS.FIDT_12.byte0=tem32.byte.byte0;
//			infoS.FIDT_12.byte1=tem32.byte.byte1;
//			infoS.FIDT_12.byte2=tem32.byte.byte2;
//			infoS.FIDT_12.byte3=tem32.byte.byte3;
//			osDelay(1);
//			TEM1.temp_cold = Get_Max31856_Cold(cs1);
//			osDelay(1);
//			//printf("tem1：tc= %0.2f,cold=%0.2f\r\n",TEM1.temp_tc,TEM1.temp_tc);
//			if(TEM1.temp_tc < 200 )
//			{
//					//更新pid温度
//					pid1.fd = TEM1.temp_tc;	//更新pid温度
//			}
//			else
//			{
//				//过温报警
//				printf("warning!tem1>200! \r\n");
//				pid1.Sv   = -200;
//        //pid2.Sv = -200;
//        //pid3.Sv = -200;
//				Tfid_flag_W = 1;
//			  //后续加入保护措施，停止加热
//				//HAL_TIM_Base_Stop_IT(&htim2);//关闭PID中断 全部停止加热
//				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);    //停止加热
//				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);    //停止加热	
//				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);    //停止加热
//				//infoS.FIDT_12.byte0=0x3c;
//				//infoS.FIDT_12.byte1=0xf6;
//				//infoS.FIDT_12.byte2=0xff;
//				//infoS.FIDT_12.byte3=0xff;
//			}
//		}
//	
//		 PID1_Calc();	
//		duty = (int32_t)pid1.out;


//		//xEventGroupSetBits(IWDGEventGroup, TASK_TEM_BIT_4);//标志位置四
//		//printf("tem2 iwdg! \r\n");
//		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,duty);
//		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,duty);
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim4);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

void TIM5_IRQHandler(void)
{
  /* USER CODE BEGIN TIM4_IRQn 0 */
	
  /* USER CODE END TIM4_IRQn 0 */
  HAL_TIM_IRQHandler(&htim5);
  /* USER CODE BEGIN TIM4_IRQn 1 */

  /* USER CODE END TIM4_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t tmp_flag = 0;
	//uint32_t temp;
	tmp_flag =__HAL_UART_GET_FLAG(&huart1,UART_FLAG_IDLE); //获取IDLE标志位
	
	if((tmp_flag != RESET))//idle标志被置位
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart1);//清除标志位
		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
		//temp = huart1.Instance->DR;  //读取数据寄存器中的数据
		//这两句和上面那句等效
		//HAL_UART_DMAStop(&huart1); //
		HAL_UART_AbortReceive(&huart1);                        //停止DMA传输
		//temp = __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);// 获取DMA中未传输的数据个数   
		//temp = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数
		usart1_rx_len =  BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);
		fifo_enqueue_frame(&uart1_rec, usart1_rx_buffer, usart1_rx_len);//入队
		//这句和上面那句等效
		//RS485_RX_CNT =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
		//recv_end_flag1 = 1;	// 接受完成标志位置1	
		if(uart1_SemHandle != NULL)
		{
			xSemaphoreGiveFromISR(uart1_SemHandle,&xHigherPriorityTaskWoken);
		}		
		//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
		__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE); //关闭IDLE中断
	 }
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t tmp_flag = 0;
	uint32_t temp;
	tmp_flag =__HAL_UART_GET_FLAG(&huart2,UART_FLAG_IDLE); //获取IDLE标志位
	if((tmp_flag != RESET))//idle标志被置位
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart2);//清除标志位
		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
		//temp = huart1.Instance->DR; //读取数据寄存器中的数据
		//这两句和上面那句等效
		HAL_UART_DMAStop(&huart2); //
		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);// 获取DMA中未传输的数据个数   
		//temp  = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
		//这句和上面那句等效
		rx_len2 =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
		//recv_end_flag2 = 1;	// 接受完成标志位置1	
		if(uart2_SemHandle != NULL)
		{
			xSemaphoreGiveFromISR(uart2_SemHandle,&xHigherPriorityTaskWoken);
		}		
		//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换
		//xSemaphoreGiveFromISR(uart2_SemHandle,&xHigherPriorityTaskWoken);
		__HAL_UART_DISABLE_IT(&huart2 , UART_IT_IDLE); //关闭IDLE中断
	 }
  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t tmp_flag = 0;
	uint32_t temp;
	tmp_flag =__HAL_UART_GET_FLAG(&huart3,UART_FLAG_IDLE); //获取IDLE标志位
	if((tmp_flag != RESET))//idle标志被置位
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart3);//清除标志位
		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
		//temp = huart1.Instance->DR; //读取数据寄存器中的数据
		//这两句和上面那句等效
		HAL_UART_DMAStop(&huart3); //
		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart3_rx);// 获取DMA中未传输的数据个数   
		//temp  = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
		//这句和上面那句等效
		rx_len3 =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
		//recv_end_flag3 = 1;	// 接受完成标志位置1
		if(uart3_SemHandle != NULL)
		{
			xSemaphoreGiveFromISR(uart3_SemHandle,&xHigherPriorityTaskWoken);
		}		
		//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换		
		//xSemaphoreGiveFromISR(uart3_SemHandle,&xHigherPriorityTaskWoken);
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_IDLE); //关闭IDLE中断
	 }
	
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles DMA1 stream7 global interrupt.
  */
void DMA1_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Stream7_IRQn 0 */

  /* USER CODE END DMA1_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_uart5_tx);
  /* USER CODE BEGIN DMA1_Stream7_IRQn 1 */

  /* USER CODE END DMA1_Stream7_IRQn 1 */
}

/**
  * @brief This function handles UART4 global interrupt.
  */
void UART4_IRQHandler(void)
{
  /* USER CODE BEGIN UART4_IRQn 0 */
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t tmp_flag = 0;
	uint32_t temp;
	tmp_flag =__HAL_UART_GET_FLAG(&huart4,UART_FLAG_IDLE); //获取IDLE标志位
	if((tmp_flag != RESET))//idle标志被置位
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart4);//清除标志位
		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
		//temp = huart1.Instance->DR; //读取数据寄存器中的数据
		//这两句和上面那句等效
		HAL_UART_DMAStop(&huart4); //
		temp  =  __HAL_DMA_GET_COUNTER(&hdma_uart4_rx);// 获取DMA中未传输的数据个数   
		//temp  = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
		//这句和上面那句等效
		rx_len4 =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
		//recv_end_flag4 = 2;	// 接受完成标志位置1	
		if(uart4_SemHandle != NULL)
		{
			xSemaphoreGiveFromISR(uart4_SemHandle,&xHigherPriorityTaskWoken);
		}		
		//portYIELD_FROM_ISR(xHigherPriorityTaskWoken);//如果需要的话进行一次任务切换		
		//xSemaphoreGiveFromISR(uart4_SemHandle,&xHigherPriorityTaskWoken);	//释放信号量到解析任务
		//__HAL_UART_DISABLE_IT(&huart1, UART_IT_IDLE); //关闭IDLE中断
	 }
  /* USER CODE END UART4_IRQn 0 */
  HAL_UART_IRQHandler(&huart4);
  /* USER CODE BEGIN UART4_IRQn 1 */

  /* USER CODE END UART4_IRQn 1 */
}

/**
  * @brief This function handles UART5 global interrupt.
  */
void UART5_IRQHandler(void)
{
  /* USER CODE BEGIN UART5_IRQn 0 */
	BaseType_t xHigherPriorityTaskWoken;
	uint32_t tmp_flag = 0;
	uint32_t temp;
	tmp_flag =__HAL_UART_GET_FLAG(&huart5,UART_FLAG_IDLE); //获取IDLE标志位
	if((tmp_flag != RESET))//idle标志被置位
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart5);//清除标志位
		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
		//temp = huart1.Instance->DR; //读取数据寄存器中的数据
		//这两句和上面那句等效
		HAL_UART_DMAStop(&huart5); //
		temp  =  __HAL_DMA_GET_COUNTER(&hdma_uart5_rx);// 获取DMA中未传输的数据个数   
		//temp  = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
		//这句和上面那句等效
		rx_len5 =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
		if(uart5_SemHandle != NULL)
		{
			xSemaphoreGiveFromISR(uart5_SemHandle,&xHigherPriorityTaskWoken);
		}		
		//recv_end_flag5 = 1;	// 接受完成标志位置1	
	 }
  /* USER CODE END UART5_IRQn 0 */
  HAL_UART_IRQHandler(&huart5);
  /* USER CODE BEGIN UART5_IRQn 1 */

  /* USER CODE END UART5_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream1 global interrupt.
  */
void DMA2_Stream1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream1_IRQn 0 */

  /* USER CODE END DMA2_Stream1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_rx);
  /* USER CODE BEGIN DMA2_Stream1_IRQn 1 */

  /* USER CODE END DMA2_Stream1_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream2 global interrupt.
  */
void DMA2_Stream2_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream2_IRQn 0 */

  /* USER CODE END DMA2_Stream2_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_rx);
  /* USER CODE BEGIN DMA2_Stream2_IRQn 1 */

  /* USER CODE END DMA2_Stream2_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream6 global interrupt.
  */
void DMA2_Stream6_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream6_IRQn 0 */

  /* USER CODE END DMA2_Stream6_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart6_tx);
  /* USER CODE BEGIN DMA2_Stream6_IRQn 1 */

  /* USER CODE END DMA2_Stream6_IRQn 1 */
}

/**
  * @brief This function handles DMA2 stream7 global interrupt.
  */
void DMA2_Stream7_IRQHandler(void)
{
  /* USER CODE BEGIN DMA2_Stream7_IRQn 0 */

  /* USER CODE END DMA2_Stream7_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_usart1_tx);
  /* USER CODE BEGIN DMA2_Stream7_IRQn 1 */

  /* USER CODE END DMA2_Stream7_IRQn 1 */
}

/**
  * @brief This function handles USART6 global interrupt.
  */
void USART6_IRQHandler(void)
{
  /* USER CODE BEGIN USART6_IRQn 0 */
	uint32_t tmp_flag = 0;
	uint32_t temp;
	tmp_flag =__HAL_UART_GET_FLAG(&huart6,UART_FLAG_IDLE); //获取IDLE标志位
	if((tmp_flag != RESET))//idle标志被置位
	{ 
		__HAL_UART_CLEAR_IDLEFLAG(&huart6);//清除标志位
		//temp = huart1.Instance->SR;  //清除状态寄存器SR,读取SR寄存器可以实现清除SR寄存器的功能
		//temp = huart1.Instance->DR; //读取数据寄存器中的数据
		//这两句和上面那句等效
		HAL_UART_DMAStop(&huart6); //
		temp  =  __HAL_DMA_GET_COUNTER(&hdma_usart6_rx);// 获取DMA中未传输的数据个数   
		//temp  = hdma_usart1_rx.Instance->NDTR;//读取NDTR寄存器 获取DMA中未传输的数据个数，
		//这句和上面那句等效
		rx_len6 =  BUFFER_SIZE - temp; //总计数减去未传输的数据个数，得到已经接收的数据个数
		recv_end_flag6 = 1;	// 接受完成标志位置1	
	 }
  /* USER CODE END USART6_IRQn 0 */
  HAL_UART_IRQHandler(&huart6);
  /* USER CODE BEGIN USART6_IRQn 1 */

  /* USER CODE END USART6_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
