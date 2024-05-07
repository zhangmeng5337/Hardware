/**
 ****************************************************************************************************
 * @file        uc-os2_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-10
 * @brief       UCOSIIʵ��2-�ź��������� ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨������ԭ�� ̽���� F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */

#include "uc-os2_demo.h"
#include "./SYSTEM/usart/usart.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/KEY/key.h"
#include "./MALLOC/malloc.h"
#include "./BSP/BEEP/beep.h"
#include "./BSP/TOUCH/touch.h"
/*uC/OS-III*********************************************************************************************/
#include "os.h"
#include "cpu.h"

/******************************************************************************************************/
/* UCOSII�������� */

/* START ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define START_TASK_PRIO                 10      /* ��ʼ��������ȼ�����Ϊ��� */
#define START_STK_SIZE                  128     /* ��ջ��С */

OS_STK START_TASK_STK[START_STK_SIZE];          /* �����ջ */
void start_task(void *pdata);                   /* ������ */


/* ���������� ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define TOUCH_TASK_PRIO                 7       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define TOUCH_STK_SIZE                  128     /* ��ջ��С */

OS_STK TOUCH_TASK_STK[TOUCH_STK_SIZE];          /* �����ջ */
void touch_task(void *pdata);                   /* ������ */


/* LED ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define LED_TASK_PRIO                   6       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define LED_STK_SIZE                    128     /* ��ջ��С */

OS_STK LED_TASK_STK[LED_STK_SIZE];              /* �����ջ */
void led_task(void *pdata);                     /* ������ */


/* ������ ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define BEEP_TASK_PRIO                  5       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define BEEP_STK_SIZE                   128     /* ��ջ��С */

OS_STK BEEP_TASK_STK[BEEP_STK_SIZE];            /* �����ջ */
void beep_task(void *pdata);                    /* ������ */


/* �� ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define MAIN_TASK_PRIO                  4       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define MAIN_STK_SIZE                   512     /* ��ջ��С */

OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];            /* �����ջ */
void main_task(void *pdata);                    /* ������ */


/* ����ɨ�� ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define KEY_TASK_PRIO                   3       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define KEY_STK_SIZE                    128     /* ��ջ��С */

OS_STK KEY_TASK_STK[KEY_STK_SIZE];              /* �����ջ */
void key_task(void *pdata);                     /* ������ */


/******************************************************************************************************/
OS_EVENT *msg_key;      /* ���������¼���ָ�� */
OS_EVENT *sem_beep;     /* �������ź���ָ�� */
void ucos_load_main_ui(void);
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color);

/**
 * @brief       uC/OS-II������ں���
 * @param       ��
 * @retval      ��
 */
void uc_os2_demo(void)
{
    ucos_load_main_ui();                    /* ���������� */
    
    OSInit();                                                   /* UCOS��ʼ�� */
    OSTaskCreateExt((void(*)(void *) )start_task,               /* ������ */
                    (void *          )0,                        /* ���ݸ��������Ĳ��� */
                    (OS_STK *        )&START_TASK_STK[START_STK_SIZE - 1], /* �����ջջ�� */
                    (INT8U           )START_TASK_PRIO,          /* �������ȼ� */
                    (INT16U          )START_TASK_PRIO,          /* ����ID����������Ϊ�����ȼ�һ�� */
                    (OS_STK *        )&START_TASK_STK[0],       /* �����ջջ�� */
                    (INT32U          )START_STK_SIZE,           /* �����ջ��С */
                    (void *          )0,                        /* �û�����Ĵ洢�� */
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP); /* ����ѡ��,Ϊ�˱���������������񶼱��渡��Ĵ�����ֵ */
    OSStart();  /* ��ʼ���� */
    
    for (;;)
    {
        /* ����������� */
    }
}

