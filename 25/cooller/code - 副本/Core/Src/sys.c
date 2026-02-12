#include "sys.h"
void delay_10us(uint32_t cnt)
{
	uint32_t i,j;
	for(i=0;i<cnt;i++)
		for(j=0;j<80;j++);
}

