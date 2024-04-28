/**
 ****************************************************************************************************
 * @file        videoplay.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       ��Ƶ������ Ӧ�ô���
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
 * ��һ�η���
 *
 ****************************************************************************************************
 */
#ifndef __VIDEOPLAYER_H
#define __VIDEOPLAYER_H 
#include <stm32f4xx.h> 
#include "./MJPEG/avi.h"
#include "./FATFS/source/ff.h"


#define AVI_AUDIO_BUF_SIZE    1024*5        /* ����avi����ʱ,��Ƶbuf��С. */
#define AVI_VIDEO_BUF_SIZE    1024*60       /* ����avi����ʱ,��Ƶbuf��С. */

uint16_t video_get_tnum(char *path);                                /* �õ�path·���£�Ŀ���ļ������� */
void video_time_show(FIL *favi,AVI_INFO *aviinfo);                  /* ��ʾ��ǰ����ʱ�� */
void video_info_show(AVI_INFO *aviinfo);                            /* ��ʾ��ǰ��Ƶ�ļ��������Ϣ */
void video_bmsg_show(uint8_t* name, uint16_t index, uint16_t total);/* ��Ƶ������Ϣ��ʾ */
void video_play(void);                                              /* ������Ƶ */
uint8_t video_play_mjpeg(uint8_t *pname);                           /* ����һ��mjpeg�ļ� */
uint8_t video_seek(FIL *favi,AVI_INFO *aviinfo,uint8_t *mbuf);      /* AVI�ļ����� */

#endif



