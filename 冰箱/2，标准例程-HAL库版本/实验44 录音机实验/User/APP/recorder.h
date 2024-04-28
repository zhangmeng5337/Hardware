/**
 ****************************************************************************************************
 * @file        audioplay.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-17
 * @brief       ¼���� Ӧ�ô���
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
 * V1.0 20211117
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#ifndef __RECORDER_H
#define __RECORDER_H
#include "./SYSTEM/SYS/sys.h"
#include "./FATFS/source/ff.h"
#include "./AUDIOCODEC/wav/wavplay.h" 


#define REC_I2S_RX_DMA_BUF_SIZE     4096        /* ����RX DMA �����С */
#define REC_I2S_RX_FIFO_SIZE        10          /* �������FIFO��С */

#define REC_SAMPLERATE              44100       /* ������,44.1Khz */
#define REC_MODE  1


uint8_t recoder_i2s_fifo_read(uint8_t **buf);
uint8_t recoder_i2s_fifo_write(uint8_t *buf);

void rec_i2s_dma_rx_callback(void);
void recoder_enter_rec_mode(void);
void recoder_wav_init(__WaveHeader* wavhead);
void recoder_msg_show(uint32_t tsec, uint32_t kbps);
void recoder_remindmsg_show(uint8_t mode);
void recoder_new_pathname(char *pname);
void wav_recorder(void);

#endif












