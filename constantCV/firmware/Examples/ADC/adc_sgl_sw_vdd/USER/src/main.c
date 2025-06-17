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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t valueAdc;

volatile uint8_t gFlagIrq;

float MCU_VDD = 0.0f;
uint16_t BGR_1_2V_value;


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
 * @brief ADC I/O初始化
 *
 */
void ADC_PortInit(void)
{
    //打开GPIO时钟
    __SYSCTRL_GPIOB_CLK_ENABLE();
    //打开ADC时钟
    __SYSCTRL_ADC_CLK_ENABLE();
    
    //set PB01 as AIN8 INPUT
    PB01_ANALOG_ENABLE();  
}

/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    ADC_InitTypeDef ADC_InitStructure = {0};

    // 系统时钟配置为8MHz
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV6);
    //LED初始化
    LED_Init();

    //使用sysTick作为1ms
    InitTick(8000000);

    //配置ADC测试IO口
    ADC_PortInit();   
    
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div8;                 // ADCCLK = PCLK/8 = 1MHz
    ADC_InitStructure.ADC_ConvertMode = ADC_ConvertMode_Once;    // 单次采样
    ADC_InitStructure.ADC_SQREns = ADC_SqrEns0to0;     // 单通道
    ADC_InitStructure.ADC_IN0.ADC_InputChannel = ADC_InputVref1P2;//采内部BGR1.2V，反推MCU供电电压VDD
    ADC_InitStructure.ADC_IN0.ADC_SampTime = ADC_SampTime42Clk;    //// 采样TS和BGR时需要保证采样时间至少40uS, 因此采样时钟至少 = 40uS / (1/1MHz) = 40个
    
    ADC_Init(&ADC_InitStructure);
	
	CW_ADC->CR_f.BGREN = 1;//使能BGR1.2V ，内部BGR的启动时间大约为30μs
	SysTickDelay(1);//在此延时1ms

    ADC_ClearITPendingAll();

#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_INTERRUPTION)
  
    ADC_ITConfig(ADC_IT_EOC, ENABLE);  
    NVIC_EnableIRQ(ADC_IRQn);
#endif

    //ADC使能
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);

    while (1)
    {
#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_POLLING)  
        while (!(CW_ADC->ISR & ADC_ISR_EOC_Msk));
        ADC_ClearITPendingBit(ADC_IT_EOC);
        valueAdc = ADC_GetConversionValue(0);
		BGR_1_2V_value = *((uint16_t*)	0x001007D2);
		MCU_VDD = (4095 * BGR_1_2V_value * 0.001f )/ valueAdc;
#ifndef SGL_CONTINUOUS_MODE_EN
        ADC_SoftwareStartConvCmd(ENABLE); //启动下一次ADC转换
#endif        
#endif
        
#if (ADC_RETRIEVE_MODE == ADC_RETRIEVE_INTERRUPTION)
        if (gFlagIrq & ADC_ISR_EOC_Msk)
        {
            valueAdc = ADC_GetConversionValue(0);
			BGR_1_2V_value = *((uint16_t*)	0x001007D2);//获取MUC的BGR1.2V的精确值，单位mV
			MCU_VDD = (4.095f * BGR_1_2V_value)/ valueAdc; //
            gFlagIrq = 0;
#ifndef SGL_CONTINUOUS_MODE_EN            
            ADC_SoftwareStartConvCmd(ENABLE); //启动下一次ADC转换
#endif
        }
#endif
        
        PB02_TOG();
        SysTickDelay(300);
    }
}

/**
 * @brief LED I/O初始化
 *
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = { 0 };

    //打开GPIO时钟
    REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOB_Msk);

    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    //LEDs are off.
    PB02_SETLOW();    
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
void assert_failed(uint8_t* file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line number,
       tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

