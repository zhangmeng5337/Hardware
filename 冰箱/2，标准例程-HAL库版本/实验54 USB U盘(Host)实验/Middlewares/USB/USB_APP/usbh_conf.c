/**
 ****************************************************************************************************
 * @file        usbh_conf.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-20
 * @brief       usbh_conf ��������
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

#include "usbh_conf.h"
#include "usbh_core.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"


HCD_HandleTypeDef g_hhcd;


/**
  * @brief  Initializes the HCD MSP.
  * @param  hhcd: HCD handle
  * @retval None
  */
void HAL_HCD_MspInit(HCD_HandleTypeDef *hhcd)
{
    GPIO_InitTypeDef gpio_init_struct= { 0 };

    if (hhcd->Instance == USB_OTG_FS)
    {
        __HAL_RCC_GPIOA_CLK_ENABLE();                           /* ʹ��GPIOAʱ�� */
        __HAL_RCC_USB_OTG_FS_CLK_ENABLE();                      /* ʹ��OTG FSʱ�� */

        gpio_init_struct.Pin = GPIO_PIN_11 | GPIO_PIN_12;
        gpio_init_struct.Mode = GPIO_MODE_AF_PP;                /* ���� */
        gpio_init_struct.Pull = GPIO_NOPULL;                    /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
        gpio_init_struct.Alternate = GPIO_AF10_OTG_FS;          /* ����ΪOTG1_FS */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);                /* ��ʼ��PA11��PA12���� */

        gpio_init_struct.Pin = GPIO_PIN_15;
        gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* �������ģʽ */
        gpio_init_struct.Pull = GPIO_NOPULL;                    /* ���� */
        gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ����ģʽ */
        HAL_GPIO_Init(GPIOA, &gpio_init_struct);                /* ��ʼ��PWR���� */
 
        USB_PWR(0);                                             /* PA15����͵�ƽ, �ر�U�̹��� */
        USBH_Delay(500);
        USB_PWR(1);                                             /* PA15����ߵ�ƽ, �ָ�U�̹��� */

        HAL_NVIC_SetPriority(OTG_FS_IRQn, 1, 0);                /* ���ȼ�����Ϊ��ռ1,�����ȼ�0 */
        HAL_NVIC_EnableIRQ(OTG_FS_IRQn);                        /* ʹ��OTG FS�ж� */
    }
    else if (hhcd->Instance == USB_OTG_HS)
    {
        /* USB OTG HS������û�õ�,�ʲ������� */
    }
}

/**
 * @brief       USB OTG �жϷ�����
 *   @note      ��������USB�ж�
 * @param       ��
 * @retval      ��
 */
void OTG_FS_IRQHandler(void)
{
    HAL_HCD_IRQHandler(&g_hhcd);
}


/******************************************************************************************/
/* ������: USBH LL HCD �����Ļص�����(HCD->USB Host Library) */

/**
 * @brief       USBH SOF�ص�����
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_SOF_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_IncTimer(hhcd->pData);
}

/**
 * @brief       USBH ���ӳɹ��ص�����
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_Connect_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("Connected!\r\n\r\n");
    USBH_LL_Connect(hhcd->pData);
}

/**
 * @brief       USBH �Ͽ����ӻص�����
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_Disconnect_Callback(HCD_HandleTypeDef *hhcd)
{
    printf("Disconnected!\r\n\r\n");
    USBH_LL_Disconnect(hhcd->pData);
}

/**
 * @brief       USBH �ӿ�ʹ�ܻص�����(V3.3.3 USB�������ص�����)
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_PortEnabled_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_PortEnabled(hhcd->pData);
}

/**
 * @brief       USBH �ӿڹرջص�����(V3.3.3 USB�������ص�����)
 * @param       hhcd        : HCD�ṹ��ָ��
 * @retval      ��
 */
void HAL_HCD_PortDisabled_Callback(HCD_HandleTypeDef *hhcd)
{
    USBH_LL_PortDisabled(hhcd->pData);
}

