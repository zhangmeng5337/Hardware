
/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2021-03-12  1.0  xiebin First version for Device Driver Library of Module.
 **
 ******************************************************************************/
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
#include "../inc/main.h"
#include "main.h"
#include "stdint.h"
#include "base_types.h"
#include "cw32l010.h"
#include "system_cw32l010.h"
#include "cw32l010_sysctrl.h"
#include "cw32l010_gpio.h"
#include "cw32l010_uart.h"
#include "cw32l010_spi.h"
#include "cw32l010_adc.h"
#include "cw32l010_i2c.h"
#include "cw32l010_flash.h"
#include "cw32l010_systick.h"
#include "cw32_eval_spi_flash.h"
#include "cw32l010_flash.h"
#include "cw32l010_gtim.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
volatile uint8_t gKey1Status,gKey2Status;
#ifdef __GNUC__
    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
    set to 'Yes') calls __io_putchar() */
    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
				
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

//KEY
	#define  KEY1_GPIO_PIN                   GPIO_PIN_4
	#define  KEY1_GPIO_PORT                  CW_GPIOB
	#define  KEY2_GPIO_PIN                   GPIO_PIN_6
	#define  KEY2_GPIO_PORT                  CW_GPIOA
	
//LED
	#define  LED1_GPIO_PIN                   GPIO_PIN_3	
	#define  LED1_GPIO_PORT                  CW_GPIOB
	#define  LED2_GPIO_PIN                   GPIO_PIN_2	
	#define  LED2_GPIO_PORT                  CW_GPIOB

	#define  LED1_ON                         PB03_SETHIGH()
	#define  LED1_OFF                        PB03_SETLOW()
	#define  LED1_TOG                        PB03_TOG()
	#define  LED2_ON                         PB02_SETHIGH()
	#define  LED2_OFF                        PB02_SETLOW()
	#define  LED2_TOG                        PB02_TOG()
	
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief
 *
 * @return int32_t
 */
int32_t main(void)
{
  RCC_Configuration(); 
		SYSCTRL_SystemCoreClockUpdate(8000000);
  InitTick(8000000);	
  NVIC_Configuration();   

		//配置GPIO
  GPIO_Configuration();

 
    while (1)
    {
					if( (gKey1Status == 1) && (gKey2Status == 0) )
		{
			LED1_TOG;
			LED2_OFF;

		}
		else if((gKey1Status == 0) && (gKey2Status == 1))
		{
			LED2_TOG;
			LED1_OFF;

		}
		else
		{
			LED1_TOG;
			LED2_TOG;
		}
		
		SysTickDelay(200);	//200ms.
    }
}


/**
 * @brief 配置RCC
 * 
 */
void RCC_Configuration(void)
{

  //SYSCLK = HSI =8MHz = HCLK = PCLK
  SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV6);
//		 CW_SYSCTRL->HSI_f.TRIM=0x0757;    //校准时钟，依mcu而定
	
    __SYSCTRL_GPIOB_CLK_ENABLE();
    __SYSCTRL_GPIOA_CLK_ENABLE();

}


/**
 * @brief 配置GPIO
 * 
 */
void GPIO_Configuration(void)
{  
    GPIO_InitTypeDef GPIO_InitStruct = {0};

	   //LED
	   GPIO_InitStruct.Pins = LED1_GPIO_PIN	| LED2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	   GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_Init(LED2_GPIO_PORT, &GPIO_InitStruct);
	

			 //KEY2
    GPIO_InitStruct.Pins = KEY1_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
	   GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStruct);

	   //KEY2
    GPIO_InitStruct.Pins = KEY2_GPIO_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
	   GPIO_InitStruct.IT = GPIO_IT_FALLING;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStruct);
				

    //LED灭
				LED1_OFF;
    LED2_OFF;

	}

	
	/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  __disable_irq(); 
	GPIOB_INTFLAG_CLR( 0xFFFF );  
	NVIC_EnableIRQ(GPIOA_IRQn);
	NVIC_EnableIRQ(GPIOB_IRQn);
  __enable_irq();  
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
       /* USER CODE END 6 */
}
#endif
