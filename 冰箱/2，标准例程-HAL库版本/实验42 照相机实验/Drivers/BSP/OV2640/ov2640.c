/**
 ****************************************************************************************************
 * @file        ov2640.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-08
 * @brief       OV2640 ��������
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
#include "./BSP/OV2640/ov2640.h"
#include "./BSP/OV2640/ov2640cfg.h"
#include "./SYSTEM/usart/usart.h"
#include "./SYSTEM/delay/delay.h"


/**
 * @brief       OV2640 ���Ĵ���
 * @param       reg :  �Ĵ�����ַ
 * @retval      �����ļĴ���ֵ
 */
uint8_t ov2640_read_reg(uint16_t reg)
{
    uint8_t data = 0;
    
    sccb_start();                       /* ����SCCB���� */
    sccb_send_byte(OV2640_ADDR);        /* д����ADDR */
    delay_us(100);
    sccb_send_byte(reg);                /* д�Ĵ�����ַ */
    delay_us(100);
    sccb_stop();
    delay_us(100);
    
    /* ���üĴ�����ַ�󣬲��Ƕ� */
    sccb_start();
    sccb_send_byte(OV2640_ADDR | 0x01); /* ���Ͷ����� */
    delay_us(100);
    data = sccb_read_byte();            /* ��ȡ���� */
    sccb_nack();
    sccb_stop();
    
    return data;
}

/**
 * @brief       OV2640 д�Ĵ���
 * @param       reg : �Ĵ�����ַ
 * @param       data: Ҫд��Ĵ�����ֵ
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov2640_write_reg(uint16_t reg, uint8_t data)
{
    uint8_t res = 0;
    
    sccb_start();                           /* ����SCCB���� */
    delay_us(100);
    if (sccb_send_byte(OV2640_ADDR))res = 1;/* д����ID */
    delay_us(100);
    if (sccb_send_byte(reg))res = 1;        /* д�Ĵ�����ַ */
    delay_us(100);
    if (sccb_send_byte(data))res = 1;       /* д���� */
    delay_us(100);
    sccb_stop();
    
    return res;
}

/**
 * @brief       ��ʼ�� OV2640
 * @param       ��
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t ov2640_init(void)
{
    uint16_t i = 0;
    uint16_t reg;
    
    GPIO_InitTypeDef gpio_init_struct;
    
    OV_PWDN_GPIO_CLK_ENABLE();      /* ʹ��OV_PWDN��ʱ�� */
    OV_RESET_GPIO_CLK_ENABLE();     /* ʹ��OV_RESET��ʱ�� */

    gpio_init_struct.Pin = OV_PWDN_GPIO_PIN;
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;            /* ������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                    /* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;     /* ���� */
    HAL_GPIO_Init(OV_PWDN_GPIO_PORT, &gpio_init_struct);    /* ��ʼ��OV_PWDN���� */

    gpio_init_struct.Pin = OV_RESET_GPIO_PIN;
    HAL_GPIO_Init(OV_RESET_GPIO_PORT, &gpio_init_struct);   /* ��ʼ��OV_RESET���� */

    OV2640_PWDN(0);     /* POWER ON */
    delay_ms(10);
    OV2640_RST(0);      /* ����������OV2640��RST��,���ϵ� */
    delay_ms(20);
    OV2640_RST(1);      /* ������λ */
    delay_ms(20);
    
    sccb_init();        /* ��ʼ��SCCB ��IO�� */
    delay_ms(5);
    ov2640_write_reg(OV2640_DSP_RA_DLMT, 0x01);     /* ����sensor�Ĵ��� */
    ov2640_write_reg(OV2640_SENSOR_COM7, 0x80);     /* ��λOV2640 */
    delay_ms(50);
    reg = ov2640_read_reg(OV2640_SENSOR_MIDH);      /* ��ȡ����ID �߰�λ */
    reg <<= 8;
    reg |= ov2640_read_reg(OV2640_SENSOR_MIDL);     /* ��ȡ����ID �Ͱ�λ */

    if (reg != OV2640_MID)      /* ID �Ƿ����� */
    {
        printf("MID:%d\r\n", reg);
        return 1;               /* ʧ�� */
    }
    
    reg = ov2640_read_reg(OV2640_SENSOR_PIDH);  /* ��ȡ����ID �߰�λ */
    reg <<= 8;
    reg |= ov2640_read_reg(OV2640_SENSOR_PIDL); /* ��ȡ����ID �Ͱ�λ */

    if (reg != OV2640_PID)      /* ID�Ƿ����� */
    {
        printf("HID:%d\r\n", reg);
        return 1;   /* ʧ�� */
    }

    /* ��ʼ�� OV2640 */
    for (i = 0; i < sizeof(ov2640_uxga_init_reg_tbl) / 2; i++)
    {
        ov2640_write_reg(ov2640_uxga_init_reg_tbl[i][0], ov2640_uxga_init_reg_tbl[i][1]);
    }

    return 0;       /* OV2640��ʼ����� */
}

