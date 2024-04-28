/**
 ****************************************************************************************************
 * @file        dac.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.2
 * @date        2021-11-03
 * @brief       DAC ��������
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
 * V1.1 20211023
 * ����dac_triangular_wave����
 * V1.2 20211103
 * ����dac_dma_wave_init,dac_dma_wave_enable����
 *
 ****************************************************************************************************
 */

#include "./BSP/DAC/dac.h"
#include "./SYSTEM/delay/delay.h"


DAC_HandleTypeDef g_dac1_handle;       /* DAC��� */

/* ���Ҳ����ͼ */


/**
 * @brief       DAC��ʼ������
 *   @note      ������֧��DAC1_OUT1/2ͨ����ʼ��
 *              DAC������ʱ������APB1, ʱ��Ƶ��=42MHz=23.8ns
 *              DAC�����buffer�رյ�ʱ��, �������ʱ����Сֵ: tSETTLING = 3us(F407�����ֲ���д)
 *              ���DAC���������ٶ�ԼΪ:333KHz, ��10����Ϊһ������, ��������33Khz���ҵĲ���
 *
 * @param       outx: Ҫ��ʼ����ͨ��
 *      @arg    1, ��ʼ��DACͨ��1
 *      @arg    2, ��ʼ��DACͨ��2
 * @retval      ��
 */
void dac_init(uint8_t outx)
{
    __HAL_RCC_DAC_CLK_ENABLE();                                 /* ʹ��DAC1��ʱ�� */
    GPIO_InitTypeDef gpio_init_struct;
    __HAL_RCC_GPIOA_CLK_ENABLE();                               /* ʹ��DAC OUT1/2��IO��ʱ��(����PA��,PA4/PA5) */
    gpio_init_struct.Pin = (outx==1) ? GPIO_PIN_4 : GPIO_PIN_5; /* STM32��Ƭ��, ����PA4=DAC1_OUT1, PA5=DAC1_OUT2 */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG; 
    gpio_init_struct.Pull = GPIO_NOPULL;      
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);

    g_dac1_handle.Instance = DAC;
    HAL_DAC_Init(&g_dac1_handle);                               /* ��ʼ��DAC */

    DAC_ChannelConfTypeDef DACCH1_Config;
    DACCH1_Config.DAC_Trigger = DAC_TRIGGER_NONE;               /* ��ʹ�ô������� */
    DACCH1_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;  /* DAC1�������ر� */
    
    switch(outx)
    {
        case 1:
            HAL_DAC_ConfigChannel(&g_dac1_handle, &DACCH1_Config, DAC_CHANNEL_1);  /* DACͨ��1���� */
            HAL_DAC_Start(&g_dac1_handle, DAC_CHANNEL_1);                          /* ����DACͨ��1 */
            break;
        case 2:
            HAL_DAC_ConfigChannel(&g_dac1_handle, &DACCH1_Config, DAC_CHANNEL_2);  /* DACͨ��2���� */
            HAL_DAC_Start(&g_dac1_handle, DAC_CHANNEL_2);                          /* ����DACͨ��1 */
            break;
        default : break;
    }
}


/**
 * @brief       ����ͨ��1/2�����ѹ
 * @param       outx: 1,ͨ��1; 2,ͨ��2
 * @param       vol : 0~3300,����0~3.3V
 * @retval      ��
 */
void dac_set_voltage(uint8_t outx, uint16_t vol)
{
    double temp = vol;
    temp /= 1000;
    temp = temp * 4096 / 3.3;

    if (temp >= 4096)temp = 4095;   /* ���ֵ���ڵ���4096, ��ȡ4095 */

    if (outx == 1)  /* ͨ��1 */
    {
        HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, temp);    /* 12λ�Ҷ������ݸ�ʽ����DACֵ */
    }
    else            /* ͨ��2 */
    {
        HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_2, DAC_ALIGN_12B_R, temp);    /* 12λ�Ҷ������ݸ�ʽ����DACֵ */
    }
}

