/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-22
 * @brief       ��дʶ�� ʵ��
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
#include "./BSP/TOUCH/touch.h"
#include "./FATFS/exfuns/exfuns.h"
#include "./BSP/SRAM/sram.h"
#include "./BSP/SDIO/sdio_sdcard.h"
#include "./BSP/NORFLASH/norflash.h"
#include "./MALLOC/malloc.h"
#include "./TEXT/text.h"
#include "./ATKNCR/atk_ncr.h"


/* ����¼�Ĺ켣����(��������) */
atk_ncr_point ncr_input_buf[200];

/**
 * @brief       ������
 * @param       x1,y1: �������
 * @param       x2,y2: �յ�����
 * @param       size : ������ϸ�̶�
 * @param       color: �ߵ���ɫ
 * @retval      ��
 */
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;

    if (x1 < size || x2 < size || y1 < size || y2 < size)
        return;

    delta_x = x2 - x1; /* ������������ */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)
    {
        incx = 1; /* ���õ������� */
    }
    else if (delta_x == 0)
    {
        incx = 0; /* ��ֱ�� */
    }
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)
    {
        incy = 1;
    }
    else if (delta_y == 0)
    {
        incy = 0; /* ˮƽ�� */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if (delta_x > delta_y)
        distance = delta_x; /* ѡȡ�������������� */
    else
        distance = delta_y;

    for (t = 0; t <= distance + 1; t++) /* ������� */
    {
        lcd_fill_circle(row, col, size, color); /* ���� */
        xerr += delta_x;
        yerr += delta_y;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

int main(void)
{
    uint8_t i = 0;
    uint8_t tcnt = 0;
    char sbuf[10];
    uint8_t key;
    uint16_t pcnt = 0;
    uint8_t mode = 4;                   /* Ĭ���ǻ��ģʽ */
    uint16_t lastpos[2];                /* ���һ�ε����� */

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��, 168Mhz */
    delay_init(168);                    /* ��ʱ��ʼ�� */
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    tp_dev.init();                      /* ��������ʼ�� */
    sram_init();                        /* ��ʼ��SRAM */
    norflash_init();                    /* ��ʼ��nor flash */
    
    my_mem_init(SRAMIN);                /* ��ʼ���ڲ��ڴ�� */
    my_mem_init(SRAMEX);                /* ��ʼ���ⲿ�ڴ�� */
    my_mem_init(SRAMCCM);               /* ��ʼ���ڲ�CCM�ڴ�� */

    exfuns_init();                      /* Ϊfatfs��ر��������ڴ� */
    f_mount(fs[0], "0:", 1);            /* ����SD�� */
    f_mount(fs[1], "1:", 1);            /* ����FLASH */
    
    alientek_ncr_init();                /* ��ʼ����дʶ�� */

    while (fonts_init())                /* ����ֿ� */
    {
        lcd_show_string(60, 50, 200, 16, 16, "Font Error!", RED);
        delay_ms(200);
        lcd_fill(60, 50, 240, 66, WHITE);   /* �����ʾ */
        delay_ms(200);
    }

RESTART:
    text_show_string(60, 10, 200, 16, "����ԭ��STM32������", 16, 0, RED);
    text_show_string(60, 30, 200, 16, "��дʶ��ʵ��", 16, 0, RED);
    text_show_string(60, 50, 200, 16, "ATOM@ALIENTEK", 16, 0, RED);
    text_show_string(60, 70, 200, 16, "KEY0:MODE KEY1:Adjust", 16, 0, RED);
    text_show_string(60, 90, 200, 16, "ʶ����:", 16, 0, RED);
    lcd_draw_rectangle(19, 114, lcddev.width - 20, lcddev.height - 5, RED);

    text_show_string(96, 207, 200, 16, "��д��", 16, 0, BLUE);
    tcnt = 100;

    while (1)
    {
        key = key_scan(0);

        if (key == KEY1_PRES && (tp_dev.touchtype & 0X80) == 0)
        {
            tp_adjust();        /* ��ĻУ׼ */
            lcd_clear(WHITE);
            goto RESTART;       /* ���¼��ؽ��� */
        }

        if (key == KEY0_PRES)
        {
            lcd_fill(20, 115, 219, 314, WHITE); /* �����ǰ��ʾ */
            mode++;

            if (mode > 4)mode = 1;

            switch (mode)
            {
                case 1:
                    text_show_string(80, 207, 200, 16, "��ʶ������", 16, 0, BLUE);
                    break;

                case 2:
                    text_show_string(64, 207, 200, 16, "��ʶ���д��ĸ", 16, 0, BLUE);
                    break;

                case 3:
                    text_show_string(64, 207, 200, 16, "��ʶ��Сд��ĸ", 16, 0, BLUE);
                    break;

                case 4:
                    text_show_string(88, 207, 200, 16, "ȫ��ʶ��", 16, 0, BLUE);
                    break;
            }

            tcnt = 100;
        }

        tp_dev.scan(0); /* ɨ�� */

        if (tp_dev.sta & TP_PRES_DOWN)  /* �а��������� */
        {
            delay_ms(1);    /* ��Ҫ����ʱ, ��������Ϊ�а������� */
            tcnt = 0;       /* �ɿ�ʱ�ļ�������� */

            if ((tp_dev.x[0] < (lcddev.width - 20 - 2) && tp_dev.x[0] >= (20 + 2)) && (tp_dev.y[0] < (lcddev.height - 5 - 2) && tp_dev.y[0] >= (115 + 2)))
            {
                if (lastpos[0] == 0XFFFF)
                {
                    lastpos[0] = tp_dev.x[0];
                    lastpos[1] = tp_dev.y[0];
                }

                lcd_draw_bline(lastpos[0], lastpos[1], tp_dev.x[0], tp_dev.y[0], 2, BLUE);  /* ���� */
                lastpos[0] = tp_dev.x[0];
                lastpos[1] = tp_dev.y[0];

                if (pcnt < 200) /* �ܵ�������200 */
                {
                    if (pcnt)
                    {
                        if ((ncr_input_buf[pcnt - 1].y != tp_dev.y[0]) && (ncr_input_buf[pcnt - 1].x != tp_dev.x[0])) /* x,y����� */
                        {
                            ncr_input_buf[pcnt].x = tp_dev.x[0];
                            ncr_input_buf[pcnt].y = tp_dev.y[0];
                            pcnt++;
                        }
                    }
                    else
                    {
                        ncr_input_buf[pcnt].x = tp_dev.x[0];
                        ncr_input_buf[pcnt].y = tp_dev.y[0];
                        pcnt++;
                    }
                }
            }
        }
        else    /* �����ɿ��� */
        {
            lastpos[0] = 0XFFFF;
            tcnt++;
            delay_ms(10);
            /* ��ʱʶ�� */
            i++;

            if (tcnt == 40)
            {
                if (pcnt)   /* ����Ч������ */
                {
                    printf("�ܵ���:%d\r\n", pcnt);
                    alientek_ncr(ncr_input_buf, pcnt, 6, mode, sbuf);
                    printf("ʶ����:%s\r\n", sbuf);
                    pcnt = 0; 
                    lcd_show_string(60 + 72, 90, 200, 16, 16, sbuf, BLUE);
                }

                lcd_fill(20, 115, lcddev.width - 20 - 1, lcddev.height - 5 - 1, WHITE);
            }
        }

        if (i == 30)
        {
            i = 0;
            LED0_TOGGLE();
        }
    }
}