/**
 * @brief       OV2640 �л�ΪJPEGģʽ
 * @param       ��
 * @retval      ��
 */
void ov2640_jpeg_mode(void)
{
    uint16_t i = 0;

    /* ����:YUV422��ʽ */
    for (i = 0; i < (sizeof(ov2640_yuv422_reg_tbl) / 2); i++)
    {
        ov2640_write_reg(ov2640_yuv422_reg_tbl[i][0], ov2640_yuv422_reg_tbl[i][1]); /* ������������ */
    }

    /* ����:���JPEG���� */
    for (i = 0; i < (sizeof(ov2640_jpeg_reg_tbl) / 2); i++)
    {
        ov2640_write_reg(ov2640_jpeg_reg_tbl[i][0], ov2640_jpeg_reg_tbl[i][1]);     /* ������������ */
    }
}

/**
 * @brief       OV2640 �л�ΪRGB565ģʽ
 * @param       ��
 * @retval      ��
 */
void ov2640_rgb565_mode(void)
{
    uint16_t i = 0;

    /* ����:RGB565��� */
    for (i = 0; i < (sizeof(ov2640_rgb565_reg_tbl) / 4); i++)
    {
        ov2640_write_reg(ov2640_rgb565_reg_tbl[i][0], ov2640_rgb565_reg_tbl[i][1]); /* ������������ */
    }
}

/* �Զ��ع����ò�����,֧��5���ȼ� */
const static uint8_t OV2640_AUTOEXPOSURE_LEVEL[5][8]=
{
    {
        0xFF, 0x01,
        0x24, 0x20,
        0x25, 0x18,
        0x26, 0x60,
    },
    {
        0xFF, 0x01,
        0x24, 0x34,
        0x25, 0x1c,
        0x26, 0x00,
    },
    {
        0xFF, 0x01,
        0x24, 0x3e,
        0x25, 0x38,
        0x26, 0x81,
    },
    {
        0xFF, 0x01,
        0x24, 0x48,
        0x25, 0x40,
        0x26, 0x81,
    },
    {
        0xFF, 0x01,
        0x24, 0x58,
        0x25, 0x50,
        0x26, 0x92,
    },
};

/**
 * @brief       OV2640 EV�عⲹ��
 * @param       level : 0~4
 * @retval      ��
 */
void ov2640_auto_exposure(uint8_t level)
{
    uint8_t i;
    uint8_t *p = (uint8_t*)OV2640_AUTOEXPOSURE_LEVEL[level];
    
    for (i = 0; i < 4; i++)
    { 
        ov2640_write_reg(p[i * 2], p[i * 2 + 1]); 
    }
}

/**
 * @brief       OV2640 ��ƽ������
 * @param       mode : 0~4, ��ƽ��ģʽ
 *   @arg       0: �Զ�   auto
 *   @arg       1: �չ�   sunny
 *   @arg       2: �칫�� office
 *   @arg       3: ����   cloudy
 *   @arg       4: ����   home
 * @retval      ��
 */
