/**
 ****************************************************************************************************
 * @file        wdg.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2021-10-14
 * @brief       ���Ź� ��������
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
 * �޸�˵��
 * V1.0 20211014
 * ��һ�η���
 * V1.1 20211014
 * ����wwdg_init����
 *
 ****************************************************************************************************
 */

#include "./BSP/WDG/wdg.h"
#include "./BSP/LED/led.h"


IWDG_HandleTypeDef iwdg_handler; /*�������Ź���� */

/**
 * @brief       ��ʼ���������Ź� 
 * @param       prer: IWDG_PRESCALER_4~IWDG_PRESCALER_256,��Ӧ4~256��Ƶ
 *   @arg       ��Ƶ���� = 4 * 2^prer. �����ֵֻ����256!
 * @param       rlr: �Զ���װ��ֵ,0~0XFFF. 
 * @note        ʱ�����(���):Tout=((4 * 2^prer) * rlr) / 32 (ms). 
 * @retval      ��
 */
void iwdg_init(uint32_t prer, uint16_t rlr)
{
    iwdg_handler.Instance = IWDG;
    iwdg_handler.Init.Prescaler = prer; /* ����IWDG��Ƶϵ�� */
    iwdg_handler.Init.Reload = rlr;     /* �Ӽ��ؼĴ��� IWDG->RLR ��װ��ֵ */
    HAL_IWDG_Init(&iwdg_handler);       /* ��ʼ��IWDG��ʹ�� */
}

/**
 * @brief       ι�������Ź�
 * @param       ��
 * @retval      ��
 */
void iwdg_feed(void)
{
    HAL_IWDG_Refresh(&iwdg_handler);    /* ι�� */
}


/************************�����Ǵ��ڿ��Ź�����********************************/

WWDG_HandleTypeDef wwdg_handler;        /* ���ڿ��Ź���� */

/**
 * @brief       ��ʼ�����ڿ��Ź�
 * @param       tr: T[6:0],������ֵ
 * @param       tw: W[6:0],����ֵ
 * @note        fprer:��Ƶϵ����WDGTB��,��Χ:WWDG_PRESCALER_1~WWDG_PRESCALER_8,��ʾ2^WDGTB��Ƶ
 *              Fwwdg=PCLK1/(4096*2^fprer). һ��PCLK1=42Mhz
 * @retval      ��
 */
void wwdg_init(uint8_t tr, uint8_t wr, uint32_t fprer)
{
    wwdg_handler.Instance = WWDG;
    wwdg_handler.Init.Prescaler = fprer;         /* ���÷�Ƶϵ�� */
    wwdg_handler.Init.Window = wr;               /* ���ô���ֵ */
    wwdg_handler.Init.Counter = tr;              /* ���ü�����ֵ */
    wwdg_handler.Init.EWIMode = WWDG_EWI_ENABLE; /* ʹ�ܴ��ڿ��Ź���ǰ�����ж� */
    HAL_WWDG_Init(&wwdg_handler);                /* ��ʼ��WWDG */
}

/**
 * @brief       WWDG�ײ�������ʱ�����ã��ж�����
                �˺����ᱻHAL_WWDG_Init()����
 * @param       hwwdg:���ڿ��Ź����
 * @retval      ��
 */
void HAL_WWDG_MspInit(WWDG_HandleTypeDef *hwwdg)
{
    __HAL_RCC_WWDG_CLK_ENABLE();            /* ʹ�ܴ��ڿ��Ź�ʱ�� */

    HAL_NVIC_SetPriority(WWDG_IRQn, 2, 3);  /* ��ռ���ȼ�2�������ȼ�Ϊ3 */
    HAL_NVIC_EnableIRQ(WWDG_IRQn);          /* ʹ�ܴ��ڿ��Ź��ж� */
}

/**
 * @brief       ���ڿ��Ź��жϷ������
 * @param       ��
 * @retval      ��
 */
void WWDG_IRQHandler(void)
{
    HAL_WWDG_IRQHandler(&wwdg_handler);     /* ����WWDG�����жϴ����� */
}

/**
 * @brief       �жϷ������������
                �˺����ᱻHAL_WWDG_IRQHandler()����
 * @param       ��
 * @retval      ��
 */
void HAL_WWDG_EarlyWakeupCallback(WWDG_HandleTypeDef *hwwdg)
{
    HAL_WWDG_Refresh(&wwdg_handler);        /* ���´��ڿ��Ź�ֵ */
    LED1_TOGGLE();                          /*  �̵���˸ */
}

