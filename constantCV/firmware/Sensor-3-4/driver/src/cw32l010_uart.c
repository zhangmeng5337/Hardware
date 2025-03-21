/**
 * @file cw32l010_uart.c
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
#include "cw32l010_uart.h"

/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/

/**
 * @brief 通过UARTx发送一个数�?16bit)
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param Data :待发送的数据
 */
void UART_SendData(UART_TypeDef* UARTx, uint16_t Data)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_DATA_16BIT(Data));

    UARTx->TDR = (Data & (uint16_t)0x01FF);
}

/**
 * @brief 通过UARTx发送一个数�?8bit)
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param Data :待发送的数据
 */
void UART_SendData_8bit(UART_TypeDef* UARTx, uint8_t Data)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_DATA_8BIT(Data));

    UARTx->TDR = Data;
}
/**
 * @brief 发送字符串
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2、�?
 * @param String :待发送的字符�?
 */
void UART_SendString(UART_TypeDef* UARTx, char* String)
{
    while (*String != '\0')
    {
        UART_SendData_8bit(UARTx, *String);
        while (UART_GetFlagStatus(UARTx, UART_FLAG_TXE) == RESET);
        String++;
    }
    while (UART_GetFlagStatus(UARTx, UART_FLAG_TXBUSY) == SET);
}

/**
 * @brief 通过UARTx接收一个数�?16bit)
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @return uint16_t :接收的数�?
 */
uint16_t UART_ReceiveData(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    return (uint16_t)(UARTx->RDR & (uint16_t)0x01FF);
}

/**
 * @brief 通过UARTx接收一个数�?8bit)
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @return uint8_t :接收的数�?
 */
uint8_t UART_ReceiveData_8bit(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    return (uint8_t)(UARTx->RDR);
}

/**
 * @brief 根据UART_InitStruct中指定的参数初始化UARTx外设
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_InitStruct :指向UART_InitTypeDef类型结构体的指针
 */
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
    uint16_t integerdivider = 0x00;
    uint16_t fractionaldivider = 0x00;
    float32_t temp = 0;

    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_BAUDRATE(UART_InitStruct->UART_BaudRate));
    assert_param(IS_UART_Over(UART_InitStruct->UART_Over));
    assert_param(IS_UART_Source(UART_InitStruct->UART_Source));
    assert_param(IS_UART_UCLKFREQ(UART_InitStruct->UART_UclkFreq));
    assert_param(IS_UART_StartBit(UART_InitStruct->UART_StartBit));
    assert_param(IS_UART_STOPBITS(UART_InitStruct->UART_StopBits));
    assert_param(IS_UART_PARITY(UART_InitStruct->UART_Parity));
    assert_param(IS_UART_MODE(UART_InitStruct->UART_Mode));
    assert_param(IS_UART_HARDWARE_FLOW_CONTROL(UART_InitStruct->UART_HardwareFlowControl));
    
    switch((uint32_t)UARTx)
    {
        case (uint32_t)CW_UART1:
            CW_SYSCTRL->APBEN1 = (CW_SYSCTRL->APBEN1 & 0x0000FFFF) | 0x5a5a0000 | SYSCTRL_APBEN1_UART1_Msk; 
            break;
        case (uint32_t)CW_UART2:
            CW_SYSCTRL->APBEN1 = (CW_SYSCTRL->APBEN1 & 0x0000FFFF) | 0x5a5a0000 | SYSCTRL_APBEN1_UART2_Msk; 
            break;
        default:
            break;
    }

    /* UARTx_CR1 */
    REGBITS_MODIFY(UARTx->CR1, UARTx_CR1_SOURCE_Msk | UARTx_CR1_OVER_Msk | UARTx_CR1_START_Msk |\
                   UARTx_CR1_STOP_Msk | UARTx_CR1_PARITY_Msk |\
                   UARTx_CR1_RXEN_Msk | UARTx_CR1_TXEN_Msk    \
                   ,\
                  UART_InitStruct->UART_Source | \
                  (uint32_t)UART_InitStruct->UART_Over |\
                   UART_InitStruct->UART_StartBit       |\
                   UART_InitStruct->UART_StopBits       |\
                   UART_InitStruct->UART_Parity         |\
                   UART_InitStruct->UART_Mode);

    /* UARTx_CR2 */
    REGBITS_MODIFY(UARTx->CR2,  UARTx_CR2_RTSEN_Msk |\
                   UARTx_CR2_CTSEN_Msk  \
                   ,\
                   UART_InitStruct->UART_HardwareFlowControl);
    /* 校验�?*/
    if (UART_InitStruct->UART_Parity != UART_Parity_No)
    {
        UARTx->CR1_f.CHLEN = 1;
    }
    else
    {
        UARTx->CR1_f.CHLEN = 0;
    }

    /* 波特率设�?*/
    if(UART_Over_16 == UART_InitStruct->UART_Over)
    {
        /* 16倍采�?*/
        temp = (float32_t)(UART_InitStruct->UART_UclkFreq) / (16 * (UART_InitStruct->UART_BaudRate));
        integerdivider = (uint16_t)temp;
        fractionaldivider = (uint16_t)((temp - integerdivider)*16 + 0.5);
        UARTx->BRRI = (uint16_t)integerdivider;
        UARTx->BRRF = (uint16_t)fractionaldivider;
    }
    else if(UART_Over_8 == UART_InitStruct->UART_Over)
    {
        /* 8倍采�?*/
        integerdivider = (uint16_t)((float32_t)(UART_InitStruct->UART_UclkFreq) / (8 * (UART_InitStruct->UART_BaudRate)) + 0.5);
        UARTx->BRRI = (uint16_t)integerdivider;
    }
    else if(UART_Over_4 == UART_InitStruct->UART_Over)
    {
        /* 4倍采�?*/
        integerdivider = (uint16_t)((float32_t)(UART_InitStruct->UART_UclkFreq) / (4 * (UART_InitStruct->UART_BaudRate)) + 0.5);
        UARTx->BRRI = (uint16_t)integerdivider;
    }
    else
    {
        /* 专用采样 */
        /* 专用采样仅适合传输时钟源为LSE或者LSI时，进行2400bps�?800bps�?600bps波特率下的UART通信 */
        integerdivider = (uint16_t)((float32_t)(UART_InitStruct->UART_UclkFreq)*256 / (UART_InitStruct->UART_BaudRate) + 0.5);
        UARTx->BRRI = (uint16_t)integerdivider;
    }
}

