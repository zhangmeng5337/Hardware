#ifndef __UART1_H
#define __UART1_H

/* ����ϵͳͷ�ļ� */
#include <stdio.h>
#include <stdarg.h>
#include "stm8l15x.h"
#include "stm8l15x_syscfg.h"
//#define BUFFERSIZE		24
/* Private define ------------------------------------------------------------*/
#ifdef _RAISONANCE_
#define PUTCHAR_PROTOTYPE int putchar (char c)
#define GETCHAR_PROTOTYPE int getchar (void)
#elif defined (_COSMIC_)
#define PUTCHAR_PROTOTYPE char putchar (char c)
#define GETCHAR_PROTOTYPE char getchar (void)
#else /* _IAR_ */
#define PUTCHAR_PROTOTYPE int putchar (int c)
#define GETCHAR_PROTOTYPE int getchar (void)
#endif /* _RAISONANCE_ */

void Uart1_Init(u32 boundrate, u8 check);
void UART1_SendByte(u8 data);
void UART1_SendStr(u8 *str);
u8 UART1_ReceiveByte(void);

#ifdef _IAR_
int fputc(int ch, FILE *f);
#endif
#endif

