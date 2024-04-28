/**
 ****************************************************************************************************
 * @file        atim.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2021-10-19
 * @brief       �߼���ʱ�� ��������
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
 * V1.0 20211019
 * ��һ�η���
 * V1.1 20211019
 * 1, ����atim_timx_comp_pwm_init����, ʵ������Ƚ�ģʽPWM�������
 *
 ****************************************************************************************************
 */

#include "./BSP/TIMER/atim.h"
#include "./BSP/LED/led.h"


TIM_HandleTypeDef g_timx_npwm_chy_handle;     /* ��ʱ��x��� */

/* g_npwm_remain��ʾ��ǰ��ʣ�¶��ٸ�����Ҫ����
 * ÿ����෢��256������
 */
static uint32_t g_npwm_remain = 0;

/**
 * @brief       �߼���ʱ��TIMX ͨ��Y ���ָ������PWM ��ʼ������
 * @note
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 168Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 168Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ
 * @param       psc: Ԥ��Ƶϵ��
 * @retval      ��
 */
void atim_timx_npwm_chy_init(uint16_t arr, uint16_t psc)
{
    GPIO_InitTypeDef gpio_init_struct;
    TIM_OC_InitTypeDef timx_oc_npwm_chy = {0};  /* ��ʱ����� */
    ATIM_TIMX_NPWM_CHY_GPIO_CLK_ENABLE();       /* TIMX ͨ��IO��ʱ��ʹ�� */
    ATIM_TIMX_NPWM_CHY_CLK_ENABLE();            /* TIMX ʱ��ʹ�� */

    g_timx_npwm_chy_handle.Instance = ATIM_TIMX_NPWM;                   /* ��ʱ��x */
    g_timx_npwm_chy_handle.Init.Prescaler = psc;                        /* ��ʱ����Ƶ */
    g_timx_npwm_chy_handle.Init.CounterMode = TIM_COUNTERMODE_UP;       /* ��������ģʽ */
    g_timx_npwm_chy_handle.Init.Period = arr;                           /* �Զ���װ��ֵ */
    g_timx_npwm_chy_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* ʹ��TIMx_ARR���л��� */
    g_timx_npwm_chy_handle.Init.RepetitionCounter = 0;                  /* �ظ���������ʼֵ */
    HAL_TIM_PWM_Init(&g_timx_npwm_chy_handle);                          /* ��ʼ��PWM */

    gpio_init_struct.Pin = ATIM_TIMX_NPWM_CHY_GPIO_PIN;                 /* ͨ��y��GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                            /* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                                /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;                      /* ���� */
    gpio_init_struct.Alternate = ATIM_TIMX_NPWM_CHY_GPIO_AF;            /* �˿ڸ��� */
    HAL_GPIO_Init(ATIM_TIMX_NPWM_CHY_GPIO_PORT, &gpio_init_struct);

    timx_oc_npwm_chy.OCMode = TIM_OCMODE_PWM1;                          /* ģʽѡ��PWM 1 */
    timx_oc_npwm_chy.Pulse = arr / 2;                                   /* ���ñȽ�ֵ,��ֵ����ȷ��ռ�ձ� */
                                                                        /* ����Ĭ�����ñȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50% */
    timx_oc_npwm_chy.OCPolarity = TIM_OCPOLARITY_HIGH;                  /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_PWM_ConfigChannel(&g_timx_npwm_chy_handle, &timx_oc_npwm_chy, ATIM_TIMX_NPWM_CHY); /* ����TIMxͨ��y */

    HAL_NVIC_SetPriority(ATIM_TIMX_NPWM_IRQn, 1, 3);                    /* �����ж����ȼ�����ռ���ȼ�1�������ȼ�3 */
    HAL_NVIC_EnableIRQ(ATIM_TIMX_NPWM_IRQn);                            /* ����ITMx�ж� */

    __HAL_TIM_ENABLE_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);        /* ��������ж� */
    HAL_TIM_PWM_Start(&g_timx_npwm_chy_handle, ATIM_TIMX_NPWM_CHY);     /* ������ӦPWMͨ�� */
}