/**
 * @brief 默认值赋值UART_InitStruct结构体成�?
 *
 * @param UART_InitStruct :指向UART_InitTypeDef类型结构体的指针
 */
void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
    /* UART_InitStruct members default value */
    UART_InitStruct->UART_BaudRate = 9600;
    UART_InitStruct->UART_Over = UART_Over_16;
    UART_InitStruct->UART_Source = UART_Source_PCLK;
    UART_InitStruct->UART_StartBit = UART_StartBit_FE;
    UART_InitStruct->UART_StopBits = UART_StopBits_1;
    UART_InitStruct->UART_Parity = UART_Parity_No ;
    UART_InitStruct->UART_Mode = UART_Mode_Rx | UART_Mode_Tx;
    UART_InitStruct->UART_HardwareFlowControl = UART_HardwareFlowControl_None;
}

/**
 * @brief UARTx同步模式初始化，CPOL=1、CPHA=1，只能做主机
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_ClockInitStruct :指向UART_ClockInitTypeDef类型结构体的指针
 */
void UART_ClockInit(UART_TypeDef* UARTx, UART_ClockInitTypeDef* UART_ClockInitStruct)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CLOCK(UART_ClockInitStruct->UART_Clock));
    assert_param(IS_UART_Source(UART_ClockInitStruct->UART_Source));

    REGBITS_MODIFY(UARTx->CR1, UARTx_CR1_SYNC_Msk | UARTx_CR1_SOURCE_Msk,\
                   (uint32_t)UART_ClockInitStruct->UART_Clock | \
                   (uint32_t)UART_ClockInitStruct->UART_Source);
}

/**
 * @brief 默认值赋值UART_ClockInitStruct结构体成�?
 *
 * @param UART_ClockInitStruct :指向UART_ClockInitTypeDef类型结构体的指针
 */
void UART_ClockStructInit(UART_ClockInitTypeDef* UART_ClockInitStruct)
{
    /* UART_ClockInitStruct members default value */
    UART_ClockInitStruct->UART_Clock = UART_Clock_Disable;
    UART_ClockInitStruct->UART_Source = UART_Source_PCLK;
}

