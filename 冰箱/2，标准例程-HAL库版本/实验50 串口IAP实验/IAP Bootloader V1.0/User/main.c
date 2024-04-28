/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-22
 * @brief       ����IAP ʵ��-IAP Bootloader V1.0
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
#include "./BSP/STMFLASH/stmflash.h"
#include "./IAP/iap.h"


int main(void)
{
    uint8_t t;
    uint8_t key;
    uint32_t lastcount = 0;     /* ��һ�δ��ڽ�������ֵ */
    uint32_t applenth = 0;      /* ���յ���app���볤�� */
    uint8_t clearflag = 0;

    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    
    lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30,  70, 200, 16, 16, "IAP TEST", RED);
    lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "KEY_UP: Copy APP2FLASH!", RED);
    lcd_show_string(30, 130, 200, 16, 16, "KEY1: Run FLASH APP", RED);
    lcd_show_string(30, 150, 200, 16, 16, "KEY0: Run SRAM APP", RED);

    while (1)
    {
        if (g_usart_rx_cnt)
        {
            if (lastcount == g_usart_rx_cnt)   /* ��������,û���յ��κ�����,��Ϊ�������ݽ������ */
            {
                applenth = g_usart_rx_cnt;
                lastcount = 0;
                g_usart_rx_cnt = 0;
                printf("�û�����������!\r\n");
                printf("���볤��:%dBytes\r\n", applenth);
            }
            else lastcount = g_usart_rx_cnt;
        }

        t++;
        delay_ms(100);

        if (t == 3)
        {
            LED0_TOGGLE();
            t = 0;

            if (clearflag)
            {
                clearflag--;

                if (clearflag == 0)
                {
                    lcd_fill(30, 190, 240, 210 + 16, WHITE);    /* �����ʾ */
                }
            }
        }

        key = key_scan(0);

        if (key == WKUP_PRES)   /* WKUP����,���¹̼���FLASH */
        {
            if (applenth)
            {
                printf("��ʼ���¹̼�...\r\n");
                lcd_show_string(30, 190, 200, 16, 16, "Copying APP2FLASH...", BLUE);

                if (((*(volatile uint32_t *)(0X20001000 + 4)) & 0xFF000000) == 0x08000000)  /* �ж��Ƿ�Ϊ0X08XXXXXX */
                {
                    iap_write_appbin(FLASH_APP1_ADDR, g_usart_rx_buf, applenth);            /* ����FLASH���� */
                    lcd_show_string(30, 190, 200, 16, 16, "Copy APP Successed!!", BLUE);
                    printf("�̼��������!\r\n");
                }
                else
                {
                    lcd_show_string(30, 190, 200, 16, 16, "Illegal FLASH APP!  ", BLUE);
                    printf("��FLASHӦ�ó���!\r\n");
                }
            }
            else
            {
                printf("û�п��Ը��µĹ̼�!\r\n");
                lcd_show_string(30, 190, 200, 16, 16, "No APP!", BLUE);
            }

            clearflag = 7; /* ��־��������ʾ,��������7*300ms�������ʾ */
        }

        if (key == KEY1_PRES)   /* KEY1��������, ����FLASH APP���� */
        {
            printf("flash addr :%x \r\n",(*(volatile uint32_t *)(FLASH_APP1_ADDR + 4)) & 0xFF000000);
            if (((*(volatile uint32_t *)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000) /* �ж�FLASH�����Ƿ���APP,�еĻ�ִ�� */
            {
                printf("��ʼִ��FLASH�û�����!!\r\n\r\n");
                delay_ms(10);
                iap_load_app(FLASH_APP1_ADDR);/* ִ��FLASH APP���� */
            }
            else
            {
                printf("û�п������еĹ̼�!\r\n");
                lcd_show_string(30, 190, 200, 16, 16, "No APP!", BLUE);
            }

            clearflag = 7; /* ��־��������ʾ,��������7*300ms�������ʾ */
        }

        if (key == KEY0_PRES)   /* KEY0���� */
        {
            printf("��ʼִ��SRAM�û�����!!\r\n\r\n");
            delay_ms(10);

            if (((*(volatile uint32_t *)(0x20001000 + 4)) & 0xFF000000) == 0x20000000)   /* �ж��Ƿ�Ϊ0X20XXXXXX */
            {
                iap_load_app(0x20001000);   /* SRAM��ַ */
            }
            else
            {
                printf("��SRAMӦ�ó���,�޷�ִ��!\r\n");
                lcd_show_string(30, 190, 200, 16, 16, "Illegal SRAM APP!", BLUE);
            }

            clearflag = 7; /* ��־��������ʾ,��������7*300ms�������ʾ */
        }
    }
}
