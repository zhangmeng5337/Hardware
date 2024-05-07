/**
 ****************************************************************************************************
 * @file        usbh_hid_mouse.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-20
 * @brief       USB��� ��������
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
 * V1.0 20220120
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __USBH_HID_MOUSE_H
#define __USBH_HID_MOUSE_H

#include "usbh_hid.h"


/* �����Ϣ�ṹ��*/
typedef struct _HID_MOUSE_Info
{
  uint8_t              x;           /* x��������ǿ��ת����signed char��ʹ�ã�*/
  uint8_t              y;           /* y��������ǿ��ת����signed char��ʹ�ã�*/
  uint8_t              z;           /* z��������ǿ��ת����signed char��ʹ�ã�*/
  uint8_t              button;      /* ��buttons�޸�Ϊbutton,�洢����״̬*/
}
HID_MOUSE_Info_TypeDef;



USBH_StatusTypeDef USBH_HID_MouseInit(USBH_HandleTypeDef *phost);
HID_MOUSE_Info_TypeDef *USBH_HID_GetMouseInfo(USBH_HandleTypeDef *phost);
USBH_StatusTypeDef USBH_HID_MouseDecode(USBH_HandleTypeDef *phost);

#endif