/**
 * @brief UART1去初始化
 *
 */
void UART1_DeInit(void)
{
    CW_SYSCTRL->APBRST1_f.UART1 = 0;
    CW_SYSCTRL->APBRST1_f.UART1 = 1;
}

/**
 * @brief UART2去初始化
 *
 */
void UART2_DeInit(void)
{
    CW_SYSCTRL->APBRST1_f.UART2 = 0;
    CW_SYSCTRL->APBRST1_f.UART2 = 1;
}


/**
 * @brief 使能/失能UARTx中断
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_IT :中断�?
 *   参数可以�?
 *     @arg UART_IT_RXBRK: 间隔段接收完成中�?
 *     @arg UART_IT_BAUD:  自动波特率检测完成中�?
 *     @arg UART_IT_TIMOV: 定时器溢出中�?
 *     @arg UART_IT_CTS:   CTS信号变化中断
 *     @arg UART_IT_PE:    奇偶校验错误中断
 *     @arg UART_IT_FE:    帧结构错误中�?
 *     @arg UART_IT_RC:    接收完成中断
 *     @arg UART_IT_TC:    发送完成中�?
 *     @arg UART_IT_TXE:   发送缓冲空中断
 * @param NewState :ENABLE or DISABLE
 */
void UART_ITConfig(UART_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_IT(UART_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    NewState ? (UARTx->IER |= UART_IT) : (UARTx->IER &= (uint16_t)~UART_IT);

    // if (NewState != DISABLE)
    // {
    //   UARTx->IER |= UART_IT;
    // }
    // else
    // {
    //   UARTx->IER &= (uint16_t)~UART_IT;
    // }
}

/**
 * @brief 获取UARTx中断标志�?
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_IT :中断�?
 *   参数可以是：
 *     @arg UART_IT_RXBRK: 间隔段接收完成中�?
 *     @arg UART_IT_BAUD:  自动波特率检测完成中�?
 *     @arg UART_IT_TIMOV: 定时器溢出中�?
 *     @arg UART_IT_CTS:   CTS信号变化中断
 *     @arg UART_IT_PE:    奇偶校验错误中断
 *     @arg UART_IT_FE:    帧结构错误中�?
 *     @arg UART_IT_RC:    接收完成中断
 *     @arg UART_IT_TC:    发送完成中�?
 *     @arg UART_IT_TXE:   发送缓冲器空中�?
 * @return ITStatus :UART_IT当前状�?(SET or RESET)
 */
ITStatus UART_GetITStatus(UART_TypeDef* UARTx, uint16_t UART_IT)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_IT(UART_IT));

    return (REGBITS_GET(UARTx->ISR, UART_IT) ? SET : RESET);
}

/**
 * @brief 清除UARTx中断标志�?
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_IT :中断�?
 *   参数可以是：
 *     @arg UART_IT_RXBRK: 间隔段接收完成中�?
 *     @arg UART_IT_BAUD:  自动波特率检测完成中�?
 *     @arg UART_IT_TIMOV: 定时器溢出中�?
 *     @arg UART_IT_CTS:   CTS信号变化中断
 *     @arg UART_IT_PE:    奇偶校验错误中断
 *     @arg UART_IT_FE:    帧结构错误中�?
 *     @arg UART_IT_RC:    接收完成中断
 *     @arg UART_IT_TC:    发送完成中�?
 * @note
 *    -TXE中断标志位只能通过写UARTx_TDR寄存器来清除
 *    -RC中断标志位只能软件清除，不能通过读UARTx_RDR寄存器来清除
 */
void UART_ClearITPendingBit(UART_TypeDef* UARTx, uint16_t UART_IT)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CLEAR_IT(UART_IT));

    REGBITS_CLR(UARTx->ICR, UART_IT);
}