void ov2640_light_mode(uint8_t mode)
{
    uint8_t regccval = 0x5E;/* Sunny */
    uint8_t regcdval = 0x41;
    uint8_t regceval = 0x54;
    
    switch (mode)
    { 
        case 0:             /* auto */
            ov2640_write_reg(0xFF, 0x00);
            ov2640_write_reg(0xC7, 0x00);    /* AWB ON  */
            return;
        case 2:             /* cloudy */
            regccval = 0x65;
            regcdval = 0x41;
            regceval = 0x4F;
            break;
        case 3:             /* office */
            regccval = 0x52;
            regcdval = 0x41;
            regceval = 0x66;
            break;
        case 4:             /* home */
            regccval = 0x42;
            regcdval = 0x3F;
            regceval = 0x71;
            break;
        default : break;
    }
    
    ov2640_write_reg(0xFF, 0x00);
    ov2640_write_reg(0xC7, 0x40);            /* AWB OFF  */
    ov2640_write_reg(0xCC, regccval);
    ov2640_write_reg(0xCD, regcdval);
    ov2640_write_reg(0xCE, regceval);
}

/**
 * @brief       OV2640 ɫ�ʱ��Ͷ�����
 * @param       set : 0~4, ����ɫ�ʱ��Ͷ� -2 ~ 2.
 * @retval      ��
 */
void ov2640_color_saturation(uint8_t sat)
{
    uint8_t reg7dval = ((sat + 2) << 4) | 0x08;
    
    ov2640_write_reg(0xFF, 0x00);
    ov2640_write_reg(0x7C, 0x00);
    ov2640_write_reg(0x7D, 0x02);
    ov2640_write_reg(0x7C, 0x03);
    ov2640_write_reg(0x7D, reg7dval);
    ov2640_write_reg(0x7D, reg7dval);
}

/**
 * @brief       OV2640 ��������
 * @param       bright : 0~5, �������� -2 ~ 2.
 * @retval      ��
 */
void ov2640_brightness(uint8_t bright)
{
    ov2640_write_reg(0xff, 0x00);
    ov2640_write_reg(0x7c, 0x00);
    ov2640_write_reg(0x7d, 0x04);
    ov2640_write_reg(0x7c, 0x09);
    ov2640_write_reg(0x7d, bright << 4); 
    ov2640_write_reg(0x7d, 0x00); 
}

/**
 * @brief       OV2640 �Աȶ�����
 * @param       contrast : 0~4, ����Աȶ� -2 ~ 2.
 * @retval      ��
 */
void ov2640_contrast(uint8_t contrast)
{
    uint8_t reg7d0val = 0x20;       /* Ĭ��Ϊ��ͨģʽ */
    uint8_t reg7d1val = 0x20;
    
    switch (contrast)
    {
        case 0:
            reg7d0val = 0x18;       /* -2 */
            reg7d1val = 0x34;
            break;
        case 1:
            reg7d0val = 0x1C;       /* -1 */
            reg7d1val = 0x2A;
            break;
        case 3:
            reg7d0val = 0x24;       /* 1 */
            reg7d1val = 0x16;
            break;
        case 4:
            reg7d0val = 0x28;       /* 2 */
            reg7d1val = 0x0C;
            break;
        default : break;
    }
    
    ov2640_write_reg(0xff, 0x00);
    ov2640_write_reg(0x7c, 0x00);
    ov2640_write_reg(0x7d, 0x04);
    ov2640_write_reg(0x7c, 0x07);
    ov2640_write_reg(0x7d, 0x20);
    ov2640_write_reg(0x7d, reg7d0val);
    ov2640_write_reg(0x7d, reg7d1val);
    ov2640_write_reg(0x7d, 0x06);
}

/**
 * @brief       OV2640 ��Ч����
 * @param       eft : 0~6, ��ЧЧ��
 *   @arg       0: ����
 *   @arg       1: ��Ƭ
 *   @arg       2: �ڰ�
 *   @arg       3: ƫ��ɫ
 *   @arg       4: ƫ��ɫ
 *   @arg       5: ƫ��ɫ
 *   @arg       6: ����
 * @retval      ��
 */
