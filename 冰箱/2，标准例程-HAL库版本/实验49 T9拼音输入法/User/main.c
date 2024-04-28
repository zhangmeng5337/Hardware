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
#include "./USMART/usmart.h"
#include "./TEXT/text.h"
#include "./T9INPUT/pyinput.h"


/* ���ֱ� */
const char *kbd_tbl[9] = {"��", "2", "3", "4", "5", "6", "7", "8", "9",};

/* �ַ��� */
const char *kbs_tbl[9] = {"DEL", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz",};

uint16_t kbdxsize;  /* ������̰������ */
uint16_t kbdysize;  /* ������̰����߶� */

/**
 * @brief       ���ؼ��̽���
 * @param       x, y : ������ʼ����
 * @retval      ��
 */
void py_load_ui(uint16_t x, uint16_t y)
{
    uint16_t i;
    lcd_draw_rectangle(x, y, x + kbdxsize * 3, y + kbdysize * 3, RED);
    lcd_draw_rectangle(x + kbdxsize, y, x + kbdxsize * 2, y + kbdysize * 3, RED);
    lcd_draw_rectangle(x, y + kbdysize, x + kbdxsize * 3, y + kbdysize * 2, RED);

    for (i = 0; i < 9; i++)
    {
        text_show_string_middle(x + (i % 3)*kbdxsize, y + 4 + kbdysize * (i / 3), (char *)kbd_tbl[i], 16, kbdxsize, BLUE);
        text_show_string_middle(x + (i % 3)*kbdxsize, y + kbdysize / 2 + kbdysize * (i / 3), (char *)kbs_tbl[i], 16, kbdxsize, BLUE);
    }
}

/**
 * @brief       ����״̬����
 * @param       x, y : ��������
 * @param       key  : ��ֵ��0~8��
 * @retval      ��Ӧ����״̬
 *   @arg       0, �ɿ�;
 *   @arg       1, ����;
 */
void py_key_staset(uint16_t x, uint16_t y, uint8_t keyx, uint8_t sta)
{
    uint16_t i = keyx / 3, j = keyx % 3;

    if (keyx > 8)return;

    if (sta) lcd_fill(x + j * kbdxsize + 1, y + i * kbdysize + 1, x + j * kbdxsize + kbdxsize - 1, y + i * kbdysize + kbdysize - 1, GREEN);
    else lcd_fill(x + j * kbdxsize + 1, y + i * kbdysize + 1, x + j * kbdxsize + kbdxsize - 1, y + i * kbdysize + kbdysize - 1, WHITE);

    text_show_string_middle(x + j * kbdxsize, y + 4 + kbdysize * i, (char *)kbd_tbl[keyx], 16, kbdxsize, BLUE);
    text_show_string_middle(x + j * kbdxsize, y + kbdysize / 2 + kbdysize * i, (char *)kbs_tbl[keyx], 16, kbdxsize, BLUE);
}

/**
 * @brief       �õ�������������
 * @param       x, y : ��������
 * @retval      ������ֵ(1~9 ��Ч��0, ��Ч;)
 */
uint8_t py_get_keynum(uint16_t x, uint16_t y)
{
    uint16_t i, j;
    static uint8_t key_x = 0;       /* 0,û���κΰ������£�1~9��1~9�Ű������� */
    uint8_t key = 0;
    tp_dev.scan(0);

    if (tp_dev.sta & TP_PRES_DOWN)  /* ������������ */
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (tp_dev.x[0] < (x + j * kbdxsize + kbdxsize) && 
                    tp_dev.x[0] > (x + j * kbdxsize) && 
                    tp_dev.y[0] < (y + i * kbdysize + kbdysize) && 
                    tp_dev.y[0] > (y + i * kbdysize))
                {
                    key = i * 3 + j + 1;
                    break;
                }
            }

            if (key)
            {
                if (key_x == key)key = 0;
                else
                {
                    py_key_staset(x, y, key_x - 1, 0);
                    key_x = key;
                    py_key_staset(x, y, key_x - 1, 1);
                }

                break;
            }
        }
    }
    else if (key_x)
    {
        py_key_staset(x, y, key_x - 1, 0);
        key_x = 0;
    }

    return key;
}

/**
 * @brief       ��ʾ���
 * @param       index: ������
 *   @arg       0    , ��ʾû��һ��ƥ��Ľ��, ���֮ǰ����ʾ
 *   @arg       ���� , ������
 * @retval      ��
 */
void py_show_result(uint8_t index)
{
    lcd_show_num(30 + 144, 125, index, 1, 16, BLUE);            /* ��ʾ��ǰ������ */
    lcd_fill(30 + 40, 125, 30 + 40 + 48, 130 + 16, WHITE);      /* ���֮ǰ����ʾ */
    lcd_fill(30 + 40, 145, lcddev.width - 1, 145 + 48, WHITE);  /* ���֮ǰ����ʾ */

    if (index)
    {
        text_show_string(30 + 40, 125, 200, 16, (char *)t9.pymb[index - 1]->py, 16, 0, BLUE);   /* ��ʾƴ�� */
        text_show_string(30 + 40, 145, lcddev.width - 70, 48, (char *)t9.pymb[index - 1]->pymb, 16, 0, BLUE);   /* ��ʾ��Ӧ�ĺ��� */
        printf("\r\nƴ��:%s\r\n", t9.pymb[index - 1]->py);    /* �������ƴ�� */
        printf("���:%s\r\n", t9.pymb[index - 1]->pymb);      /* ���������� */
    }
}