/**
 * @brief 获取UARTx标志�?
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param  UART_FLAG :标志
 *   参数可以是：
 *     @arg UART_FLAG_RXBRK:   间隔段接收完成中�?
 *     @arg UART_FLAG_BAUD:    自动波特率检测完成中�?
 *     @arg UART_FLAG_TIMOV:   定时器溢出中�?
 *     @arg UART_FLAG_TXBUSY:  发送忙标志
 *     @arg UART_FLAG_CTSLV:   CTS信号电平状态标�?
 *     @arg UART_FLAG_CTS:     CTS信号变化标志
 *     @arg UART_FLAG_MATCH:   从机地址匹配标志
 *     @arg UART_FLAG_PE:      奇偶校验错误标志
 *     @arg UART_FLAG_FE:      帧结构错误标�?
 *     @arg UART_FLAG_RC:      接收完成标志
 *     @arg UART_FLAG_TC:      发送完成标�?
 *     @arg UART_FLAG_TXE:     发送缓冲器空标�?
 * @return FlagStatus :UART_FLAG当前状�?(SET or RESET)
 */
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint16_t UART_FLAG)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_FLAG(UART_FLAG));

    return (REGBITS_GET(UARTx->ISR, UART_FLAG) ? SET : RESET);
}

/**
 * @brief 清除UARTx标志�?
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param  UART_FLAG :标志
 *   参数可以是：
 *     @arg UART_FLAG_RXBRK:   间隔段接收完成中�?
 *     @arg UART_FLAG_BAUD:    自动波特率检测完成中�?
 *     @arg UART_FLAG_TIMOV:   定时器溢出中�?
 *     @arg UART_FLAG_CTS:     CTS信号变化标志
 *     @arg UART_FLAG_PE:      奇偶校验错误标志
 *     @arg UART_FLAG_FE:      帧结构错误标�?
 *     @arg UART_FLAG_RC:      接收完成标志
 *     @arg UART_FLAG_TC:      发送完成标�?
 * @note
 *    -TXBUSY、CTSLV、MATCH标志位由硬件自动设置和清�?
 *    -TXE标志位只能通过写UARTx_TDR寄存器来清除
 *    -RC标志位只能软件清除，不能通过读UARTx_RDR寄存器来清除
 */
void UART_ClearFlag(UART_TypeDef* UARTx, uint16_t UART_FLAG)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_CLEAR_FLAG(UART_FLAG));

    REGBITS_CLR(UARTx->ICR, UART_FLAG);
}

/**
 * @brief 使能/失能TXD输出、RXD输入
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_DirectionMode :传输方向
 *   参数可以是：
 *     @arg UART_Mode_Tx: TXD输出
 *     @arg UART_Mode_Rx: RXD输入
 * @param NewState :ENABLE or DISABLE
 */
void UART_DirectionModeCmd(UART_TypeDef* UARTx, uint16_t UART_DirectionMode, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_MODE(UART_DirectionMode));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    NewState ? (UARTx->CR1 |= UART_DirectionMode) : (UARTx->CR1 &= (uint16_t)~UART_DirectionMode);

    // if (NewState != DISABLE)
    // {
    //   UARTx->CR1 |= UART_DirectionMode;
    // }
    // else
    // {
    //   UARTx->CR1 &= (uint16_t)~UART_DirectionMode;
    // }
}

/**
 * @brief 使能/失能TXD输出信号、RXD输入信号反相
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_InvPin :反相引脚
 *   参数可以是：
 *     @arg UART_InvPin_Tx: TXD引脚输出信号反相
 *     @arg UART_InvPin_Rx: RXD引脚输入信号反相
 * @param NewState :ENABLE or DISABLE
 */
void UART_InvPinCmd(UART_TypeDef* UARTx, uint16_t UART_InvPin, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_INVERSTION_PIN(UART_InvPin));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    NewState ? (UARTx->CR2 |= UART_InvPin) : (UARTx->CR2 &= (uint16_t)~UART_InvPin);

    // if (NewState != DISABLE)
    // {
    //   UARTx->CR2 |= UART_InvPin;
    // }
    // else
    // {
    //   UARTx->CR2 &= (uint16_t)~UART_InvPin;
    // }
}

/**
 * @brief 使能/失能UARTx半双工通信
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param NewState :ENABLE or DISABLE
 */

void UART_HalfDuplexCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    UARTx->CR1_f.SIGNAL = NewState;
}

/**
 * @brief 使能/失能UARTx DMA
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_DMAReq :DMA请求
 *   参数可以是：
 *     @arg UART_DMAReq_Tx: DMA发送请�?
 *     @arg UART_DMAReq_Rx: DMA接收请求
 * @param NewState :ENABLE or DISABLE
 */
