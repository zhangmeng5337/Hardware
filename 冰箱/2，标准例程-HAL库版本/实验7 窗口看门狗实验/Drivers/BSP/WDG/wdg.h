/**
 ****************************************************************************************************
 * @file        wdg.h
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.1
 * @date        2021-10-14
 * @brief       看门狗 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 探索者 F407开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20211014
 * 第一次发布
 * V1.1 20211014
 * 新增wwdg_init函数
 *
 ****************************************************************************************************
 */

#ifndef __WDG_H
#define __WDG_H

#include "./SYSTEM/sys/sys.h"


void iwdg_init(uint32_t prer, uint16_t rlr);    /* 初始化IWDG，并使能IWDG */
void iwdg_feed(void);                           /* 喂狗 */

void wwdg_init(uint8_t tr, uint8_t wr, uint32_t fprer); /*窗口看门狗初始化*/
void wwdg_set_counter(uint8_t cnt);                     /*喂窗口看门狗*/

#endif




























