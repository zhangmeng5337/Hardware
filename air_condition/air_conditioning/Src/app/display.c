#include "74hc595.h"
#include "display.h"

#define DOT		0x80

unsigned char table[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
//unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
const unsigned char seg_table[16]={0};
void display(float dat,unsigned char updateflag,unsigned char dattypes)
{
	static unsigned char buf[4];
	uint32_t tmp;
	static unsigned char bitselect=1;
  if(updateflag == 1)
  {
		if(dattypes == 0)
		{
			if(dat<10)
			{
					tmp = dat*1000;
					buf[0] = table[tmp/1000]-DOT;
					buf[1] = table[tmp%1000/100];
					buf[2] = table[tmp%10/100];
					buf[3] = table[tmp%10];
			}
			else if(dat>=10&&dat<100)
			{
					tmp = dat*100;
					buf[0] =  table[tmp/1000];
					buf[1] =  table[tmp%1000/100]-DOT;
					buf[2] =  table[tmp%10/100];
					buf[3] =  table[tmp%10];
			}
			else if(dat>=100&&dat<1000)
			{
					tmp = dat*10;
					buf[0] =  table[tmp/1000];
					buf[1] =  table[tmp%1000/100];
					buf[2] =  table[tmp%10/100]-DOT;
					buf[3] =  table[tmp%10];
			}
			else
			{
					// tmp = dat*10;
					buf[0] =  table[tmp/1000];
					buf[1] =  table[tmp%1000/100];
					buf[2] =  table[tmp%10/100];
					buf[3] =  table[tmp%10]-DOT;
			}
				
		}
		else
		{
			if(dat<10)
			{
				tmp = dat*1000;
				buf[0] = table[tmp/1000];
				buf[1] = table[tmp%1000/100];
				buf[2] = table[tmp%10/100];
				buf[3] = table[tmp%10];
			}
			else if(dat>=10&&dat<100)
			{
				tmp = dat*100;
				buf[0] =  table[tmp/1000];
				buf[1] =  table[tmp%1000/100];
				buf[2] =  table[tmp%10/100];
				buf[3] =  table[tmp%10];
			}
			else if(dat>=100&&dat<1000)
			{
				tmp = dat*10;
				buf[0] =  table[tmp/1000];
				buf[1] =  table[tmp%1000/100];
				buf[2] =  table[tmp%10/100];
				buf[3] =  table[tmp%10];
			}
			else
			{
				// tmp = dat*10;
				buf[0] =  table[tmp/1000];
				buf[1] =  table[tmp%1000/100];
				buf[2] =  table[tmp%10/100];
				buf[3] =  table[tmp%10];
			}
		}
		#if DEBUG_USER
		printf("display is:   %d\n",dat);
		#endif
		bitselect = 1;	

  }
  else
  {
     M74HC595_WriteData(buf[bitselect-1]);
     seg_select(bitselect);
	   bitselect = bitselect +1;
	   if(bitselect == 4)
		   bitselect = 1;
  }

}

void display_off(void)
{
	seg_select(4);
}

void display_proc()
{
	uint32_t gettime;
	
	if((HAL_GetTick()-gettime)>=DISPLAY_PERIOD)
	{
		display(0,0,0);

	}
	else
	   gettime = HAL_GetTick();
}
