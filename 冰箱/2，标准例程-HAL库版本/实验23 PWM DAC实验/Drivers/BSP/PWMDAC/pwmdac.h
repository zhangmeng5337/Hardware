/**
 ****************************************************************************************************
 * @file        pwmdac.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-03
 * @brief       PWM DAC��� ��������
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
 * V1.0 20211103
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __PWMDAC_H
#define __PWMDAC_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* PWM DAC ���� �� ��ʱ�� ���� */

/* PWMDAC Ĭ����ʹ�� PA3, ��Ӧ�Ķ�ʱ��Ϊ TIM9_CH2, �����Ҫ�޸ĳ�����IO���, ����Ӧ
 * �Ķ�ʱ����ͨ��ҲҪ�����޸�. �����ʵ����������޸�.
 */
#define PWMDAC_GPIO_PORT                    GPIOA
#define PWMDAC_GPIO_PIN                     GPIO_PIN_3
#define PWMDAC_GPIO_CLK_ENABLE()            do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */
#define PWMDAC_GPIO_AFTIMX                  GPIO_AF3_TIM9

#define PWMDAC_TIMX                         TIM9
#define PWMDAC_TIMX_CHY                     TIM_CHANNEL_2                                   /* ͨ��Y,  1<= Y <=4 */
#define PWMDAC_TIMX_CCRX                    PWMDAC_TIMX->CCR2                               /* ͨ��Y������ȽϼĴ��� */
#define PWMDAC_TIMX_CLK_ENABLE()            do{ __HAL_RCC_TIM9_CLK_ENABLE(); }while(0)      /* TIM9 ʱ��ʹ�� */

/******************************************************************************************/

void pwmdac_init(uint16_t arr, uint16_t psc);   /* PWM DAC��ʼ�� */
void pwmdac_set_voltage(uint16_t vol);          /* PWM DAC���������ѹ */

#endif






