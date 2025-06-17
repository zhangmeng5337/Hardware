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
void SYSCTRL_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void delay(uint16_t ms);
void ShowTime(void);
void RTC_IRQHandlerCallBack(void);
void UART1_Configure(void);

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
/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample toggle GPIOA.00
 **
 ******************************************************************************/

/**
 * @brief  RTC时钟测试，初始化日历，使用间隔中断0.5秒通过Log输出日期时间
 * @return int32_t
 */
int32_t main(void)
{
    RTC_InitTypeDef RTC_InitStruct = {0};
    RTC_AlarmTypeDef RTC_AlarmStruct = {0};

    /* System Clocks Configuration */
    SYSCTRL_Configuration();

    /* GPIO Configuration */
    GPIO_Configuration();

    UART1_Configure();

     /* NVIC Configuration */
    NVIC_Configuration();

    printf("RTC Init...\r\n");
    printf("   (RTC CR0:%04x,CR1:%04x,CR2:%04x,RESET FLAG:0x%08x)\r\n",CW_RTC->CR0,CW_RTC->CR1,CW_RTC->CR2,CW_SYSCTRL->RESETFLAG);

    SYSCTRL_LSE_Enable( SYSCTRL_LSE_MODE_OSC,SYSCTRL_LSE_DRIVER_LEVEL5);// 选择LSE为RTC时钟

    RTC_InitStruct.DateStruct.Day = 0x21;             //设置日期，DAY、MONTH、YEAR必须为BCD方式，星期为0~6，代表星期日，星期一至星期六
    RTC_InitStruct.DateStruct.Month = RTC_Month_June;
    RTC_InitStruct.DateStruct.Week = RTC_Weekday_Monday;
    RTC_InitStruct.DateStruct.Year = 0x21;

    printf("-------Set Date as 20%x/%x/%x\r\n", RTC_InitStruct.DateStruct.Year,RTC_InitStruct.DateStruct.Month,RTC_InitStruct.DateStruct.Day);

    RTC_InitStruct.TimeStruct.Hour = 0x32;        //设置时间，HOUR、MINIUTE、SECOND必须为BCD方式，用户须保证HOUR、AMPM、H24之间的关联正确性
    RTC_InitStruct.TimeStruct.Minute = 0x59;
    RTC_InitStruct.TimeStruct.Second = 0x51;
    RTC_InitStruct.TimeStruct.AMPM = 0;
    RTC_InitStruct.TimeStruct.H24 = 0;
    printf("-------Set Time as %02x:%02x:%02x\r\n", RTC_InitStruct.TimeStruct.Hour,RTC_InitStruct.TimeStruct.Minute,RTC_InitStruct.TimeStruct.Second);
    RTC_InitStruct.RTC_ClockSource = RTC_RTCCLK_FROM_LSE;
    RTC_Init(&RTC_InitStruct);    //  用户需选定需要使用的时钟源

    printf("=====Set interval period as 0.5s...\r\n");
    RTC_SetInterval(RTC_INTERVAL_EVERY_0_5S);

    /* 设置闹钟为工作日上午的6：45 */
    printf("=====Set AlarmA at 6:45 on workday...\r\n");

    RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_WeekMON | RTC_AlarmMask_WeekTUE |
                                    RTC_AlarmMask_WeekWED | RTC_AlarmMask_WeekTHU |
                                    RTC_AlarmMask_WeekFRI;
    RTC_AlarmStruct.RTC_AlarmTime.Hour = 6;
    RTC_AlarmStruct.RTC_AlarmTime.Minute = 0x45;
    RTC_AlarmStruct.RTC_AlarmTime.Second = 0;
    RTC_SetAlarm(RTC_Alarm_A, &RTC_AlarmStruct);
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);

    printf("=====Enable ALRAMA and INTERVAL IT...\r\n");

    RTC_ITConfig(RTC_IT_ALARMA | RTC_IT_INTERVAL, ENABLE);

    while(1)
    {

    }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void SYSCTRL_Configuration(void)
{
    SYSCTRL_HSI_Enable(SYSCTRL_HSIOSC_DIV6);
    SYSCTRL_LSI_Enable();
    SYSCTRL_LSE_Enable( SYSCTRL_LSE_MODE_OSC,SYSCTRL_LSE_DRIVER_LEVEL5);
    SYSCTRL_APBPeriphClk_Enable2(SYSCTRL_APB2_PERIPH_RTC, ENABLE);
	SYSCTRL_AHBPeriphClk_Enable(DEBUG_UART_GPIO_CLK, ENABLE);
    DEBUG_UART_APBClkENx(DEBUG_UART_CLK, ENABLE);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
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

     //UART TX RX 复用
    DEBUG_UART_AFTX;
    DEBUG_UART_AFRX; 
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq();
    NVIC_EnableIRQ(RTC_IRQn);
    __enable_irq();
}

void delay(uint16_t ms)
{
    uint32_t i;
    for( ; ms >0; ms--)
    {
        i = 796;
        while(i--);
    }
}

void ShowTime(void)
{
    RTC_TimeTypeDef RTC_TimeStruct = {0};
    RTC_DateTypeDef RTC_DateStruct = {0};
    static char *WeekdayStr[7]= {"SUN","MON","TUE","WED","THU","FRI","SAT"};
    static char *H12AMPMStr[2][2]= {{"AM","PM"},{"",""}};

    RTC_GetDate(&RTC_DateStruct);
    RTC_GetTime(&RTC_TimeStruct);

    printf(".Date is 20%02x/%02x/%02x(%s).Time is %02x%s:%02x:%02x\r\n", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Day, WeekdayStr[RTC_DateStruct.Week], RTC_TimeStruct.Hour, H12AMPMStr[RTC_TimeStruct.H24][RTC_TimeStruct.AMPM],RTC_TimeStruct.Minute, RTC_TimeStruct.Second);
}

void RTC_IRQHandlerCallBack(void)
{
    if (RTC_GetITState(RTC_IT_ALARMA))
    {
        RTC_ClearITPendingBit(RTC_IT_ALARMA);
        printf("*********Alarm!!!!\r\n");
    }
    if (RTC_GetITState(RTC_IT_INTERVAL))
    {
        RTC_ClearITPendingBit(RTC_IT_INTERVAL);
        ShowTime();
    }
}
void UART1_Configure(void)
{
	//UART1的时钟打开
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
