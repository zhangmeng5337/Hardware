#include "gpio_irq.h"
#include "tm7780.h"

void irq_init(void)
{
	P_SW2 = P_SW2|0x80;
	P3IM0 = 0xff;
	P3IM1 = 0x00;
	P3INTE = 0xff;
	//EA = 1;
}

void gpio_Isr() interrupt 13
{
	unsigned char intf;
	intf = P3INTF;
	if(intf)
	{
		if(intf&0x20)//p3.5
	    {
			VI_Interrupt();
		}
		if(intf&0x40)//p3.6
	    {
			P_Interrupt();
		}		
	}
}

