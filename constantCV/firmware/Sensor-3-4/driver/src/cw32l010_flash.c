/**
 * @file cw32l010_flash.c
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
#include "cw32l010_flash.h"

/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/

/**
 * @brief 设置FLASH的读等待周期
 *
 * @param FLASH_Latency ：FLASH_Latency_1  FLASH_Latency_2  FLASH_Latency_3 FLASH_Latency_4
 */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
    /* Check the parameters */
    assert_param(IS_FLASH_LATENCY(FLASH_Latency));

    //CW_SYSCTRL->AHBEN_f.FLASH = 1;
    REGBITS_SET(CW_SYSCTRL->AHBEN,0x5A5A0000 | bv1);
    CW_FLASH->CR2 = 0x5A5A0000 | FLASH_Latency;
}



/**
 * @brief 获取当前的读保护等级
 *
 * @return uint8_t  �?/1/2/3
 */
uint8_t FLASH_GetReadOutLevel(void)
{
    return(CW_FLASH->CR1_f.SECURITY);
}

/**
 * @brief 设置MCU的读保护等级,RdLevel的有效取值范围为0,1,2,3
 * //将等级写入MCU后，MCU会自动重启并完成读保护等级修�?
 * //如果从非0等级变为等级0，则MCU会擦除FLASH内的所有数�?
 * //Level0，ISP可读写，SWD可读�?
 * //Level1，ISP可降级，SWD可降级；数据不可读出
 * //Level2，ISP可降级，SWD无功能；数据不可读出
 * //Level3，ISP无功能，SWD无功能；数据不可读出
 * @param RdLevel  : FLASH_RDLEVEL0   FLASH_RDLEVEL1
 *                 : FLASH_RDLEVEL2   FLASH_RDLEVEL3
 */
void FLASH_SetReadOutLevel(uint16_t RdLevel)
{
    assert_param(IS_FLASH_RDLEVEL(RdLevel));

    if (FLASH_GetReadOutLevel() != RdLevel)
    {
        RdLevel = 0x5A50 | (RdLevel & 0x03u);
        *((volatile uint32_t*)(0x4000431C)) = RdLevel;
        *((volatile uint32_t*)(0x40004370)) = 0x5A5AABCD;   //MCU重启以更新读保护等级

        while (1);  //MCU重启以更新保护等�?
    }
}

/**
 * @brief 解锁所有页�?
 *
 */
void FLASH_UnlockAllPages(void)
{
    CW_FLASH->PAGELOCK = 0x5A5Affff;   //解锁所有页�?

}

/**
 * @brief 锁定所有页�?
 *
 */
void FLASH_LockAllPages(void)
{
    CW_FLASH->PAGELOCK = 0x5A5A0000;
}

/**
 * @brief 解锁指定页面
 *
 * @param Page_Number : 0 - 127
 * @return uint8_t    : 正常返回0x00
 */
uint8_t FLASH_UnlockPage(uint16_t Page_Number)
{
    uint16_t Currentlockstatus;

    assert_param(IS_FLASH_PAGE_Number(Page_Number));

    Currentlockstatus = CW_FLASH->PAGELOCK;   //读取当前锁定�?
    CW_FLASH->PAGELOCK = 0x5A5A0000 | Currentlockstatus | (1UL << (Page_Number / 8));   //解锁指定页面
    return(0x00);
}

/**
 * @brief 解锁指定地址范围内所有页�?返回0x00代表成功，非0代表失败
 *
 * @param StartAddr : 0x00000000 - 0x0000FFFF
 * @param EndAddr   : 0x00000000 - 0x0000FFFF
 * @return uint8_t  ：错误返�?x80，正确返�?x00
 */
uint8_t FLASH_UnlockPages(uint32_t StartAddr, uint32_t EndAddr)
{
    uint16_t Currentlockstatus;
    uint16_t UnlockMask, UnlockIdx;

    UnlockMask = 0x00;

    if (StartAddr > EndAddr)
    {
        return (FLASH_ERROR_ADDR);     //待解锁页面地址出错
    }

    if ((StartAddr > 0x0000FFFF) || (EndAddr > 0x0000FFFF))
    {
        return(FLASH_ERROR_ADDR);
    }

    while( StartAddr <= EndAddr )
    {
        UnlockIdx = StartAddr / 512 / 8;
        UnlockMask |= (1UL << UnlockIdx);
        StartAddr  += 512;
    }

    if (UnlockMask == 0x00)
    {
        return(FLASH_ERROR_ADDR);     //待解锁页面地址出错
    }

    Currentlockstatus = CW_FLASH->PAGELOCK;   //读取当前锁定�?
    CW_FLASH->PAGELOCK = 0x5A5A0000 | Currentlockstatus | UnlockMask;   //解锁页面所在的区块
    return(0x00);
}

