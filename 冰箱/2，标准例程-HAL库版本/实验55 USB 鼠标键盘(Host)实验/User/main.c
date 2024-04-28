/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2022-01-20
 * @brief       USB������(Host) ʵ��
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
 ****************************************************************************************************
 */

#include "./SYSTEM/sys/sys.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"
#include "./BSP/LED/led.h"
#include "./BSP/LCD/lcd.h"
#include "./BSP/SRAM/sram.h"
#include "./MALLOC/malloc.h"
#include "usbh_hid.h"
#include "usbh_hid_parser.h"


USBH_HandleTypeDef hUSBHost;    /* USB Host����ṹ�� */

/* Ӧ��״̬�ṹ�����Ͷ��� */
typedef enum
{
    APPLICATION_IDLE = 0,       /* ����״̬ */
    APPLICATION_DISCONNECT,     /* �Ͽ����� */
    APPLICATION_START,          /* Ӧ�ÿ�ʼ���������ˣ� */
    APPLICATION_READY,          /* ׼����ɣ�����ִ�����Ӧ�ô����ˣ� */
    APPLICATION_RUNNING,        /* ������ */
} HID_ApplicationTypeDef;


extern HID_MOUSE_Info_TypeDef mouse_info;
HID_ApplicationTypeDef App_State = APPLICATION_IDLE;

uint8_t g_usb_first_plugin_flag = 0;    /* USB��һ�β����־,���Ϊ1,˵���ǵ�һ�β��� */


/**
 * @brief       USB��Ϣ��ʾ
 * @param       msg   : ��Ϣ����
 *   @arg               0, USB������
 *   @arg               1, USB����
 *   @arg               2, USB���
 *   @arg               3, ��֧�ֵ�USB�豸
 * @retval      ��
 */
void usbh_msg_show(uint8_t msgx)
{
    switch (msgx)
    {
        case 0: /* USB������ */
            lcd_show_string(30, 130, 200, 16, 16, "USB Connecting...", RED);
            lcd_fill(0, 150, lcddev.width, lcddev.height, WHITE);
            break;

        case 1: /* USB���� */
            lcd_show_string(30, 130, 200, 16, 16, "USB Connected    ", RED);
            lcd_show_string(30, 150, 200, 16, 16, "USB KeyBoard", RED);
            lcd_show_string(30, 180, 210, 16, 16, "KEYVAL:", RED);
            lcd_show_string(30, 200, 210, 16, 16, "INPUT STRING:", RED);
            break;

        case 2: /* USB��� */
            lcd_show_string(30, 130, 200, 16, 16, "USB Connected    ", RED);
            lcd_show_string(30, 150, 200, 16, 16, "USB Mouse", RED);
            lcd_show_string(30, 180, 210, 16, 16, "BUTTON:", RED);
            lcd_show_string(30, 200, 210, 16, 16, "X POS:", RED);
            lcd_show_string(30, 220, 210, 16, 16, "Y POS:", RED);
            lcd_show_string(30, 240, 210, 16, 16, "Z POS:", RED);
            break;

        case 3: /* ��֧�ֵ�USB�豸 */
            lcd_show_string(30, 130, 200, 16, 16, "USB Connected    ", RED);
            lcd_show_string(30, 150, 200, 16, 16, "Unknow Device", RED);
            break;
    }
}

/**
 * @brief       ���USBH��ͬ�û�״̬�µ����ݴ���
 * @param       phost : USBH���ָ��
 * @param       id    : USBH��ǰ���û�״̬
 * @retval      ��
 */
void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
    uint8_t dev_type = 0XFF;    /* �豸����,1,����;2,���;����,��֧�ֵ��豸 */

    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_DISCONNECTION:
            usbh_msg_show(0);   /* ��ʾ�Ѿ��Ͽ�����,׼���������� */
            App_State = APPLICATION_DISCONNECT;
            break;

        case HOST_USER_CLASS_ACTIVE:
            App_State = APPLICATION_READY;
            dev_type = phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bInterfaceProtocol;

            if (dev_type == HID_KEYBRD_BOOT_CODE)       /* �����豸 */
            {
                g_usb_first_plugin_flag = 1;    /* ��ǵ�һ�β��� */
                usbh_msg_show(1);   /* ��ʾ���̽��� */
            }
            else if (dev_type == HID_MOUSE_BOOT_CODE)   /* ����豸 */
            {
                g_usb_first_plugin_flag = 1;    /* ��ǵ�һ�β��� */
                usbh_msg_show(2);   /* ��ʾ������ */
            }
            else
            {
                usbh_msg_show(3);   /* ��ʾ��֧�� */
            }

            break;

        case HOST_USER_CONNECTION:
            App_State = APPLICATION_START;
            break;

        default:
            break;
    }
}

