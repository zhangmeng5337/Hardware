/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       ���ֲ����� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
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
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./MALLOC/malloc.h"
#include "./USMART/usmart.h"
#include "./TEXT/text.h"
#include "./BSP/ES8388/es8388.h"
#include "./APP/audioplay.h"


int main(void)
{
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ����1��ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    sram_init();                            /* SRAM��ʼ�� */
    norflash_init();                        /* ��ʼ��NORFLASH */

    my_mem_init(SRAMIN);                    /* ��ʼ���ڲ�SRAM�ڴ�� */
    my_mem_init(SRAMEX);                    /* ��ʼ���ⲿSRAM�ڴ�� */
    
    while (sd_init())                       /* ���SD�� */
    {
        lcd_show_string(30, 50, 200, 16, 16, "SD Card Failed!", RED);
        delay_ms(200);
        lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
        delay_ms(200);
    }
    exfuns_init();                          /* Ϊfatfs��ر��������ڴ� */
    f_mount(fs[0], "0:", 1);                /* ����SD�� */
    f_mount(fs[1], "1:", 1);                /* ����FLASH */

    while (fonts_init())                    /* ����ֿ� */
    {
        lcd_show_string(30, 50, 200, 16, 16, "Font Error!", RED);
        delay_ms(200);
        lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
        delay_ms(200);
    }
    
    es8388_init();              /* ES8388��ʼ�� */
    es8388_adda_cfg(1, 0);      /* ����DAC�ر�ADC */
    es8388_output_cfg(1, 1);    /* DACѡ��ͨ����� */
    es8388_hpvol_set(25);       /* ���ö������� */
    es8388_spkvol_set(30);      /* ������������ */
    
    text_show_string(30, 30, 200, 16, "����ԭ��STM32������", 16, 0, RED);
    text_show_string(30, 50, 200, 16, "���ֲ�����ʵ��", 16, 0, RED);
    text_show_string(30, 70, 200, 16, "����ԭ��@ALIENTEK", 16, 0, RED);
    text_show_string(30, 90, 200, 16, "2021��11��16��", 16, 0, RED);
    text_show_string(30, 110, 200, 16, "KEY0:NEXT   KEY2:PREV", 16, 0, RED);
    text_show_string(30, 130, 200, 16, "KEY_UP:PAUSE/PLAY", 16, 0, RED);
    
    while (1)
    {
        audio_play();           /* �������� */
    }
}