/**
 * @brief       USBH ֪ͨURB�仯�ص�����
 * @param       hhcd        : HCD�ṹ��ָ��
 * @param       chnum       : �˵���
 * @param       urb_state   : URB״̬
 * @retval      ��
 */
void HAL_HCD_HC_NotifyURBChange_Callback(HCD_HandleTypeDef *hhcd, uint8_t chnum, HCD_URBStateTypeDef urb_state)
{
    /* To be used with OS to sync URB state with the global state machine */
}


/******************************************************************************************/
/* ������: USBH LL HCD �����ӿں���(HCD->USB Host Library) */

/**
 * @brief       USBH �ײ��ʼ������
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_Init(USBH_HandleTypeDef *phost)
{
#ifdef USE_USB_FS
    /* ����LL������ز��� */
    g_hhcd.Instance = USB_OTG_FS;               /* ʹ��USB OTG */
    g_hhcd.Init.Host_channels = 11;             /* ����ͨ����Ϊ11�� */
    g_hhcd.Init.dma_enable = 0;                 /* ��ʹ��DMA */
    g_hhcd.Init.low_power_enable = 0;           /* ��ʹ�ܵ͹���ģʽ */
    g_hhcd.Init.phy_itface = HCD_PHY_EMBEDDED;  /* ʹ���ڲ�PHY */
    g_hhcd.Init.Sof_enable = 0;                 /* ��ֹSOF�ж� */
    g_hhcd.Init.speed = HCD_SPEED_FULL;         /* USBȫ��(12Mbps) */
    g_hhcd.Init.vbus_sensing_enable = 0;        /* ��ʹ��VBUS��� */

    g_hhcd.pData = phost;                       /* g_hhcd��pDataָ��phost */
    phost->pData = &g_hhcd;                     /* phost��pDataָ��g_hhcd */

    HAL_HCD_Init(&g_hhcd);                      /* ��ʼ��LL���� */
#endif

#ifdef USE_USB_HS
    /* δʵ�� */
#endif
    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&g_hhcd));
    return USBH_OK;
}

/**
 * @brief       USBH �ײ�ȡ����ʼ��(�ָ�Ĭ�ϸ�λ״̬)����
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_DeInit(USBH_HandleTypeDef *phost)
{
    HAL_HCD_DeInit(phost->pData);
    return USBH_OK;
}

/**
 * @brief       USBH �ײ�������ʼ����
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_Start(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Start(phost->pData);
    return USBH_OK;
}

/**
 * @brief       USBH �ײ�����ֹͣ����
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_Stop(USBH_HandleTypeDef *phost)
{
    HAL_HCD_Stop(phost->pData);
    return USBH_OK;
}

/**
 * @brief       USBH ��ȡUSB�豸���ٶ�
 * @param       phost       : USBH���ָ��
 * @retval      USBH�豸�ٶ�
 */
USBH_SpeedTypeDef USBH_LL_GetSpeed(USBH_HandleTypeDef *phost)
{
    USBH_SpeedTypeDef speed = USBH_SPEED_FULL;

    switch (HAL_HCD_GetCurrentSpeed(phost->pData))
    {
        case 0:
            speed = USBH_SPEED_HIGH;
            printf("USB Host [HS]\r\n");
            break;

        case 1:
            speed = USBH_SPEED_FULL;
            printf("USB Host [FS]\r\n");
            break;

        case 2:
            speed = USBH_SPEED_LOW;
            printf("USB Host [LS]\r\n");
            break;

        default:
            speed = USBH_SPEED_FULL;
            printf("USB Host [FS]\r\n");
            break;
    }

    return speed;
}

/**
 * @brief       USBH ��λUSB HOST�˿�
 * @param       phost       : USBH���ָ��
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_ResetPort(USBH_HandleTypeDef *phost)
{
    HAL_HCD_ResetPort(phost->pData);
    printf("USB Reset Port\r\n");
    return USBH_OK;
}

/**
 * @brief       USBH ��ȡ���һ�δ���İ���С
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @retval      ����С
 */
uint32_t USBH_LL_GetLastXferSize(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return HAL_HCD_HC_GetXferCount(phost->pData, pipe);
}

