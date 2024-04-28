/**
 ****************************************************************************************************
 * @file        stm32f407_audio.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-20
 * @brief       stm32f407_audio ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F407������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20220120
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./SYSTEM/USART/usart.h"
#include "stm32f407_audio.h"
#include "usbd_audio_if.h"
#include "usbd_audio.h"


uint8_t g_volume = 0;                       /* ��ǰ���� */
extern uint8_t g_usbd_audio_dataout_en;     /* USB�������״̬,��usbd_audio.c���涨�� */


/**
 * @brief       ����ʱ,I2S DMA����ص�����
 * @param       ��
 * @retval      ��
 */
void bsp_audio_i2s_dma_callback(void)
{
    BSP_AUDIO_OUT_TransferComplete_CallBack();      /* ִ�лص�����,�������� */
}


/**
 * @brief  ������Ƶ�ӿ�
 * @param  OutputDevice :����豸ѡ��,δ�õ�.
 * @param  Volume       :������С, 0~100
 * @param  AudioFreq    :��Ƶ������
 * @retval               �������
 *   @arg                0    , �ɹ�
 *   @arg                ���� , �������
 */
uint8_t BSP_AUDIO_OUT_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
    es8388_i2s_cfg(0, 3);                           /* �����ֱ�׼,16λ���ݳ��� */
    i2s_init(I2S_STANDARD_PHILIPS, I2S_MODE_MASTER_TX, I2S_CPOL_LOW, I2S_DATAFORMAT_16B_EXTENDED); /* �����ֱ�׼,��������,ʱ�ӵ͵�ƽ��Ч,16λ��չ֡���� */
    i2s_samplerate_set(AudioFreq);                  /* ���ò����� */
    BSP_AUDIO_OUT_SetVolume(Volume);                /* �������� */
    i2s_tx_callback = bsp_audio_i2s_dma_callback;   /* �ص�����ָ bsp_audio_i2s_dma_callback */
    
    printf("EVAL_AUDIO_Init:%d,%d\r\n", Volume, AudioFreq);
    
    return 0;
}

/**
 * @brief  ��ʼ������Ƶ����
 * @param  pBuffer  : ��Ƶ�������׵�ַָ��
 * @param  Size     : ��������С(��λ:�ֽ�)
 * @retval            �������
 *   @arg             0    , �ɹ�
 *   @arg             ���� , �������
 */
uint8_t BSP_AUDIO_OUT_Play(uint16_t* pBuffer, uint32_t Size)
{
    i2s_tx_dma_init((uint8_t*) pBuffer, 0, Size);   /* ����I2S TX DMA */
    I2S_TX_DMASx->CR &= ~(1 << 8);  /* ����ģʽ */
    I2S_TX_DMASx->CR &= ~(1 << 18); /* �ĳɵ�����ģʽ */
    i2s_play_start();               /* ����DMA */
    
    printf("EVAL_AUDIO_Play:%x,%d\r\n", pBuffer, Size);
    
    return 0;
}

/**
 * @brief  �л���Ƶ������buffer,ÿ��DMA�ж���ɻ���øú���
 * @param  pData    : ��Ƶ�����������׵�ַ
 * @param  Size     : ��Ƶ��������С(��λ:harf word,Ҳ����2���ֽ�)
 * @retval          : ��
  */
void BSP_AUDIO_OUT_ChangeBuffer(uint16_t *pData, uint16_t Size)
{
    if (g_usbd_audio_dataout_en == 1)       /* USB����Ƶ������� */
    {
        g_usbd_audio_dataout_en = 0;        /* ������λ */
    }
    else if (g_usbd_audio_dataout_en == 0)  /* û���յ��κ�����,ȫ�����0 */
    {
        my_mem_set(pData, 0, AUDIO_TOTAL_BUF_SIZE);
        g_usbd_audio_dataout_en = 2;        /* ����Ѿ�������buffer,ֹͣ������� */
    }

    if (Size != AUDIO_TOTAL_BUF_SIZE)
    {
        //printf("chgbuf:%x,%d\r\n", pData, Size);  /* ��۲�����,����printf,���ǻ�����΢��Ӱ������ */
    }

    I2S_TX_DMASx->CR &= ~(1 << 0);          /* �ر�DMA���� */

    while (I2S_TX_DMASx->CR & 0X1);         /* ȷ��DMA���Ա����� */

    I2S_TX_DMASx->NDTR = Size ;             /* ���ô��䳤�� */
    I2S_TX_DMASx->CR |= 1 << 0;             /* ����DMA���� */
}

/**
 * @brief  ֹͣ����
 * @retval            �������
 *   @arg             0    , �ɹ�
 *   @arg             ���� , �������
 */
uint8_t BSP_AUDIO_OUT_Pause(void)
{
    printf("EVAL_AUDIO_Stop\r\n");
    return 0;
}

/**
 * @brief  ֹͣ����
 * @param  Option   : δ�õ�
 * @retval            �������
 *   @arg             0    , �ɹ�
 *   @arg             ���� , �������
 */
uint8_t BSP_AUDIO_OUT_Stop(uint32_t Option)
{
    printf("EVAL_AUDIO_Stop\r\n");
    return 0;
}

/**
 * @brief  ��������
 * @param  Volume: ������С, 0~100
 * @retval      �������
 *   @arg       0    , �ɹ�
 *   @arg       ���� , �������
 */
uint8_t BSP_AUDIO_OUT_SetVolume(uint8_t Volume)
{
    g_volume = Volume;
    es8388_hpvol_set(g_volume * 0.3);
    es8388_spkvol_set(g_volume * 0.3);
    return 0;
}

/**
 * @brief  ��������
 * @param  Cmd      : 0,������; 1,����
 * @retval            �������
 *   @arg             0    , �ɹ�
 *   @arg             ���� , �������
 */
uint8_t BSP_AUDIO_OUT_SetMute(uint32_t Cmd)
{
    if(Cmd)
    {
        es8388_output_cfg(0, 0);    /* �ر���� ,���Ǿ��� */
    }else
    {
        es8388_output_cfg(1, 1);    /* ����� ,ȡ������ */
    }
    
    printf("BSP_AUDIO_OUT_SetMute:%d\r\n", Cmd);
    return 0;
}





















