#ifndef __LED_H__
#define __LED_H__
#include "sys.h"
#include "Uart.h"
//**************************************************************************** 

void  ws2812_writ_byte(u8 dat);
void  Function_Init();
void  Function_All();
extern  u16  xdata  T_NUMBER;
extern  u16  xdata  T_NUMBER_1;
extern  u16  xdata  number_one;
#endif 



