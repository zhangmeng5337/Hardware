#include <AI8H.h>
#include <GN1640.h>
#include "adc.h"
void Init()
{
	P0M0=0x00;                                
    P0M1=0x00;
	P1M0=0x00;                                
    P1M1=0x00;
	P2M0=0x00;
	P2M1=0x00;
	P3M0=0x00;
	P3M1=0x00;
	P5M0=0x00;
	P5M1=0x00;
}

void main()
{
	Init();
	adc_init();
	DP_disp(0,1234);
	DP_disp(1,2345);
	DP_disp(2,9876);
	led_dd(1);
	printf("init compplete\r\n");
	while(1)
	{
		out_disp();	
		//printf("run\r\n");
	}
}