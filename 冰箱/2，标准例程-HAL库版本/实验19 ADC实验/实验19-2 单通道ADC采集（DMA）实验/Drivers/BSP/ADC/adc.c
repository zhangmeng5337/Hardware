/**
 ****************************************************************************************************
 * @file        adc.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2021-10-18
 * @brief       ADC 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211018
 * 第一次发布
 * V1.1 20211018
 * 1,支持ADC单通道DMA采集 
 * 2,新增adc_dma_init和adc_dma_enable函数
 *
 ****************************************************************************************************
 */

#include "./BSP/ADC/adc.h"
#include "./SYSTEM/delay/delay.h"


ADC_HandleTypeDef g_adc_handle;   /* ADC句柄 */

/* 单通道ADC采集 DMA读取 */
DMA_HandleTypeDef g_dma_adc_handle;     /* 与ADC关联的DMA句柄 */
uint8_t g_adc_dma_sta = 0;              /* DMA传输状态标志, 0,未完成; 1, 已完成 */

/**
 * @brief       ADC初始化函数
 *   @note      本函数支持ADC1/ADC2任意通道, 但是不支持ADC3
 *              我们使用12位精度, ADC采样时钟=21M, 转换时间为: 采样周期 + 12个ADC周期
 *              设置最大采样周期: 480, 则转换时间 = 492 个ADC周期 = 23.42us
 * @param       无
 * @retval      无
 */
void adc_init(void)
{
    g_adc_handle.Instance = ADC_ADCX;
    g_adc_handle.Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV4;        /* 4分频，ADCCLK = PCLK2/4 = 84/4 = 21Mhz */
    g_adc_handle.Init.Resolution = ADC_RESOLUTION_12B;                      /* 12位模式 */
    g_adc_handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;                      /* 右对齐 */
    g_adc_handle.Init.ScanConvMode = DISABLE;                               /* 非扫描模式 */
    g_adc_handle.Init.ContinuousConvMode = ENABLE;                          /* 开启连续转换 */
    g_adc_handle.Init.NbrOfConversion = 1;                                  /* 1个转换在规则序列中 也就是只转换规则序列1 */
    g_adc_handle.Init.DiscontinuousConvMode = DISABLE;                      /* 禁止不连续采样模式 */
    g_adc_handle.Init.NbrOfDiscConversion = 0;                              /* 不连续采样通道数为0 */
    g_adc_handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;                /* 软件触发 */
    g_adc_handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE; /* 使用软件触发 */
    g_adc_handle.Init.DMAContinuousRequests = ENABLE;                       /* 开启DMA请求 */
    HAL_ADC_Init(&g_adc_handle);                                            /* 初始化 */
}

/**
 * @brief       ADC底层驱动，引脚配置，时钟使能
 *   @note      此函数会被HAL_ADC_Init()调用
 * @param       hadc:ADC句柄
 * @retval      无
 */
void HAL_ADC_MspInit(ADC_HandleTypeDef *hadc)
{
    if(hadc->Instance == ADC_ADCX)
    {
        GPIO_InitTypeDef gpio_init_struct;
        ADC_ADCX_CHY_CLK_ENABLE();      /* 使能ADCx时钟 */
        ADC_ADCX_CHY_GPIO_CLK_ENABLE(); /* 开启GPIO时钟 */

        /* AD采集引脚模式设置, 模拟输入 */
        gpio_init_struct.Pin = ADC_ADCX_CHY_GPIO_PIN;
        gpio_init_struct.Mode = GPIO_MODE_ANALOG;
        gpio_init_struct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(ADC_ADCX_CHY_GPIO_PORT, &gpio_init_struct);
    }
}

/**
 * @brief       设置ADC通道采样时间
 * @param       adcx : adc句柄指针,ADC_HandleTypeDef
 * @param       ch   : 通道号, ADC_CHANNEL_0~ADC_CHANNEL_17
 * @param       stime: 采样时间  0~7, 对应关系为:
 *   @arg       ADC_SAMPLETIME_3CYCLES,  3个ADC时钟周期        ADC_SAMPLETIME_15CYCLES, 15个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_28CYCLES, 28个ADC时钟周期       ADC_SAMPLETIME_56CYCLES, 56个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_84CYCLES, 84个ADC时钟周期       ADC_SAMPLETIME_112CYCLES,112个ADC时钟周期
 *   @arg       ADC_SAMPLETIME_144CYCLES,144个ADC时钟周期      ADC_SAMPLETIME_480CYCLES,480个ADC时钟周期
 * @param       rank: 多通道采集时需要设置的采集编号,
                假设你定义channel1的rank=1，channel2 的rank=2，
                那么对应你在DMA缓存空间的变量数组AdcDMA[0] 就i是channel1的转换结果，AdcDMA[1]就是通道2的转换结果。 
                单通道DMA设置为 ADC_REGULAR_RANK_1
 *   @arg       编号1~16：ADC_REGULAR_RANK_1~ADC_REGULAR_RANK_16
 * @retval      无
 */
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch, uint32_t rank, uint32_t stime)
{
    /* 配置对应ADC通道 */
    ADC_ChannelConfTypeDef adc_channel;
    adc_channel.Channel = ch;               /* 设置ADCX对通道ch */
    adc_channel.Rank = rank;                /* 设置采样序列 */
    adc_channel.SamplingTime = stime;       /* 设置采样时间 */
    HAL_ADC_ConfigChannel(adc_handle, &adc_channel);
}

/**
 * @brief       获得ADC转换后的结果
 * @param       ch: 通道值 0~17，取值范围为：ADC_CHANNEL_0~ADC_CHANNEL_17
 * @retval      无
 */
