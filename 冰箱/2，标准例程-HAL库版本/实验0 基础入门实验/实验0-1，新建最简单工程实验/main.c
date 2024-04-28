/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-12-29
 * @brief       ���STM32����, ���������ļ�(.s�ļ�), δʹ���κο��ļ�
 *              �ô���ʵ�ֹ��ܣ�ͨ��PF9����DS0��˸
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
 * V1.0 20201229
 * ��һ�η���
 *
 ****************************************************************************************************
 */


/* ���߻���ַ���� */
#define PERIPH_BASE         0x40000000                          /* �������ַ */

#define APB1PERIPH_BASE     PERIPH_BASE                         /* APB1���߻���ַ */
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000)          /* APB2���߻���ַ */
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000)          /* AHB1���߻���ַ */
#define AHB2PERIPH_BASE     (PERIPH_BASE + 0x10000000)          /* AHB2���߻���ַ */

/* �������ַ���� */
#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800)          /* RCC����ַ */
#define GPIOF_BASE          (AHB1PERIPH_BASE + 0x1400)         /* GPIOF����ַ */


/* ������ؼĴ���ӳ��(����) */
#define RCC_AHB1ENR         *(volatile unsigned int *)(RCC_BASE + 0x30)     /* RCC_AHB1ENR�Ĵ���ӳ�� */

#define GPIOF_MODER         *(volatile unsigned int *)(GPIOF_BASE + 0x00)   /* GPIOF_MODER�Ĵ���ӳ�� */
#define GPIOF_OTYPER        *(volatile unsigned int *)(GPIOF_BASE + 0x04)   /* GPIOF_OTYPER�Ĵ���ӳ�� */
#define GPIOF_OSPEEDR       *(volatile unsigned int *)(GPIOF_BASE + 0x08)   /* GPIOF_OSPEEDR�Ĵ���ӳ�� */
#define GPIOF_PUPDR         *(volatile unsigned int *)(GPIOF_BASE + 0x0C)   /* GPIOF_PUPDR�Ĵ���ӳ�� */
#define GPIOF_IDR           *(volatile unsigned int *)(GPIOF_BASE + 0x10)   /* GPIOF_IDR�Ĵ���ӳ�� */
#define GPIOF_ODR           *(volatile unsigned int *)(GPIOF_BASE + 0x14)   /* GPIOF_ODR�Ĵ���ӳ�� */


/* ��ʱ���� */
static void delay_x(volatile unsigned int t)
{
    while(t--);
}

/* main���� */
int main(void)
{
    /* δִ���κ�PLLʱ������, Ĭ��ʹ��HSI(8M)����, �൱�ڹ�������Ƶ8MhzƵ���� */

    RCC_AHB1ENR |= 1 << 5;              /* GPIOF ʱ��ʹ�� */

    GPIOF_MODER &= ~(0X03UL << 18);     /* MODER9[1:0] ���� */
    GPIOF_MODER |= 0X01UL << 18;        /* MODER9[1:0]=1, PF9���ģʽ */
    GPIOF_OTYPER &= ~(0X01UL << 9);     /* OT9, ����, ������� */
    GPIOF_OSPEEDR &= ~(0X03UL << 18);   /* OSPEEDR9[1:0], ���� */
    GPIOF_OSPEEDR |= 0X01UL << 18;      /* OSPEEDR9[1:0]=1, ���� */
    GPIOF_PUPDR &= ~(0X03UL << 18);     /* PUPDR9[1:0], ���� */
    GPIOF_PUPDR |= 0X01UL << 18;        /* PUPDR9[1:0]=1, ���� */

    while(1)
    {
        GPIOF_ODR |= 1 << 9;            /* PF9 = 1, LED0�� */
        delay_x(500000);                /* ��ʱһ��ʱ�� */
        GPIOF_ODR &= ~(1UL << 9);       /* PF9 = 0, LED0�� */
        delay_x(500000);                /* ��ʱһ��ʱ�� */
    }
}
























