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
#include "cw32l010_systick.h"
#include "cw32l010_sysctrl.h"
#include "cw32l010_gpio.h"
#include "cw32l010_adc.h"
#include "cw32l010_gtim.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t valueAdc;
uint32_t valueAdcAcc;
volatile uint8_t gFlagIrq;
uint16_t gCntEoc = 0;
uint8_t cntSample;
float fTsDegree;

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
 * @brief ADC 配置
 *
 */
void ADC_Configuration(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};
    __SYSCTRL_ADC_CLK_ENABLE();
    
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div2;
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Once;
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to0;
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputTs;
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime166Clk;   
    
    ADC_Init(&ADC_InitStructure);
    ADC_SetTs(ADC_TsEnable);    
    ADC_Enable();    
}

/**
 * @brief LED I/O初始化
 *
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //打开GPIO时钟
    __SYSCTRL_GPIOB_CLK_ENABLE();   

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    //LEDs are off.
    PB02_SETLOW();
}

/**
 * @brief GTIM初始化
 *
 */
void GTIM_Init(void)
{
    GTIM_InitTypeDef GTIM_InitStruct = { 0 };

    //打开GTIM时
    __SYSCTRL_GTIM1_CLK_ENABLE();
    
    GTIM_InitStruct.AlignMode = GTIM_ALIGN_MODE_EDGE;
    GTIM_InitStruct.ARRBuffState = GTIM_ARR_BUFF_DIS;
    GTIM_InitStruct.Direction = GTIM_DIRECTION_UP;
    GTIM_InitStruct.EventOption = GTIM_EVENT_NORMAL;
    GTIM_InitStruct.Prescaler = 8000-1;    //实际分频为 Prescaler+1 = 8000, 频率为1kHz
    GTIM_InitStruct.PulseMode = GTIM_PULSE_MODE_DIS;
    GTIM_InitStruct.ReloadValue = 1000-1;    // 1秒
    GTIM_InitStruct.UpdateOption = GTIM_UPDATE_EN;
    
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
    
    GTIM_MasterTRGOCfg(CW_GTIM1, GTIM_MASTER_TRGO_UPDATE);
    
}

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV6);

    //LED初始化
    LED_Init();
    
    ADC_Configuration();

    GTIM_Init();
    
   //ADC外部中断触发源配置
    ADC_ExtTrigCfg(ADC_TRIG_GTIM1TRGO, ENABLE);
    GTIM_Cmd(CW_GTIM1, ENABLE);

    while (1)
    {
        if (CW_ADC->ISR_f.EOC)
        {
            PB02_TOG();
            CW_ADC->ICR = 0;
            ADC_GetSqr0Result(&valueAdc);
            fTsDegree = ADC_GetTs(3.3, valueAdc);
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
  * @return None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

