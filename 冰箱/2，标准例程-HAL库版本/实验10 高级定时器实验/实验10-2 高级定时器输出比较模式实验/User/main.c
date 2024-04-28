/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-19
 * @brief       高级定时器输出比较模式 实验
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
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/KEY/key.h"
#include "./BSP/TIMER/atim.h"


int main(void)
{
    uint8_t t = 0;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */
    
    atim_timx_comp_pwm_init(1000 - 1, 168 - 1); /* 1Mhz的计数频率, 1Khz的周期. */

    ATIM_TIMX_COMP_CH1_CCRX = 250 - 1;          /* 通道1 相位25% */
    ATIM_TIMX_COMP_CH2_CCRX = 500 - 1;          /* 通道2 相位50% */
    ATIM_TIMX_COMP_CH3_CCRX = 750 - 1;          /* 通道3 相位75% */
    ATIM_TIMX_COMP_CH4_CCRX = 1000 - 1;         /* 通道4 相位100% */
    
    while (1)
    {
        t++;
        delay_ms(10);

        if (t > 50)                             /* 控制LED0闪烁, 提示程序运行状态 */
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}
