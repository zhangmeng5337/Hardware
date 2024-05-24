#include "sys.h"
void delay(){;}
void delay1(uchar x)
{
	uchar y,z;
	for(y=x;y>0;y--)
	 for(z=150;z>0;z--);
}

void delay_ms(unsigned int  x)
{
	uchar y,z;
	for(y=x;y>0;y--)
	 for(z=150;z>0;z--);
}