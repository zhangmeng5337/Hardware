/**
 * @file main.c
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2021
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
#include "..\inc\main.h"
#include "cw32l010_sysctrl.h"
#include "cw32l010_gpio.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


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
 ** This sample HSI switch to LSI, LSI switch to HSI.
 **
 ******************************************************************************/
int32_t main(void)
{
    uint8_t res = 0U;


    GPIO_InitTypeDef GPIO_InitStruct = {0};

    //PB02
    __SYSCTRL_GPIOB_CLK_ENABLE();
    GPIO_InitStruct.Pins = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;

    GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
    PB02_SETLOW();

    SYSCTRL_PCLK_OUT();//PA02--输出PCLK                                               
    CW_GPIOB->ANALOG_f.PIN3 = 0;
   	CW_GPIOB->DIR_f.PIN3 = 0;
   	CW_GPIOB->AFRL_f.AFR3 = 4;//PB03复用为MCO
	   CW_SYSCTRL->MCO_f.SOURCE = 1;//MCO时钟源为HCLK


    while(1)
    {
        //============================================================================
        //以下从HSI切换到LSI
        SYSCTRL_LSI_Enable( );                                                   //开外部高速时钟LSI
        res = SYSCTRL_SysClk_Switch( SYSCTRL_SYSCLKSRC_LSI );                        //切换系统时钟到LSI
        if( res == 0x00 )                                                    //系统时钟切换成功
        {            
            SYSCTRL_HSI_Disable();                                               //切换时钟后关闭源时钟HSI
            res = 10;
            while(res--)
            {
                PB02_TOG();
                FirmwareDelay(1000);
            } 
        }
        else
        {
            while(1)
            {
                PB02_SETHIGH();
            }
        }

        //============================================================================
        //以下从LSI切换到HSI
        SYSCTRL_HSI_Enable( SYSCTRL_HSIOSC_DIV2 );                                   //开内部高速时钟HSI = HSIOSC / 2
        res = SYSCTRL_SysClk_Switch( SYSCTRL_SYSCLKSRC_HSI );                        //切换系统时钟到HSI
        if( res == 0x00 )                                                    //系统时钟切换成功
        {
            SYSCTRL_LSI_Disable();
            res = 10;
            while(res--)
            {
                PB02_TOG();
                FirmwareDelay(200000);
            } 
        }
        else
        {
            while(1)
            {
                PB02_SETHIGH();
            }
        }
    }
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

