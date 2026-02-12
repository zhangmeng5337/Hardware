/**--------------File Info------------------------------------------------------------------------------
** File name:			delay.h
** Last modified Date:  2017-08-5
** Last Version:		V1.02
** Descriptions:		Time Config
**------------------------------------------------------------------------------------------------------
** Created date:		2017-08-5
** Version:				  V1.02
** Descriptions:		Time Config
**
********************************************************************************************************/
#ifndef __TIME_H
#define __TIME_H

#ifdef   __TIME_MANL
#define  __EXT_TIME
#else 
#define  __EXT_TIME    extern
#endif

__EXT_TIME unsigned int tmr1s;          //  200ms计数
__EXT_TIME unsigned int flag1s;         //  200ms溢出标志位

void TIM3_INIT(unsigned int arr,unsigned int psc);

#endif

