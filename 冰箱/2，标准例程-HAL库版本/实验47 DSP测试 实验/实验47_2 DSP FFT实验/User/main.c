/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-20
 * @brief       DSP FFT ʵ��
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
#include "./BSP/TIMER/btim.h"
#include "./CMSIS/DSP/Include/arm_math.h"


/* FFT���ȣ�Ĭ����1024��FFT 
 * ��ѡ��ΧΪ: 16, 64, 256, 1024.
 */
#define FFT_LENGTH      1024

float fft_inputbuf[FFT_LENGTH * 2];     /* FFT�������� */
float fft_outputbuf[FFT_LENGTH];        /* FFT������� */

uint8_t g_timeout;

int main(void)
{
    float time;
    char buf[50];
    arm_cfft_radix4_instance_f32 scfft;
    uint8_t key, t = 0;
    uint16_t i;

    
    HAL_Init();                             /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);     /* ����ʱ��,168Mhz */
    delay_init(168);                        /* ��ʱ��ʼ�� */
    usart_init(115200);                     /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                             /* ��ʼ��LED */
    key_init();                             /* ��ʼ������ */
    lcd_init();                             /* ��ʼ��LCD */

    btim_timx_int_init(65535, 8400 - 1);    /* 10Khz����Ƶ��,����ʱ6.5�볬�� */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "DSP FFT TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 130, 200, 16, 16, "KEY0:Run FFT", RED); /* ��ʾ��ʾ��Ϣ */
    lcd_show_string(30, 160, 200, 16, 16, "FFT runtime:", RED); /* ��ʾ��ʾ��Ϣ */
    arm_cfft_radix4_init_f32(&scfft, FFT_LENGTH, 0, 1);         /* ��ʼ��scfft�ṹ�壬�趨FFT��ز��� */

    while (1)
    {
        key = key_scan(0);

        if (key == KEY0_PRES)
        {
            for (i = 0; i < FFT_LENGTH; i++)    /* �����ź����� */
            {
                fft_inputbuf[2 * i] = 100 +
                                      10 * arm_sin_f32(2 * PI * i / FFT_LENGTH) +
                                      30 * arm_sin_f32(2 * PI * i * 4 / FFT_LENGTH) +
                                      50 * arm_cos_f32(2 * PI * i * 8 / FFT_LENGTH);    /* ���������ź�ʵ�� */
                fft_inputbuf[2 * i + 1] = 0;    /* �鲿ȫ��Ϊ0 */
            }

            BTIM_TIMX_INT->CNT = 0;; /* ����BTIM_TIMX_INT��ʱ���ļ�����ֵ */
            g_timeout = 0;

            arm_cfft_radix4_f32(&scfft, fft_inputbuf);                      /* FFT���㣨��4�� */
            
            time = BTIM_TIMX_INT->CNT + (uint32_t)g_timeout * 65536;        /* ��������ʱ�� */
            sprintf((char *)buf, "%0.1fms\r\n", time / 10);
            lcd_show_string(30 + 12 * 8, 160, 100, 16, 16, buf, BLUE);      /* ��ʾ����ʱ�� */
            
            arm_cmplx_mag_f32(fft_inputbuf, fft_outputbuf, FFT_LENGTH);     /* ��������������ģ�÷�ֵ */
            
            printf("\r\n%d point FFT runtime:%0.1fms\r\n", FFT_LENGTH, time / 10);
            printf("FFT Result:\r\n");

            for (i = 0; i < FFT_LENGTH; i++)
            {
                printf("fft_outputbuf[%d]:%f\r\n", i, fft_outputbuf[i]);
            }
        }
        else
        {
            delay_ms(10);
        }
        
        t++;

        if ((t % 20) == 0)
        {
            LED0_TOGGLE();
        }
    }
}