/**
 * @brief 擦除指定�?
 *
 * @param Page_Number : 0 - 127
 * @return uint8_t    ：错误返�?x80，正确返�?x00
 */
uint8_t FLASH_ErasePage(uint16_t Page_Number)
{
    uint16_t  CR1BAK, CR2BAK;

    if( ( Page_Number > 128 ) )
    {
        return(FLASH_ERROR_ADDR);
    }

    while (CW_FLASH->ISR & bv5);                       //等待�?次操作完成，未作超时处理

    CW_FLASH->ICR = 0x00;                                  //clear all int flag
    CR2BAK = CW_FLASH->CR2;
    CR1BAK = CW_FLASH->CR1;
    CW_FLASH->CR1 = 0x5A5A0000 | (CR1BAK | 0x02u);        // 设置为页擦模�?

    *((volatile uint8_t*)(Page_Number * 512)) = 0x00;       //erase page

    while (CW_FLASH->ISR & bv5);                            //等待操作完成

    CW_FLASH->CR1 = 0x5A5A0000 | CR1BAK;
    CW_FLASH->CR2 = 0x5A5A0000 | CR2BAK;
    return(CW_FLASH->ISR);
}


/**
 * @brief 擦除从STARTADDR到ENDADDR所在的页面,返回0x00代表成功，非0代表失败
 *
 * @param StartAddr   : 0x00000000 - 0x0000FFFF
 * @param EndAddr     : 0x00000000 - 0x0000FFFF
 * @return uint8_t    ：错误返�?x80，正确返�?x00
 */
uint8_t FLASH_ErasePages(uint32_t StartAddr, uint32_t EndAddr)
{
    uint16_t  CR1BAK;

    if ((StartAddr > 0x0000FFFF) || (EndAddr > 0x0000FFFF))
    {
        return(FLASH_ERROR_ADDR);
    }

    while(CW_FLASH->ISR_f.BUSY);                          //等待�?次操作完成，未作超时处理

    CW_FLASH->ICR = 0x00;                                  //clear all int flag
    CR1BAK = CW_FLASH->CR1;

    CW_FLASH->CR1 = 0x5A5A0000 | ( CR1BAK | 0x02u  );        // 设置为页擦模�?

   while( StartAddr <= EndAddr )
    {
        *((volatile uint32_t *)(StartAddr)) = 0x00;             //erase page
        while(CW_FLASH->ISR_f.BUSY);                          //等待操作完成
        StartAddr  += 512;
    }


   CW_FLASH->CR1 = 0x5A5A0000 | CR1BAK;//设置FLASH操作模式为读模式
    return(CW_FLASH->ISR);
}


/**
 * @brief 以WriteAddr作为起始地址，依次写入pWrBuf内的WrByteCnt个字节的数据
 * //待写入起始地址和结束地址需要位于同一个页面以�?
 * //待写入地址的页面已做擦除，并解除保�?
 * //返回0x00代表成功，非0代表失败
 * @param WriteAddr   : 0x00000000 - 0x0000FFFF
 * @param pWrBuf      : 数据地址
 * @param WrByteCnt   �?-65535
 * @return uint8_t    ：错误返�?x80，正确返�?x00
 */
uint8_t FLASH_WriteBytes(uint32_t WriteAddr, uint8_t* pWrBuf, uint16_t WrByteCnt)
{
    uint16_t  CR1BAK;

    if( WriteAddr + WrByteCnt - 1> 0xFFFF )
    {
        return(FLASH_ERROR_ADDR);
    }

    while( CW_FLASH->ISR_f.BUSY);                          //等待�?次操作完成，未作超时处理

    CW_FLASH->ICR = 0x00;                                    //clear all int flag
    CR1BAK = CW_FLASH->CR1;
    CW_FLASH->CR1 = 0x5A5A0000 | ( CR1BAK | 0x01u  );        // 设置为页写模�?

   while( WrByteCnt )
    {
        *((volatile uint8_t *)(WriteAddr)) = *pWrBuf;        //write byte
        while( CW_FLASH->ISR_f.BUSY);                        //等待操作完成
        WriteAddr++;
        WrByteCnt--;
        pWrBuf++;
    }
    CW_FLASH->CR1 = 0x5A5A0000 | CR1BAK;
    return( CW_FLASH->ISR );
}

