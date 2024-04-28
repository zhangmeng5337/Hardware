/**
 ****************************************************************************************************
 * @file        adc.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2021-10-18
 * @brief       ADC ��������
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
 * V1.0 20211018
 * ��һ�η���
 * V1.1 20211018
 * 1,֧��ADC��ͨ��DMA�ɼ� 
 * 2,����adc_dma_init��adc_dma_enable����
 *
 ****************************************************************************************************
 */

#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"


ADC_HandleTypeDef g_adc_handle;   /* ADC��� */

/* ��ͨ��ADC�ɼ� DMA��ȡ */
DMA_HandleTypeDef g_dma_adc_handle;     /* ��ADC������DMA��� */
uint8_t g_adc_dma_sta = 0;              /* DMA����״̬��־, 0,δ���; 1, ����� */

/**
 * @brief       ADC��ʼ������
 *   @note      ������֧��ADC1/ADC2����ͨ��, ���ǲ�֧��ADC3
 *              ����ʹ��12λ����, ADC����ʱ��=21M, ת��ʱ��Ϊ: �������� + 12��ADC����
 *              ��������������: 480, ��ת��ʱ�� = 492 ��ADC���� = 23.42us
 * @param       ��
 * @retval      ��
 */
void adc_init(void)
{
    g_adc_handle.Instance = ADC_ADCX;
    g_adc_handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;        /* 4��Ƶ��ADCCLK = PCLK2/4 = 84/4 = 21Mhz */
    g_adc_handle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 12λģʽ */
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* �Ҷ��� */
    g_adc_handle.Init.ScanConvMode = DISABLE;                               /* ��ɨ��ģʽ */
    g_adc_handle.Init.ContinuousConvMode = ENABLE;                          /* ��������ת�� */
    g_adc_handle.Init.NbrOfConversion = 1;                                  /* 1��ת���ڹ��������� Ҳ����ֻת����������1 */
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;                      /* ��ֹ����������ģʽ */
    g_adc_handle.Init.NbrOfDiscConversion = 0;                              /* ����������ͨ����Ϊ0 */
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* ������� */
    g_adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* ʹ��������� */
    g_adc_handle.Init.DMAContinuousRequests = ENABLE;                       /* ����DMA���� */
    HAL_ADC_Init(&g_adc_handle);                                            /* ��ʼ�� */
}

/**
 * @brief       ADC�ײ��������������ã�ʱ��ʹ��
 *   @note      �˺����ᱻHAL_ADC_Init()����
 * @param       hadc:ADC���
 * @retval      ��
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC_ADCX)
    {
        GPIO_InitTypeDef gpio_init_struct;
        ADC_ADCX_CHY_CLK_ENABLE();      /* ʹ��ADCxʱ�� */
        ADC_ADCX_CHY_GPIO_CLK_ENABLE(); /* ����GPIOʱ�� */

        /* AD�ɼ�����ģʽ����, ģ������ */
        gpio_init_struct.Pin = ADC_ADCX_CHY_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        gpio_init_struct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT, &gpio_init_struct);
    }
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
                �����㶨��channel1��rank=1��channel2 ��rank=2��
                ��ô��Ӧ����DMA����ռ�ı�������AdcDMA[0] ��i��channel1��ת�������AdcDMA[1]����ͨ��2��ת������� 
                ��ͨ��DMA����Ϊ ADC_REGULAR_RANK_1
 *   @arg       ���1~16��ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      ��
 */
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    /* ���ö�ӦADCͨ�� */
    ADC_ChannelConfTypeDef adc_channel;
    adc_channel.Channel = ch;               /* ����ADCX��ͨ��ch */
    adc_channel.Rank = rank;                /* ���ò������� */
    adc_channel.SamplingTime = stime;       /* ���ò���ʱ�� */
    HAL_ADC_ConfigChannel(adc_handle, &adc_channel);
}

/**
 * @brief       ���ADCת����Ľ��
 * @param       ch: ͨ��ֵ 0~17��ȡֵ��ΧΪ��ADC_CHANNEL_0~ADC_CHANNEL_17
 * @retval      ��
 */
uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&g_adc_handle, ch, 1, ADC_SAMPLETIME_480CYCLES);   /* ����ͨ�������кͲ���ʱ�� */
    HAL_ADC_Start(&g_adc_handle);                                       /* ����ADC */
    HAL_ADC_PollForConversion(&g_adc_handle, 10);                       /* ��ѯת�� */

    return (uint16_t)HAL_ADC_GetValue(&g_adc_handle);                   /* �������һ��ADC1�������ת����� */
}

