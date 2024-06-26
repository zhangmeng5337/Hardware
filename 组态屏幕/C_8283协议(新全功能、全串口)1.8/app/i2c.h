#ifndef __I2C_H__
#define __I2C_H__

#include "sys.h"
#include "Uart.h"

void    Clock();


//#define	      SELECT_RTC_TYPE			1	            //RX8130=1;SD2058=2 ;ÎÞRTC=0



extern u16    xdata   SysTick_RTC;

#endif