/**
 * @brief       �߼���ʱ��TIMX NPWM����PWM����
 * @param       rcr: PWM�ĸ���, 1~2^32�η���
 * @retval      ��
 */
void atim_timx_npwm_chy_set(uint32_t npwm)
{
    if (npwm == 0) return;

    g_npwm_remain = npwm;                                                   /* ����������� */
    HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE); /* ����һ�θ����¼�,���ж����洦��������� */
    __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);                              /* ʹ�ܶ�ʱ��TIMX */
}

/**
 * @brief       �߼���ʱ��TIMX NPWM�жϷ�����
 * @param       ��
 * @retval      ��
 */
void ATIM_TIMX_NPWM_IRQHandler(void)
{
    uint16_t npwm = 0;

    /* ���´���û��ʹ�ö�ʱ��HAL�⹲�ô���������������ֱ��ͨ���ж��жϱ�־λ�ķ�ʽ */
    if (__HAL_TIM_GET_FLAG(&g_timx_npwm_chy_handle, TIM_FLAG_UPDATE) != RESET)
    {
        if (g_npwm_remain >= 256)           /* ���д���256��������Ҫ���� */
        {
            g_npwm_remain = g_npwm_remain - 256;
            npwm = 256;
        }
        else if (g_npwm_remain % 256)       /* ����λ��������256��������Ҫ���� */
        {
            npwm = g_npwm_remain % 256;
            g_npwm_remain = 0;              /* û�������� */
        }

        if (npwm) /* ������Ҫ���� */
        {
            ATIM_TIMX_NPWM->RCR = npwm - 1;                                         /* �����ظ������Ĵ���ֵΪnpwm-1, ��npwm������ */
            HAL_TIM_GenerateEvent(&g_timx_npwm_chy_handle, TIM_EVENTSOURCE_UPDATE); /* ����һ�θ����¼�,���ж����洦��������� */
            __HAL_TIM_ENABLE(&g_timx_npwm_chy_handle);                              /* ʹ�ܶ�ʱ��TIMX */
        }
        else
        {
            ATIM_TIMX_NPWM->CR1 &= ~(1 << 0); /* �رն�ʱ��TIMX��ʹ��HAL Disable�����PWMͨ����Ϣ���˴����� */
        }

        __HAL_TIM_CLEAR_IT(&g_timx_npwm_chy_handle, TIM_IT_UPDATE);  /* �����ʱ������жϱ�־λ */
    }
}

/*******************************�����Ǹ߼���ʱ������Ƚ�ģʽʵ�����**************************************/

TIM_HandleTypeDef g_timx_comp_pwm_handle;       /* ��ʱ��x��� */

/**
 * @brief       �߼���ʱ��TIMX ����Ƚ�ģʽ ��ʼ��������ʹ������Ƚ�ģʽ��
 * @note
 *              ���ø߼���ʱ��TIMX 4·����Ƚ�ģʽPWM���,ʵ��50%ռ�ձ�,��ͬ��λ����
 *              ע��,����������Ƚ�ģʽ,ÿ2���������ڲ������һ��PWM���,������Ƶ�ʼ���
 *              ����,���ǻ����Կ����ж����ж������޸�CCRx,�Ӷ�ʵ�ֲ�ͬƵ��/��ͬ��λ�Ŀ���
 *              �������ǲ��Ƽ���ôʹ��,��Ϊ����ܵ��·ǳ�Ƶ�����ж�,�Ӷ�ռ�ô���CPU��Դ
 *
 *              �߼���ʱ����ʱ������APB2, ��PCLK2 = 168Mhz, ��������PPRE2����Ƶ, ���
 *              �߼���ʱ��ʱ�� = 168Mhz
 *              ��ʱ�����ʱ����㷽��: Tout = ((arr + 1) * (psc + 1)) / Ft us.
 *              Ft=��ʱ������Ƶ��,��λ:Mhz
 *
 * @param       arr: �Զ���װֵ��
 * @param       psc: Ԥ��Ƶϵ��
 * @retval      ��
 */