int main(void)
{
    uint8_t i = 0;
    uint8_t result_num;
    uint8_t cur_index;
    uint8_t key;
    uint8_t inputstr[7];    /* �������6���ַ�+������ */
    uint8_t inputlen;       /* ���볤�� */

    HAL_Init();                         /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7); /* ����ʱ��, 168Mhz */
    delay_init(168);                    /* ��ʱ��ʼ�� */
    usmart_dev.init(84);
    usart_init(115200);                 /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                         /* ��ʼ��LED */
    lcd_init();                         /* ��ʼ��LCD */
    key_init();                         /* ��ʼ������ */
    tp_dev.init();                      /* ��������ʼ�� */
    sram_init();                        /* ��ʼ��SRAM */
    
    my_mem_init(SRAMIN);                /* ��ʼ���ڲ��ڴ�� */
    my_mem_init(SRAMEX);                /* ��ʼ���ⲿ�ڴ�� */
    my_mem_init(SRAMCCM);               /* ��ʼ���ڲ�CCM�ڴ�� */

    exfuns_init();                      /* Ϊfatfs��ر��������ڴ� */
    f_mount(fs[0], "0:", 1);            /* ����SD�� */
    f_mount(fs[1], "1:", 1);            /* ����FLASH */
    
RESTART:
    while (fonts_init())     /* ����ֿ� */
    {
        lcd_show_string(60, 50, 200, 16, 16, "Font Error!", RED);
        delay_ms(200);
        lcd_fill(60, 50, 240, 66, WHITE);   /* �����ʾ */
        delay_ms(200);
    }

    text_show_string(30, 5, 200, 16, "����ԭ��STM32������", 16, 0, RED);
    text_show_string(30, 25, 200, 16, "ƴ�����뷨ʵ��", 16, 0, RED);
    text_show_string(30, 45, 200, 16, "ATOM@ALIENTEK", 16, 0, RED);
    text_show_string(30, 65, 200, 16, "KEY_UP:У׼", 16, 0, RED);
    text_show_string(30, 85, 200, 16, "KEY0:��ҳ  KEY1:���", 16, 0, RED);
    text_show_string(30, 105, 200, 16, "����:        ƥ��:  ", 16, 0, RED);
    text_show_string(30, 125, 200, 16, "ƴ��:        ��ǰ:  ", 16, 0, RED);
    text_show_string(30, 145, 210, 32, "���:", 16, 0, RED);

    /* ����LCD�ֱ������ð�����С */
    if (lcddev.id == 0X5310)
    {
        kbdxsize = 86;    
        kbdysize = 43;
    }
    else if (lcddev.id == 0X5510)
    {
        kbdxsize = 140;
        kbdysize = 70;
    }
    else
    {
        kbdxsize = 60;
        kbdysize = 40;
    }

    py_load_ui(30, 195);
    my_mem_set(inputstr, 0, 7); /* ȫ������ */
    inputlen = 0;   /* ���볤��Ϊ0 */
    result_num = 0; /* ��ƥ�������� */
    cur_index = 0;

    while (1)
    {
        i++;
        delay_ms(10);
        key = py_get_keynum(30, 195);

        if (key)
        {
            if (key == 1)   /* ɾ�� */
            {
                if (inputlen)inputlen--;

                inputstr[inputlen] = '\0'; /* ��ӽ����� */
            }
            else
            {
                inputstr[inputlen] = key + '0'; /* �����ַ� */

                if (inputlen < 7)inputlen++;
            }

            if (inputstr[0] != NULL)
            {
                key = t9.getpymb(inputstr);	/* �õ�ƥ��Ľ���� */

                if (key)    /* �в���ƥ��/��ȫƥ��Ľ�� */
                {
                    result_num = key & 0X7F;    /* ��ƥ���� */
                    cur_index = 1;              /* ��ǰΪ��һ������ */

                    if (key & 0X80)             /* �ǲ���ƥ�� */
                    {
                        inputlen = key & 0X7F;  /* ��Чƥ��λ�� */
                        inputstr[inputlen] = '\0';  /* ��ƥ���λ��ȥ�� */

                        if (inputlen > 1)result_num = t9.getpymb(inputstr); /* ���»�ȡ��ȫƥ���ַ��� */
                    }
                }
                else    /* û���κ�ƥ�� */
                {
                    inputlen--;
                    inputstr[inputlen] = '\0';
                }
            }
            else
            {
                cur_index = 0;
                result_num = 0;
            }

            lcd_fill(30 + 40, 105, 30 + 40 + 48, 110 + 16, WHITE);      /* ���֮ǰ����ʾ */
            lcd_show_num(30 + 144, 105, result_num, 1, 16, BLUE);       /* ��ʾƥ��Ľ���� */
            text_show_string(30 + 40, 105, 200, 16, (char *)inputstr, 16, 0, BLUE); /* ��ʾ��Ч�����ִ� */
            py_show_result(cur_index);  /* ��ʾ��cur_index��ƥ���� */
        }

        key = key_scan(0);

        if (key == WKUP_PRES && tp_dev.touchtype == 0)  /* WKUP����,���ǵ����� */
        {
            tp_dev.adjust();
            lcd_clear(WHITE);
            goto RESTART;
        }

        if (result_num) /* ����ƥ��Ľ�� */
        {
            switch (key)
            {
                case KEY0_PRES: /* �·� */
                    if (cur_index < result_num)cur_index++;
                    else cur_index = 1;

                    py_show_result(cur_index);  /* ��ʾ��cur_index��ƥ���� */
                    break;

                case KEY1_PRES: /* ������� */
                    lcd_fill(30 + 40, 145, lcddev.width - 1, 145 + 48, WHITE);  /* ���֮ǰ����ʾ */
                    goto RESTART;
            }
        }

        if (i == 30)
        {
            i = 0;
            LED0_TOGGLE();
        }
    }
}