/**
 * @brief       ��ȡͨ��ch��ת��ֵ��ȡtimes��, Ȼ��ƽ��
 * @param       ch      : ͨ����, 0~17
 * @param       times   : ��ȡ����
 * @retval      ͨ��ch��times��ת�����ƽ��ֵ
 */
uint32_t adc_get_result_average(uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)     /* ��ȡtimes������ */
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }

    return temp_val / times;        /* ����ƽ��ֵ */
}

/***************************************��ͨ��ADC�ɼ�(DMA��ȡ)����*****************************************/

/**
 * @brief       ADC DMA��ȡ ��ʼ������
 *   @note      ����������ʹ��adc_init��ADC���д󲿷�����,�в���ĵط��ٵ�������
 * @param       par         : �����ַ
 * @param       mar         : �洢����ַ
 * @retval      ��
 */
void adc_dma_init(uint32_t mar)
{
    if ((uint32_t)ADC_ADCX_DMASx > (uint32_t)DMA2)     /* ����DMA1_Stream7, ��ΪDMA2 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                    /* DMA2ʱ��ʹ�� */
    }
    else 
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                    /* DMA1ʱ��ʹ�� */
    }

    /* DMA���� */
    g_dma_adc_handle.Instance = ADC_ADCX_DMASx;                             /* ����DMA������ */
    g_dma_adc_handle.Init.Channel = DMA_CHANNEL_0;                          /* ����DMAͨ�� */
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;                 /* DIR = 1 ,  ���赽�洢��ģʽ */
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                     /* ���������ģʽ */
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                         /* �洢������ģʽ */
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* �������ݳ���:16λ */
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       /* �洢�����ݳ���:16λ */
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;                                /* ��������ģʽ */
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                   /* �е����ȼ� */
    HAL_DMA_Init(&g_dma_adc_handle);                                        /* ��ʼ��DMA */
    HAL_DMA_Start(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0);          /* ����DMA������� */

    g_adc_handle.DMA_Handle = &g_dma_adc_handle;    /* ����ADC��Ӧ��DMA */
    adc_init();     /* ��ʼ��ADC */
    /**
     *   ��Ҫ�����õ�ʱ�򿪣�������Ϊ�˱�֤�����֮ǰ�Ĵ��룬
     *   ���һ������g_adc_handle.Init.ContinuousConvMode = ENABLE;
     *   ����ADC����ת��, DMA����ADC����
     */
    SET_BIT(g_adc_handle.Instance->CR2, ADC_CR2_CONT);          /* CONT = 1, ����ת��ģʽ */

    /* ���ö�ӦADCͨ�� */
    adc_channel_set(&g_adc_handle , ADC_ADCX_CHY, 1, ADC_SAMPLETIME_480CYCLES);
    
    HAL_NVIC_SetPriority(ADC_ADCX_DMASx_IRQn, 3, 3);            /* ����DMA�ж����ȼ�Ϊ3�������ȼ�Ϊ3 */
    HAL_NVIC_EnableIRQ(ADC_ADCX_DMASx_IRQn);                    /* ʹ��DMA�ж� */
    HAL_ADC_Start_DMA(&g_adc_handle, &mar, sizeof(uint16_t));   /* ��ʼDMA���ݴ��� */
    __HAL_DMA_ENABLE_IT(&g_dma_adc_handle, DMA_IT_TC);          /* TCIE =1 , ʹ�ܴ�������ж� */
}

/**
 * @brief       ʹ��һ��ADC DMA���� 
 * @param       ndtr: DMA����Ĵ���
 * @retval      ��
 */
void adc_dma_enable(uint16_t ndtr)
{
    __HAL_ADC_DISABLE(&g_adc_handle);           /* �ȹر�ADC */
    
    __HAL_DMA_DISABLE(&g_dma_adc_handle);       /* �ر�DMA���� */
    g_dma_adc_handle.Instance->NDTR = ndtr;     /* ����DMA���������� */
    __HAL_DMA_ENABLE(&g_dma_adc_handle);        /* ����DMA���� */
    
    __HAL_ADC_ENABLE(&g_adc_handle);            /* ��������ADC */
    ADC_ADCX->CR2 |= 1 << 30;                   /* ��������ת��ͨ�� */
}

/**
 * @brief       ADC DMA�ɼ��жϷ�����
 * @param       ��
 * @retval      ��
 */
void ADC_ADCX_DMASx_IRQHandler(void)
{
    if (ADC_ADCX_DMASx_IS_TC())     /* �ж�DMA���ݴ������ */
    {
        g_adc_dma_sta = 1;          /* ���DMA������� */
        ADC_ADCX_DMASx_CLR_TC();    /* ���DMA2 ������4 ��������ж� */
    }
}







