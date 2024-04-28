/**
 ****************************************************************************************************
 * @file        wavplay.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       wav���� ����
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20211116
 * 1,֧��16λ/24λWAV�ļ�����
 * 2,��߿���֧�ֵ�192K/24bit��WAV��ʽ. 
 *
 ****************************************************************************************************
 */
 
#ifndef __WAVPLAY_H
#define __WAVPLAY_H
#include "./SYSTEM/SYS/sys.h" 

#define WAV_I2S_TX_DMA_BUFSIZE    8192      /* ����WAV TX DMA �����С(����192Kbps@24bit��ʱ��,��Ҫ����8192��Ų��Ῠ) */
 
typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"RIFF",��0X46464952 */
    uint32_t ChunkSize ;        /* ���ϴ�С;�ļ��ܴ�С-8 */
    uint32_t Format;            /* ��ʽ;WAVE,��0X45564157 */
}ChunkRIFF;     /* RIFF�� */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"fmt ",��0X20746D66 */
    uint32_t ChunkSize ;        /* �Ӽ��ϴ�С(������ID��Size);����Ϊ:20. */
    uint16_t AudioFormat;       /* ��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM */
    uint16_t NumOfChannels;     /* ͨ������;1,��ʾ������;2,��ʾ˫����; */
    uint32_t SampleRate;        /* ������;0X1F40,��ʾ8Khz */
    uint32_t ByteRate;          /* /�ֽ�����; */
    uint16_t BlockAlign;        /* �����(�ֽ�); */
    uint16_t BitsPerSample;     /* �����������ݴ�С;4λADPCM,����Ϊ4 */
//    uint16_t ByteExtraData;   /* ���ӵ������ֽ�;2��; ����PCM,û��������� */
}ChunkFMT;      /* fmt�� */

typedef __PACKED_STRUCT 
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"fact",��0X74636166; */
    uint32_t ChunkSize ;        /* �Ӽ��ϴ�С(������ID��Size);����Ϊ:4. */
    uint32_t NumOfSamples;      /* ����������; */
}ChunkFACT;     /* fact�� */

typedef __PACKED_STRUCT 
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"LIST",��0X74636166; */
    uint32_t ChunkSize ;        /* �Ӽ��ϴ�С(������ID��Size);����Ϊ:4. */
}ChunkLIST;     /* LIST�� */

typedef __PACKED_STRUCT
{
    uint32_t ChunkID;           /* chunk id;����̶�Ϊ"data",��0X5453494C */
    uint32_t ChunkSize ;        /* �Ӽ��ϴ�С(������ID��Size) */
}ChunkDATA;     /* data�� */

typedef __PACKED_STRUCT
{ 
    ChunkRIFF riff;             /* riff�� */
    ChunkFMT fmt;               /* fmt�� */
//    ChunkFACT fact;           /* fact�� ����PCM,û������ṹ�� */
    ChunkDATA data;             /* data�� */
}__WaveHeader;  /* wavͷ */

typedef __PACKED_STRUCT
{ 
    uint16_t audioformat;       /* ��Ƶ��ʽ;0X01,��ʾ����PCM;0X11��ʾIMA ADPCM */
    uint16_t nchannels;         /* ͨ������;1,��ʾ������;2,��ʾ˫����; */
    uint16_t blockalign;        /* �����(�ֽ�); */
    uint32_t datasize;          /* WAV���ݴ�С */

    uint32_t totsec ;           /* ���׸�ʱ��,��λ:�� */
    uint32_t cursec ;           /* ��ǰ����ʱ�� */

    uint32_t bitrate;           /* ������(λ��) */
    uint32_t samplerate;        /* ������ */
    uint16_t bps;               /* λ��,����16bit,24bit,32bit */

    uint32_t datastart;         /* ����֡��ʼ��λ��(���ļ������ƫ��) */
}__wavctrl;     /* wav ���ſ��ƽṹ�� */ 


uint8_t wav_decode_init(char* fname, __wavctrl* wavx);              /* WAV������ʼ�� */
uint32_t wav_buffill(uint8_t *buf, uint16_t size, uint8_t bits);    /* ���buf */
void wav_i2s_dma_tx_callback(void);     /* WAV����ʱ,I2S DMA����ص����� */
uint8_t wav_play_song(char* fname);     /* ����ĳ��WAV�ļ� */
#endif
















