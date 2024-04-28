/**
 ****************************************************************************************************
 * @file        adc3.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-10-23
 * @brief       ADC3 ��������
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
 ****************************************************************************************************
 */

#ifndef __ADC3_H
#define __ADC3_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* ADC ���� */

#define ADC_ADCX                            ADC3
#define ADC3_CHY_CLK_ENABLE()               do{ __HAL_RCC_ADC3_CLK_ENABLE(); }while(0)      /* ADC1 ʱ��ʹ�� */

/******************************************************************************************/

void adc3_init(void);                           /* ADC3��ʼ�� */
void adc3_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch,uint32_t rank, uint32_t stime);    /* ADC3ͨ������ */
uint32_t adc3_get_result(uint32_t ch);          /* ���ĳ��ͨ��ֵ */
uint32_t adc3_get_result_average(uint32_t ch, uint8_t times);                                       /* �õ�ĳ��ͨ����������������ƽ��ֵ */

#endif 















