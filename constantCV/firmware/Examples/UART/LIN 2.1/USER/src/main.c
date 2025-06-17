/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2022
 *
 */
/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
	

//#define SLAVE  //从机节点

//UARTx
#define  DEBUG_UARTx                   CW_UART1
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART1
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           19200
#define  DEBUG_UART_UclkFreq           8000000

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOA)
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOA
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_6
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_5

//GPIO AF
#define  DEBUG_UART_AFTX               PA06_AFx_UART1TXD()
#define  DEBUG_UART_AFRX               PA05_AFx_UART1RXD()

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void UART_Configuration(void);
void NVIC_Configuration(void);
void GTIM_Init(void);
uint8_t wha=0;
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ******************************************************************************/
int32_t main(void)
{
    //配置RCC
    RCC_Configuration();

    //配置GPIO
    GPIO_Configuration();

    GTIM_Init();
	
    //配置UART
    UART_Configuration();

    NVIC_Configuration();
				
    FirmwareDelay(1000000);
    while(1)
    {
    Lin_SendData_Process();
    }
}

/**
 * @brief 配置RCC
 *
 */
void RCC_Configuration(void)
{
    //SYSCLK = HSI = 8MHz = HCLK = PCLK
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV6);
    CW_SYSCTRL->HSI_f.TRIM=0x757;
    //外设时钟使能
    SYSCTRL_AHBPeriphClk_Enable(DEBUG_UART_GPIO_CLK, ENABLE);
    DEBUG_UART_APBClkENx(DEBUG_UART_CLK, ENABLE);
}

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = DEBUG_UART_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStructure);
	
    GPIO_InitStructure.Pins = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

     //UART TX RX 复用
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX;
				
    PB02_SETHIGH();
    PB03_SETHIGH();	
}

void GTIM_Init(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = {0};

    __SYSCTRL_GTIM1_CLK_ENABLE();
			
    GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;
    GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_EN;
    GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler = 71;
    GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue =999;
    GTIM_InitStruct.UpdateOption = GTIM_UPDATE_EN;
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
    GTIM_ITConfig(CW_GTIM1, GTIM_IT_UPDATA , ENABLE);
    GTIM_Cmd(CW_GTIM1, ENABLE);			
}

typedef struct
{
	uint16_t lin_Count_value;
	
}TIM1_valu;

	TIM1_valu T1;
void GTIM1_IRQHandler(void)
{
		int sla=0;
		#ifdef  SLAVE
			sla=1;
		#endif
		if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_UPDATA) != RESET) //检查中断发生与否
	{
		T1.lin_Count_value++;
		if(T1.lin_Count_value%100==0)
		{
		linTxMsg.ID=0x23;
		if((Lin_Sending_Flag==DISABLE)&&linTxMsg.ID==0x07&&sla==0)
		{
 				Lin_Send_Msg();
		}
			if((Lin_Sending_Flag==DISABLE)&&linTxMsg.ID==0x23&&sla==0)
		{

 				Lin_Send_Wharf();
		}
		}
		GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_UPDATA);  //清除GTIM的中断待处理位
	}

}



void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(GTIM1_IRQn);
	   NVIC_EnableIRQ(UART1_IRQn);
    __enable_irq();
}
/**
 * @brief 配置UART
 *
 */
void UART_Configuration(void)
{
    UART_InitTypeDef UART_InitStructure = {0};

    UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = DEBUG_UART_UclkFreq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;	
    UART_Init(DEBUG_UARTx, &UART_InitStructure);
    CW_UART1->CR3_f.LIN=1;
    #ifdef SLAVE
    CW_UART1->CR2_f.TIMCR=0x06;
    #endif
    UART_ClearFlag(CW_UART1,UART_FLAG_RC|UART_FLAG_RXBRK);
    UART_ITConfig(CW_UART1,UART_IT_RC|UART_IT_RXBRK,ENABLE);
    //参数初始化
    linTxMsg.Send_Complete=ENABLE;
    linRxMsg.Length=8;
    linTxMsg.Length=0x08;
}


/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