/**
 * @brief 以WriteAddr作为起始地址，依次写入pWrBuf内的WrByteCnt个半字（16bit）的数据
 *
 * @param WriteAddr
 * @param pWrBuf
 * @param WrByteCnt
 * @return uint8_t
 */
uint8_t FLASH_WriteHalfWords(uint32_t WriteAddr, uint16_t* pWrBuf, uint16_t WrByteCnt)
{
    uint16_t  CR1BAK, CR2BAK;

    if (WriteAddr + WrByteCnt > 0x10000)
    {
        return(FLASH_ERROR_ADDR);
    }

    if ((WriteAddr & 0x00000001) != 0)    // 地址未对�?
    {
        return(FLASH_ERROR_ADDR);
    }

    while (CW_FLASH->ISR & bv5);                          //等待�?次操作完成，未作超时处理

    CW_FLASH->ICR = 0x00;                                    //clear all int flag
    CR2BAK = CW_FLASH->CR2;
    CR1BAK = CW_FLASH->CR1;
    CW_FLASH->CR1 = 0x5A5A0000 | (CR1BAK | 0x01u);        // 设置为页写模�?

    while (WrByteCnt)
    {
        *((volatile uint16_t*)(WriteAddr)) = *pWrBuf;        //write byte

        while (CW_FLASH->ISR & bv5);                        //等待操作完成

        WriteAddr += 2;
        WrByteCnt--;
        pWrBuf++;
    }

    CW_FLASH->CR1 = 0x5A5A0000 | CR1BAK;
    CW_FLASH->CR2 = 0x5A5A0000 | CR2BAK;
    return(CW_FLASH->ISR);

}

/**
 * @brief 以WriteAddr作为起始地址，依次写入pWrBuf内的WrByteCnt个字�?2bit）的数据
 *
 * @param WriteAddr
 * @param pWrBuf
 * @param WrByteCnt
 * @return uint8_t
 */
uint8_t FLASH_WriteWords(uint32_t WriteAddr, uint32_t* pWrBuf, uint16_t WrByteCnt)
{
    uint16_t  CR1BAK, CR2BAK;

    if (WriteAddr + WrByteCnt > 0x10000)
    {
        return(FLASH_ERROR_ADDR);
    }

    if ((WriteAddr & 0x00000003) != 0)    // 地址未对�?
    {
        return(FLASH_ERROR_ADDR);
    }

    while (CW_FLASH->ISR & bv5);                          //等待�?次操作完成，未作超时处理

    CW_FLASH->ICR = 0x00;                                    //clear all int flag
    CR2BAK = CW_FLASH->CR2;
    CR1BAK = CW_FLASH->CR1;
    CW_FLASH->CR1 = 0x5A5A0000 | (CR1BAK | 0x01u);        // 设置为页写模�?

    while (WrByteCnt)
    {
        *((volatile uint32_t*)(WriteAddr)) = *pWrBuf;        //write byte

        while (CW_FLASH->ISR & bv5);                        //等待操作完成

        WriteAddr += 4;
        WrByteCnt--;
        pWrBuf++;
    }

    CW_FLASH->CR1 = 0x5A5A0000 | CR1BAK;
    CW_FLASH->CR2 = 0x5A5A0000 | CR2BAK;
    return(CW_FLASH->ISR);
}
/**
 * @brief 锁定指定页面,返回0x00代表成功，非0代表失败
 *
 * @param Page_Number : 0 - 127
 * @return uint8_t    : 正常返回0x00
 */
uint8_t FLASH_LockPage(uint16_t Page_Number)
{
    uint16_t Currentlockstatus;

    assert_param(IS_FLASH_PAGE_Number(Page_Number));

    Currentlockstatus = CW_FLASH->PAGELOCK;   //读取当前锁定�?
    CW_FLASH->PAGELOCK = (0x5A5A0000 | Currentlockstatus) & (~(1UL << (Page_Number / 8)));   //锁定指定页面
    return(0x00);
}

