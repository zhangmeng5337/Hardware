/**
 ****************************************************************************************************
 * @file        dac.h
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

#ifndef __DAC_H
#define __DAC_H

#include "./SYSTEM/sys/sys.h"


void dac_init(uint8_t outx);                        /* DACͨ��1��ʼ�� */ 
void dac_set_voltage(uint8_t outx, uint16_t vol);   /* ����ͨ��1/2�����ѹ */ 
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n);   /* ������ǲ� */
void dac_dma_wave_init(uint8_t outx);               /* DAC DMA������Ҳ���ʼ������ */
void dac_dma_wave_enable(uint8_t outx, uint16_t ndtr, uint16_t arr, uint16_t psc);

#endif