void ov2640_special_effects(uint8_t eft)
{
    uint8_t reg7d0val = 0x00;   /* Ĭ��Ϊ��ͨģʽ */
    uint8_t reg7d1val = 0x80;
    uint8_t reg7d2val = 0x80; 
    
    switch(eft)
    {
        case 1:     /* ��Ƭ */
            reg7d0val = 0x40; 
            break;
        case 2:     /* �ڰ� */
            reg7d0val = 0x18; 
            break;
        case 3:     /* ƫ��ɫ */
            reg7d0val = 0x18; 
            reg7d1val = 0x40;
            reg7d2val = 0xC0; 
            break;
        case 4:     /* ƫ��ɫ */
            reg7d0val = 0x18; 
            reg7d1val = 0x40;
            reg7d2val = 0x40; 
            break;
        case 5:     /* ƫ��ɫ */
            reg7d0val = 0x18; 
            reg7d1val = 0xA0;
            reg7d2val = 0x40; 
            break;
        case 6:     /* ���� */
            reg7d0val = 0x18; 
            reg7d1val = 0x40;
            reg7d2val = 0xA6; 
            break;
    }
    
    ov2640_write_reg(0xff, 0x00);
    ov2640_write_reg(0x7c, 0x00);
    ov2640_write_reg(0x7d, reg7d0val);
    ov2640_write_reg(0x7c, 0x05);
    ov2640_write_reg(0x7d, reg7d1val);
    ov2640_write_reg(0x7d, reg7d2val); 
}

/**
 * @brief       OV2640 ��������
 * @param       mode : 0~2
 *   @arg       0: �ر� 
 *   @arg       1: ����
 * @retval      ��
 */
void ov2640_color_bar(uint8_t mode)
{
    uint8_t reg;
    
    ov2640_write_reg(0xFF, 0x01);
    reg = ov2640_read_reg(0x12);
    reg &= ~(1 << 1);
    if (mode)reg |=1 << 1;
    ov2640_write_reg(0x12, reg);
}

/**
 * @bref        ���ô������������
 * @param       sx    : ��ʼ��ַx
 * @param       sy    : ��ʼ��ַy
 * @param       width : ���(��Ӧ:horizontal)
 * @param       hight : �߶�(��Ӧ:vertical)
 * @retval      ��
 */
void ov2640_window_set(uint16_t sx, uint16_t sy, uint16_t width, uint16_t height)
{
    uint16_t endx;
    uint16_t endy;
    uint8_t temp; 
    
    endx = sx + width / 2;
    endy = sy + height / 2;

    ov2640_write_reg(0xFF, 0x01);    
    temp = ov2640_read_reg(0x03);       /* ��ȡVref֮ǰ��ֵ */
    temp &= 0xF0;
    temp |= ((endy & 0x03) << 2) | (sy & 0x03);
    ov2640_write_reg(0x03, temp);       /* ����Vref��start��end�����2λ */ 
    ov2640_write_reg(0x19, sy >> 2);    /* ����Vref��start��8λ */
    ov2640_write_reg(0x1A, endy >> 2);  /* ����Vref��end�ĸ�8λ */

    temp = ov2640_read_reg(0x32);       /* ��ȡHref֮ǰ��ֵ */
    temp &= 0xC0;
    temp |= ((endx & 0x07) << 3) | (sx & 0x07);
    ov2640_write_reg(0x32, temp);       /* ����Href��start��end�����3λ */
    ov2640_write_reg(0x17, sx >> 3);    /* ����Href��start��8λ */
    ov2640_write_reg(0x18, endx >> 3);  /* ����Href��end�ĸ�8λ */
}

/** 
 * @bref        ����ͼ�������С
 * @@param      width : ���(��Ӧ:horizontal)
 * @param       height: �߶�(��Ӧ:vertical)
 * @note        OV2640���ͼ��Ĵ�С(�ֱ���),��ȫ�ɸú���ȷ��
 *              width��height������4�ı���
 * @retval      0     : ���óɹ�
 *              ����  : ����ʧ��
 */
uint8_t ov2640_outsize_set(uint16_t width, uint16_t height)
{
    uint16_t outh;
    uint16_t outw;
    uint8_t temp;

    if (width % 4) return 1;
    if (height % 4) return 2;

    outw = width / 4;
    outh = height/ 4;
    ov2640_write_reg(0xFF, 0x00);    
    ov2640_write_reg(0xE0, 0x04);
    ov2640_write_reg(0x5A, outw & 0xFF);    /* ����OUTW�ĵͰ�λ */
    ov2640_write_reg(0x5B, outh & 0xFF);    /* ����OUTH�ĵͰ�λ */

    temp = (outw >> 8) & 0x03;
    temp |= (outh >> 6) & 0x04;
    ov2640_write_reg(0x5C, temp);           /* ����OUTH/OUTW�ĸ�λ */
    ov2640_write_reg(0xE0, 0x00);

    return 0;
}

