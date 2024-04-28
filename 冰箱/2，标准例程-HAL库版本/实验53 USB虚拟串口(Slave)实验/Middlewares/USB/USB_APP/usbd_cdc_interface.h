/**
 ****************************************************************************************************
 * @file        usbd_cdc_interface.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-20
 * @brief       USB CDC ��������
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

#ifndef __USBD_CDC_IF_H
#define __USBD_CDC_IF_H

#include "usbd_cdc.h"


#define USB_USART_REC_LEN       200     /* USB���ڽ��ջ���������ֽ��� */

/* ��ѯ���ڣ����65ms����С1ms */
#define CDC_POLLING_INTERVAL    1       /* ��ѯ���ڣ����65ms����С1ms */

extern uint8_t  g_usb_usart_rx_buffer[USB_USART_REC_LEN];   /* ���ջ���,���USB_USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� */
extern uint16_t g_usb_usart_rx_sta;                         /* ����״̬��� */

extern USBD_CDC_ItfTypeDef  USBD_CDC_fops;


void cdc_vcp_data_tx(uint8_t *buf,uint32_t len);
void cdc_vcp_data_rx(uint8_t* buf, uint32_t len);
void usb_printf(char* fmt,...); 

#endif 