/* ��ʱ����,���ڴ���������/������������(4.3��,����������2016�ֽ�) */
__ALIGNED(4) uint8_t g_temp_buffer[2017];

/**
 * @brief       USB������ݴ���
 * @param       data  : USB������ݽṹ��ָ��
 * @retval      ��
 */
void mouse_data_process(HID_MOUSE_Info_TypeDef *data)
{
    static signed short x, y, z;

    if (g_usb_first_plugin_flag)    /* ��һ�β���,���������� */
    {
        g_usb_first_plugin_flag = 0;
        x = y = z = 0;
    }

    x += (signed char)data->x;

    if (x > 9999)x = 9999;

    if (x < -9999)x = -9999;

    y += (signed char)data->y;

    if (y > 9999)y = 9999;

    if (y < -9999)y = -9999;

    z += (signed char)data->z;

    if (z > 9999)z = 9999;

    if (z < -9999)z = -9999;

    sprintf((char *)g_temp_buffer, "BUTTON:");

    if (data->button & 0X01)strcat((char *)g_temp_buffer, "LEFT ");

    if ((data->button & 0X03) == 0X02)
    {
        strcat((char *)g_temp_buffer, "RIGHT");
    }
    else
    {
        if ((data->button & 0X03) == 0X03)strcat((char *)g_temp_buffer, "+RIGHT");
    }

    if ((data->button & 0X07) == 0X04)
    {
        strcat((char *)g_temp_buffer, "MID  ");
    }
    else
    {
        if ((data->button & 0X07) > 0X04)strcat((char *)g_temp_buffer, "+MID");
    }

    lcd_fill(30 + 56, 180, lcddev.width - 1, 180 + 16, WHITE);
    lcd_show_string(30, 180, 210, 16, 16, (char *)g_temp_buffer, BLUE);
    sprintf((char *)g_temp_buffer, "X POS:%05d", x);
    lcd_show_string(30, 200, 200, 16, 16, (char *)g_temp_buffer, BLUE);
    sprintf((char *)g_temp_buffer, "Y POS:%05d", y);
    lcd_show_string(30, 220, 200, 16, 16, (char *)g_temp_buffer, BLUE);
    sprintf((char *)g_temp_buffer, "Z POS:%05d", z);
    lcd_show_string(30, 240, 200, 16, 16, (char *)g_temp_buffer, BLUE);
    //printf("btn,X,Y,Z:0x%x,%d,%d,%d\r\n",data->button,(signed char)data->x,(signed char)data->y,(signed char)data->z);
}

/**
 * @brief       USB�������ݴ���
 * @param       data  : USB���̼�ֵ
 * @retval      ��
 */
void keybrd_data_process(uint8_t data)
{
    static uint16_t pos;
    static uint16_t endx, endy;
    static uint16_t maxinputchar;

    uint8_t buf[4];

    if (g_usb_first_plugin_flag)    /* ��һ�β���,���������� */
    {
        g_usb_first_plugin_flag = 0;
        endx = ((lcddev.width - 30) / 8) * 8 + 30;      /* �õ�endxֵ */
        endy = ((lcddev.height - 220) / 16) * 16 + 220; /* �õ�endyֵ */
        maxinputchar = ((lcddev.width - 30) / 8);
        maxinputchar *= (lcddev.height - 220) / 16;     /* ��ǰLCD��������ʾ���ַ��� */
        pos = 0;
    }

    sprintf((char *)buf, "%02X", data);
    lcd_show_string(30 + 56, 180, 200, 16, 16, (char *)buf, BLUE);  /* ��ʾ��ֵ */

    if (data >= ' ' && data <= '~')
    {
        g_temp_buffer[pos++] = data;
        g_temp_buffer[pos] = 0; /* ��ӽ����� */

        if (pos > maxinputchar)pos = maxinputchar;  /* ���������ô�� */
    }
    else if (data == 0X0D)      /* �˸�� */
    {
        if (pos)pos--;

        g_temp_buffer[pos] = 0; /* ��ӽ����� */
    }

    if (pos <= maxinputchar)    /* û�г�����ʾ�� */
    {
        lcd_fill(30, 220, endx, endy, WHITE);
        lcd_show_string(30, 220, endx - 30, endy - 220, 16, (char *)g_temp_buffer, BLUE);
    }

    //printf("KEY Board Value:%02X\r\n",data);
    //printf("KEY Board Char:%c\r\n",data);
}

/**
 * @brief       USB����/�����ʾdemo���ݴ���
 * @param       phost : USBH���ָ��
 * @retval      ��
 */
