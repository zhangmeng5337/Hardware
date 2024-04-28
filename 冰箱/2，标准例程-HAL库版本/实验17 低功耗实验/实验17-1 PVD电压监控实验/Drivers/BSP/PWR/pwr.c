/**
 ****************************************************************************************************
 * @file        pwr.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-18
 * @brief       低功耗模式 驱动代码
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
 * V1.0 20211018
 * 第一次发布
 *
 ****************************************************************************************************
 */

#include "./BSP/PWR/pwr.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/RTC/rtc.h"


/**
 * @brief       初始化PVD电压监视器
 * @param       pls: 电压等级(PWR_PVD_detection_level)
 *   @arg       PWR_PVDLEVEL_0,2.2V;
 *   @arg       PWR_PVDLEVEL_1,2.3V;
 *   @arg       PWR_PVDLEVEL_2,2.4V;
 *   @arg       PWR_PVDLEVEL_3,2.5V;
 *   @arg       PWR_PVDLEVEL_4,2.6V;
 *   @arg       PWR_PVDLEVEL_5,2.7V;
 *   @arg       PWR_PVDLEVEL_6,2.8V;
 *   @arg       PWR_PVDLEVEL_7,2.9V;
 * @retval      无
 */
void pwr_pvd_init(uint32_t pls)
{
    PWR_PVDTypeDef pvd_handle = {0};

    __HAL_RCC_PWR_CLK_ENABLE();

    pvd_handle.PVDLevel = pls;
    pvd_handle.Mode = PWR_PVD_MODE_IT_RISING_FALLING;
    HAL_PWR_ConfigPVD(&pvd_handle);

    HAL_NVIC_SetPriority(PVD_IRQn, 3, 3);       /* 抢占优先级3，子优先级3 */
    HAL_NVIC_EnableIRQ(PVD_IRQn);
    HAL_PWR_EnablePVD();
}

/**
 * @brief       PVD中断服务函数
 * @param       无
 * @retval      无
 */
void PVD_IRQHandler(void)
{
    HAL_PWR_PVD_IRQHandler();
}

/**
 * @brief       PVD/AVD中断服务回调函数
 * @param       无
 * @retval      无
 */
void HAL_PWR_PVDCallback(void)
{
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO))   /* 电压比PLS所选电压还低 */
    {
        lcd_show_string(30, 130, 200, 16, 16, "PVD Low Voltage!", RED); /* LCD显示电压低 */
        LED1(0);    /* 点亮绿灯, 表明电压低了 */
    }
    else
    {
        lcd_show_string(30, 130, 200, 16, 16, "PVD Voltage OK! ", BLUE);/* LCD显示电压正常 */
        LED1(1);    /* 灭掉绿灯 */
    }
}