/**
 * @brief       ����DAC_OUT1������ǲ�
 *   @note      ���Ƶ�� �� 1000 / (dt * samples) Khz, ������dt��С��ʱ��,����С��5usʱ, ����delay_us
 *              ����Ͳ�׼��(���ú���,����ȶ���Ҫʱ��,��ʱ��С��ʱ��,��Щʱ���Ӱ�쵽��ʱ), Ƶ�ʻ�ƫС.
 *
 * @param       maxval : ���ֵ(0 < maxval < 4096), (maxval + 1)������ڵ���samples/2
 * @param       dt     : ÿ�����������ʱʱ��(��λ : us)
 * @param       samples: ������ĸ���, samples����С�ڵ���(maxval + 1) * 2, ��maxval���ܵ���0
 * @param       n      : ������θ���,0~65535
 * @retval      ��
 */
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n)
{
    uint16_t i, j;
    float incval;   /* ������ */
    float Curval;   /* ��ǰֵ */
    
    if((maxval + 1) <= samples)return ;     /* ���ݲ��Ϸ� */
        
    incval = (maxval + 1) / (samples / 2);  /* ��������� */
    
    for(j = 0; j < n; j++)
    { 
        Curval = 0;
        HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);  /* �����0 */
        for(i = 0; i < (samples / 2); i++)  /* ��������� */
        {
            Curval  +=  incval;             /* �µ����ֵ */
            HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);
            delay_us(dt);
        } 
        for(i = 0; i < (samples / 2); i++)  /* ����½��� */
        {
            Curval  -=  incval;             /* �µ����ֵ */
            HAL_DAC_SetValue(&g_dac1_handle, DAC_CHANNEL_1, DAC_ALIGN_12B_R, Curval);
            delay_us(dt);
        }
    }
}

/***************************************DAC������Ҳ�����*****************************************/

DAC_HandleTypeDef g_dac_dma_handle;             /* DAC��� */
DMA_HandleTypeDef g_dma_dac_handle;             /* ��DAC������DMA��� */

extern uint16_t g_dac_sin_buf[4096];            /* �������ݻ����� */

/**
 * @brief       DAC DMA������γ�ʼ������
 *   @note      ������֧��DAC1_OUT1/2ͨ����ʼ��
 *              DAC������ʱ������APB1, ʱ��Ƶ��=42MHz=23.8ns
 *              DAC�����buffer�رյ�ʱ��, �������ʱ����Сֵ: tSETTLING = 3us(F407�����ֲ���д)
 *              ���DAC���������ٶ�ԼΪ:333KHz, ��10����Ϊһ������, ��������33Khz���ҵĲ���
 *
 * @param       outx    : Ҫ��ʼ����ͨ��. 1,ͨ��1; 2,ͨ��2
 * @retval      ��
 */
