/*===========================================================================
* ��ַ ��http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* ���� ������  ԭ �ں͵��ӹ�����  �� �ڰ��ص��ӿƼ����޹�˾                 *
* �ʼ� ��yihe_liyong@126.com                                                *
* �绰 ��18615799380                                                        *
============================================================================*/
#include "bsp.h"
#include "main.h"
#include "stdio.h"

extern UART_HandleTypeDef huart1;
/*******************************************************************************
 * ����: fputc
 * ����: �ض���c�⺯��printf��UART1
 * �β�: ��
 * ����: Ҫ��ӡ���ַ�
 * ˵��: ��printf���� 
 ******************************************************************************/
#ifdef KEIL
int fputc(int ch, FILE *f)
{  
	/* ��Printf���ݷ������� */
	HAL_UART_Transmit(&huart1, (unsigned char *)&ch, 1, 10);
	
	return (ch);
}
#else
PUTCHAR_PROTOTYPE
{
   /* Write a character to the UART1 */
	UART1_SendByte(c);
	return (c);
}
#endif