void UART_DMACmd(UART_TypeDef* UARTx, uint16_t UART_DMAReq, FunctionalState NewState)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_DMAREQ(UART_DMAReq));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    NewState ? (UARTx->CR2 |= UART_DMAReq) : (UARTx->CR2 &= (uint16_t)~UART_DMAReq);
}

/**
 * @brief UARTx多机通信配置
 *
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_Address :从机地址
 * @param UART_AddressMsK :从机地址掩码
 * @note
 *        主机和从机均需配置为自定义校验
 */
void UART_SetMultiMode(UART_TypeDef* UARTx, uint8_t UART_Address, uint8_t UART_AddressMsK)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_ADDRESS(UART_Address));
    assert_param(IS_UART_ADDRESSMSK(UART_AddressMsK));

    REGBITS_SET(UARTx->CR2, UARTx_CR2_ADDREN_Msk);
    REGBITS_MODIFY(UARTx->ADDR, UARTx_ADDR_ADDR_Msk, (uint32_t)UART_Address);
    REGBITS_MODIFY(UARTx->MASK, UARTx_MASK_MASK_Msk, (uint32_t)UART_AddressMsK);
}

/**
 * @brief 使能/失能LIN模式
 * 
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param NewState :ENABLE or DISABLE
 */
void UART_LINCmd(UART_TypeDef* UARTx, FunctionalState NewState)
{
  assert_param(IS_UART_ALL_PERIPH(UARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  UARTx->CR3_f.LIN = NewState;
}


/**
 * @brief 设置LIN模式接收间隔段的长度阈�?
 * 
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_LINBreakDetectLength :间隔段长度阈�?
  *   参数可以是：
 *     @arg UART_LINBreakDetectLength_10b: 10bit
 *     @arg UART_LINBreakDetectLength_11b: 11bit
 */
void UART_LINBreakDetectLengthConfig(UART_TypeDef* UARTx, uint16_t UART_LINBreakDetectLength)
{
  assert_param(IS_UART_ALL_PERIPH(UARTx));
  assert_param(IS_UART_LIN_BREAK_DETECT_LENGTH(UART_LINBreakDetectLength));
   
  REGBITS_MODIFY(UARTx->CR3, UARTx_CR3_BRKL_Msk, (uint32_t)UART_LINBreakDetectLength);
}

/**
 * @brief 定时器工作模式配�?
 * 
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param UART_TimerMode :定时器工作模�?
  *   参数可以是：
 *     @arg UART_TimerMode_Stop:          停止定时�?
 *     @arg UART_TimerMode_TimeOut:       等待超时检测模�?
 *     @arg UART_TimerMode_Idle:          接收空闲检测模�?
 *     @arg UART_TimerMode_AutoBaudRate1: 自动波特率侦测模�?
 *     @arg UART_TimerMode_AutoBaudRate2: 自动波特率侦测模�?
 *     @arg UART_TimerMode_GeneralTimer:  通用定时功能
 */
void UART_TimerModeConfig(UART_TypeDef* UARTx, uint16_t UART_TimerMode)
{
  assert_param(IS_UART_ALL_PERIPH(UARTx));
  assert_param(IS_UART_TIMER_MODE(UART_TimerMode));

  REGBITS_MODIFY(UARTx->CR2, UARTx_CR2_TIMCR_Msk, (uint32_t)UART_TimerMode);
}

/**
 * @brief 设置定时器重载�?
 * 
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @param Autoreload :定时器重载�?范围0x000000~0xFFFFFF
 */
void UART_SetAutoReload(UART_TypeDef* UARTx, uint32_t AutoReload)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));
    assert_param(IS_UART_AUTORELOAD(AutoReload));

    UARTx->TIMARR = AutoReload;
}

/**
 * @brief 获取定时器当前计数�?
 * 
 * @param UARTx :UARTx外设
 *        参数可以�?
 *           CW_UART1、CW_UART2�?
 * @return uint32_t :定时器当前计数�?
 */
uint32_t UART_GetCounter(UART_TypeDef* UARTx)
{
    assert_param(IS_UART_ALL_PERIPH(UARTx));

    return UARTx->TIMCNT;
}







