/**
 ****************************************************************************************************
 * @file        adc3.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       ADC3 ��������
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
 * V1.0 20211023
 * ��һ�η���
 ****************************************************************************************************
 */

#include "./BSP/ADC/adc3.h"
#include "./SYSTEM/delay/delay.h"


ADC_HandleTypeDef g_adc3_handle;        /* ADC��� */

/********************************************************************/
/**
 * @brief       ADC3��ʼ������
 *   @note      ������֧��ADC1/ADC2����ͨ��, ���ǲ�֧��ADC3
 *              ����ʹ��12λ����, ADC����ʱ��=21M, ת��ʱ��Ϊ: �������� + 12.5��ADC����
 *              ��������������: 239.5, ��ת��ʱ�� = 252 ��ADC���� = 21us
 * @param       ��
 * @retval      ��
 */
void adc3_init(void)
{
    ADC3_CHY_CLK_ENABLE();  /* ADCʱ��ʹ�� */

    g_adc3_handle.Instance = ADC_ADCX;
    g_adc3_handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;   /* 4��Ƶ��ADCCLK=PCLK2/4=84/4=21MHz */
    g_adc3_handle.Init.Resolution = ADC_RESOLUTION12b;          /* 12λģʽ */
    g_adc3_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;         /* �Ҷ��� */
    g_adc3_handle.Init.ScanConvMode = DISABLE;                  /* ��ɨ��ģʽ */
    g_adc3_handle.Init.EOCSelection = DISABLE;                  /* �ر�EOC�ж� */

    g_adc3_handle.Init.ContinuousConvMode = DISABLE;            /* �ر�����ת�� */
    g_adc3_handle.Init.NbrOfConversion = 1;                     /* 1��ת���ڹ��������� Ҳ����ֻת����������1 */
    g_adc3_handle.Init.DiscontinuousConvMode = DISABLE;         /* ��ֹ����������ģʽ */
    g_adc3_handle.Init.NbrOfDiscConversion = 0;                 /* ����������ͨ����Ϊ0 */
    g_adc3_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;   /* ������� */
    g_adc3_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;    /* ʹ��������� */
    HAL_ADC_Init(&g_adc3_handle);   /* ��ʼ�� */
}

/**
 * @brief       ����ADCͨ������ʱ��
 * @param       adcx : adc���ָ��,ADC_HandleTypeDef
 * @param       ch   : ͨ����, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       stime: ����ʱ��  0~7, ��Ӧ��ϵΪ:
 *   @arg       ADC_SAMPLETIME_3CYCLES,  3��ADCʱ������        ADC_SAMPLETIME_15CYCLES, 15��ADCʱ������
 *   @arg       ADC_SAMPLETIME_28CYCLES, 28��ADCʱ������       ADC_SAMPLETIME_56CYCLES, 56��ADCʱ������
 *   @arg       ADC_SAMPLETIME_84CYCLES, 84��ADCʱ������       ADC_SAMPLETIME_112CYCLES,112��ADCʱ������
 *   @arg       ADC_SAMPLETIME_144CYCLES,144��ADCʱ������      ADC_SAMPLETIME_480CYCLES,480��ADCʱ������
 * @param       rank: ��ͨ���ɼ�ʱ��Ҫ���õĲɼ����,
 *              �����㶨��channel1��rank=1��channel2 ��rank=2��
 *              ��ô��Ӧ����DMA����ռ�ı�������AdcDMA[0] ��i��channel1��ת�������AdcDMA[1]����ͨ��2��ת������� 
 *              ��ͨ��DMA����Ϊ ADC_REGULAR_RANK_1
 *   @arg       ���1~16��ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      ��
 */
void adc3_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch,uint32_t rank, uint32_t stime)
{
    /* ���ö�ӦADCͨ�� */
    ADC_ChannelConfTypeDef adc_channel;
    adc_channel.Channel = ch;
    adc_channel.Rank = rank;
    adc_channel.SamplingTime = stime;
    HAL_ADC_ConfigChannel( adc_handle, &adc_channel);    /* ����ADCX��ͨ������ʱ�� */
}

/**
 * @brief       ���ADCת����Ľ��
 * @param       ch: ͨ��ֵ 0~17��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_17
 * @retval      ��
 */
uint32_t adc3_get_result(uint32_t ch)
{
    adc3_channel_set(&g_adc3_handle , ch, 1, ADC_SAMPLETIME_480CYCLES);    /* ����ͨ�������кͲ���ʱ�� */
    HAL_ADC_Start(&g_adc3_handle);                            /* ����ADC */
    HAL_ADC_PollForConversion(&g_adc3_handle, 10);            /* ��ѯת�� */

    return (uint16_t)HAL_ADC_GetValue(&g_adc3_handle);        /* �������һ��ADC1�������ת����� */
}

/**
 * @brief       ��ȡͨ��ch��ת��ֵ,ȡtimes��,Ȼ��ƽ��
 * @param       ch      : ͨ����, 0~17
 * @param       times   : ��ȡ����
 * @retval      ͨ��ch��times��ת�����ƽ��ֵ
 */
uint32_t adc3_get_result_average(uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)     /* ��ȡtimes������ */
    {
        temp_val += adc3_get_result(ch);
        delay_ms(5);
    }

    return temp_val / times;        /* ����ƽ��ֵ */
}






