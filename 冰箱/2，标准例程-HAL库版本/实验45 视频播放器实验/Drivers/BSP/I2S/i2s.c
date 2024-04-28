/**
 ****************************************************************************************************
 * @file        i2s.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       I2S ��������
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
 * V1.0 20211116
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#include "./BSP/I2S/i2s.h"  
#include "./SYSTEM/delay/delay.h"


I2S_HandleTypeDef g_i2s_handler;         /* I2S��� */
DMA_HandleTypeDef g_i2s_txdma_handler;   /* I2S����DMA��� */

/**
 * @brief       I2S��ʼ��
 * @param       i2s_standard        : I2S��׼
 *   @note      ��������            : I2S_STANDARD_PHILIPS/I2S_STANDARD_MSB/
 *                                    I2S_STANDARD_LSB/I2S_STANDARD_PCM_SHORT/I2S_STANDARD_PCM_LONG
 * @param       i2s_mode            : I2S����ģʽ
 *   @note      ��������            : I2S_MODE_SLAVE_TX/I2S_MODE_SLAVE_RX/I2S_MODE_MASTER_TX/I2S_MODE_MASTER_RX
 * @param       i2s_clock_polarity  : ��ʾ���ֵ�λ��
 * @param       i2s_dataformat      : ���ݳ���
 *   @note      ��������            : I2S_DATAFORMAT_16B/I2S_DATAFORMAT_16B_EXTENDED/I2S_DATAFORMAT_24B/I2S_DATAFORMAT_32B
 * @retval      ��
 */
void i2s_init(uint32_t i2s_standard, uint32_t i2s_mode, uint32_t i2s_clock_polarity, uint32_t i2s_dataformat)
{
    g_i2s_handler.Instance = I2S_SPI;
    g_i2s_handler.Init.Mode = i2s_mode;                     /* IISģʽ */
    g_i2s_handler.Init.Standard = i2s_standard;             /* IIS��׼ */
    g_i2s_handler.Init.DataFormat = i2s_dataformat;         /* IIS���ݳ��� */
    g_i2s_handler.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;  /* ��ʱ�����ʹ�� */
    g_i2s_handler.Init.AudioFreq = I2S_AUDIOFREQ_DEFAULT;   /* IISƵ������ */
    g_i2s_handler.Init.CPOL = i2s_clock_polarity;           /* ����״̬ʱ�ӵ�ƽ */
    g_i2s_handler.Init.ClockSource = I2S_CLOCK_PLL;         /* IISʱ��ԴΪPLL */
    HAL_I2S_Init(&g_i2s_handler); 

    I2S_SPI->CR2 |= 1<<1;               /* SPI2/I2S2 TX DMA����ʹ��. */
    __HAL_I2S_ENABLE(&g_i2s_handler);   /* ʹ��I2S2 */
}

/**
 * @brief       I2S�ײ�������ʱ��ʹ�ܣ��������ã�DMA����
 * @note        �˺����ᱻHAL_I2S_Init()����
 * @param       hi2s:I2S���
 * @retval      ��
 */
void HAL_I2S_MspInit(I2S_HandleTypeDef *hi2s)
{
    GPIO_InitTypeDef gpio_init_struct;

    I2S_SPI_CLK_ENABLE();           /* ʹ��SPI2/I2S2ʱ�� */
    I2S_LRCK_GPIO_CLK_ENABLE();     /* ʹ��I2S_LRCKʱ�� */
    I2S_SCLK_GPIO_CLK_ENABLE();     /* ʹ��I2S_SCLKʱ�� */
    I2S_SDOUT_GPIO_CLK_ENABLE();    /* ʹ��I2S_SDOUTʱ�� */
    I2S_SDIN_GPIO_CLK_ENABLE();     /* ʹ��I2S_SDINʱ�� */
    I2S_MCLK_GPIO_CLK_ENABLE();     /* ʹ��I2S_MCLKʱ�� */

    gpio_init_struct.Pin = I2S_LRCK_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ���츴�� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_HIGH;               /* ���� */
    gpio_init_struct.Alternate = GPIO_AF_I2S_SPI;           /* ����ΪSPI/I2S */
    HAL_GPIO_Init(I2S_LRCK_GPIO_PORT, &gpio_init_struct);   /* ��ʼ��I2S_LRCK���� */

    gpio_init_struct.Pin = I2S_SCLK_GPIO_PIN; 
    HAL_GPIO_Init(I2S_SCLK_GPIO_PORT,&gpio_init_struct);    /* ��ʼ��I2S_SCLK���� */
    
    gpio_init_struct.Pin = I2S_SDOUT_GPIO_PIN; 
    HAL_GPIO_Init(I2S_SDOUT_GPIO_PORT,&gpio_init_struct);   /* ��ʼ��I2S_SDOUT���� */
    
    gpio_init_struct.Pin = I2S_SDIN_GPIO_PIN; 
    HAL_GPIO_Init(I2S_SDIN_GPIO_PORT,&gpio_init_struct);    /* ��ʼ��I2S_SDIN���� */
    
    gpio_init_struct.Pin = I2S_MCLK_GPIO_PIN; 
    HAL_GPIO_Init(I2S_MCLK_GPIO_PORT,&gpio_init_struct);    /* ��ʼ��I2S_MCLK���� */
}

