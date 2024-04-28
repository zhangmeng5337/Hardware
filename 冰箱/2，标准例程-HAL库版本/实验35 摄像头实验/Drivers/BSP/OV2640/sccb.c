/**
 ****************************************************************************************************
 * @file        sccb.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-08
 * @brief       SCCB ��������
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
 * V1.0 20211108
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#include "./BSP/OV2640/sccb.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       ��ʼ��SCCB
 * @param       ��
 * @retval      ��
 */
void sccb_init(void)
{
    SCCB_SCL_GPIO_CLK_ENABLE();     /* SCL����ʱ��ʹ�� */
    SCCB_SDA_GPIO_CLK_ENABLE();     /* SDA����ʱ��ʹ�� */

    /* SCL����ģʽ����,��©���,���� */
    GPIO_InitTypeDef gpio_initure;
    gpio_initure.Pin=SCCB_SCL_GPIO_PIN;
    gpio_initure.Mode=GPIO_MODE_OUTPUT_PP;      
    gpio_initure.Pull=GPIO_PULLUP;          
    gpio_initure.Speed=GPIO_SPEED_FREQ_VERY_HIGH;
    HAL_GPIO_Init(SCCB_SCL_GPIO_PORT,&gpio_initure);    /* SCL����ģʽ����,��©���,���� */

    /* SDA����ģʽ����,��©���,����, �����Ͳ���������IO������, ��©�����ʱ��(=1), Ҳ���Զ�ȡ�ⲿ�źŵĸߵ͵�ƽ */
    gpio_initure.Pin=SCCB_SDA_GPIO_PIN;
 #if OV_SCCB_TYPE_NOD
     gpio_initure.Mode = GPIO_MODE_OUTPUT_PP;      
 #else
     gpio_initure.Mode = GPIO_MODE_OUTPUT_OD;
 #endif
    HAL_GPIO_Init(SCCB_SDA_GPIO_PORT,&gpio_initure);    /* SDA����ģʽ����,��©���,���� */

#if OV_SCCB_TYPE_NOD
    SCCB_SDA_OUT();      /* ����SDAΪ��� */
    delay_us(50);
#endif
    sccb_stop();    /* ֹͣ�����������豸 */
}

/**
 * @brief       SCCB��ʱ����,���ڿ���IIC��д�ٶ�
 * @param       ��
 * @retval      ��
 */
static void sccb_delay(void)
{
    delay_us(50);
}

/**
 * @brief       ����SCCB��ʼ�ź�
 * @param       ��
 * @retval      ��
 */
void sccb_start(void)
{
    SCCB_SDA(1);
    SCCB_SCL(1);
    sccb_delay();
    SCCB_SDA(0);    /* START�ź�: ��SCLΪ��ʱ, SDA�Ӹ߱�ɵ�, ��ʾ��ʼ�ź� */
    sccb_delay();
    SCCB_SCL(0);    /* ǯסSCCB���ߣ�׼�����ͻ�������� */
}

/**
 * @brief       ����SCCBֹͣ�ź�
 * @param       ��
 * @retval      ��
 */
void sccb_stop(void)
{
    SCCB_SDA(0);    /* STOP�ź�: ��SCLΪ��ʱ, SDA�ӵͱ�ɸ�, ��ʾֹͣ�ź� */
    sccb_delay();
    SCCB_SCL(1);
    sccb_delay();
    SCCB_SDA(1);    /* ����SCCB���߽����ź� */
    sccb_delay();
}

/**
 * @brief       ������ACKӦ��
 * @param       ��
 * @retval      ��
 */
void sccb_nack(void)
{
    sccb_delay();
    SCCB_SDA(1);    /* SCL 0 -> 1  ʱ SDA = 1, ��ʾ��Ӧ�� */
    SCCB_SCL(1);
    sccb_delay();
    SCCB_SCL(0);    /* ����һ��ʱ�� */
    sccb_delay();
    SCCB_SDA(0);
    sccb_delay();
}

/**
 * @brief       SCCB  ����һ���ֽ�
 * @param       data: Ҫ���͵�����
 * @retval      res
 */
uint8_t sccb_send_byte(uint8_t data)
{
    uint8_t t, res;
    
    for (t = 0; t < 8; t++)
    {
        SCCB_SDA((data & 0x80) >> 7);   /* ��λ�ȷ��� */
        sccb_delay();
        SCCB_SCL(1);
        sccb_delay();
        SCCB_SCL(0);
        data <<= 1;     /* ����1λ,������һ�η��� */
    }
#if OV_SCCB_TYPE_NOD
    SCCB_SDA_IN();      /* ����SDAΪ���� */
    sccb_delay();
#endif
    SCCB_SDA(1);        /* �������, �����ͷ�SDA�� */
    sccb_delay();
    SCCB_SCL(1);        /* ���յھ�λ,���ж��Ƿ��ͳɹ� */
    sccb_delay();

    if (SCCB_READ_SDA)
    {
        res = 1;        /* SDA=1����ʧ�ܣ�����1 */
    }
    else 
    {
        res = 0;        /* SDA=0���ͳɹ�������0 */
    } 

    SCCB_SCL(0);
#if OV_SCCB_TYPE_NOD
    SCCB_SDA_OUT();      /* ����SDAΪ��� */
    delay_us(50);
#endif    
    return res;
}

/**
 * @brief       SCCB ��ȡһ���ֽ�
 * @param       ��
 * @retval      ��ȡ��������
 */
uint8_t sccb_read_byte(void)
{
    uint8_t i, receive = 0;
#if OV_SCCB_TYPE_NOD
    SCCB_SDA_IN();          /* ����SDAΪ���� */
    delay_us(50);
#endif
    for (i = 0; i < 8; i++) /* ����1���ֽ����� */
    {
        sccb_delay();
        receive <<= 1;      /* ��λ�����,�������յ�������λҪ���� */
        SCCB_SCL(1);

        if (SCCB_READ_SDA)
        {
            receive++;
        }
        sccb_delay();
        SCCB_SCL(0);
    }
#if OV_SCCB_TYPE_NOD
    SCCB_SDA_OUT();         /* ����SDAΪ��� */
    delay_us(50);
#endif
    return receive;
}

















