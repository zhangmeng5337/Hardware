/**
 ****************************************************************************************************
 * @file        can.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.1
 * @date        2023-06-06
 * @brief       CAN ��������
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
 * V1.0 20211025
 * ��һ�η���
 * V1.1 20230606
 * 1, �Ż�can_send_msg����, �������ͳ�ʱ�������
 ****************************************************************************************************
 */

#ifndef __CAN_H
#define __CAN_H

#include "./SYSTEM/sys/sys.h"


/******************************************************************************************/
/* CAN ���� ���� */

#define CAN_RX_GPIO_PORT                GPIOA
#define CAN_RX_GPIO_PIN                 GPIO_PIN_11
#define CAN_RX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */

#define CAN_TX_GPIO_PORT                GPIOA
#define CAN_TX_GPIO_PIN                 GPIO_PIN_12
#define CAN_TX_GPIO_CLK_ENABLE()        do{ __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)     /* PA��ʱ��ʹ�� */

/******************************************************************************************/

/* CAN����RX0�ж�ʹ�� */
#define CAN_RX0_INT_ENABLE              CAN1_RX0_IRQHandler

/* �������� */
uint8_t can_receive_msg(uint32_t id, uint8_t *buf);                                     /* CAN��������, ��ѯ */
uint8_t can_send_msg(uint32_t id, uint8_t *msg, uint8_t len);                           /* CAN�������� */
uint8_t can_init(uint32_t tsjw,uint32_t tbs2,uint32_t tbs1,uint16_t brp,uint32_t mode); /* CAN��ʼ�� */

#endif