/**
 * @brief       ����ͼ�񿪴���С
 *   @note      OV2640���ͼ��Ĵ�С(�ֱ���),��ȫ�ɸú���ȷ��
 *              ���ڿ�����scale����,���������ͼ�񴰿�Ϊ: xsize - 2 * offx, ysize - 2 * offy
 *              ʵ�����(width,height), ����: xsize - 2 * offx, ysize - 2 * offy�Ļ����Ͻ������Ŵ���.
 *              һ������offx��offy��ֵΪ16��4,��СҲ�ǿ���,����Ĭ����16��4
 * @param       offx,offy    : ���ͼ����ov2640_image_window_set�趨����(���賤��Ϊxsize��ysize)�ϵ�ƫ��
 * @param       width,height : ʵ�����ͼ��Ŀ�Ⱥ͸߶�,������4�ı���
 * @retval      0, �ɹ�; ����, ʧ��;
 */
uint8_t ov2640_image_win_set(uint16_t offx, uint16_t offy, uint16_t width, uint16_t height)
{
    uint16_t hsize;
    uint16_t vsize;
    uint8_t temp;

    if (width % 4) return 1;
    if (height % 4) return 2;
    hsize = width / 4;
    vsize = height / 4;
    ov2640_write_reg(0xFF, 0x00);
    ov2640_write_reg(0xE0, 0x04);
    ov2640_write_reg(0x51, hsize & 0xFF);           /* ����H_SIZE�ĵͰ�λ */
    ov2640_write_reg(0x52, vsize & 0xFF);           /* ����V_SIZE�ĵͰ�λ */
    ov2640_write_reg(0x53, offx & 0xFF);            /* ����offx�ĵͰ�λ */
    ov2640_write_reg(0x54, offy & 0xFF);            /* ����offy�ĵͰ�λ */
    temp = (vsize >> 1) & 0x80;
    temp |= (offy >> 4) & 0x70;
    temp |= (hsize>>5) & 0x08;
    temp |= (offx >> 8) & 0x07; 
    ov2640_write_reg(0x55, temp);                   /* ����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ */
    ov2640_write_reg(0x57, (hsize >> 2) & 0x80);    /* ����H_SIZE/V_SIZE/OFFX,OFFY�ĸ�λ */
    ov2640_write_reg(0xE0, 0x00);

    return 0;
}

/**
 * @bref    �ú�������ͼ��ߴ��С,Ҳ������ѡ��ʽ������ֱ���
 * @note    UXGA:1600*1200,SVGA:800*600,CIF:352*288
 * @param   width  : ͼ����
 * @param   height : ͼ��߶�
 *
 * @retval  0      : ���óɹ�
 *          ����   : ����ʧ��
 */
uint8_t ov2640_imagesize_set(uint16_t width, uint16_t height)
{ 
    uint8_t temp; 
    
    ov2640_write_reg(0xFF, 0x00);
    ov2640_write_reg(0xE0, 0x04);
    ov2640_write_reg(0xC0, (width) >> 3 & 0xFF);    /* ����HSIZE��10:3λ */
    ov2640_write_reg(0xC1, (height) >> 3 & 0xFF);   /* ����VSIZE��10:3λ */

    temp = (width & 0x07) << 3;
    temp |= height & 0x07;
    temp |= (width >> 4) & 0x80;

    ov2640_write_reg(0x8C, temp);
    ov2640_write_reg(0xE0, 0x00);

    return 0;
}

/**
 * @bref        ������ⲿ�źſ���
 * @param       sw : 0,�ر�
 *                   1,��
 * @retval      ��
 */
void ov2640_flash_extctrl(uint8_t sw)
{
    if(sw) OV2640_FLASH(1);
    else OV2640_FLASH(0);
}

/**
 * @bref        ������ڲ��źſ���
 * @note        �����źŴ�OV2640��STROBE�����Զ����
 * @retval      ��
 */
void ov2640_flash_intctrl(void)
{
    ov2640_write_reg(0xFF, 0x01);
    ov2640_write_reg(0x4B, 0x81);
    ov2640_write_reg(0x4B, 0x01);
}


