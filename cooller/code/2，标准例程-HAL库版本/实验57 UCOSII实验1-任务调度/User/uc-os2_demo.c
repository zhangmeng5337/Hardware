/**
 ****************************************************************************************************
 * @file        uc-os2_demo.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2023-02-10
 * @brief       UCOSIIʵ��1-������� ʵ��
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
#include "./MALLOC/malloc.h"
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


/* LED0 ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define LED0_TASK_PRIO                  7       /* ��ʼ��������ȼ�����Ϊ��� */
#define LED0_STK_SIZE                   128     /* ��ջ��С */
OS_STK LED0_TASK_STK[LED0_STK_SIZE];            /* �����ջ */
void led0_task(void *pdata);                    /* ������ */

/* LED1 ���� ����
 * ����: �������ȼ� ��ջ��С ��
 */
#define LED1_TASK_PRIO                  6       /* ��ʼ��������ȼ�����Ϊ��� */
#define LED1_STK_SIZE                   128     /* ��ջ��С */
OS_STK LED1_TASK_STK[LED0_STK_SIZE];            /* �����ջ */
void led1_task(void *pdata);                    /* ������ */

/******************************************************************************************************/
/**
 * @brief       uC/OS-II������ں���
 * @param       ��
 * @retval      ��
 */
void uc_os2_demo(void)
{
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

    OSStatInit();                   /* ����ͳ������ */
    
    /* �������õĽ���Ƶ������SysTick */
    cnts = (CPU_INT32U)(HAL_RCC_GetSysClockFreq() / OS_TICKS_PER_SEC);
    OS_CPU_SysTickInit(cnts);
    OS_ENTER_CRITICAL();            /* �����ٽ���(�ر��ж�) */
    
    OSTaskCreateExt((void(*)(void *) )led0_task,
                    (void *          )0,
                    (OS_STK *        )&LED0_TASK_STK[LED0_STK_SIZE - 1],
                    (INT8U           )LED0_TASK_PRIO,
                    (INT16U          )LED0_TASK_PRIO,
                    (OS_STK *        )&LED0_TASK_STK[0],
                    (INT32U          )LED0_STK_SIZE,
                    (void *          )0,
                    (INT16U          )OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR | OS_TASK_OPT_SAVE_FP);
    /* LED1���� */
    OSTaskCreateExt((void(*)(void *) )led1_task,
                    (void *          )0,
                    (OS_STK *        )&LED1_TASK_STK[LED1_STK_SIZE - 1],
                    (INT8U           )LED1_TASK_PRIO,
                    (INT16U          )LED1_TASK_PRIO,
                    (OS_STK *        )&LED1_TASK_STK[0],
                    (INT32U          )LED1_STK_SIZE,
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
void led0_task(void *p_arg)
{
    while (1)
    {
        LED0(0);
        OSTimeDly(80);
        LED0(1);
        OSTimeDly(920);
    }
}

/**
 * @brief       LED1����
 * @param       p_arg : �������(δ�õ�)
 * @retval      ��
 */
void led1_task(void *p_arg)
{
    while (1)
    {
        LED1(0);
        OSTimeDly(300);
        LED1(1);
        OSTimeDly(300);
    }
}