/**
 * @brief       USBH ��һ���ܵ�(ͨ��)
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @param       epnum       : �˵��
 * @param       dev_address : �豸��ַ
 * @param       speed       : �豸�ٶ�
 * @param       ep_type     : �˵�����
 * @param       mps         : �˵�������С
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_OpenPipe(USBH_HandleTypeDef *phost,
                                    uint8_t pipe,
                                    uint8_t epnum,
                                    uint8_t dev_address,
                                    uint8_t speed,
                                    uint8_t ep_type, uint16_t mps)
{
    HAL_HCD_HC_Init(phost->pData, pipe, epnum, dev_address, speed, ep_type, mps);
    return USBH_OK;
}

/**
 * @brief       USBH �ر�һ���ܵ�(ͨ��)
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_ClosePipe(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    HAL_HCD_HC_Halt(phost->pData, pipe);
    return USBH_OK;
}

/**
 * @brief       USBH �ύһ���µ�URB
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @param       direction   : ��������
 * @param       ep_type     : �˵�����
 * @param       token       : �˵��־
 * @param       pbuff       : URB�������׵�ַ
 * @param       length      : URB���ݳ���
 * @param       do_ping     : ����do ping protocol,USB HS���õ�
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_SubmitURB(USBH_HandleTypeDef *phost,
                                     uint8_t pipe,
                                     uint8_t direction,
                                     uint8_t ep_type,
                                     uint8_t token,
                                     uint8_t *pbuff,
                                     uint16_t length, uint8_t do_ping)
{
    HAL_HCD_HC_SubmitRequest(phost->pData, pipe, direction, ep_type, token, pbuff, length, do_ping);
    return USBH_OK;
}

/**
 * @brief       USBH ��ȡURB״̬
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @retval      URB״̬
 */
USBH_URBStateTypeDef USBH_LL_GetURBState(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    return (USBH_URBStateTypeDef) HAL_HCD_HC_GetURBState(phost->pData, pipe);
}

/**
 * @brief       USBH ����VBUS״̬
 * @param       phost       : USBH���ָ��
 * @param       state       : vbus״̬. 0,����; 1,������
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_DriverVBUS(USBH_HandleTypeDef *phost, uint8_t state)
{
#ifdef USE_USB_FS

    if (state == 0)
    {
    }
    else
    {
    }

#endif
    HAL_Delay(500);
    return USBH_OK;
}

/**
 * @brief       USBH ���ùܵ��ķ�ת
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @param       toggle      : ��ת״̬
 * @retval      USB״̬
 *   @arg       USBH_OK(0)   , ����;
 *   @arg       USBH_BUSY(1) , æ;
 *   @arg       ����         , ʧ��;
 */
USBH_StatusTypeDef USBH_LL_SetToggle(USBH_HandleTypeDef *phost, uint8_t pipe, uint8_t toggle)
{
    if (g_hhcd.hc[pipe].ep_is_in)
    {
        g_hhcd.hc[pipe].toggle_in = toggle;
    }
    else
    {
        g_hhcd.hc[pipe].toggle_out = toggle;
    }

    return USBH_OK;
}

/**
 * @brief       USBH ��ȡ�ܵ���ת״̬
 * @param       phost       : USBH���ָ��
 * @param       pipe        : �ܵ����
 * @retval      ��ת״̬
 */
uint8_t USBH_LL_GetToggle(USBH_HandleTypeDef *phost, uint8_t pipe)
{
    uint8_t toggle = 0;

    if (g_hhcd.hc[pipe].ep_is_in)
    {
        toggle = g_hhcd.hc[pipe].toggle_in;
    }
    else
    {
        toggle = g_hhcd.hc[pipe].toggle_out;
    }

    return toggle;
}

/**
 * @brief       USBH ��ʱ����(��msΪ��λ)
 * @param       Delay       : ��ʱ��ms��
 * @retval      ��ת״̬
 */
void USBH_Delay(uint32_t Delay)
{
    delay_ms(Delay);
}










