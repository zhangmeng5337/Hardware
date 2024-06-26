#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include "sys.h"
#include "Uart.h"
//**************************************************************************** 

bit   Compare_Dgus(u16 addr1,u16 addr2,u8 len);            //比较两个Dgus变量的值，相等返回1
void  Norflash_Read(u16 lib_addr,u16 Dgus_addr,u16 len);   //所有参数需是偶数
void  Norflash_Write(u16 lib_addr,u16 Dgus_addr,u16 len);  //所有参数需是偶数
void  Function_Init();
void  Function_All();
extern  u16  xdata  T_NUMBER;
extern  u16  xdata  T_NUMBER_1;
extern  u16  xdata  number_one;
#endif 



