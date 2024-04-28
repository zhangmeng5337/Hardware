/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-19
 * @brief       高级定时器输出指定个数PWM 实验
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
    uint8_t key = 0;
    uint8_t t = 0;
    GPIO_InitTypeDef gpio_init_struct;
    
    HAL_Init();                                 /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7);         /* 设置时钟,168Mhz */
    delay_init(168);                            /* 延时初始化 */
    usart_init(115200);                         /* 串口初始化为115200 */
    led_init();                                 /* 初始化LED */
    key_init();                                 /* 初始化按键 */
    
    /* 将 LED1 引脚设置为输入模式, 避免和PC6冲突 */
    gpio_init_struct.Pin = LED1_GPIO_PIN;                   /* LED1引脚 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;                /* 设置为输入 */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* 高速模式 */
    HAL_GPIO_Init(LED1_GPIO_PORT, &gpio_init_struct);       /* 初始化LED1引脚 */
    
    atim_timx_npwm_chy_init(10000 - 1, 8400 - 1);           /* 20Khz的计数频率,2Hz的PWM频率. */

    ATIM_TIMX_NPWM_CHY_CCRX = 5000; /* 设置PWM占空比,50%,这样可以控制每一个PWM周期,LED1(GREEN)
                                     * 有一半时间是亮的,一半时间是灭的,LED1亮灭一次,表示一个PWM波
                                     */
    atim_timx_npwm_chy_set(5);      /* 输出5个PWM波(控制LED1(GREEN)闪烁5次) */
    
    while (1)
    {
       key = key_scan(0);

        if (key == KEY0_PRES)           /* KEY0按下 */
        {
            atim_timx_npwm_chy_set(5);  /* 输出5个PWM波(控制TIM8_CH1, 即PC6输出5个脉冲) */
        }

        t++;
        delay_ms(10);

        if (t > 50)                     /* 控制LED0闪烁, 提示程序运行状态 */
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}
