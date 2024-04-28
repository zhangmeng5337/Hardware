/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-20
 * @brief       USB声卡(Slave) 实验
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
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/ES8388/es8388.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_audio.h"
#include "usbd_audio_if.h"


USBD_HandleTypeDef USBD_Device;                 /* USB Device处理结构体 */

extern volatile uint8_t g_device_state;         /* USB连接 情况 */
extern uint8_t g_volume;                        /* 音量(可通过按键设置) */


int main(void)
{
    uint8_t key;
    uint8_t t = 0;
    uint8_t device_sta = 0XFF;
    
    HAL_Init();                         /* 初始化HAL库 */
    sys_stm32_clock_init(336, 8, 2, 7); /* 设置时钟,168Mhz */
    delay_init(168);                    /* 延时初始化 */
    usart_init(115200);                 /* 串口初始化为115200 */
    led_init();                         /* 初始化LED */
    lcd_init();                         /* 初始化LCD */
    key_init();                         /* 初始化按键 */
    sram_init();                        /* SRAM初始化 */

    my_mem_init(SRAMIN);                /* 初始化内部SRAM内存池 */
    my_mem_init(SRAMEX);                /* 初始化外部SRAM内存池 */
    my_mem_init(SRAMCCM);               /* 初始化CCM内存池 */

    es8388_init();                      /* ES8388初始化 */
    es8388_adda_cfg(1, 0);              /* 开启DAC关闭ADC */
    es8388_output_cfg(1, 1);            /* DAC选择通道输出 */
    es8388_hpvol_set(25);               /* 设置耳机音量 */
    es8388_spkvol_set(30);              /* 设置喇叭音量 */
    
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "USB Sound Card TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY2:Vol-  KEY0:vol+", RED);
    
    lcd_show_string(30, 140, 200, 16, 16, "VOLUME:", BLUE);                 /* 提示音量 */
    lcd_show_xnum(30 + 56, 140, AUDIO_DEFAULT_VOLUME, 3, 16, 0X80, BLUE);   /* 显示音量值 */
    lcd_show_string(30, 180, 200, 16, 16, "USB Connecting...", RED);        /* 提示正在建立连接 */

    USBD_Init(&USBD_Device, &AUDIO_Desc, 0);
    USBD_RegisterClass(&USBD_Device, USBD_AUDIO_CLASS);
    USBD_AUDIO_RegisterInterface(&USBD_Device, &USBD_AUDIO_fops);
    USBD_Start(&USBD_Device);

    while (1)
    {
        key = key_scan(1);  /* 支持连按 */

        if (key)
        {
            if (key == KEY0_PRES)       /* KEY0按下,音量增加 */
            {
                g_volume++;

                if (g_volume > 100)     /* 范围限定 */
                {
                    g_volume = 100;
                }
            }
            else if (key == KEY2_PRES)  /* KEY2按下,音量减少 */
            {
                if (g_volume)           /* 范围限定 */
                {
                    g_volume--;
                }
            }

            es8388_hpvol_set(g_volume * 0.3);
            es8388_spkvol_set(g_volume * 0.3);
            lcd_show_xnum(30 + 56, 140, g_volume, 3, 16, 0X80, BLUE);   /* 显示音量值 */
            delay_ms(20);
        }

        if (device_sta != g_device_state)   /* 状态改变了 */
        {
            if (g_device_state == 1)
            {
                lcd_show_string(30, 180, 200, 16, 16, "USB Connected    ", RED);    /* 提示USB连接已经建立 */
            }
            else
            {
                lcd_show_string(30, 180, 200, 16, 16, "USB DisConnected ", RED);    /* 提示USB连接失败 */
            }

            device_sta = g_device_state;
        }

        delay_ms(20);
        t++;

        if (t > 10)
        {
            t = 0;
            LED0_TOGGLE();  /* 提示系统在运行 */
        }
    }
}






















