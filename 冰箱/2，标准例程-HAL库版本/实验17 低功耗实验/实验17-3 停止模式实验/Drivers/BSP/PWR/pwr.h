/**
 ****************************************************************************************************
 * @file        pwr.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.2
 * @date        2021-10-18
 * @brief       �͹���ģʽ ��������
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
 * V1.0 20211018
 * ��һ�η���
 * V1.1 20211018
 * 1, ֧�ֽ���˯��ģʽ������
 * 2, ����pwr_wkup_key_init��pwr_enter_sleep����
 * V1.2 20211018
 * 1, ֧�ֽ���ֹͣģʽ������
 * 2, ����pwr_enter_stop����
 *
 ****************************************************************************************************
 */

#ifndef __PWR_H
#define __PWR_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* PWR WKUP ���� ���ź��ж� ���� 
 * ����ͨ��WK_UP�������� MCU,  ��˱��붨��������������Ӧ���жϷ����� 
 */

#define PWR_WKUP_GPIO_PORT              GPIOA
#define PWR_WKUP_GPIO_PIN               GPIO_PIN_0
#define PWR_WKUP_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */
  
#define PWR_WKUP_INT_IRQn               EXTI0_IRQn
#define PWR_WKUP_INT_IRQHandler         EXTI0_IRQHandler


/******************************************************************************************/

void pwr_pvd_init(uint32_t pls); /* PVD��ѹ����ʼ������ */
void pwr_wkup_key_init(void);    /* ���Ѱ�����ʼ�� */
void pwr_enter_sleep(void);      /* ����˯��ģʽ */
void pwr_enter_stop(void);       /* ����ֹͣģʽ */

#endif




















