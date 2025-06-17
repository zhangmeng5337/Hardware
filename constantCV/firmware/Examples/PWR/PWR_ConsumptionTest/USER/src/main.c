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
#include "cw32l010_lptim.h"
#include "cw32l010_uart.h"
#include "cw32l010_iwdt.h"
#include "cw32l010_rtc.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
#define IWDT_RUNNING              1UL
#define RTC_RUNNING               (1UL << 1)
#define LPTIM_RUNNING             (1UL << 2)
#define UART1_RUNNING             (1UL << 3)

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

volatile uint8_t gKeyStatus = 0;  /* set to 1 after User Button interrupt  */
volatile uint8_t gLPTIMWakeUpIrq = 0;  /* set to 1 after LPTIM interrupt  */
volatile uint8_t gIWDTWakeUpIrq = 0;  /* set to 1 after IWDT alarm interrupt    */
volatile uint8_t gRTCWakeUpIrq = 0;  /* set to 1 after RTC alarm interrupt    */
volatile uint8_t gUARTWakeUpIrq = 0;  /* set to 1 after UART alarm interrupt    */
volatile uint8_t gRecvChar = 0;


/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
 void DebugUART_Configuration(uint32_t UclkFreq);
 void DebugUART_Close(void);
 void RTC_Configuration(void);
 void LPTIM_Configuration(void);
 void UART1_Configuration(void);

// 
// #ifdef __GNUC__
//    /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
//    set to 'Yes') calls __io_putchar() */
//    #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
//#else
    #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
// #endif /* __GNUC__ */
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
 //UARTx
#define  DEBUG_UARTx                   CW_UART2
#define  DEBUG_UART_CLK                SYSCTRL_APB1_PERIPH_UART2
#define  DEBUG_UART_APBClkENx          SYSCTRL_APBPeriphClk_Enable1
#define  DEBUG_UART_BaudRate           9600
#define  DEBUG_UART_UclkFreq           4000000

//UARTx GPIO
#define  DEBUG_UART_GPIO_CLK           (SYSCTRL_AHB_PERIPH_GPIOB)
#define  DEBUG_UART_TX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_TX_GPIO_PIN        GPIO_PIN_5
#define  DEBUG_UART_RX_GPIO_PORT       CW_GPIOB
#define  DEBUG_UART_RX_GPIO_PIN        GPIO_PIN_6

//GPIO AF
#define  DEBUG_UART_AFTX               PB05_AFx_UART2TXD()
#define  DEBUG_UART_AFRX               PB06_AFx_UART2RXD()
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 * @brief Main function of project
 *
 * @return int
 */
