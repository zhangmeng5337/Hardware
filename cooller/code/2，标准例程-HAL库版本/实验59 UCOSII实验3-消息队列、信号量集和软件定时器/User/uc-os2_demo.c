/**
 ****************************************************************************************************
 * @file        uc-os2_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-10
 * @brief       UCOSIIʵ��3-��Ϣ���С��ź������������ʱ�� ʵ��
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
#include "./SYSTEM/delay/delay.h"
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


/* ������Ϣ��ʾ ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define QMSGSHOW_TASK_PRIO              5       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define QMSGSHOW_STK_SIZE               128     /* ��ջ��С */

OS_STK QMSGSHOW_TASK_STK[QMSGSHOW_STK_SIZE];    /* �����ջ */
void qmsgshow_task(void *pdata);                /* ������ */


/* �� ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define MAIN_TASK_PRIO                  4       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define MAIN_STK_SIZE                   512     /* ��ջ��С */

OS_STK MAIN_TASK_STK[MAIN_STK_SIZE];            /* �����ջ */
void main_task(void *pdata);                    /* ������ */


/* �ź����� ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define FLAGS_TASK_PRIO                 3       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define FLAGS_STK_SIZE                  512     /* ��ջ��С */

OS_STK FLAGS_TASK_STK[FLAGS_STK_SIZE];          /* �����ջ */
void flags_task(void *pdata);                   /* ������ */


/* ����ɨ�� ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define KEY_TASK_PRIO                   2       /* ���ȼ�����(ԽС���ȼ�Խ��) */
#define KEY_STK_SIZE                    512     /* ��ջ��С */

OS_STK KEY_TASK_STK[KEY_STK_SIZE];              /* �����ջ */
void key_task(void *pdata);                     /* ������ */


/******************************************************************************************************/
OS_EVENT *msg_key;          /* ���������¼��� */
OS_EVENT *q_msg;            /* ��Ϣ���� */
OS_TMR    *tmr1;            /* �����ʱ��1 */
OS_TMR    *tmr2;            /* �����ʱ��2 */
OS_TMR    *tmr3;            /* �����ʱ��3 */
OS_FLAG_GRP *flags_key;     /* �����ź����� */

void *MsgGrp[256];          /* ��Ϣ���д洢��ַ,���֧��256����Ϣ */

/* ��Щ������main��������ʵ�� */
void tmr1_callback(OS_TMR *ptmr, void *p_arg);
void tmr2_callback(OS_TMR *ptmr, void *p_arg);
void tmr3_callback(OS_TMR *ptmr, void *p_arg);

void ucos_load_main_ui(void);
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t size, uint16_t color);

/**
 * @brief       uC/OS-II������ں���
 * @param       ��
 * @retval      ��
 */
