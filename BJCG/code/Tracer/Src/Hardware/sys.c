#include "sys.h"
#include "stdio.h"
extern UART_HandleTypeDef huart3;

extern UART_HandleTypeDef huart1;
struct __FILE
{
int handle;
};
FILE __stdout;

int fputc(int ch, FILE *f)
{
				//发送数据到串口
	unsigned char tmp;
	tmp =(unsigned char )ch;
    HAL_UART_Transmit(&huart3, &tmp, 1, 500);
	 HAL_UART_Transmit(&huart1, &tmp, 1, 500);
	return ch;
}

