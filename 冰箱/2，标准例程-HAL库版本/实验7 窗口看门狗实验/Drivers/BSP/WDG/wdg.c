/**
 ****************************************************************************************************
 * @file        wdg.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2021-10-14
 * @brief       看门狗 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
 * V1.1 20211014
 * 新增wwdg_init函数
 *
 ****************************************************************************************************
 */

#include "./BSP/WDG/wdg.h"
#include "./BSP/LED/led.h"


IWDG_HandleTypeDef iwdg_handler; /*独立看门狗句柄 */

/**
 * @brief       初始化独立看门狗 
 * @param       prer: IWDG_PRESCALER_4~IWDG_PRESCALER_256,对应4~256分频
 *   @arg       分频因子 = 4 * 2^prer. 但最大值只能是256!
 * @param       rlr: 自动重装载值,0~0XFFF. 
 * @note        时间计算(大概):Tout=((4 * 2^prer) * rlr) / 32 (ms). 
 * @retval      无
 */
void iwdg_init(uint32_t prer, uint16_t rlr)
{
    iwdg_handler.Instance = IWDG;
    iwdg_handler.Init.Prescaler = prer; /* 设置IWDG分频系数 */
    iwdg_handler.Init.Reload = rlr;     /* 从加载寄存器 IWDG->RLR 重装载值 */
    HAL_IWDG_Init(&iwdg_handler);       /* 初始化IWDG并使能 */
}

/**
 * @brief       喂独立看门狗
 * @param       无
 * @retval      无
 */
void iwdg_feed(void)
{
    HAL_IWDG_Refresh(&iwdg_handler);    /* 喂狗 */
}


/************************以下是窗口看门狗代码********************************/

WWDG_HandleTypeDef wwdg_handler;        /* 窗口看门狗句柄 */

/**
 * @brief       初始化窗口看门狗
 * @param       tr: T[6:0],计数器值
 * @param       tw: W[6:0],窗口值
 * @note        fprer:分频系数（WDGTB）,范围:WWDG_PRESCALER_1~WWDG_PRESCALER_8,表示2^WDGTB分频
 *              Fwwdg=PCLK1/(4096*2^fprer). 一般PCLK1=42Mhz
 * @retval      无
 */
void wwdg_init(uint8_t tr, uint8_t wr, uint32_t fprer)
{
    wwdg_handler.Instance = WWDG;
    wwdg_handler.Init.Prescaler = fprer;         /* 设置分频系数 */
    wwdg_handler.Init.Window = wr;               /* 设置窗口值 */
    wwdg_handler.Init.Counter = tr;              /* 设置计数器值 */
    wwdg_handler.Init.EWIMode = WWDG_EWI_ENABLE; /* 使能窗口看门狗提前唤醒中断 */
    HAL_WWDG_Init(&wwdg_handler);                /* 初始化WWDG */
}

/**
 * @brief       WWDG底层驱动，时钟配置，中断配置
                此函数会被HAL_WWDG_Init()调用
 * @param       hwwdg:窗口看门狗句柄
 * @retval      无
 */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
    __HAL_RCC_WWDG_CLK_ENABLE();            /* 使能窗口看门狗时钟 */

    HAL_NVIC_SetPriority(WWDG_IRQn, 2, 3);  /* 抢占优先级2，子优先级为3 */
    HAL_NVIC_EnableIRQ(WWDG_IRQn);          /* 使能窗口看门狗中断 */
}

/**
 * @brief       窗口看门狗中断服务程序
 * @param       无
 * @retval      无
 */
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&wwdg_handler);     /* 调用WWDG共用中断处理函数 */
}

/**
 * @brief       中断服务函数处理过程
                此函数会被HAL_WWDG_IRQHandler()调用
 * @param       无
 * @retval      无
 */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    HAL_WWDG_Refresh(&wwdg_handler);        /* 更新窗口看门狗值 */
    LED1_TOGGLE();                          /*  绿灯闪烁 */
}