/**
 * �����ʼ��㹫ʽ:Fs=I2SxCLK/[256*(2*I2SDIV+ODD)]
 * I2SxCLK=(HSE/pllm)*PLLI2SN/PLLI2SR
 * һ��HSE=8Mhz 
 * pllm:��Sys_Clock_Set���õ�ʱ��ȷ����һ����8
 * PLLI2SN:һ����192~432
 * PLLI2SR:2~7
 * I2SDIV:2~255
 * ODD:0/1
 * I2S��Ƶϵ����@pllm=8,HSE=8Mhz,��vco����Ƶ��Ϊ1Mhz
 * ���ʽ:������/10,PLLI2SN,PLLI2SR,I2SDIV,ODD
 */
const uint16_t I2S_PSC_TBL[][5]=
{
    {   800, 256, 5, 12, 1 },   /* 8Khz������ */
    {  1102, 429, 4, 19, 0 },   /* 11.025Khz������ */
    {  1600, 213, 2, 13, 0 },   /* 16Khz������ */
    {  2205, 429, 4,  9, 1 },   /* 22.05Khz������ */
    {  3200, 213, 2,  6, 1 },   /* 32Khz������ */
    {  4410, 271, 2,  6, 0 },   /* 44.1Khz������ */
    {  4800, 258, 3,  3, 1 },   /* 48Khz������ */
    {  8820, 316, 2,  3, 1 },   /* 88.2Khz������ */
    {  9600, 344, 2,  3, 1 },   /* 96Khz������ */
    { 17640, 361, 2,  2, 0 },   /* 176.4Khz������ */
    { 19200, 393, 2,  2, 0 },   /* 192Khz������ */
};

/**
 * @brief       ����I2S��DMA����
 * @param       ��
 * @retval      ��
 */
void i2s_dma_enable(void)
{
    uint32_t tempreg = 0;
    tempreg = I2S_SPI->CR2;     /* �ȶ�����ǰ������ */
    tempreg |= 1 << 1;          /* ʹ��DMA */
    I2S_SPI->CR2 = tempreg;     /* д��CR2�Ĵ����� */
}

/**
 * @brief       ����I2S�Ĳ�����
 * @param       samplerate:������, ��λ:Hz
 * @retval      0,���óɹ�
 *              1,�޷�����
 */
uint8_t i2s_samplerate_set(uint32_t samplerate)
{   
    uint8_t i = 0; 
    uint32_t tempreg = 0;
    RCC_PeriphCLKInitTypeDef rcc_i2s_clkinit_struct;

    for (i = 0; i < (sizeof(I2S_PSC_TBL) / 10); i++)    /* �����Ĳ������Ƿ����֧�� */
    {
        if ((samplerate / 10) == I2S_PSC_TBL[i][0])
        {
            break;
        }
    }
    if (i == (sizeof(I2S_PSC_TBL) / 10))
    {
        return 1;   /* �Ҳ��� */
    }

    rcc_i2s_clkinit_struct.PeriphClockSelection = RCC_PERIPHCLK_I2S;        /* ����ʱ��Դѡ�� */
    rcc_i2s_clkinit_struct.PLLI2S.PLLI2SN = (uint32_t)I2S_PSC_TBL[i][1];    /* ����PLLI2SN */
    rcc_i2s_clkinit_struct.PLLI2S.PLLI2SR = (uint32_t)I2S_PSC_TBL[i][2];    /* ����PLLI2SR */
    HAL_RCCEx_PeriphCLKConfig(&rcc_i2s_clkinit_struct);                     /* ����ʱ�� */

    RCC->CR |= 1 << 26;                 /* ����I2Sʱ�� */
    while((RCC->CR & 1 << 27) == 0);    /* �ȴ�I2Sʱ�ӿ����ɹ�. */
    tempreg = I2S_PSC_TBL[i][3] << 0;   /* ����I2SDIV */
    tempreg |= I2S_PSC_TBL[i][4] << 8;  /* ����ODDλ */
    tempreg |= 1 << 9;                  /* ʹ��MCKOEλ,���MCK */
    SPI2->I2SPR = tempreg;              /* ����I2SPR�Ĵ��� */
    return 0;
}

