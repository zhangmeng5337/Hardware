/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-12-29
 * @brief       最精简STM32工程, 除了启动文件(.s文件), 未使用任何库文件
 *              该代码实现功能：通过PF9控制DS0闪烁
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20201229
 * 第一次发布
 *
 ****************************************************************************************************
 */


/* 总线基地址定义 */
#define PERIPH_BASE         0x40000000                          /* 外设基地址 */

#define APB1PERIPH_BASE     PERIPH_BASE                         /* APB1总线基地址 */
#define APB2PERIPH_BASE     (PERIPH_BASE + 0x00010000)          /* APB2总线基地址 */
#define AHB1PERIPH_BASE     (PERIPH_BASE + 0x00020000)          /* AHB1总线基地址 */
#define AHB2PERIPH_BASE     (PERIPH_BASE + 0x10000000)          /* AHB2总线基地址 */

/* 外设基地址定义 */
#define RCC_BASE            (AHB1PERIPH_BASE + 0x3800)          /* RCC基地址 */
#define GPIOF_BASE          (AHB1PERIPH_BASE + 0x1400)         /* GPIOF基地址 */


/* 外设相关寄存器映射(定义) */
#define RCC_AHB1ENR         *(volatile unsigned int *)(RCC_BASE + 0x30)     /* RCC_AHB1ENR寄存器映射 */

#define GPIOF_MODER         *(volatile unsigned int *)(GPIOF_BASE + 0x00)   /* GPIOF_MODER寄存器映射 */
#define GPIOF_OTYPER        *(volatile unsigned int *)(GPIOF_BASE + 0x04)   /* GPIOF_OTYPER寄存器映射 */
#define GPIOF_OSPEEDR       *(volatile unsigned int *)(GPIOF_BASE + 0x08)   /* GPIOF_OSPEEDR寄存器映射 */
#define GPIOF_PUPDR         *(volatile unsigned int *)(GPIOF_BASE + 0x0C)   /* GPIOF_PUPDR寄存器映射 */
#define GPIOF_IDR           *(volatile unsigned int *)(GPIOF_BASE + 0x10)   /* GPIOF_IDR寄存器映射 */
#define GPIOF_ODR           *(volatile unsigned int *)(GPIOF_BASE + 0x14)   /* GPIOF_ODR寄存器映射 */


/* 延时函数 */
static void delay_x(volatile unsigned int t)
{
    while(t--);
}

/* main函数 */
int main(void)
{
    /* 未执行任何PLL时钟配置, 默认使用HSI(8M)工作, 相当于工作在主频8Mhz频率下 */

    RCC_AHB1ENR |= 1 << 5;              /* GPIOF 时钟使能 */

    GPIOF_MODER &= ~(0X03UL << 18);     /* MODER9[1:0] 清零 */
    GPIOF_MODER |= 0X01UL << 18;        /* MODER9[1:0]=1, PF9输出模式 */
    GPIOF_OTYPER &= ~(0X01UL << 9);     /* OT9, 清零, 推挽输出 */
    GPIOF_OSPEEDR &= ~(0X03UL << 18);   /* OSPEEDR9[1:0], 清零 */
    GPIOF_OSPEEDR |= 0X01UL << 18;      /* OSPEEDR9[1:0]=1, 中速 */
    GPIOF_PUPDR &= ~(0X03UL << 18);     /* PUPDR9[1:0], 清零 */
    GPIOF_PUPDR |= 0X01UL << 18;        /* PUPDR9[1:0]=1, 上拉 */

    while(1)
    {
        GPIOF_ODR |= 1 << 9;            /* PF9 = 1, LED0灭 */
        delay_x(500000);                /* 延时一定时间 */
        GPIOF_ODR &= ~(1UL << 9);       /* PF9 = 0, LED0亮 */
        delay_x(500000);                /* 延时一定时间 */
    }
}
























