/**
 ****************************************************************************************************
 * @file        usbh_hid_mouse.c
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

#include "usbh_hid_mouse.h"
#include "usbh_hid_parser.h"



HID_MOUSE_Info_TypeDef    mouse_info;           /* �����Ϣ(����+��ť״̬)*/
uint8_t mouse_report_data[HID_QUEUE_SIZE];      /* ����ϱ����ݳ���,���HID_QUEUE_SIZE���ֽ�*/


/**
 * @brief       USBH ����ʼ��
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_HID_MouseInit(USBH_HandleTypeDef *phost)
{
    HID_HandleTypeDef *HID_Handle =  (HID_HandleTypeDef *) phost->pActiveClass->pData;
    mouse_info.x = 0;
    mouse_info.y = 0;
    mouse_info.button = 0;

    if (HID_Handle->length > sizeof(mouse_report_data))
    {
        HID_Handle->length = sizeof(mouse_report_data);
    }

    HID_Handle->pData = (uint8_t *)mouse_report_data;
    USBH_HID_FifoInit(&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE);
    return USBH_OK;
}

/**
 * @brief       USBH ��ȡ�����Ϣ
 * @param       phost       : USBH���ָ��
 * @retval      �����Ϣ(HID_MOUSE_Info_TypeDef)
 */
HID_MOUSE_Info_TypeDef *USBH_HID_GetMouseInfo(USBH_HandleTypeDef *phost)
{
    if (USBH_HID_MouseDecode(phost) == USBH_OK)
    {
        return &mouse_info;
    }
    else
    {
        return NULL;
    }
}

/**
 * @brief       USBH ������ݽ�������
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_HID_MouseDecode(USBH_HandleTypeDef *phost)
{
    HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;

    if (HID_Handle->length == 0)return USBH_FAIL;

    if (USBH_HID_FifoRead(&HID_Handle->fifo, &mouse_report_data, HID_Handle->length) == HID_Handle->length) /* ��ȡFIFO*/
    {
        mouse_info.button = mouse_report_data[0];
        mouse_info.x      = mouse_report_data[1];
        mouse_info.y      = mouse_report_data[2];
        mouse_info.z      = mouse_report_data[3];
        return USBH_OK;
    }

    return   USBH_FAIL;
}

