int main(void)
{
    static uint8_t TestState = 0;
    static uint32_t IsPeriphRun =0;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
   
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);
    // 配置打印串口
    DebugUART_Configuration(4000000);
    printf("\r\nCW32L010 PWR TEST Example\r\n");
    printf("SysClk=HCLK=PCLK=4MHz\r\n");
    InitTick(4000000);    //使用sysTick
	SysTickDelay(1000); // 提供1s时间用于测量在4MHz时，MCU内核功耗1.2mA@3.3V
    
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV6);
    DebugUART_Configuration(8000000);    
    printf("SysClk=HCLK=PCLK=8MHz\r\n");
    InitTick(8000000);
	SysTickDelay(1000); // 提供1s时间用于测量在8MHz时，MCU内核功耗1.5mA@3.3V
    
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV3);
    DebugUART_Configuration(16000000);    
    printf("SysClk=HCLK=PCLK=16MHz\r\n");
    InitTick(16000000);
	SysTickDelay(1000); // 提供1s时间用于测量在16MHz时，MCU内核功耗2.2mA@3.3V
    
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV2);
    DebugUART_Configuration(24000000);    
    printf("SysClk=HCLK=PCLK=24MHz\r\n");
    InitTick(24000000);
	SysTickDelay(1000); // 提供1s时间用于测量在24MHz时，MCU内核功耗2.8mA@3.3V
    
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV1);
    DebugUART_Configuration(48000000);    
    printf("SysClk=HCLK=PCLK=48MHz\r\n");
    InitTick(48000000);
	SysTickDelay(1000); // 提供1s时间用于测量在48MHz时，MCU内核功耗3.9mA@3.3V
    
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV12);
    InitTick(4000000); 
    
    // 配置PA06中断唤醒
    GPIO_InitStructure.IT = GPIO_IT_FALLING;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_InitStructure.Pins = GPIO_PIN_6;
    
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
    NVIC_EnableIRQ(GPIOA_IRQn);

    while(1)
    {
        if (gKeyStatus)
        {
            gKeyStatus = 0;
            SysTickDelay(100);    // 按键延时100ms去抖
            if (PA06_GETVALUE() == 0)    // 确认按下
            {
                TestState++;     //切换下一个测试状态
            }
            else
            {
                continue;    // 保持当前状态
            }
        }
        
        switch (TestState)
        {
            default:
            case 0:    // GPIOA 打开 整体功耗440nA
                // 关所有外设
                CW_SYSCTRL->APBEN1 = 0x5a5a0000;
                CW_SYSCTRL->APBEN2 = 0x5a5a0000;
                CW_SYSCTRL->AHBEN  = 0x5a5a0000;
                // 仅开GPIOA，保证GPIOA中断可用
                SYSCTRL_AHBPeriphClk_Enable(SYSCTRL_AHB_PERIPH_GPIOA, ENABLE);                
                break;
            case 1:    // GPIOA + IWDT (IWDT会打开LSI) 功耗1.02uA
                if (!(IsPeriphRun & IWDT_RUNNING))
                {
                    IWDT_SetPeriod(IWDT_4_SECS);
                    IsPeriphRun |= IWDT_RUNNING;
                    printf("IWDT is runging now!\r\n");
                }
                break;
            case 2:    // GPIOA + LPTIM@LSI 功耗1.1uA 
                if (!(IsPeriphRun & LPTIM_RUNNING))
                {
                    // 先关闭IWDT
                    IWDT_Stop();
                    SYSCTRL_APBPeriphReset2(SYSCTRL_APB2_PERIPH_IWDT, ENABLE);
                    SYSCTRL_APBPeriphReset2(SYSCTRL_APB2_PERIPH_IWDT, DISABLE);
                    SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_IWDT, DISABLE);
                    IsPeriphRun &= ~IWDT_RUNNING;
                    
                    // 开启LPTIM
                    LPTIM_Configuration();
                    
                    IsPeriphRun |= LPTIM_RUNNING;
                    printf("LPTIM is runging now!\r\n");
                }                 
                break;
            case 3:    // GPIOA + UART1@LSI 功耗1.2uA 
                if (!(IsPeriphRun & UART1_RUNNING))
                {
                    // 先关闭LPTIM
                    LPTIM_Cmd(DISABLE);
                    SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_LPTIM, DISABLE);
                    IsPeriphRun &= ~LPTIM_RUNNING;
                    
                    UART1_Configuration();
                    IsPeriphRun |= UART1_RUNNING;
                    printf("UART1 is runging now!\r\n");                                    
                }                             
                break;
            case 4:    // GPIOA + RTC@LSI   功耗1.08uA    RTC的初始化限制只能在上电复位时可配置，故不关闭RTC，进入下一个case。          
                if (!(IsPeriphRun & RTC_RUNNING))
                {
                    // 先关闭UART1                    
                    SYSCTRL_APBPeriphReset1(SYSCTRL_APB1_PERIPH_UART1, ENABLE);
                    SYSCTRL_APBPeriphReset1(SYSCTRL_APB1_PERIPH_UART1, DISABLE);
                    SYSCTRL_APBPeriphClk_Enable1(SYSCTRL_APB1_PERIPH_UART1, DISABLE);
                    IsPeriphRun &= ~UART1_RUNNING;
                    
                    // 开启RTC                    
                    RTC_Configuration();
                    IsPeriphRun |= RTC_RUNNING;
                    printf("RTC is runging now!\r\n");
                }                
                break;
            
            case 5:   // GPIOA + IWDT + RTC + LPTIM + UART +LSI 功耗1.3uA 
                if (!(IsPeriphRun & IWDT_RUNNING))
                {
                    IWDT_SetPeriod(IWDT_4_SECS);
                    IsPeriphRun |= IWDT_RUNNING;
                    printf("IWDT is runging now!\r\n");
                }
                if (!(IsPeriphRun & RTC_RUNNING))
                { 
                    // 开启RTC                    
                    RTC_Configuration();
                    IsPeriphRun |= RTC_RUNNING;
                    printf("RTC is runging now!\r\n");
                }
                if (!(IsPeriphRun & LPTIM_RUNNING))
                {                    
                    // 开启LPTIM
                    LPTIM_Configuration();
                    
                    IsPeriphRun |= LPTIM_RUNNING;
                    printf("LPTIM is runging now!\r\n");
                }
                if (!(IsPeriphRun & UART1_RUNNING))
                {
                    // 开启UART1
                    UART1_Configuration();
                    IsPeriphRun |= UART1_RUNNING;
                    printf("UART1 is runging now!\r\n");                                    
                }
                IWDT_Refresh();
                break;
            case 6:
                TestState = 0;
                IsPeriphRun = 0;
                while(1);    //等待4s后 IWDT复位
                break; 
        }
        // 休眠前，关闭串口
        DebugUART_Close();
        SYSCTRL_GotoDeepSleep();
        //唤醒后配置串口，打印信息
        DebugUART_Configuration(4000000);
        if (gRTCWakeUpIrq)
        {
            gRTCWakeUpIrq = 0;
            printf("MCU is waked up by RTC!\r\n");            
        }
        if (gLPTIMWakeUpIrq)
        {
            gLPTIMWakeUpIrq = 0;
            printf("MCU is waked up by LPTIM!\r\n");  
        }
        if (gUARTWakeUpIrq)
        {
            gUARTWakeUpIrq = 0;
            printf("MCU is waked up by UART!\r\n");
            printf("UART received char: %c\r\n", gRecvChar);             
        }
    }
    
}

 void RTC_Configuration(void)
 {
     RTC_InitTypeDef RTC_InitStruct ={0};
     
     RTC_InitStruct.DateStruct.Day = 1;
     RTC_InitStruct.DateStruct.Month = RTC_Month_January;
     RTC_InitStruct.DateStruct.Year = 24;
     RTC_InitStruct.DateStruct.Week = RTC_Weekday_Sunday;
     RTC_InitStruct.RTC_ClockSource = RTC_RTCCLK_FROM_LSI;
     RTC_InitStruct.TimeStruct.AMPM = RTC_H12_AM;
     RTC_InitStruct.TimeStruct.H24 = RTC_HOUR12;
     RTC_InitStruct.TimeStruct.Hour = 1;
     RTC_InitStruct.TimeStruct.Minute = 0;
     RTC_InitStruct.TimeStruct.Second = 0;
     
     SYSCTRL_LSI_Enable();    // 使用LSI作为RTC的时钟源，必须在配置RTC前准备好时钟     
     RTC_Init(&RTC_InitStruct);
     RTC_SetInterval(RTC_INTERVAL_EVERY_1S);    // 1s间隔一次产生中断
     RTC_ITConfig(RTC_IT_INTERVAL, ENABLE);
     RTC_ClearITPendingBit(RTC_IT_ALL);
     
     NVIC_EnableIRQ(RTC_IRQn);
     RTC_Cmd(ENABLE);     
 }


