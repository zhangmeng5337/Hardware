/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-25
 * @brief       CANͨ�� ʵ��
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
#include "./BSP/CAN/can.h"


int main(void)
{
    uint8_t key;
    uint8_t i = 0, t = 0;
    uint8_t cnt = 0;
    uint8_t canbuf[8];
    uint8_t rxlen = 0;
    uint8_t res;
    uint8_t mode = 1; /* CAN����ģʽ: 0,��ͨģʽ; 1,����ģʽ */
    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    usmart_dev.init(84);                    /* ��ʼ��USMART */
    led_init();                             /* ��ʼ��LED */
    lcd_init();                             /* ��ʼ��LCD */
    key_init();                             /* ��ʼ������ */
    can_init(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 6, CAN_MODE_LOOPBACK);  /* CAN��ʼ��, ����ģʽ, ������500Kbps */
    
    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "CAN TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 110, 200, 16, 16, "LoopBack Mode", RED);
    lcd_show_string(30, 130, 200, 16, 16, "KEY0:Send KEK_UP:Mode", RED);    /* ��ʾ��ʾ��Ϣ */

    lcd_show_string(30, 150, 200, 16, 16, "Count:", RED);                   /* ��ʾ��ǰ����ֵ */
    lcd_show_string(30, 170, 200, 16, 16, "Send Data:", RED);               /* ��ʾ���͵����� */
    lcd_show_string(30, 230, 200, 16, 16, "Receive Data:", RED);            /* ��ʾ���յ������� */

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)           /* KEY0����,����һ������ */
        {
            for (i = 0; i < 8; i++)
            {
                canbuf[i] = cnt + i;    /* ��䷢�ͻ����� */

                if (i < 4)
                {
                    lcd_show_xnum(30 + i * 32, 190, canbuf[i], 3, 16, 0x80, BLUE);          /* ��ʾ���� */
                }
                else
                {
                    lcd_show_xnum(30 + (i - 4) * 32, 210, canbuf[i], 3, 16, 0x80, BLUE);    /* ��ʾ���� */
                }
            }

            res = can_send_msg(0x12, canbuf, 8);    /* ID = 0x12, ����8���ֽ� */

            if (res)
            {
                lcd_show_string(30 + 80, 170, 200, 16, 16, "Failed", BLUE); /* ��ʾ����ʧ�� */
            }
            else
            {
                lcd_show_string(30 + 80, 170, 200, 16, 16, "OK    ", BLUE); /* ��ʾ���ͳɹ� */
            }
        }
        else if (key == WKUP_PRES)      /* WK_UP����, �ı�CAN�Ĺ���ģʽ */
        {
            mode = !mode;
            /* CAN��ʼ��, ��ͨ(0)/�ػ�(1)ģʽ, ������500Kbps */
            can_init(CAN_SJW_1TQ, CAN_BS2_6TQ, CAN_BS1_7TQ, 6, mode ? CAN_MODE_LOOPBACK : CAN_MODE_NORMAL);

            if (mode == 0)  /* ��ͨģʽ, ��Ҫ2�������� */
            {
                lcd_show_string(30, 110, 200, 16, 16, "Normal Mode  ", RED);
            }
            else            /* �ػ�ģʽ,һ��������Ϳ��Բ�����. */
            {
                lcd_show_string(30, 110, 200, 16, 16, "LoopBack Mode", RED);
            }
        }

        rxlen = can_receive_msg(0x12, canbuf);  /* CAN ID = 0x12, �������ݲ�ѯ */

        if (rxlen) /* ���յ������� */
        {
            lcd_fill(30, 270, 130, 310, WHITE); /* ���֮ǰ����ʾ */

            for (i = 0; i < rxlen; i++)
            {
                if (i < 4)
                {
                    lcd_show_xnum(30 + i * 32, 250, canbuf[i], 3, 16, 0X80, BLUE);          /* ��ʾ���� */
                }
                else
                {
                    lcd_show_xnum(30 + (i - 4) * 32, 270, canbuf[i], 3, 16, 0X80, BLUE);    /* ��ʾ���� */
                }
            }
        }

        t++;
        delay_ms(10);

        if (t == 20)
        {
            LED0_TOGGLE();  /* ��ʾϵͳ�������� */
            t = 0;
            cnt++;
            lcd_show_xnum(30 + 48, 150, cnt, 3, 16, 0X80, BLUE);                            /* ��ʾ���� */
        }
    }
}