uint32_t adc_get_result(uint32_t ch)
{
    adc_channel_set(&g_adc_handle, ch, 1, ADC_SAMPLETIME_480CYCLES);   /* 设置通道，序列和采样时间 */
    HAL_ADC_Start(&g_adc_handle);                                       /* 开启ADC */
    HAL_ADC_PollForConversion(&g_adc_handle, 10);                       /* 轮询转换 */

    return (uint16_t)HAL_ADC_GetValue(&g_adc_handle);                   /* 返回最近一次ADC1规则组的转换结果 */
}

/**
 * @brief       获取通道ch的转换值，取times次, 然后平均
 * @param       ch      : 通道号, 0~17
 * @param       times   : 获取次数
 * @retval      通道ch的times次转换结果平均值
 */
uint32_t adc_get_result_average(uint32_t ch, uint8_t times)
{
    uint32_t temp_val = 0;
    uint8_t t;

    for (t = 0; t < times; t++)     /* 获取times次数据 */
    {
        temp_val += adc_get_result(ch);
        delay_ms(5);
    }

    return temp_val / times;        /* 返回平均值 */
}

/***************************************单通道ADC采集(DMA读取)程序*****************************************/

/**
 * @brief       ADC DMA读取 初始化函数
 *   @note      本函数还是使用adc_init对ADC进行大部分配置,有差异的地方再单独配置
 * @param       par         : 外设地址
 * @param       mar         : 存储器地址
 * @retval      无
 */
void adc_dma_init(uint32_t mar)
{
    if ((uint32_t)ADC_ADCX_DMASx > (uint32_t)DMA2)     /* 大于DMA1_Stream7, 则为DMA2 */
    {
        __HAL_RCC_DMA2_CLK_ENABLE();                    /* DMA2时钟使能 */
    }
    else 
    {
        __HAL_RCC_DMA1_CLK_ENABLE();                    /* DMA1时钟使能 */
    }

    /* DMA配置 */
    g_dma_adc_handle.Instance = ADC_ADCX_DMASx;                             /* 设置DMA数据流 */
    g_dma_adc_handle.Init.Channel = DMA_CHANNEL_0;                          /* 设置DMA通道 */
    g_dma_adc_handle.Init.Direction = DMA_PERIPH_TO_MEMORY;                 /* DIR = 1 ,  外设到存储器模式 */
    g_dma_adc_handle.Init.PeriphInc = DMA_PINC_DISABLE;                     /* 外设非增量模式 */
    g_dma_adc_handle.Init.MemInc = DMA_MINC_ENABLE;                         /* 存储器增量模式 */
    g_dma_adc_handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;    /* 外设数据长度:16位 */
    g_dma_adc_handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;       /* 存储器数据长度:16位 */
    g_dma_adc_handle.Init.Mode = DMA_NORMAL;                                /* 外设流控模式 */
    g_dma_adc_handle.Init.Priority = DMA_PRIORITY_MEDIUM;                   /* 中等优先级 */
    HAL_DMA_Init(&g_dma_adc_handle);                                        /* 初始化DMA */
    HAL_DMA_Start(&g_dma_adc_handle, (uint32_t)&ADC1->DR, mar, 0);          /* 配置DMA传输参数 */

    g_adc_handle.DMA_Handle = &g_dma_adc_handle;    /* 设置ADC对应的DMA */
    adc_init();     /* 初始化ADC */
    /**
     *   需要在配置的时候开，但这里为了保证不变更之前的代码，
     *   另加一行设置g_adc_handle.Init.ContinuousConvMode = ENABLE;
     *   配置ADC连续转换, DMA传输ADC数据
     */
    SET_BIT(g_adc_handle.Instance->CR2, ADC_CR2_CONT);          /* CONT = 1, 连续转换模式 */

    /* 配置对应ADC通道 */
    adc_channel_set(&g_adc_handle , ADC_ADCX_CHY, 1, ADC_SAMPLETIME_480CYCLES);
    
    HAL_NVIC_SetPriority(ADC_ADCX_DMASx_IRQn, 3, 3);            /* 设置DMA中断优先级为3，子优先级为3 */
    HAL_NVIC_EnableIRQ(ADC_ADCX_DMASx_IRQn);                    /* 使能DMA中断 */
    HAL_ADC_Start_DMA(&g_adc_handle, &mar, sizeof(uint16_t));   /* 开始DMA数据传输 */
    __HAL_DMA_ENABLE_IT(&g_dma_adc_handle, DMA_IT_TC);          /* TCIE =1 , 使能传输完成中断 */
}

/**
 * @brief       使能一次ADC DMA传输 
 * @param       ndtr: DMA传输的次数
 * @retval      无
 */
void adc_dma_enable(uint16_t ndtr)
{
    __HAL_ADC_DISABLE(&g_adc_handle);           /* 先关闭ADC */
    
    __HAL_DMA_DISABLE(&g_dma_adc_handle);       /* 关闭DMA传输 */
    g_dma_adc_handle.Instance->NDTR = ndtr;     /* 重设DMA传输数据量 */
    __HAL_DMA_ENABLE(&g_dma_adc_handle);        /* 开启DMA传输 */
    
    __HAL_ADC_ENABLE(&g_adc_handle);            /* 重新启动ADC */
    ADC_ADCX->CR2 |= 1 << 30;                   /* 启动规则转换通道 */
}

/**
 * @brief       ADC DMA采集中断服务函数
 * @param       无
 * @retval      无
 */
void ADC_ADCX_DMASx_IRQHandler(void)
{
    if (ADC_ADCX_DMASx_IS_TC())     /* 判断DMA数据传输完成 */
    {
        g_adc_dma_sta = 1;          /* 标记DMA传输完成 */
        ADC_ADCX_DMASx_CLR_TC();    /* 清除DMA2 数据流4 传输完成中断 */
    }
}