/**
 * @brief       I2S TX DMA����
 *  @note       ����Ϊ˫����ģʽ,������DMA��������ж�
 * @param       buf0 : M0AR��ַ.
 * @param       buf1 : M1AR��ַ.
 * @param       num  : ÿ�δ���������
 * @retval      ��
 */
void i2s_tx_dma_init(uint8_t* buf0, uint8_t *buf1, uint16_t num)
{  
    I2S_TX_DMA_CLK_ENABLE();                                                /* ʹ��I2S TX DMAʱ�� */
    __HAL_LINKDMA(&g_i2s_handler, hdmatx, g_i2s_txdma_handler);             /* ��DMA��I2S��ϵ���� */

    g_i2s_txdma_handler.Instance = I2S_TX_DMASx;                            /* ����I2S TX DMA������  */
    g_i2s_txdma_handler.Init.Channel = I2S_TX_DMASx_Channel;                /* ����I2S TX DMAͨ�� */
    g_i2s_txdma_handler.Init.Direction = DMA_MEMORY_TO_PERIPH;              /* �洢��������ģʽ */
    g_i2s_txdma_handler.Init.PeriphInc = DMA_PINC_DISABLE;                  /* ���������ģʽ */
    g_i2s_txdma_handler.Init.MemInc = DMA_MINC_ENABLE;                      /* �洢������ģʽ */
    g_i2s_txdma_handler.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD; /* �������ݳ���:16λ */
    g_i2s_txdma_handler.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;    /* �洢�����ݳ���:16λ */
    g_i2s_txdma_handler.Init.Mode = DMA_CIRCULAR;                           /* ʹ��ѭ��ģʽ */
    g_i2s_txdma_handler.Init.Priority = DMA_PRIORITY_HIGH;                  /* �����ȼ� */
    g_i2s_txdma_handler.Init.FIFOMode = DMA_FIFOMODE_DISABLE;               /* ��ʹ��FIFO */
    g_i2s_txdma_handler.Init.MemBurst = DMA_MBURST_SINGLE;                  /* �洢������ͻ������ */
    g_i2s_txdma_handler.Init.PeriphBurst = DMA_PBURST_SINGLE;               /* ����ͻ�����δ��� */
    HAL_DMA_DeInit(&g_i2s_txdma_handler);                                   /* �������ǰ������ */
    HAL_DMA_Init(&g_i2s_txdma_handler);                                     /* ��ʼ��DMA */

    HAL_DMAEx_MultiBufferStart(&g_i2s_txdma_handler, (uint32_t)buf0, (uint32_t)&I2S_SPI->DR, (uint32_t)buf1, num);  /* ����˫���� */
    __HAL_DMA_DISABLE(&g_i2s_txdma_handler);                                /* �ȹر�DMA */
    delay_us(10);                                                           /* 10us��ʱ����ֹ-O2�Ż������� */
    __HAL_DMA_ENABLE_IT(&g_i2s_txdma_handler, DMA_IT_TC);                   /* ������������ж� */
    __HAL_DMA_CLEAR_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG);          /* ���DMA��������жϱ�־λ */
    HAL_NVIC_SetPriority(I2S_TX_DMASx_IRQ, 0, 0);                           /* DMA�ж����ȼ� */
    HAL_NVIC_EnableIRQ(I2S_TX_DMASx_IRQ);
} 

void (*i2s_tx_callback)(void); /* I2S DMA�ص�����ָ�� */
 
/**
 * @brief       I2S TX DMA �жϷ�����
 * @param       ��
 * @retval      ��
 */
void I2S_TX_DMASx_Handle(void)
{
    if (__HAL_DMA_GET_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG) != RESET)   /* DMA������� */
    {
        __HAL_DMA_CLEAR_FLAG(&g_i2s_txdma_handler, I2S_TX_DMASx_FLAG);          /* ���DMA��������жϱ�־λ */

        if (i2s_tx_callback != NULL)
        {
            i2s_tx_callback();  /* ִ�лص�����,��ȡ���ݵȲ����������洦�� */
        } 
    }
} 

/**
 * @brief       I2S��ʼ����
 * @param       ��
 * @retval      ��
 */
void i2s_play_start(void)
{
    __HAL_DMA_ENABLE(&g_i2s_txdma_handler);     /* ����DMA TX���� */
}

/**
 * @brief       I2Sֹͣ����
 * @param       ��
 * @retval      ��
 */
void i2s_play_stop(void)
{
    __HAL_DMA_DISABLE(&g_i2s_txdma_handler);    /* �ر�DMA TX���� */
}







