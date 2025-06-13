/**
 * @file cw32l010_sysctrl.c
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
 * 代码许可和免责信�?
 * 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
 * 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源�?
 * 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或�?
 * 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
 * 或条件�?
 * 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
 * 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
 * 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
 * 损失�?
 * 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些�?
 * 全部上述排除或限制可能并不适用于您�?
 *
 *******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "cw32l010_sysctrl.h"

/* Private_TypesDefinitions --------------------------------------------------*/


/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

const uint32_t hsiprs_tbl[] = {16, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/

/**
 * @brief 配置SysClk �?HCLK的分频系�?
 *
 * @param HCLKPRS ：SYSCTRL_HCLK_DIV1/2/4/8/16/32/64/128
 */
void SYSCTRL_HCLKPRS_Config(uint32_t HCLKPRS)
{
    assert_param(IS_SYSCTRL_HCLKPRS(HCLKPRS));
    CW_SYSCTRL->CR0 = ((CW_SYSCTRL->CR0 & (~(SYSCTRL_KEY_MASK | SYSCTRL_CR0_HCLKPRS_Msk))) | SYSCTRL_KEY | HCLKPRS);
}
/**
 * @brief 配置HCLK �?PCLK的分频系�?
 *
 * @param PCLKPRS ：SYSCTRL_PCLK_DIV1/2/4/8
 */
void SYSCTRL_PCLKPRS_Config(uint32_t PCLKPRS)
{
    assert_param(IS_SYSCTRL_HCLKPRS(PCLKPRS));
    CW_SYSCTRL->CR0 = ((CW_SYSCTRL->CR0 & (~(SYSCTRL_KEY_MASK | SYSCTRL_CR0_PCLKPRS_Msk))) | SYSCTRL_KEY | PCLKPRS);
}

/**
 * @brief 配置SysClk的时钟源
 *
 * @param SYSCLKSRC ：SYSCTRL_SYSCLKSRC_HSI SYSCTRL_SYSCLKSRC_HSE
 *                    SYSCTRL_SYSCLKSRC_LSI SYSCTRL_SYSCLKSRC_LSE
 */
void SYSCTRL_SYSCLKSRC_Config(uint32_t SYSCLKSRC)
{
    assert_param(IS_SYSCTRL_SYSCLKSRC(SYSCLKSRC));
    CW_SYSCTRL->CR0 = ((CW_SYSCTRL->CR0 & (~(SYSCTRL_KEY_MASK | SYSCTRL_CR0_SYSCLK_Msk))) | SYSCTRL_KEY | SYSCLKSRC);
    SYSCTRL_SystemCoreClockUpdate(SYSCTRL_GetHClkFreq());
}

/**
 * @brief 配置CLKCCS  LSECCS 功能
 *
 * @param CCS  ：SYSCTRL_LSECCSEN  SYSCTRL_CLKCCSEN SYSCTRL_HSECCSEN
 * @param NewState  :ENABLE  DISABLE
 */
void SYSCTRL_CCS_Config(uint32_t CCS, FunctionalState NewState)
{
    assert_param(IS_SYSCTRL_CCS_STATUS(CCS));

    if (NewState != DISABLE)
    {
        CW_SYSCTRL->CR1 = (((CW_SYSCTRL->CR1 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) | CCS);
    }
    else
    {
        CW_SYSCTRL->CR1 = (((CW_SYSCTRL->CR1 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) & (~CCS));
    }
}

/**
 * @brief 配置LSELOCK功能：使能后软件无法关闭LSE时钟
 *
 * @param LSELOCK �?SYSCTRL_LSELOCKEN  SYSCTRL_LSELOCKDIS
 */
void SYSCTRL_LSELOCK_Config(uint32_t LSELOCK)
{
    assert_param(IS_SYSCTRL_LSELOCK_STATUS(LSELOCK));
    CW_SYSCTRL->CR1 = ((CW_SYSCTRL->CR1 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY | LSELOCK);
}

/**
 * @brief 配置WAKEUPCLK功能:决定从深度休眠模式唤醒后所使用的系统时钟源
 *
 * @param WAKEUPCLK ：SYSCTRL_WAKEUPCLKEN   SYSCTRL_WAKEUPCLKDIS
 */
void SYSCTRL_WAKEUPCLK_Config(uint32_t WAKEUPCLK)
{
    assert_param(IS_SYSCTRL_WAKEUPCLK_STATUS(WAKEUPCLK));
    CW_SYSCTRL->CR2 = ((CW_SYSCTRL->CR2 & (~(SYSCTRL_KEY_MASK | SYSCTRL_CR2_WAKEUPCLK_Msk))) | SYSCTRL_KEY | WAKEUPCLK);
}

/**
 * @brief 配置LOCK功能：决定异常故障后是否产生LOCKUP复位
 *
 * @param LOCKUP ：SYSCTRL_LOCKUPEN  SYSCTRL_LOCKUPDIS
 */
void SYSCTRL_LOCKUP_Config(uint32_t LOCKUP)
{
    assert_param(IS_SYSCTRL_LOCKUP_STATUS(LOCKUP));
    CW_SYSCTRL->CR2 = ((CW_SYSCTRL->CR2 & (~(SYSCTRL_KEY_MASK | SYSCTRL_CR2_LOCKUP_Msk))) | SYSCTRL_KEY | LOCKUP);
}

/**
 * @brief 配置端口SWDIO功能
 *
 * @param SWDIO  ：SYSCTRL_SWDIOEN   SYSCTRL_SWDIODIS
 */
void SYSCTRL_SWDIO_Config(uint32_t SWDIO)
{
    assert_param(IS_SYSCTRL_SWDIO_STATUS(SWDIO));
    CW_SYSCTRL->CR2 = ((CW_SYSCTRL->CR2 & (~(SYSCTRL_KEY_MASK | SYSCTRL_CR2_SWDIO_Msk))) | SYSCTRL_KEY | SWDIO);
}

/**
  * @brief  Resets the SYSCTRL clock configuration to the default reset state.
  * @param  None
  * @retval 0:success, 1:fault
  * CAUTION:
  * 1、LSEEN Bit Not Changed ,so it will not affect RTC,BUT LSELOCK BIT Changed.
  * 2、Before CALL this funciton,we should set FLASH_CR2.WAT
  */
int SYSCTRL_DeInit(void)
{
    uint16_t timeout = 0xffff;
    /* HSIEN Enable*/
    CW_SYSCTRL->CR1 = ((CW_SYSCTRL->CR1 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY | SYSCTRL_HSIEN);

    //Here need wait HSI STBALE
    while (((CW_SYSCTRL->HSI & SYSCTRL_HSI_STABLE_Msk) == 0) && timeout)
    {
        timeout--;
    }        //Wait stable

    if (timeout == 0) return 1;

    /* SWITCH TO HSI*/
    CW_SYSCTRL->CR0 = ((CW_SYSCTRL->CR0 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY | SYSCTRL_SYSCLKSRC_HSI);

    /* HCLKPRS SET TO 000,PCLKPRS SET TO 00*/
    CW_SYSCTRL->CR0 = ((CW_SYSCTRL->CR0 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY | SYSCTRL_HCLK_DIV1 | \
                       SYSCTRL_PCLK_DIV1 | SYSCTRL_SYSCLKSRC_HSI);

    /* CLKCCS/LSECCS/LSELOCK/LSIEN/HEXEN Disable   CLKCCS / LSEEN NO CHANGE*/
    CW_SYSCTRL->CR1 = (((CW_SYSCTRL->CR1 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY | SYSCTRL_HSIEN) \
                       & (~SYSCTRL_CLKCCSEN) & (~SYSCTRL_LSECCSEN) \
                       & (~SYSCTRL_LSELOCKEN) & (~SYSCTRL_LSIEN) & (~SYSCTRL_HSEEN));

    /* SWDIO Enable,LOCKUP/WAKEUPCLK Disable*/
    CW_SYSCTRL->CR2 = (((((CW_SYSCTRL->CR2 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) \
                         & (~SYSCTRL_SWDIOEN)) & (~SYSCTRL_LOCKUPEN)) & (~SYSCTRL_WAKEUPCLKEN));

    /* Disable all interrupts */
    CW_SYSCTRL->IER = 0x5A5AFE00;      //IER have KEY attribute

    /* Clear all pending bits  */
    CW_SYSCTRL->ICR &= 0xFFFFFE00;

    return 0;

}

/**
 * @brief
 *
 * @param NewFreq
 */
void SYSCTRL_SystemCoreClockUpdate(uint32_t NewFreq)
{
    SystemCoreClock = NewFreq;
}

/**
 * @brief 获得系统时钟（HCLK）频率�?
 *
 * @return uint32_t   HCLK频率�?Hz)
 */
uint32_t SYSCTRL_GetHClkFreq(void)
{
    uint32_t u32rtnVal = 0;
    uint32_t sysclksource = 0;
    uint32_t hsiprs = 0;

    sysclksource = CW_SYSCTRL->CR0_f.SYSCLK;
    hsiprs = CW_SYSCTRL->HSI_f.DIV;

    switch (sysclksource)
    {
        case 0x00:  /* HSI used as system clock */
            u32rtnVal = (HSIOSC_VALUE / hsiprs_tbl[hsiprs]);
            break;

        case 0x01:  /* HSE used as system clock */
            u32rtnVal = HSE_VALUE;
            break;

        case 0x03:  /* LSI used as system clock */
            u32rtnVal = LSI_VALUE;
            break;

        case 0x04:  /* LSE used as system clock */
            u32rtnVal = LSE_VALUE;
            break;

        default:
            u32rtnVal = (HSIOSC_VALUE / hsiprs_tbl[hsiprs]);
            break;
    }

    u32rtnVal = (u32rtnVal >> CW_SYSCTRL->CR0_f.HCLKPRS);
    return u32rtnVal;
}

/**
 * @brief 获得外设时钟（PCLK）频率�?
 *
 * @return uint32_t    PCLK频率�?Hz)
 */
uint32_t SYSCTRL_GetPClkFreq(void)
{
    uint32_t u32Val = 0;

    u32Val = SYSCTRL_GetHClkFreq();
    u32Val = (u32Val >> (CW_SYSCTRL->CR0_f.PCLKPRS));
    return u32Val;
}

/**
* @brief  Enable HSI
* @param  HSIDiv:
*   SYSCTRL_HSIOSC_DIV1
*   SYSCTRL_HSIOSC_DIV2
*   SYSCTRL_HSIOSC_DIV3
*   SYSCTRL_HSIOSC_DIV4
*   SYSCTRL_HSIOSC_DIV5
*   SYSCTRL_HSIOSC_DIV6
*   SYSCTRL_HSIOSC_DIV7
*   SYSCTRL_HSIOSC_DIV8
*   SYSCTRL_HSIOSC_DIV9
*   SYSCTRL_HSIOSC_DIV10
*   SYSCTRL_HSIOSC_DIV11
*   SYSCTRL_HSIOSC_DIV12
*   SYSCTRL_HSIOSC_DIV13
*   SYSCTRL_HSIOSC_DIV14
*   SYSCTRL_HSIOSC_DIV15
*   SYSCTRL_HSIOSC_DIV16
*
* @retval 0:success, 1:fault
*/
int SYSCTRL_HSI_Enable(uint32_t HSIDiv)
{
    uint16_t timeout = 0xffff;

    assert_param(IS_SYSCTRL_HSIOSC_DIV(HSIDiv));    

    if (HSIDiv == SYSCTRL_HSIOSC_DIV1)
    {
        if ((CW_SYSCTRL->CR0 & SYSCTRL_CR0_SYSCLK_Msk) == SYSCTRL_SYSCLKSRC_HSI)
        {
            REGBITS_SET(CW_SYSCTRL->CR2,0x5A5A0000 | bv4);    // 防止直接切入高速时钟时，FLASH没有加入等待周期，造成宕机
        }
    }

    CW_SYSCTRL->HSI = (HSIDiv) | ((*((volatile uint16_t*)SYSCTRL_HSI_TRIMCODEADDR)) & 0x7FF);
    
    CW_SYSCTRL->CR1 |= SYSCTRL_KEY | SYSCTRL_CR1_HSIEN_Msk;                   //Enable HSI

    while (((CW_SYSCTRL->HSI & SYSCTRL_HSI_STABLE_Msk) == 0) && timeout)
    {
        timeout--;
    }//Wait stable

    if (timeout == 0) return 1;

    return 0;
}

/**
* @brief  Disable HSI
* @param  None.
* @retval 0:success, 1:fault
*/
void SYSCTRL_HSI_Disable(void)
{
    CW_SYSCTRL->CR1 = (((CW_SYSCTRL->CR1 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) & \
                       (~SYSCTRL_CR1_HSIEN_Msk));
}


/**
* @brief  Enable LSI
* @param  None:
* @retval None
*/
int SYSCTRL_LSI_Enable(void)
{
    uint16_t timeout = 0xffff;
    CW_SYSCTRL->LSI = *((uint16_t*)SYSCTRL_LSI_TRIMCODEADDR) & 0x3FF;                       
    CW_SYSCTRL->LSI |= SYSCTRL_LSI_WAITCYCLE_258;                                     //WaitCycle = 258
    CW_SYSCTRL->CR1 |= SYSCTRL_KEY | SYSCTRL_CR1_LSIEN_Msk;                    //Enable LSI

    while (((CW_SYSCTRL->LSI & SYSCTRL_LSI_STABLE_Msk) == 0) && timeout)
    {
        timeout--;
    }        //Wait stable

    if (timeout == 0) return 1;

    return 0;

}

/**
* @brief  Disable LSI
* @param  None.
* @retval None.
*/
void SYSCTRL_LSI_Disable(void)
{
    CW_SYSCTRL->CR1 = (((CW_SYSCTRL->CR1 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) & \
                       (~SYSCTRL_CR1_LSIEN_Msk));
}

/**
* @brief  Set the HSE WAITCYCLE
* @param  WaitClock: SYSCTRL_HSE_WAITCYCLE_8192,SYSCTRL_HSE_WAITCYCLE_32768,
*                    SYSCTRL_HSE_WAITCYCLE_131072, SYSCTRL_HSE_WAITCYCLE_262144
* @retval None.
*/
void SYSCTRL_HSEWaitClockSet(uint32_t WaitClock)
{
   
    REGBITS_MODIFY(CW_SYSCTRL->HSE, SYSCTRL_HSE_WAITCYCLE_Msk, WaitClock);
}

/**
* @brief  Set the HEX enable pin polarity
* @param  Polarity: SYSCTRL_HEX_ENPOL_HIGH,SYSCTRL_HEX_ENPOL_LOW
*
* @retval None.
*/
void SYSCTRL_HEXEnablePinPolSet(uint32_t Polarity)
{
    // HEXEN pin PA05
    CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOA_Msk;
    CW_GPIOA->DIR_f.PIN5 = 0;
    CW_GPIOA->ANALOG_f.PIN5 = 0;
    CW_GPIOA->AFRL_f.AFR5 = 5;
    
    REGBITS_MODIFY(CW_SYSCTRL->HSE, SYSCTRL_HSE_HEXENPOL_Msk, Polarity);
}

/**
* @brief  Enable HSE
* @param  Mode:   SYSCTRL_HSE_MODE_OSC: Oscillator,
*                 SYSCTRL_HSE_MODE_CLK: Input Clk
*
* @param  FreqIn: 4000000Hz ~ 32000000Hz
*
* @param  Driver: SYSCTRL_HSE_DRIVER_LEVEL0~SYSCTRL_HSE_DRIVER_LEVEL9 0:最�?
*
* @param  Flt: SYSCTRL_HSE_FLT_OPEN,
*              SYSCTRL_HSE_FLT_CLOSE
*
* @retval None

注意�?、选择负载电容小的晶体更容易起振�?
2、Flt: 强干扰环境打开,一般不打开
*/
int SYSCTRL_HSE_Enable(uint32_t Mode, uint32_t FreqIn, uint8_t Driver, uint32_t Flt)
{   
    uint16_t timeout = 0xffff;
    //参数断言
    assert_param(IS_SYSCTRL_HSE_MODE(Mode));
   
  
    //根据HSE模式设置IO口功�?
    CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOA_Msk; //Open GPIOF Clk
    if (Mode == SYSCTRL_HSE_MODE_OSC)                 //Oscillator
    {
        CW_GPIOA->ANALOG |= (bv0 | bv1);          //PA00 PA01 as analog pin
    }
    else                                          //Input clk
    {
        CW_GPIOA->ANALOG &= ~bv0;                 //PA00 as input clk pin
        CW_GPIOA->DIR    |=  bv0;                 //PA00 input
        CW_GPIOA->PUR    &= ~bv0;
    }
    
    //设置HSE参数
    CW_SYSCTRL->HSE = 0x00200000 /* PDRIVER = 01 */ | ((8000000000 / FreqIn)<< 8) | \
                      (Mode) | \
                      (SYSCTRL_HSE_WAITCYCLE_131072) | \
                       (Driver) | \
                       (Flt);
    //使能HSE,同时使能CLKCCS和HSECCS，使能时钟失效切�?
    CW_SYSCTRL->CR1 |= SYSCTRL_KEY | SYSCTRL_CR1_CLKCCS_Msk | SYSCTRL_CR1_HSECCS_Msk | SYSCTRL_CR1_HSEEN_Msk;  //Enable HSE
    //等待HSE稳定
    while (((CW_SYSCTRL->HSE & SYSCTRL_HSE_STABLE_Msk) != SYSCTRL_HSE_STABLE_Msk) && timeout--);        //Wait stable
    if (timeout == 0) return 1;
    return 0;
}

/**
* @brief  Disable HSE
* @param  None.
* @retval None.
*/
void SYSCTRL_HSE_Disable(void)
{
    CW_SYSCTRL->CR1 = (((CW_SYSCTRL->CR1 & (~ SYSCTRL_CR1_KEY_Msk)) | SYSCTRL_KEY) & \
                       (~SYSCTRL_CR1_HSEEN_Msk));
}


/**
* @brief  Enable LSE
* @param  Mode:   SYSCTRL_LSE_MODE_OSC: Oscillator,
*                 SYSCTRL_LSE_MODE_CLK: Input Clk
* @param  Driver: SYSCTRL_LSE_DRIVER_LEVEL0~15
*
* @retval  0：succes�?1：fault

驱动配置建议�?
Driver：SYSCTRL_LSE_DRIVER_LEVEL2
*/

int SYSCTRL_LSE_Enable(uint32_t Mode, uint8_t Driver)
{
    uint32_t timeout = 0x4ffff;
    //参数断言
    assert_param(IS_SYSCTRL_LSE_MODE(Mode));
    assert_param(IS_SYSCTRL_LSE_DRIVER(Driver));

    //根据LSE模式，设置IO功能
    CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOB_Msk; //Open GPIOB clk

    if (Mode == SYSCTRL_LSE_MODE_OSC)                //Oscillator
    {
        CW_GPIOB->ANALOG |= (bv0 | bv1);      //PB00 PB01 as analog pin
    }
    else                                          //Input clk
    {
        CW_GPIOB->ANALOG &= ~bv1;                //PB01 as input clk pin
        CW_GPIOB->DIR |= bv1;                //PB01 input
        CW_GPIOB->PUR &= bv1;
    }

    //设置LSE参数
    CW_SYSCTRL->LSE = 0x00000A20 |(Mode) | (Driver);

    //LSE使能,同时使能CLKCCS和LSECCS，使能时钟失效切�?
    CW_SYSCTRL->CR1 |= SYSCTRL_KEY | SYSCTRL_CLKCCSEN | SYSCTRL_CR1_LSECCS_Msk | SYSCTRL_CR1_LSEEN_Msk;                    //Enable LSE

    //等待LSE稳定
    while ((CW_SYSCTRL->LSE_f.STABLE == 0) && timeout)
    {
        timeout--;
    }//Wait stable

    if (timeout == 0) return 1;

    return 0;
}

/**
* @brief  Disable LSE
* @param  None
* @retval None
*/
void SYSCTRL_LSE_Disable(void)
{
    CW_SYSCTRL->CR1 = (((CW_SYSCTRL->CR1 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) & \
                       (~SYSCTRL_CR1_LSEEN_Msk));
}

/**
* @brief  CLKSRC switch
* @param  NewClk:
*                SYSCTRL_SYSCLKSRC_HSI,
*                SYSCTRL_SYSCLKSRC_HSE,
*                SYSCTRL_SYSCLKSRC_LSI,
*                SYSCTRL_SYSCLKSRC_LSE
* @retval 0: PASS, 1: Fail
*
* NOTE:调用此函数之前需要保证新时钟源已经使能（ENABLE函数中已包含等待时钟稳定过程）�?
*/

int SYSCTRL_SysClk_Switch(uint32_t NewClk)
{
    assert_param(IS_SYSCTRL_SYSCLKSRC(NewClk));

    switch (NewClk)
    {
        default:
        case SYSCTRL_SYSCLKSRC_HSI:
            if ((CW_SYSCTRL->ISR_f.HSIRDY == 0) || (CW_SYSCTRL->ISR_f.HSISTABLE == 0))
                return 1;

            break;

        case SYSCTRL_SYSCLKSRC_HSE:
            if (CW_SYSCTRL->ISR_f.HSESTABLE == 0)
                return 1;

            break;

        case SYSCTRL_SYSCLKSRC_LSI:
            if ((CW_SYSCTRL->ISR_f.LSIRDY == 0) || (CW_SYSCTRL->ISR_f.LSISTABLE == 0))
                return 1;

            break;

        case SYSCTRL_SYSCLKSRC_LSE:
            if ((CW_SYSCTRL->ISR_f.LSEFAIL == 1) || (CW_SYSCTRL->ISR_f.LSERDY == 0) || \
                    (CW_SYSCTRL->ISR_f.LSESTABLE == 0) || (CW_SYSCTRL->ISR_f.LSEFAIL == 1))
                return 1;

            break;
    }

    CW_SYSCTRL->CR0 = ((((CW_SYSCTRL->CR0 & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) & (~SYSCTRL_CR0_SYSCLK_Msk)) | (NewClk));

    SYSCTRL_SystemCoreClockUpdate(SYSCTRL_GetHClkFreq());

    return (((CW_SYSCTRL->CR0 & SYSCTRL_CR0_SYSCLK_Msk) == NewClk) ? 0U : 1U);
}

/**
 * @brief SYSCTRL中断配置
 *
 * @param SYSCTRL_IT ：SYSCTRL_IT_LSEFAULT SYSCTRL_IT_LSEFAIL
 *                 SYSCTRL_IT_LSERDY     SYSCTRL_IT_LSIRDY
 *                 SYSCTRL_IT_HSIRDY
 * @param NewState  :ENABLE  DISABLE
 */
void SYSCTRL_ITConfig(uint32_t SYSCTRL_IT, FunctionalState NewState)
{
    assert_param(IS_SYSCTRL_IT(SYSCTRL_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)     //IER have KEY attribute
    {
        CW_SYSCTRL->IER = ((CW_SYSCTRL->IER & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) | SYSCTRL_IT;
    }
    else                         //IER have KEY attribute
    {
        CW_SYSCTRL->IER = ((CW_SYSCTRL->IER & (~SYSCTRL_KEY_MASK)) | SYSCTRL_KEY) & (~SYSCTRL_IT);
    }
}

/**
 * @brief 获取SYSCTRL指定中断状�?
 *
 * @param SYSCTRL_IT �?SYSCTRL_IT_LSEFAULT    SYSCTRL_IT_LSEFAIL
 *                 SYSCTRL_IT_LSERDY     SYSCTRL_IT_LSIRDY
 *                 SYSCTRL_IT_HSIRDY
 *
 * @return ITStatus  : SET   RESET
 */
ITStatus SYSCTRL_GetITStatus(uint32_t SYSCTRL_IT)
{
    assert_param(IS_SYSCTRL_GET_IT(SYSCTRL_IT));

    return (CW_SYSCTRL->ISR & SYSCTRL_IT) ? SET : RESET;
}


/**
 * @brief 清除SYSCTRL指定中断标志
 *
 * @param SYSCTRL_IT ：SYSCTRL_IT_LSEFAULT     SYSCTRL_IT_LSEFAIL
 *                 SYSCTRL_IT_LSERDY     SYSCTRL_IT_LSIRDY
 *                 SYSCTRL_IT_HSIRDY
 *
 */
void SYSCTRL_ClearITPendingBit(uint32_t SYSCTRL_IT)
{
    assert_param(IS_SYSCTRL_GET_IT(SYSCTRL_IT));

    CW_SYSCTRL->ICR &= (~SYSCTRL_IT);
}

/**
 * @brief 获取指定时钟源稳定标�?
 *
 * @param SYSCTRL_STABLEFLAG ：SYSCTRL_FLAG_HSISTABLE  SYSCTRL_FLAG_HEXSTABLE
 *                         SYSCTRL_FLAG_LSESTABLE
 *                         SYSCTRL_FLAG_LSISTABLE
 * @return FlagStatus ：SET   RESET
 */
FlagStatus SYSCTRL_GetStableFlag(uint32_t SYSCTRL_STABLEFLAG)
{
    assert_param(IS_SYSCTRL_STABLEFLAG(SYSCTRL_STABLEFLAG));

    return(((CW_SYSCTRL->ISR) & SYSCTRL_STABLEFLAG) ? SET : RESET);
}

/**
 * @brief  获取所有时钟源稳定标志
 *
 * @return uint32_t ：返回ISR寄存器内�?
 */
uint32_t SYSCTRL_GetAllStableFlag(void)
{
    return(CW_SYSCTRL->ISR);
}


/**
 * @brief AHB外设时钟设置
 *
 * @param Periph  ：SYSCTRL_AHB_PERIPH_GPIOA   SYSCTRL_AHB_PERIPH_GPIOB
 *                  SYSCTRL_AHB_PERIPH_CRC     SYSCTRL_AHB_PERIPH_FLASH
 * @param NewState  ENABLE  DISABLE
 */
void SYSCTRL_AHBPeriphClk_Enable(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_SYSCTRL_AHB_PERIPH(Periph));

    if (NewState != DISABLE)
    {
        CW_SYSCTRL->AHBEN |= SYSCTRL_KEY | Periph;
    }
    else
    {
        CW_SYSCTRL->AHBEN = SYSCTRL_KEY | (CW_SYSCTRL->AHBEN & (~Periph));
    }
}

/**
 * @brief APB外设时钟设置1
 *
 * @param Periph   �? SYSCTRL_APB1_PERIPH_ATIM    SYSCTRL_APB1_PERIPH_GTIM1 
 *                     SYSCTRL_APB1_PERIPH_UART2   SYSCTRL_APB1_PERIPH_UART1
 *                     SYSCTRL_APB1_PERIPH_SPI    SYSCTRL_APB1_PERIPH_VC
 *                     SYSCTRL_APB1_PERIPH_ADC    
 * @param NewState  ENABLE  DISABLE
 */
void SYSCTRL_APBPeriphClk_Enable1(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_SYSCTRL_APB1_PERIPH(Periph));

    if (NewState != DISABLE)
    {
        CW_SYSCTRL->APBEN1 |= SYSCTRL_KEY | Periph;
    }
    else
    {
        CW_SYSCTRL->APBEN1 = SYSCTRL_KEY | (CW_SYSCTRL->APBEN1 & (~Periph));
    }
}

/**
 * @brief APB外设时钟设置2
 *
 * @param Periph �? SYSCTRL_APB2_PERIPH_I2C       SYSCTRL_APB2_PERIPH_LPTIM
 *                   SYSCTRL_APB2_PERIPH_IWDT      SYSCTRL_APB2_PERIPH_RTC
 *                   SYSCTRL_APB2_PERIPH_BTIM123   
 * @param NewState :  ENABLE  DISABLE
 */
void SYSCTRL_APBPeriphClk_Enable2(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_SYSCTRL_APB2_PERIPH(Periph));

    if (NewState != DISABLE)
    {
        CW_SYSCTRL->APBEN2 |= SYSCTRL_KEY | Periph;
    }
    else
    {
        CW_SYSCTRL->APBEN2 = SYSCTRL_KEY | (CW_SYSCTRL->APBEN2 & (~Periph));
    }
}

/**
 * @brief AHB外设复位设置
 *
 * @param Periph  ：SYSCTRL_AHB_PERIPH_GPIOA   SYSCTRL_AHB_PERIPH_GPIOB
 *                  SYSCTRL_AHB_PERIPH_CRC     SYSCTRL_AHB_PERIPH_FLASH
 * @param NewState  : ENABLE  DISABLE
 */
void SYSCTRL_AHBPeriphReset(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_SYSCTRL_AHB_PERIPH(Periph));

    if (NewState != ENABLE)
    {
        CW_SYSCTRL->AHBRST |= Periph;
    }
    else
    {
        CW_SYSCTRL->AHBRST &= (~Periph);
    }
}

/**
 * @brief APB外设复位设置1
 *
 * @param Periph   �? SYSCTRL_APB1_PERIPH_ATIM    SYSCTRL_APB1_PERIPH_GTIM1 
 *                     SYSCTRL_APB1_PERIPH_UART2   SYSCTRL_APB1_PERIPH_UART1
 *                     SYSCTRL_APB1_PERIPH_SPI1    SYSCTRL_APB1_PERIPH_VC
 *                     SYSCTRL_APB1_PERIPH_ADC 
 * @param NewState  : ENABLE  DISABLE
 */
void SYSCTRL_APBPeriphReset1(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_SYSCTRL_APB1_PERIPH(Periph));

    if (NewState != ENABLE)
    {
        CW_SYSCTRL->APBRST1 |= Periph;
    }
    else
    {
        CW_SYSCTRL->APBRST1 &= (~Periph);
    }
}

/**
 * @brief APB外设复位设置2
 *
 * @param Periph �? SYSCTRL_APB2_PERIPH_I2C       SYSCTRL_APB2_PERIPH_LPTIM
 *                   SYSCTRL_APB2_PERIPH_IWDT      SYSCTRL_APB2_PERIPH_RTC
 *                   SYSCTRL_APB2_PERIPH_BTIM123 
 * @param NewState : ENABLE  DISABLE
 */
void SYSCTRL_APBPeriphReset2(uint32_t Periph, FunctionalState NewState)
{
    assert_param(IS_SYSCTRL_APB1_PERIPH(Periph));

    if (NewState != ENABLE)
    {
        CW_SYSCTRL->APBRST2 |= Periph;
    }
    else
    {
        CW_SYSCTRL->APBRST2 &= (~Periph);
    }
}

/**
 * @brief 获取指定的系统复位标�?
 *
 * @param SYSCTRL_RSTFLAG  ：SYSCTRL_FLAG_PORRST   SYSCTRL_FLAG_SYSRESETREQRST
 *                           SYSCTRL_FLAG_LVDRST   SYSCTRL_FLAG_LOCKUPRST
 *                           SYSCTRL_FLAG_IWDTRST  SYSCTRL_FLAG_RSTBRST                 
 * @return FlagStatus  SET RESET
 */
FlagStatus SYSCTRL_GetRstFlag(uint32_t SYSCTRL_RSTFLAG)
{
    assert_param(IS_SYSCTRL_RSTFLAG(SYSCTRL_RSTFLAG));

    return(((CW_SYSCTRL->RESETFLAG) & SYSCTRL_RSTFLAG) ? SET : RESET);
}

/**
 * @brief 获取复位标志寄存器内�?
 *
 * @return uint32_t
 */
uint32_t SYSCTRL_GetAllRstFlag(void)
{
    return(CW_SYSCTRL->RESETFLAG);
}

/**
 * @brief 清除指定或者全部复位标�?
 *
 * @param SYSCTRL_RSTFLAG  ：SYSCTRL_FLAG_PORRST   SYSCTRL_FLAG_SYSRESETREQRST
 *                           SYSCTRL_FLAG_LVDRST   SYSCTRL_FLAG_LOCKUPRST
 *                           SYSCTRL_FLAG_IWDTRST  SYSCTRL_FLAG_RSTBRST
 * @return
 */
void SYSCTRL_ClearRstFlag(uint32_t SYSCTRL_RSTFLAG)
{
    assert_param(IS_SYSCTRL_RSTFLAG(SYSCTRL_RSTFLAG));
    CW_SYSCTRL->RESETFLAG &= (~SYSCTRL_RSTFLAG);
}

/**
 * @brief 配置PCLK_OUT引脚(PA02)输出PCLK时钟
 *
 */
void SYSCTRL_PCLK_OUT(void)
{
	   __SYSCTRL_GPIOA_CLK_ENABLE();
    CW_GPIOA->ANALOG_f.PIN2 = 0U;
    CW_GPIOA->DIR_f.PIN2 = 0U;
    CW_GPIOA->AFRL_f.AFR2 = 1U;
}

/**
 * @brief  配置MCO_OUT引脚(PA04)输出指定的时钟源及分频比
 *
 * @param Source  ：SYSCTRL_MCO_SRC_NONE    SYSCTRL_MCO_SRC_HCLK
 *                  SYSCTRL_MCO_SRC_PCLK    SYSCTRL_MCO_SRC_HSI
 *                  SYSCTRL_MCO_SRC_LSI     SYSCTRL_MCO_SRC_HSE
 *                  SYSCTRL_MCO_SRC_LSE
 *                 
 * @param Div     ：SYSCTRL_MCO_DIV1/2/8/64/128/256/512/1024
 */
void SYSCTRL_MCO_OUT(uint8_t Source, uint8_t Div)
{
    assert_param(IS_SYSCTRL_MCO_SRC(Source));
    assert_param(IS_SYSCTRL_MCO_DIV(Div));

    CW_SYSCTRL->AHBEN |= SYSCTRL_KEY | SYSCTRL_AHBEN_GPIOA_Msk; //Open GPIOA clk

    CW_GPIOA->ANALOG &= ~GPIOx_ANALOG_PIN4_Msk;  //Digit
    CW_GPIOA->AFRL = ((CW_GPIOA->AFRL) & (~(uint32_t)(GPIOx_AFRL_AFR4_Msk))) | (uint32_t)(0x4U << GPIOx_AFRL_AFR4_Pos);
    CW_GPIOA->DIR &= ~GPIOx_DIR_PIN4_Msk;     //Output

    CW_SYSCTRL->MCO = (uint32_t)(Source | Div);
}
void SYSCTRL_GotoSleep(void)
{
    SCB->SCR &= ~(SCB_SCR_SLEEPDEEP_Msk);
    __DSB();
    __WFI();
}

void SYSCTRL_GotoDeepSleep(void)
{
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
    __DSB();
    __WFI();
}

void SYSCTRL_ConfigSleepOnExit(FunctionalState state)
{
    if (state == ENABLE)
    {
        SCB->SCR |= SCB_SCR_SLEEPONEXIT_Msk;
    }
    else
    {
        SCB->SCR &= ~SCB_SCR_SLEEPONEXIT_Msk;
    }
}
