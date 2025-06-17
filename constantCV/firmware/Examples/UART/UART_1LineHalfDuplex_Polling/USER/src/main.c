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
//UARTx
#define  DEBUG_UARTx                   CW_UART2
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART2
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           9600
#define  DEBUG_UART_UclkFreq           8000000

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOB)
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_5
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_6

//GPIO AF
#define  DEBUG_UART_AFTX               PB05_AFx_UART2TXD()
#define  DEBUG_UART_AFRX               PB06_AFx_UART2RXD()

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



/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
uint8_t UART1_send_Data = 0;
uint8_t UART1_recv_Data = 0;
uint8_t UART2_recv_Data = 0;

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

    //配置UART
    UART_Configuration();

    while(1)
    {
		//PB05作为UART2的TXD脚，平时处于接收状态
		//写数据到UARTx_TDR寄存器后，UARTx_TXD引脚立即进入发送状态
		//发送完成后，UARTx_TXD引脚恢复到常态的接收状态
		
		//1、UART1通过PA06发送数据给UART2
		UART_SendData_8bit(CW_UART1, UART1_send_Data);
		while(UART_GetFlagStatus(CW_UART1, UART_FLAG_TXE) == RESET);
		
		//2、UART2通过PB05接收UART1发送的数据
		while(UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_RC) == RESET);
		UART2_recv_Data = UART_ReceiveData(DEBUG_UARTx);
		UART_ClearFlag(DEBUG_UARTx,UART_FLAG_RC);
		
		//3、UART2通过PB05将接收的数据发送给UART1
		UART_SendData_8bit(DEBUG_UARTx, UART2_recv_Data);
		while(UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);
		
		//4、UART1通过PA06接收UART2发送的数据
		while(UART_GetFlagStatus(CW_UART1, UART_FLAG_RC) == RESET);
		UART1_recv_Data = UART_ReceiveData(CW_UART1);
		UART_ClearFlag(CW_UART1,UART_FLAG_RC);
		
		if(UART1_recv_Data != UART1_send_Data)
		{
			PB02_SETHIGH();
		}
		
		if(UART1_send_Data == 0xFF)
		{
			UART1_send_Data = 0;
		}
		else
		{
			UART1_send_Data++;
		}
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

    //外设时钟使能
    SYSCTRL_AHBPeriphClk_Enable(DEBUG_UART_GPIO_CLK, ENABLE);
    DEBUG_UART_APBClkENx(DEBUG_UART_CLK, ENABLE);
	__SYSCTRL_GPIOA_CLK_ENABLE();
	__SYSCTRL_UART1_CLK_ENABLE();
}

/**
 * @brief 配置GPIO
 *
 */
void GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
	//1、UART2的通信引脚配置---PB05，开漏输出
    GPIO_WritePin(DEBUG_UART_TX_GPIO_PORT, DEBUG_UART_TX_GPIO_PIN,GPIO_Pin_SET);    // 设置TXD的默认电平为高，空闲

    GPIO_InitStructure.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;//开漏输出
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

     //UART TX RX 复用
    DEBUG_UART_AFTX;
	
	//2、UART1的通信引脚配置---PA06，开漏输出
	GPIO_WritePin(CW_GPIOA, GPIO_PIN_6,GPIO_Pin_SET);
	
	GPIO_InitStructure.Pins = GPIO_PIN_6;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;//开漏输出
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
	PA06_AFx_UART1TXD();
	
	//LED2配置
	PB02_SETLOW();
	GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;//推挽输出
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
}

/**
 * @brief 配置UART
 *
 */
void UART_Configuration(void)
{
    UART_InitTypeDef UART_InitStructure = {0};

	//1、UART1配置成单线半双工模式
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
	
	DEBUG_UARTx->CR1_f.SIGNAL = 1;//配置串口为单线半双工模式
	
	//2、UART2配置成单线半双工模式
	UART_Init(CW_UART1, &UART_InitStructure);
	CW_UART1->CR1_f.SIGNAL = 1;//配置串口为单线半双工模式
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