void dac_dma_wave_init(uint8_t outx)
{
    DAC_ChannelConfTypeDef dac_ch_conf={0};

    GPIO_InitTypeDef gpio_init_struct;

    __HAL_RCC_GPIOA_CLK_ENABLE();               /* DACͨ�����Ŷ˿�ʱ��ʹ�� */
    __HAL_RCC_DAC_CLK_ENABLE();                 /* DAC����ʱ��ʹ�� */
    __HAL_RCC_DMA1_CLK_ENABLE();                /* DMAʱ��ʹ�� */

    gpio_init_struct.Pin = (outx == 1) ? GPIO_PIN_4 : GPIO_PIN_5;   /* PA4/5 */
    gpio_init_struct.Mode = GPIO_MODE_ANALOG;   /* ģ�� */
    gpio_init_struct.Pull = GPIO_NOPULL;        /* ���������� */
    HAL_GPIO_Init(GPIOA, &gpio_init_struct);    /* ��ʼ��DAC���� */

    g_dac_dma_handle.Instance = DAC1;
    HAL_DAC_Init(&g_dac_dma_handle);            /* DAC��ʼ�� */
    
    g_dma_dac_handle.Instance = (outx == 1) ? DMA1_Stream5 : DMA1_Stream6;  /* ʹ�õ�DAM1 Stream5/6 */
    g_dma_dac_handle.Init.Channel = DMA_CHANNEL_7;
    g_dma_dac_handle.Init.Direction = DMA_MEMORY_TO_PERIPH;                 /* �洢��������ģʽ */
    g_dma_dac_handle.Init.PeriphInc = DMA_PINC_DISABLE;                     /* �����ַ��ֹ���� */
    g_dma_dac_handle.Init.MemInc = DMA_MINC_ENABLE;                         /* �洢����ַ���� */
    g_dma_dac_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* �������ݳ���:16λ */
    g_dma_dac_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       /* �洢�����ݳ���:16λ */
    g_dma_dac_handle.Init.Mode = DMA_CIRCULAR;                              /* ѭ��ģʽ */
    g_dma_dac_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                   /* �е����ȼ� */
    g_dma_dac_handle.Init.FIFOMode = DMA_FIFOMODE_DISABLE;                  /* ��ʹ��FIFO */
    HAL_DMA_Init(&g_dma_dac_handle);                                        /* ��ʼ��DMA */

    __HAL_LINKDMA(&g_dac_dma_handle, DMA_Handle1, g_dma_dac_handle);        /* DMA�����DAC������� */

    dac_ch_conf.DAC_Trigger = DAC_TRIGGER_T7_TRGO;                          /* ���ö�ʱ��7���� */
    dac_ch_conf.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;                 /* ʹ��������� */
    HAL_DAC_ConfigChannel(&g_dac_dma_handle, &dac_ch_conf, DAC_CHANNEL_1);  /* DACͨ��������� */
}

/**
 * @brief       DAC DMAʹ�ܲ������
 *   @note      TIM7������ʱ��Ƶ��(f)����APB1, f = 42 * 2 = 84Mhz.
 *              DAC����Ƶ�� ftrgo = f / ((psc + 1) * (arr + 1))
 *              ����Ƶ�� = ftrgo / ndtr;
 * @param       outx        : DACͨ��1/2
 * @param       ndtr        : DMAͨ�����δ���������
 * @param       arr         : TIM7���Զ���װ��ֵ
 * @param       psc         : TIM7�ķ�Ƶϵ��
 * @retval      ��
 */
void dac_dma_wave_enable(uint8_t outx, uint16_t ndtr, uint16_t arr, uint16_t psc)
{
    TIM_HandleTypeDef tim7_handle = {0};
    TIM_MasterConfigTypeDef master_config = {0};

    __HAL_RCC_TIM7_CLK_ENABLE();                                        /* TIM7ʱ��ʹ�� */

    tim7_handle.Instance = TIM7;                                        /* ѡ��ʱ��7 */
    tim7_handle.Init.Prescaler = psc;                                   /* ��Ƶϵ�� */
    tim7_handle.Init.CounterMode = TIM_COUNTERMODE_UP;                  /* ���ϼ��� */
    tim7_handle.Init.Period = arr;                                      /* ��װ��ֵ */
    tim7_handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE; /* �Զ���װ */
    HAL_TIM_Base_Init(&tim7_handle);                                    /* ��ʼ����ʱ��7 */

    master_config.MasterOutputTrigger = TIM_TRGO_UPDATE;
    master_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
    HAL_TIMEx_MasterConfigSynchronization(&tim7_handle, &master_config);/* ����TIM7 TRGO */
    HAL_TIM_Base_Start(&tim7_handle);                                   /* ʹ�ܶ�ʱ��7 */

    HAL_DAC_Stop_DMA(&g_dac_dma_handle, (outx == 1)  ? DAC_CHANNEL_1 : DAC_CHANNEL_2); /* ��ֹ֮ͣǰ�Ĵ��� */
    HAL_DAC_Start_DMA(&g_dac_dma_handle, (outx == 1) ? DAC_CHANNEL_1 : DAC_CHANNEL_2, (uint32_t *)g_dac_sin_buf, ndtr, DAC_ALIGN_12B_R);
}