void LPTIM_Configuration(void)
{
    LPTIM_InitTypeDef LPTIM_InitStruct = {0};  
    
    LPTIM_InitStruct.LPTIM_ClockSource = LPTIM_CLOCK_SOURCE_MCLK;
    LPTIM_InitStruct.LPTIM_CounterMode = LPTIM_COUNTER_MODE_TIME;
    LPTIM_InitStruct.LPTIM_Period = 32800-1;
    LPTIM_InitStruct.LPTIM_Prescaler = LPTIM_PRS_DIV2;
    LPTIM_Init(&LPTIM_InitStruct);
    
    SYSCTRL_LSI_Enable();    // 使用LSI作为时钟源  
    LPTIM_InternalClockConfig(LPTIM_ICLK_LSI);

    LPTIM_ITConfig(LPTIM_IT_ARRM, ENABLE);
    CW_LPTIM->ICR = 0x00;
	NVIC_EnableIRQ(LPTIM_IRQn);
    
    LPTIM_Cmd(ENABLE);   
    LPTIM_SelectOnePulseMode(LPTIM_OPERATION_REPETITIVE);    
}


void UART1_Configuration(void)
{
    UART_InitTypeDef UART_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    
    GPIO_InitStructure.Pins = GPIO_PIN_5;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);   
    
    PA05_AFx_UART1RXD();
    
    UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
    UART_InitStructure.UART_Over = UART_Over_sp;    // 使用LSI必须用特殊采样率
    UART_InitStructure.UART_Source = UART_Source_LSI;
    UART_InitStructure.UART_UclkFreq = 32800;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx;
    UART_Init(CW_UART1, &UART_InitStructure); 
    
    UART_ITConfig(CW_UART1, UART_IT_RC, ENABLE);
    UART_ClearFlag(CW_UART1, UART_IT_RC);
    NVIC_EnableIRQ(UART1_IRQn);        
}
/**
 * @brief 配置UART
 *
 */
