/**
 ****************************************************************************************************
 * @file        pwmdac.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-03
 * @brief       PWM DAC输出 驱动代码
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
 * V1.0 20211103
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/PWMDAC/pwmdac.h"


TIM_HandleTypeDef g_tim9_handler;     /* 定时器句柄 */
TIM_OC_InitTypeDef g_tim9_ch2handler; /* 定时器2通道4句柄 */

/**
 * @brief       PWM DAC初始化, 实际上就是初始化定时器
 * @note
 *              定时器的时钟来自APB1 / APB2, 当APB1 / APB2 分频时, 定时器频率自动翻倍
 *              所以, 一般情况下, 我们所有定时器的频率, 都是84Mhz 等于系统时钟频率
 *              定时器溢出时间计算方法: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft = 定时器工作频率, 单位: Mhz
 *
 * @param       arr: 自动重装值。
 * @param       psc: 时钟预分频数
 * @retval      无
 */
void pwmdac_init(uint16_t arr, uint16_t psc)
{
    g_tim9_handler.Instance = PWMDAC_TIMX;                  /* 定时器9 */
    g_tim9_handler.Init.Prescaler = psc;                    /* 定时器分频 */
    g_tim9_handler.Init.CounterMode = TIM_COUNTERMODE_UP;   /* 向上计数模式 */
    g_tim9_handler.Init.Period = arr;                       /* 自动重装载值 */
    HAL_TIM_PWM_Init(&g_tim9_handler);                      /* 初始化PWM */

    g_tim9_ch2handler.OCMode = TIM_OCMODE_PWM1;                                         /* CH1/2 PWM模式1 */
    g_tim9_ch2handler.Pulse = arr / 2;                                                  /* 设置比较值,此值用来确定占空比，默认比较值为自动重装载值的一半，即占空比为50% */
    g_tim9_ch2handler.OCPolarity = TIM_OCPOLARITY_HIGH;                                 /* 输出比较极性为高 */
    HAL_TIM_PWM_ConfigChannel(&g_tim9_handler, &g_tim9_ch2handler, PWMDAC_TIMX_CHY);    /* 配置TIM2通道4 */

    HAL_TIM_PWM_Start(&g_tim9_handler, PWMDAC_TIMX_CHY);    /* 开启PWM通道4 */
}

/**
 * @brief       定时器底层驱动，时钟使能，引脚配置
 * @note
 *              此函数会被HAL_TIM_PWM_Init()调用
 * @param       htim:定时器句柄
 * @retval      无
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef gpio_init_struct;

    if (htim->Instance == PWMDAC_TIMX)
    {
        PWMDAC_TIMX_CLK_ENABLE();           /* 使能定时器 */
        PWMDAC_GPIO_CLK_ENABLE();           /* PWM DAC GPIO 时钟使能 */

        gpio_init_struct.Pin = PWMDAC_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
        gpio_init_struct.Alternate = PWMDAC_GPIO_AFTIMX;
        HAL_GPIO_Init(PWMDAC_GPIO_PORT, &gpio_init_struct);         /* TIMX PWM CHY 引脚模式设置 */
    }
}

/**
 * @brief       设置PWM DAC输出电压
 * @param       vol : 0~3300,代表0~3.3V
 * @retval      无
 */
void pwmdac_set_voltage(uint16_t vol)
{
    float temp = vol;
    temp /= 100;                /* 缩小100倍, 得到实际电压值 */
    temp = temp * 256 / 3.3f;   /* 将电压转换成PWM占空比 */
    __HAL_TIM_SET_COMPARE(&g_tim9_handler, PWMDAC_TIMX_CHY, temp);  /* 设置新的占空比 */
}