void usb_demo(USBH_HandleTypeDef *phost)
{
    char c;
    HID_KEYBD_Info_TypeDef *k_pinfo;
    HID_MOUSE_Info_TypeDef *m_pinfo;

    if (App_State == APPLICATION_READY)
    {
        if (USBH_HID_GetDeviceType(phost) == HID_KEYBOARD)  /* �����豸 */
        {
            k_pinfo = USBH_HID_GetKeybdInfo(phost);         /* ��ȡ������Ϣ */

            if (k_pinfo != NULL)
            {
                c = USBH_HID_GetASCIICode(k_pinfo);         /* ת����ASCII�� */
                keybrd_data_process(c);                     /* ��LCD����ʾ�������ַ� */
            }
        }
        else if (USBH_HID_GetDeviceType(phost) == HID_MOUSE)/* ����豸 */
        {
            m_pinfo = USBH_HID_GetMouseInfo(phost);         /* ��ȡ�����Ϣ */

            if (m_pinfo != NULL)
            {
                mouse_data_process(&mouse_info);            /* LCD����ʾ�����Ϣ */
            }
        }
    }
}

/****************************************************************************************************************************/
/* ���´��� usbh_check_enume_dead �� usbh_hid_reconnect ����HID��������,��ߴ��뽡׳�� */

/**
 * @brief       USBö��״̬�������,��ֹUSBö��ʧ�ܵ��µ�����
 * @param       phost : USB HOST�ṹ��ָ��
 * @retval      0, û������
 *              1, ������,�ⲿ������������USB����.
 */
uint8_t usbh_check_enume_dead(USBH_HandleTypeDef *phost)
{
    static uint16_t errcnt = 0;

    /* ���״̬,�����������,��˵��USB������ */
    if (phost->gState == HOST_ENUMERATION && (phost->EnumState == ENUM_IDLE || phost->EnumState == ENUM_GET_FULL_DEV_DESC))
    {
        errcnt++;

        if (errcnt > 2000)              /* ������ */
        {
            errcnt = 0;
            return 1;
        }
    }
    else 
    {
        errcnt = 0;
    }

    return 0;
}

/**
 * @brief       USB HID��������
 * @param       ��
 * @retval      ��
 */
void usbh_hid_reconnect(void)
{
    /* �ر�֮ǰ������ */
    HID_Class.DeInit(&hUSBHost);    /* ��λHID */
    USBH_DeInit(&hUSBHost);         /* ��λUSB HOST */
    
    /* ���¸�λUSB */
    RCC->AHB2RSTR |= 1 << 7;        /* USB OTG FS ��λ */
    delay_ms(5);
    RCC->AHB2RSTR &= ~(1 << 7);     /* ��λ���� */

    memset(&hUSBHost, 0, sizeof(hUSBHost)); /* �������� */
    
    /* ��������USB HID�豸 */
    USBH_Init(&hUSBHost, USBH_UserProcess, 0);
    USBH_RegisterClass(&hUSBHost, USBH_HID_CLASS);
    USBH_Start(&hUSBHost);
}
/****************************************************************************************************************************/

int main(void)
{
    HAL_Init();                                 /* ��ʼ��HAL�� */
    sys_stm32_clock_init(336, 8, 2, 7);         /* ����ʱ��,168Mhz */
    delay_init(168);                            /* ��ʱ��ʼ�� */
    usart_init(115200);                         /* ���ڳ�ʼ��Ϊ115200 */
    led_init();                                 /* ��ʼ��LED */
    lcd_init();                                 /* ��ʼ��LCD */
    sram_init();                                /* ��ʼ���ⲿSRAM */

    my_mem_init(SRAMIN);                        /* ��ʼ���ڲ�SRAM�ڴ�� */
    my_mem_init(SRAMEX);                        /* ��ʼ���ⲿSRAM�ڴ�� */
    my_mem_init(SRAMCCM);                       /* ��ʼ��CCM�ڴ�� */

    lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
    lcd_show_string(30, 70, 200, 16, 16, "USB MOUSE/KEYBOARD TEST", RED);
    lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
    lcd_show_string(30, 130, 200, 16, 16, "USB Connecting...", RED);

    USBH_Init(&hUSBHost, USBH_UserProcess, 0);
    USBH_RegisterClass(&hUSBHost, USBH_HID_CLASS);
    USBH_Start(&hUSBHost);

    while (1)
    {
        USBH_Process(&hUSBHost);
        usb_demo(&hUSBHost);

        if(usbh_check_enume_dead(&hUSBHost))    /* ���USB HOST ö���Ƿ�������?������,�����³�ʼ�� */
        {
            printf("Error! USB HID reconnect!\r\n");
            usbh_hid_reconnect();//����
        }
    }
}












