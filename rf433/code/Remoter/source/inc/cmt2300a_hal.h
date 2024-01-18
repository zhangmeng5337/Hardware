/*
 * THE FOLLOWING FIRMWARE IS PROVIDED: (1) "AS IS" WITH NO WARRANTY; AND
 * (2)TO ENABLE ACCESS TO CODING INFORMATION TO GUIDE AND FACILITATE CUSTOMER.
 * CONSEQUENTLY, CMOSTEK SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE CONTENT
 * OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING INFORMATION
 * CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
 *
 * Copyright (C) CMOSTEK SZ.
 */

/*!
 * @file    cmt2300a_hal.h
 * @brief   CMT2300A hardware abstraction layer
 *
 * @version 1.2
 * @date    Jul 17 2017
 * @author  CMOSTEK R@D
 */
 
#ifndef __CMT2300A_HAL_H
#define __CMT2300A_HAL_H
#include "BSP_cmt2300a.h"
#include "typedefs.h"
#include "gpio_defs.h"
#include "ddl.h"
#include "main.h"

#ifdef __cplusplus 
extern "C" { 
#endif

/* ************************************************************************
*  The following need to be modified by user
*  ************************************************************************ */
#define CMT2300A_SetGpio1In()           Gpio_InitIOExt(CMT_GPIO1_GPIO,INT1, GpioDirIn, TRUE, FALSE, FALSE, 0)
#define CMT2300A_SetGpio2In()           Gpio_InitIOExt(CMT_GPIO2_GPIO,INT2, GpioDirIn, TRUE, FALSE, FALSE, 0)
#define CMT2300A_SetGpio3In()           ;//SET_GPIO_IN(CMT_GPIO3_GPIO)
#define CMT2300A_ReadGpio1()            Gpio_GetIO(CMT_GPIO1_GPIO, INT1)//READ_GPIO_PIN(CMT_GPIO1_GPIO)
#define CMT2300A_ReadGpio2()            Gpio_GetIO(CMT_GPIO2_GPIO, INT2)
#define CMT2300A_ReadGpio3()           ;// Gpio_GetIO(CMT_GPIO1_GPIO, INT1)READ_GPIO_PIN(CMT_GPIO3_GPIO)
#define CMT2300A_DelayMs(ms)            system_delay_ms(ms)
#define CMT2300A_DelayUs(us)            system_delay_us(us)
#define CMT2300A_GetTickCount()         g_nSysTickCount
/* ************************************************************************ */

void CMT2300A_InitGpio(void);

u8 CMT2300A_ReadReg(u8 addr);
void CMT2300A_WriteReg(u8 addr, u8 dat);

void CMT2300A_ReadFifo(u8 buf[], u16 len);
void CMT2300A_WriteFifo(const u8 buf[], u16 len);

#ifdef __cplusplus 
} 
#endif

#endif
