/**
 ****************************************************************************************************
 * @file        pwmdac.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-03
 * @brief       PWM DAC��� ��������
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
 * V1.0 20211103
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/PWMDAC/pwmdac.h"


TIM_HandleTypeDef g_tim9_handler;     /* ��ʱ����� */
TIM_OC_InitTypeDef g_tim9_ch2handler; /* ��ʱ��2ͨ��4��� */

/**
 * @brief       PWM DAC��ʼ��, ʵ���Ͼ��ǳ�ʼ����ʱ��
 * @note
 *              ��ʱ����ʱ������APB1 / APB2, ��APB1 / APB2 ��Ƶʱ, ��ʱ��Ƶ���Զ�����
 *              ����, һ�������, �������ж�ʱ����Ƶ��, ����84Mhz ����ϵͳʱ��Ƶ��
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft = ��ʱ������Ƶ��, ��λ: Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: ʱ��Ԥ��Ƶ��
 * @retval      ��
 */
void pwmdac_init(uint16_t arr, uint16_t psc)
{
    g_tim9_handler.Instance = PWMDAC_TIMX;                  /* ��ʱ��9 */
    g_tim9_handler.Init.Prescaler = psc;                    /* ��ʱ����Ƶ */
    g_tim9_handler.Init.CounterMode = TIM_COUNTERMODE_UP;   /* ���ϼ���ģʽ */
    g_tim9_handler.Init.Period = arr;                       /* �Զ���װ��ֵ */
    HAL_TIM_PWM_Init(&g_tim9_handler);                      /* ��ʼ��PWM */

    g_tim9_ch2handler.OCMode = TIM_OCMODE_PWM1;                                         /* CH1/2 PWMģʽ1 */
    g_tim9_ch2handler.Pulse = arr / 2;                                                  /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ�룬��ռ�ձ�Ϊ50% */
    g_tim9_ch2handler.OCPolarity = TIM_OCPOLARITY_HIGH;                                 /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_tim9_handler, &g_tim9_ch2handler, PWMDAC_TIMX_CHY);    /* ����TIM2ͨ��4 */

    HAL_TIM_PWM_Start(&g_tim9_handler, PWMDAC_TIMX_CHY);    /* ����PWMͨ��4 */
}

/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
 * @note
 *              �˺����ᱻHAL_TIM_PWM_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef gpio_init_struct;

    if (htim->Instance == PWMDAC_TIMX)
    {
        PWMDAC_TIMX_CLK_ENABLE();           /* ʹ�ܶ�ʱ�� */
        PWMDAC_GPIO_CLK_ENABLE();           /* PWM DAC GPIO ʱ��ʹ�� */

        gpio_init_struct.Pin = PWMDAC_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_PULLUP;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_LOW;
        gpio_init_struct.Alternate = PWMDAC_GPIO_AFTIMX;
        HAL_GPIO_Init(PWMDAC_GPIO_PORT, &gpio_init_struct);         /* TIMX PWM CHY ����ģʽ���� */
    }
}

/**
 * @brief       ����PWM DAC�����ѹ
 * @param       vol : 0~3300,����0~3.3V
 * @retval      ��
 */
void pwmdac_set_voltage(uint16_t vol)
{
    float temp = vol;
    temp /= 100;                /* ��С100��, �õ�ʵ�ʵ�ѹֵ */
    temp = temp * 256 / 3.3f;   /* ����ѹת����PWMռ�ձ� */
    __HAL_TIM_SET_COMPARE(&g_tim9_handler, PWMDAC_TIMX_CHY, temp);  /* �����µ�ռ�ձ� */
}