void atim_timx_comp_pwm_init(uint16_t arr, uint16_t psc)
{
    TIM_OC_InitTypeDef timx_oc_comp_pwm = {0};

    g_timx_comp_pwm_handle.Instance = ATIM_TIMX_COMP;                  /* ��ʱ��8 */
    g_timx_comp_pwm_handle.Init.Prescaler = psc;                       /* Ԥ��Ƶϵ�� */
    g_timx_comp_pwm_handle.Init.CounterMode = TIM_COUNTERMODE_UP;      /* ���ϼ���ģʽ */
    g_timx_comp_pwm_handle.Init.Period = arr;                          /* �Զ���װ��ֵ */
    g_timx_comp_pwm_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* ��ʹ��Ӱ�ӼĴ���TIMx_ARR */
    HAL_TIM_OC_Init(&g_timx_comp_pwm_handle);    /* ����Ƚ�ģʽ��ʼ�� */

    timx_oc_comp_pwm.OCMode = TIM_OCMODE_TOGGLE;                        /* �Ƚ����ģʽ��ת���� */
    timx_oc_comp_pwm.Pulse = 250 - 1;                                   /* ��������ȽϼĴ�����ֵ */
    timx_oc_comp_pwm.OCPolarity = TIM_OCPOLARITY_HIGH;                  /* ����Ƚϼ���Ϊ�� */
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_1); /* ��ʼ����ʱ��������Ƚ�ͨ��1 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);                 /* ͨ��1 Ԥװ��ʹ�� */

    timx_oc_comp_pwm.Pulse = 500 - 1;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_2); /* ��ʼ����ʱ��������Ƚ�ͨ��2 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);                 /* ͨ��2 Ԥװ��ʹ�� */

    timx_oc_comp_pwm.Pulse = 750 - 1;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm, TIM_CHANNEL_3); /* ��ʼ����ʱ��������Ƚ�ͨ��3 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);                 /* ͨ��3 Ԥװ��ʹ�� */

    timx_oc_comp_pwm.Pulse = 1000 - 1;
    timx_oc_comp_pwm.OCIdleState = TIM_OCIDLESTATE_RESET;
    HAL_TIM_OC_ConfigChannel(&g_timx_comp_pwm_handle, &timx_oc_comp_pwm,  TIM_CHANNEL_4); /* ��ʼ����ʱ��������Ƚ�ͨ��4 */
    __HAL_TIM_ENABLE_OCxPRELOAD(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);                  /* ͨ��4 Ԥװ��ʹ�� */

    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_1);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_2);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_3);
    HAL_TIM_OC_Start(&g_timx_comp_pwm_handle, TIM_CHANNEL_4);
}

/**
 * @brief       ��ʱ���ײ�������ʱ��ʹ�ܣ���������
                �˺����ᱻHAL_TIM_OC_Init()����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == ATIM_TIMX_COMP)
    {
        GPIO_InitTypeDef gpio_init_struct;

        ATIM_TIMX_COMP_CLK_ENABLE();            /* ʹ�ܶ�ʱ��ʱ�� */

        ATIM_TIMX_COMP_CH1_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH2_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH3_GPIO_CLK_ENABLE();
        ATIM_TIMX_COMP_CH4_GPIO_CLK_ENABLE();

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH1_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;
        gpio_init_struct.Pull = GPIO_NOPULL;
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
        gpio_init_struct.Alternate = ATIM_TIMX_COMP_GPIO_AF;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH1_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH2_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH2_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH3_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH3_GPIO_PORT, &gpio_init_struct);

        gpio_init_struct.Pin = ATIM_TIMX_COMP_CH4_GPIO_PIN;
        HAL_GPIO_Init(ATIM_TIMX_COMP_CH4_GPIO_PORT, &gpio_init_struct);
    }
}

