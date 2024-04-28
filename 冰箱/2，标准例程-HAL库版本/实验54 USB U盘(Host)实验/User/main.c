/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-20
 * @brief       U��(Host) ʵ��
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
#include "./USMART/usmart.h"
#include "./BSP/KEY/key.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./TEXT/text.h"
#include "./PICTURE/piclib.h"
#include "usbh_core.h"
#include "usbh_msc.h"


USBH_HandleTypeDef hUSBHost;    /* USB Host����ṹ�� */


static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
    uint32_t total, free;
    uint8_t res = 0;
    printf("id:%d\r\n", id);

    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_DISCONNECTION:
            f_mount(0, "2:", 1);        /* ж��U�� */
            text_show_string(30, 120, 200, 16, "�豸������...", 16, 0, RED);
            lcd_fill(30, 160, 239, 220, WHITE);
            break;

        case HOST_USER_CLASS_ACTIVE:
            text_show_string(30, 120, 200, 16, "�豸���ӳɹ�!", 16, 0, RED);
            f_mount(fs[2], "2:", 1);    /* ���¹���U�� */
            res = exfuns_get_free("2:", &total, &free);

            if (res == 0)
            {
                lcd_show_string(30, 160, 200, 16, 16, "FATFS OK!", BLUE);
                lcd_show_string(30, 180, 200, 16, 16, "U Disk Total Size:     MB", BLUE);
                lcd_show_string(30, 200, 200, 16, 16, "U Disk  Free Size:     MB", BLUE);
                lcd_show_num(174, 180, total >> 10, 5, 16, BLUE);   /* ��ʾU�������� MB */
                lcd_show_num(174, 200, free >> 10, 5, 16, BLUE);
            }
            else
            {
                printf("U�̴洢�ռ��ȡʧ��\r\n");
            }

            break;

        case HOST_USER_CONNECTION:
            break;

        default:
            break;
    }
}

int main(void)
{
    uint8_t t = 0;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* USMART��ʼ�� */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    sram_init();                            /* ��ʼ���ⲿSRAM */
    norflash_init();                        /* �ⲿFlash��ʼ�� */

    my_mem_init(SRAMIN);                    /* ��ʼ���ڲ�SRAM�ڴ�� */
    my_mem_init(SRAMEX);                    /* ��ʼ���ⲿSRAM�ڴ�� */
    my_mem_init(SRAMCCM);                   /* ��ʼ��CCM�ڴ�� */

    exfuns_init();                          /* Ϊfatfs��ر��������ڴ� */
    f_mount(fs[0], "0:", 1);                /* ����SD�� */
    f_mount(fs[1], "1:", 1);                /* ����FLASH */


    piclib_init();                          /* ��ʼ����ͼ */
    
    while (fonts_init())                    /* ����ֿ� */
    {
        lcd_show_string(30, 50, 200, 16, 16, "Font Error!", RED);
        delay_ms(200);
        lcd_fill(30, 50, 240, 66, WHITE);   /* �����ʾ */
        delay_ms(200);
    }

    text_show_string(30, 50, 200, 16, "STM32", 16, 0, RED);
    text_show_string(30, 70, 200, 16, "USB U�� ʵ��", 16, 0, RED);
    text_show_string(30, 90, 200, 16, "ATOM@ALIENTEK", 16, 0, RED);
    text_show_string(30, 120, 200, 16, "�豸������...", 16, 0, RED);

    USBH_Init(&hUSBHost, USBH_UserProcess, 0);
    USBH_RegisterClass(&hUSBHost, USBH_MSC_CLASS);
    USBH_Start(&hUSBHost);

    while (1)
    {
        USBH_Process(&hUSBHost);
        delay_ms(10);
        t++;

        if (t == 50)
        {
            t = 0;
            LED0_TOGGLE();
        }
    }
}




