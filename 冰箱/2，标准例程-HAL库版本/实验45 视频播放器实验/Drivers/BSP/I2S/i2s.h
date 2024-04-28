/**
 ****************************************************************************************************
 * @file        i2s.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2021-11-16
 * @brief       I2S ��������
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
 * V1.0 20211116
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#ifndef __I2S_H
#define __I2S_H
#include "./SYSTEM/SYS/sys.h"


#define GPIO_AF_I2S_SPI                 GPIO_AF5_SPI2

#define I2S_LRCK_GPIO_PORT              GPIOB
#define I2S_LRCK_GPIO_PIN               GPIO_PIN_12
#define I2S_LRCK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB��ʱ��ʹ�� */

#define I2S_SCLK_GPIO_PORT              GPIOB
#define I2S_SCLK_GPIO_PIN               GPIO_PIN_13
#define I2S_SCLK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)     /* PB��ʱ��ʹ�� */

#define I2S_SDOUT_GPIO_PORT             GPIOC
#define I2S_SDOUT_GPIO_PIN              GPIO_PIN_2
#define I2S_SDOUT_GPIO_CLK_ENABLE()     do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

#define I2S_SDIN_GPIO_PORT              GPIOC
#define I2S_SDIN_GPIO_PIN               GPIO_PIN_3
#define I2S_SDIN_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

#define I2S_MCLK_GPIO_PORT              GPIOC
#define I2S_MCLK_GPIO_PIN               GPIO_PIN_6
#define I2S_MCLK_GPIO_CLK_ENABLE()      do{ __HAL_RCC_GPIOC_CLK_ENABLE(); }while(0)     /* PC��ʱ��ʹ�� */

#define I2S_SPI                         SPI2
#define I2S_SPI_CLK_ENABLE()            do{ __HAL_RCC_SPI2_CLK_ENABLE(); }while(0)      /* I2S2ʱ��ʹ��*/

#define I2S_TX_DMASx                    DMA1_Stream4
#define I2S_TX_DMASx_Channel            DMA_CHANNEL_0
#define I2S_TX_DMASx_Handle             DMA1_Stream4_IRQHandler
#define I2S_TX_DMASx_IRQ                DMA1_Stream4_IRQn
#define I2S_TX_DMASx_FLAG               DMA_FLAG_TCIF0_4
#define I2S_TX_DMA_CLK_ENABLE()         do{ __HAL_RCC_DMA1_CLK_ENABLE(); }while(0)   /* I2S2 TX DMAʱ��ʹ�� */


/*****************************************************************************************************/

extern void (*i2s_tx_callback)(void);   /* I2S TX�ص�����ָ��  */

void i2s_init(uint32_t i2s_standard, uint32_t i2s_mode, uint32_t i2s_clock_polarity, uint32_t i2s_dataformat);  /* I2S��ʼ�� */
void i2s_dma_enable(void);      /* ����I2S��DMA���� */
uint8_t i2s_samplerate_set(uint32_t samplerate);    /* ����I2S�Ĳ����� */
void i2s_tx_dma_init(uint8_t* buf0, uint8_t *buf1, uint16_t num);   /* I2S TX DMA���� */
void i2s_play_start(void);      /* I2S��ʼ���� */
void i2s_play_stop(void);       /* I2Sֹͣ���� */

#endif





