/**
 * @brief       start_task
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void start_task(void *p_arg)
{
    OS_CPU_SR cpu_sr = 0;
    CPU_INT32U cnts;

    msg_key = OSMboxCreate((void *)0);  /* ������Ϣ���� */
    sem_beep = OSSemCreate(0);          /* �����ź��� */
    
    OSStatInit();                       /* ����ͳ������ */
    /* �������õĽ���Ƶ������SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OS_TICKS_PER_SEC);
    OS_CPU_SysTickInit(cnts);
    
    OS_ENTER_CRITICAL();                /* �����ٽ���(�ر��ж�) */
    
    /* �������� */
    OSTaskCreateExt((void(*)(void *) )touch_task,
                    (void *          )0,
                    (OS_STK *        )&TOUCH_TASK_STK[TOUCH_STK_SIZE - 1],
                    (INT8U           )TOUCH_TASK_PRIO,
                    (INT16U          )TOUCH_TASK_PRIO,
                    (OS_STK *        )&TOUCH_TASK_STK[0],
                    (INT32U          )TOUCH_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
    /* LED���� */
    OSTaskCreateExt((void(*)(void *) )led_task,
                    (void *          )0,
                    (OS_STK *        )&LED_TASK_STK[LED_STK_SIZE - 1],
                    (INT8U           )LED_TASK_PRIO,
                    (INT16U          )LED_TASK_PRIO,
                    (OS_STK *        )&LED_TASK_STK[0],
                    (INT32U          )LED_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
    /* ���������� */
    OSTaskCreateExt((void(*)(void *) )beep_task,
                    (void *          )0,
                    (OS_STK *        )&BEEP_TASK_STK[BEEP_STK_SIZE - 1],
                    (INT8U           )BEEP_TASK_PRIO,
                    (INT16U          )BEEP_TASK_PRIO,
                    (OS_STK *        )&BEEP_TASK_STK[0],
                    (INT32U          )BEEP_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
    /* ������ */
    OSTaskCreateExt((void(*)(void *) )main_task,
                    (void *          )0,
                    (OS_STK *        )&MAIN_TASK_STK[MAIN_STK_SIZE - 1],
                    (INT8U           )MAIN_TASK_PRIO,
                    (INT16U          )MAIN_TASK_PRIO,
                    (OS_STK *        )&MAIN_TASK_STK[0],
                    (INT32U          )MAIN_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
    /* �������� */
    OSTaskCreateExt((void(*)(void *) )key_task,
                    (void *          )0,
                    (OS_STK *        )&KEY_TASK_STK[KEY_STK_SIZE - 1],
                    (INT8U           )KEY_TASK_PRIO,
                    (INT16U          )KEY_TASK_PRIO,
                    (OS_STK *        )&KEY_TASK_STK[0],
                    (INT32U          )KEY_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
    OS_EXIT_CRITICAL();             /* �˳��ٽ���(���ж�) */
    OSTaskSuspend(START_TASK_PRIO); /* ����ʼ���� */
}
/**
 * @brief       LED0����
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void led_task(void *p_arg)
{    
    uint8_t t;
    
    while (1)
    {
        t++;
        OSTimeDly(10);

        if (t == 8)LED0(1); /* LED0�� */

        if (t == 100)       /* LED0�� */
        {
            t = 0;
            LED0(0);
        }
    }
}

/**
 * @brief       ����������
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void beep_task(void *p_arg)
{
    uint8_t err;

    while (1)
    {
        OSSemPend(sem_beep, 0, &err); /* �����ź��� */
        BEEP(1);                      /* �򿪷����� */
        OSTimeDly(60);
        BEEP(0);                      /* �رշ����� */
        OSTimeDly(940);
    }
}

/**
 * @brief       ����������
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void touch_task(void *p_arg)
{
    uint32_t cpu_sr;
    uint16_t lastpos[2];    /* ���һ�ε����� */

    while (1)
    {
        tp_dev.scan(0);

        if (tp_dev.sta & TP_PRES_DOWN)  /* ������������ */
        {
            if (tp_dev.x[0] < lcddev.width && tp_dev.y[0] < lcddev.height && tp_dev.y[0] > 120)
            {
                if (lastpos[0] == 0XFFFF)
                {
                    lastpos[0] = tp_dev.x[0];
                    lastpos[1] = tp_dev.y[0];
                }

                OS_ENTER_CRITICAL();                                                      /* �����ٽ��,��ֹ��������,���LCD����,����Һ������ */
                lcd_draw_bline(lastpos[0], lastpos[1], tp_dev.x[0], tp_dev.y[0], 2, RED); /* ���� */
                OS_EXIT_CRITICAL();
                lastpos[0] = tp_dev.x[0];
                lastpos[1] = tp_dev.y[0];
            }
        }
        else
        {
            lastpos[0] = 0XFFFF;
            OSTimeDly(10);   /* û�а������µ�ʱ�� */
        }
    }
}