void uc_os2_demo(void)
{
    ucos_load_main_ui();                                        /* ���������� */
    
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
    uint8_t err;
    OS_CPU_SR cpu_sr = 0;
    CPU_INT32U cnts;    
    /* �������õĽ���Ƶ������SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OS_TICKS_PER_SEC);
    OS_CPU_SysTickInit(cnts);
    
    msg_key = OSMboxCreate((void *)0);  /* ������Ϣ���� */
    q_msg = OSQCreate(&MsgGrp[0], 256); /* ������Ϣ���� */
    flags_key = OSFlagCreate(0, &err);  /* �����ź����� */
    OSStatInit();                       /* ����ͳ������ */
    OS_ENTER_CRITICAL();                /* �����ٽ���(�ر��ж�) */
    
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
    /* ��Ϣ������ʾ���� */
    OSTaskCreateExt((void(*)(void *) )qmsgshow_task,
                    (void *          )0,
                    (OS_STK *        )&QMSGSHOW_TASK_STK[QMSGSHOW_STK_SIZE - 1],
                    (INT8U           )QMSGSHOW_TASK_PRIO,
                    (INT16U          )QMSGSHOW_TASK_PRIO,
                    (OS_STK *        )&QMSGSHOW_TASK_STK[0],
                    (INT32U          )QMSGSHOW_STK_SIZE,
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
    /* �ź��������� */
    OSTaskCreateExt((void(*)(void *) )flags_task,
                    (void *          )0,
                    (OS_STK *        )&FLAGS_TASK_STK[FLAGS_STK_SIZE - 1],
                    (INT8U           )FLAGS_TASK_PRIO,
                    (INT16U          )FLAGS_TASK_PRIO,
                    (OS_STK *        )&FLAGS_TASK_STK[0],
                    (INT32U          )FLAGS_STK_SIZE,
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
void touch_task(void *p_arg)
{
    uint32_t cpu_sr;
    uint16_t lastpos[2]; /* ���һ�ε����� */

    while (1)
    {
        tp_dev.scan(0);

        if (tp_dev.sta & TP_PRES_DOWN)  /* ������������ */
        {
            if (tp_dev.x[0] < (130 - 1) && tp_dev.y[0] < lcddev.height && tp_dev.y[0] > (220 + 1))
            {
                if (lastpos[0] == 0XFFFF)
                {
                    lastpos[0] = tp_dev.x[0];
                    lastpos[1] = tp_dev.y[0];
                }

                OS_ENTER_CRITICAL();    /* �����ٽ��,��ֹ��������,���LCD����,����Һ������ */
                lcd_draw_bline(lastpos[0], lastpos[1], tp_dev.x[0], tp_dev.y[0], 2, RED);   /* ���� */
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
 * @brief       ������Ϣ��ʾ����
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void qmsgshow_task(void *p_arg)
{
    char *p;
    uint8_t err;

    while (1)
    {
        printf("qmsgshow_task\r\n");
        p = OSQPend(q_msg, 0, &err);                  /* ������Ϣ���� */
        lcd_show_string(5, 170, 240, 16, 16, p, RED); /* ��ʾ��Ϣ */
        myfree(SRAMIN, p);
        OSTimeDly(500);
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
    uint8_t tmr2sta = 1;    /* �����ʱ��2����״̬ */
    uint8_t tmr3sta = 0;    /* �����ʱ��3����״̬ */
    uint8_t flagsclrt = 0;  /* �ź�������ʾ���㵹��ʱ */
    tmr1 = OSTmrCreate(10, 10, OS_TMR_OPT_PERIODIC, (OS_TMR_CALLBACK)tmr1_callback, 0, (unsigned char*)"tmr1", &err);   /* 100msִ��һ�� */
    tmr2 = OSTmrCreate(10, 20, OS_TMR_OPT_PERIODIC, (OS_TMR_CALLBACK)tmr2_callback, 0, (unsigned char*)"tmr2", &err);   /* 200msִ��һ�� */
    tmr3 = OSTmrCreate(10, 10, OS_TMR_OPT_PERIODIC, (OS_TMR_CALLBACK)tmr3_callback, 0, (unsigned char*)"tmr3", &err);   /* 100msִ��һ�� */
    OSTmrStart(tmr1, &err); /* ���������ʱ��1 */
    OSTmrStart(tmr2, &err); /* ���������ʱ��2 */

    while (1)
    {
        key = (uint32_t)OSMboxPend(msg_key, 10, &err);

        if (key)
        {
            flagsclrt = 51; /* 500ms����� */
            OSFlagPost(flags_key, 1 << (key - 1), OS_FLAG_SET, &err); /* ���ö�Ӧ���ź���Ϊ1 */
        }

        if (flagsclrt)   /* ����ʱ */
        {
            flagsclrt--;

            if (flagsclrt == 1)lcd_fill(140, 162, 239, 162 + 16, WHITE); /* �����ʾ */
        }

        switch (key)
        {
            case KEY0_PRES:/* �����ʱ��2 ����,������ */
                tmr2sta = !tmr2sta;

                if (tmr2sta)
                {
                    OSTmrStart(tmr2, &err); /* ���������ʱ��2 */
                }
                else
                {
                    OSTmrStop(tmr2, OS_TMR_OPT_NONE, 0, &err);                /* �ر������ʱ��2 */
                    lcd_show_string(148, 262, 240, 16, 16, "TMR2 STOP", RED); /* ��ʾ��ʱ��2�ر��� */
                }

                lcd_fill(0, 221, 129, lcddev.height - 1, WHITE); /* ˳������ */
                break;

            case KEY1_PRES:/* ���������ʱ��3 */
                tmr3sta = !tmr3sta;

                if (tmr3sta)
                {
                    OSTmrStart(tmr3, &err);
                }
                else
                {
                    OSTmrStop(tmr3, OS_TMR_OPT_NONE, 0, &err);  /* �ر������ʱ��3 */
                }
                
                break;

            case WKUP_PRES:/* У׼ */
                OSTaskSuspend(TOUCH_TASK_PRIO);             /* ������������ */
                OSTaskSuspend(QMSGSHOW_TASK_PRIO);          /* ���������Ϣ��ʾ���� */
                OSTmrStop(tmr1, OS_TMR_OPT_NONE, 0, &err);  /* �ر������ʱ��1 */

                if (tmr2sta)OSTmrStop(tmr2, OS_TMR_OPT_NONE, 0, &err);  /* �ر������ʱ��2 */

                if ((tp_dev.touchtype & 0X80) == 0)
                {
                    tp_adjust();
                }
                
                OSTmrStart(tmr1, &err);                 /* ���¿��������ʱ��1 */

                if (tmr2sta)OSTmrStart(tmr2, &err);     /* ���¿��������ʱ��2 */

                OSTaskResume(TOUCH_TASK_PRIO);          /* ��� */
                OSTaskResume(QMSGSHOW_TASK_PRIO);       /* ��� */
                ucos_load_main_ui();                    /* ���¼��������� */
                break;
        }

        OSTimeDly(10);
    }
}

/**
 * @brief       �ź�������������
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void flags_task(void *p_arg)
{
    uint16_t flags;
    uint8_t err;

    while (1)
    {
        flags = OSFlagPend(flags_key, 0X0007, OS_FLAG_WAIT_SET_ANY, 0, &err);   /* �ȴ��ź��� */

        if (flags & 0X0001)lcd_show_string(140, 162, 240, 16, 16, "KEY0 DOWN  ", RED);

        if (flags & 0X0002)lcd_show_string(140, 162, 240, 16, 16, "KEY1 DOWN  ", RED);

        if (flags & 0X0004)lcd_show_string(140, 162, 240, 16, 16, "KEY_2 DOWN  ", RED);

        BEEP(1);
        OSTimeDly(50);
        BEEP(0);
        OSFlagPost(flags_key, 0X0007, OS_FLAG_CLR, &err); /* ȫ���ź������� */
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
 * @brief       �����ʱ��1�Ļص�����
 * @note        ÿ100msִ��һ��,������ʾCPUʹ���ʺ��ڴ�ʹ����
 * @param       ptmr : �����ʱ��ָ��
 * @param       p_arg: ����ָ��(δ�õ�)
 * @retval      ��
 */
void tmr1_callback(OS_TMR *ptmr, void *p_arg)
{
    static uint16_t cpuusage = 0;
    static uint8_t tcnt = 0;

    if (tcnt == 5)
    {
        lcd_show_xnum(202, 10, cpuusage / 5, 3, 16, 0, BLUE); /* ��ʾCPUʹ���� */
        cpuusage = 0;
        tcnt = 0;
    }

    cpuusage += OSCPUUsage;
    tcnt++;
    lcd_show_xnum(202, 30, my_mem_perused(SRAMIN) / 10, 3, 16, 0, BLUE);                  /* ��ʾ�ڴ�ʹ���� */
    lcd_show_xnum(202, 50, ((OS_Q *)(q_msg->OSEventPtr))->OSQEntries, 3, 16, 0X80, BLUE); /* ��ʾ���е�ǰ�Ĵ�С */
}

/**
 * @brief       �����ʱ��2�Ļص�����
 * @note        ÿ200msִ��һ��
 * @param       ptmr : �����ʱ��ָ��
 * @param       p_arg: ����ָ��(δ�õ�)
 * @retval      ��
 */
void tmr2_callback(OS_TMR *ptmr, void *p_arg)
{
    static uint8_t sta = 0;

    switch (sta)
    {
        case 0:
            lcd_fill(131, 221, lcddev.width - 1, lcddev.height - 1, RED);
            break;

        case 1:
            lcd_fill(131, 221, lcddev.width - 1, lcddev.height - 1, GREEN);
            break;

        case 2:
            lcd_fill(131, 221, lcddev.width - 1, lcddev.height - 1, BLUE);
            break;

        case 3:
            lcd_fill(131, 221, lcddev.width - 1, lcddev.height - 1, MAGENTA);
            break;

        case 4:
            lcd_fill(131, 221, lcddev.width - 1, lcddev.height - 1, CYAN);
            break;

        case 5:
            lcd_fill(131, 221, lcddev.width - 1, lcddev.height - 1, YELLOW);
            break;

        case 6:
            lcd_fill(131, 221, lcddev.width - 1, lcddev.height - 1, BRRED);
            break;
    }

    sta++;

    if (sta > 6)sta = 0;
}

/**
 * @brief       �����ʱ��3�Ļص�����
 * @note        ÿ300msִ��һ��
 * @param       ptmr : �����ʱ��ָ��
 * @param       p_arg: ����ָ��(δ�õ�)
 * @retval      ��
 */
void tmr3_callback(OS_TMR *ptmr, void *p_arg)
{
    uint8_t *p;
    uint8_t err;
    static uint8_t msg_cnt = 0; /* msg��� */
    p = mymalloc(SRAMIN, 13);   /* ����13���ֽڵ��ڴ� */

    if (p)
    {
        sprintf((char *)p, "ALIENTEK %03d", msg_cnt);
        msg_cnt++;
        err = OSQPost(q_msg, p);/* ���Ͷ��� */

        if (err != OS_ERR_NONE) /* ����ʧ�� */
        {
            myfree(SRAMIN, p);  /* �ͷ��ڴ� */
            OSTmrStop(tmr3, OS_TMR_OPT_NONE, 0, &err);  /* �ر������ʱ��3 */
        }
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
    lcd_show_string(10, 10, 200, 16, 16, "STM32", RED);
    lcd_show_string(10, 30, 200, 16, 16, "UCOSII TEST3", RED);
    lcd_show_string(10, 50, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(10, 75, 240, 16, 16, "KEY0:TMR2 SW & CLR KEY1:Q SW", RED);
    lcd_show_string(10, 95, 240, 16, 16, "KEY_UP:ADJUST", RED);
    lcd_draw_line(0, 70, lcddev.width - 1, 70, RED);
    lcd_draw_line(150, 0, 150, 70, RED);

    lcd_draw_line(0, 120, lcddev.width - 1, 120, RED);
    lcd_draw_line(0, 220, lcddev.width - 1, 220, RED);
    lcd_draw_line(130, 120, 130, lcddev.height - 1, RED);

    lcd_show_string(5, 125, 240, 16, 16, "QUEUE MSG", RED);     /* ������Ϣ */
    lcd_show_string(5, 150, 240, 16, 16, "Message:", RED);
    lcd_show_string(5 + 130, 125, 240, 16, 16, "FLAGS", RED);   /* �ź����� */
    lcd_show_string(5, 225, 240, 16, 16, "TOUCH", RED);         /* ������ */
    lcd_show_string(5 + 130, 225, 240, 16, 16, "TMR2", RED);    /* ������Ϣ */
    
    lcd_show_string(170, 10, 200, 16, 16, "CPU:   %", BLUE);
    lcd_show_string(170, 30, 200, 16, 16, "MEM:   %", BLUE);
    lcd_show_string(170, 50, 200, 16, 16, " Q :000", BLUE);

    delay_ms(300);
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

    if ( delta_x > delta_y)distance = delta_x;  /* ѡȡ�������������� */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )        /* ������� */
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