void DebugUART_Configuration(uint32_t UclkFreq)
{
    UART_InitTypeDef UART_InitStructure = {0};
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    GPIO_InitStructure.Pins = DEBUG_UART_TX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_Init(DEBUG_UART_TX_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = DEBUG_UART_RX_GPIO_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
    GPIO_Init(DEBUG_UART_RX_GPIO_PORT, &GPIO_InitStructure);
    
    //UART TX RX 复用
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX;    

    UART_InitStructure.UART_BaudRate = DEBUG_UART_BaudRate;
    UART_InitStructure.UART_Over = UART_Over_16;
    UART_InitStructure.UART_Source = UART_Source_PCLK;
    UART_InitStructure.UART_UclkFreq = UclkFreq;
    UART_InitStructure.UART_StartBit = UART_StartBit_FE;
    UART_InitStructure.UART_StopBits = UART_StopBits_1;
    UART_InitStructure.UART_Parity = UART_Parity_No ;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_None;
    UART_InitStructure.UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_Init(DEBUG_UARTx, &UART_InitStructure);
}

void DebugUART_Close(void)
{
    SYSCTRL_AHBPeriphReset(SYSCTRL_AHB_PERIPH_GPIOB, ENABLE);
    SYSCTRL_AHBPeriphReset(SYSCTRL_AHB_PERIPH_GPIOB, DISABLE);
    SYSCTRL_APBPeriphReset1(SYSCTRL_APB1_PERIPH_UART2, ENABLE);
    SYSCTRL_APBPeriphReset1(SYSCTRL_APB1_PERIPH_UART2, DISABLE);
    
    SYSCTRL_AHBPeriphClk_Enable(SYSCTRL_AHB_PERIPH_GPIOB, DISABLE);
    SYSCTRL_APBPeriphClk_Enable1(SYSCTRL_APB1_PERIPH_UART2, DISABLE);
}

/**
 * @brief Retargets the C library printf function to the UART.
 *
 */
PUTCHAR_PROTOTYPE
{
    UART_SendData_8bit(DEBUG_UARTx, (uint8_t)ch);

    while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);

    return ch;
}

size_t __write(int handle, const unsigned char * buffer, size_t size)
{
    size_t nChars = 0;

    if (buffer == 0)
    {
        /*
         * This means that we should flush internal buffers.  Since we
         * don't we just return.  (Remember, "handle" == -1 means that all
         * handles should be flushed.)
         */
        return 0;
    }


    for (/* Empty */; size != 0; --size)
    {
        UART_SendData_8bit(DEBUG_UARTx, *buffer++);
        while (UART_GetFlagStatus(DEBUG_UARTx, UART_FLAG_TXE) == RESET);
        ++nChars;
    }

    return nChars;
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