/**
 * @brief 锁定指定地址范围内所有页�?返回0x00代表成功，非0代表失败
 *
 * @param StartAddr  : 0x00000000 - 0x0000FFFF
 * @param EndAddr    : 0x00000000 - 0x0000FFFF
 * @return uint8_t   : 错误返回0x80，正常返�?x00
 */
uint8_t FLASH_LockPages(uint32_t StartAddr, uint32_t EndAddr)
{
    uint16_t UnlockMask, UnlockIdx;
    uint16_t Currentlockstatus;

    UnlockMask = 0x00;

    if (EndAddr > StartAddr)
    {
        return (FLASH_ERROR_ADDR);     //待锁定页面地址出错
    }

    if ((StartAddr > 0x0000FFFF) || (EndAddr > 0x0000FFFF))
    {
        return(FLASH_ERROR_ADDR);
    }

    while( StartAddr <= EndAddr )
    {
        UnlockIdx = StartAddr / 512 / 8;
        UnlockMask |= (1UL << UnlockIdx);
        StartAddr  += 512;
    }

    if (UnlockMask == 0x00)
    {
        return(FLASH_ERROR_ADDR);     //待锁定页面地址出错
    }

    Currentlockstatus = CW_FLASH->PAGELOCK;   //读取当前锁定�?
    CW_FLASH->PAGELOCK = ( (0x5A5A0000 | Currentlockstatus) & (~UnlockMask) );   //锁定页面所在的区块
    return(0x00);
}

/**
 * @brief FLASH中断配置
 *
 * @param FLASH_IT :  FLASH_IT_PROG
 *                    FLASH_IT_PAGELOCK
 *                    FLASH_IT_PC
 * @param NewState  : ENABLE  DISABLE
 */
void FLASH_ITConfig(uint32_t FLASH_IT, FunctionalState NewState)
{
    assert_param(IS_FLASH_IT(FLASH_IT));
    assert_param(IS_FUNCTIONAL_STATE(NewState));

    if (NewState != DISABLE)
    {
        CW_FLASH->IER |= FLASH_IT;
    }
    else
    {
        CW_FLASH->IER &= (~FLASH_IT);
    }
}

/**
 * @brief 获取FLASH指定中断状�?
 *
 * @param FLASH_IT :  FLASH_IT_PROG
 *                    FLASH_IT_PAGELOCK
 *                    FLASH_IT_PC
 * @return ITStatus : SET  RESET
 */
ITStatus FLASH_GetITStatus(uint32_t FLASH_IT)
{
    assert_param(IS_FLASH_GET_IT(FLASH_IT));

    return (ITStatus)(!!(CW_FLASH->ISR & FLASH_IT));
}


/**
 * @brief 清除FLASH指定中断标志
 *
 * @param FLASH_IT  : FLASH_IT_PROG
 *                    FLASH_IT_PAGELOCK
 *                    FLASH_IT_PC
 */
void FLASH_ClearITPendingBit(uint32_t FLASH_IT)
{
    assert_param(IS_FLASH_IT(FLASH_IT));

    CW_FLASH->ICR &= (~FLASH_IT);
}

/**
 * @brief 获取FLASH模块的状�?
 *
 * @return uint8_t 00:正常
 *         FLASH_FLAG_BSY �?
 *         FLASH_FLAG_PCERR  擦写PC指针所在页错误
 *         FLASH_FLAG_WRPRTERR 擦写未解锁保护页错误
 *         FLASH_FLAG_PGERR   编程目标地址数据不全�?XFF错误
 */
uint32_t FLASH_GetStatus(void)
{
    uint32_t flashstatus;

    flashstatus = CW_FLASH->ISR;

    if (flashstatus & FLASH_FLAG_BSY)
    {
        return FLASH_FLAG_BSY;
    }

    if (flashstatus & FLASH_FLAG_PCERR)
    {
        return FLASH_FLAG_PCERR;
    }

    if (flashstatus & FLASH_FLAG_WRPRTERR)
    {
        return FLASH_FLAG_WRPRTERR;
    }

    if (flashstatus & FLASH_FLAG_PGERR)
    {
        return FLASH_FLAG_PGERR;
    }

    return FLASH_FLAG_OK;
}