/**
 * @brief       ������
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void main_task(void *p_arg)
{
    uint32_t key = 0;
    uint8_t err;
    uint8_t semmask = 0;
    uint8_t tcnt = 0;

    while (1)
    {
        key = (uint32_t)OSMboxPend(msg_key, 10, &err);

        switch (key)
        {
            case KEY0_PRES: /* ����DS1,������������� */
                LED1_TOGGLE();
                lcd_fill(0, 121, lcddev.width - 1, lcddev.height - 1, WHITE);
                break;

            case KEY1_PRES: /* �����ź��� */
                semmask = 1;
                OSSemPost(sem_beep);
                break;

            case WKUP_PRES:                     /* У׼ */
                OSTaskSuspend(TOUCH_TASK_PRIO); /* ������������ */

                if ((tp_dev.touchtype & 0X80) == 0)
                {
                    tp_adjust();
                }

                OSTaskResume(TOUCH_TASK_PRIO); /* ��� */
                ucos_load_main_ui();           /* ���¼��������� */
                break;
            }

            if (semmask || sem_beep->OSEventCnt) /* ��Ҫ��ʾsem */
            {
                lcd_show_xnum(192, 50, sem_beep->OSEventCnt, 3, 16, 0X80, BLUE); /* ��ʾ�ź�����ֵ */

                if (sem_beep->OSEventCnt == 0)
                    semmask = 0; /* ֹͣ���� */
            }

            if (tcnt == 10) /* 0.6�����һ��CPUʹ���� */
            {
                tcnt = 0;
                lcd_show_xnum(192, 30, OSCPUUsage, 3, 16, 0, BLUE); /* ��ʾCPUʹ���� */
            }

        tcnt++;
        OSTimeDly(10);
    }
}

/**
 * @brief       ����ɨ������
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void key_task(void *p_arg)
{
    uint32_t key;

    while (1)
    {
        key = key_scan(0);

        if (key)OSMboxPost(msg_key, (void *)key); /* ������Ϣ */

        OSTimeDly(10);
    }
}

/**
 * @brief       ����������
 * @param       ��
 * @retval      ��
 */
void ucos_load_main_ui(void)
{
    lcd_clear(WHITE);   /* ���� */
    lcd_show_string(30, 10, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 30, 200, 16, 16, "UCOSII TEST2", RED);
    lcd_show_string(30, 50, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 75, 200, 16, 16, "KEY0:DS1 AND CLEAR", RED);
    lcd_show_string(30, 95, 200, 16, 16, "KEY1:BEEP  KEY_UP:ADJUST", RED);
    lcd_show_string(80, 210, 200, 16, 16, "Touch Area", RED);
    lcd_draw_line(0, 120, lcddev.width - 1, 120, RED);
    lcd_draw_line(0, 70, lcddev.width - 1, 70, RED);
    lcd_draw_line(150, 0, 150, 70, RED);
    
    lcd_show_string(160, 30, 200, 16, 16, "CPU:   %", BLUE);
    lcd_show_string(160, 50, 200, 16, 16, "SEM:000", BLUE);
}

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

    if (x1 < size || x2 < size || y1 < size || y2 < size)return;

    delta_x = x2 - x1;  /* ������������ */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)incx = 1;       /* ���õ������� */
    else if (delta_x == 0)incx = 0; /* ��ֱ�� */
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
        incy = 0;   /* ˮƽ�� */
    }
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x; /* ѡȡ�������������� */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )    /* ������� */
    {
        lcd_fill_circle(row, col, size, color); /* ���� */
        xerr += delta_x ;
        yerr += delta_y ;

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

