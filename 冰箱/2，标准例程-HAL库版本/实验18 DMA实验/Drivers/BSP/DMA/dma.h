/**
 ****************************************************************************************************
 * @file        dma.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-20
 * @brief       DMA ��������
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
 * V1.0 20211020
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __DMA_H
#define	__DMA_H

#include "./SYSTEM/sys/sys.h"


extern DMA_HandleTypeDef g_dma_handle; 

void dma_init(DMA_Stream_TypeDef *dma_stream_handle, uint32_t ch);  /* ����DMAx_CHx */

#endif






























