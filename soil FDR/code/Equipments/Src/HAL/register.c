#include "register.h"
#include "Protocol_C.h"

REG_VALUE_stru register_map;
#define xF_ADDR_START    0xf001
#define xE_ADDR_START    0xe000
#define x1_ADDR_START    0x1001
#define x12_ADDR_START   0x1200
#define x4_ADDR_START    0x4001
#define x44_ADDR_START   0x44f1

unsigned char *ReadRegister(unsigned int p)
{
   unsigned int tmp;
   tmp = 0xffff;
	if(p>=x1_ADDR_START&&p<x12_ADDR_START)
	{
	    tmp = p-x1_ADDR_START;
		
	}
	else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
	{
		tmp = p-x12_ADDR_START;
	}
	else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
	{
		tmp = p-x4_ADDR_START;
	}
	else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
	{
		tmp = p-x44_ADDR_START;
	}
	else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
	{
		tmp = p-xE_ADDR_START;
	}
	else if(p>=xF_ADDR_START&&p<xF_ADDR_START+13)
	{
		tmp = p-xF_ADDR_START;
	}
	else
		tmp = 0xffff;
	if(tmp != 0xffff)
		return &(register_map.value[tmp]);
	else
		return NULL;

}
unsigned char WriteRegisteSet(unsigned int p,unsigned char *reg,unsigned char len)
{
   unsigned int tmp;
   unsigned char i,result;
   tmp = 0xffff;
   result = 0;
	if(p>=x1_ADDR_START&&p<x12_ADDR_START)
	{
	    tmp = p-x1_ADDR_START;
		
	}
	else if(p>=x12_ADDR_START&&p<x4_ADDR_START)
	{
		tmp = p-x12_ADDR_START;
	}
	else if(p>=x4_ADDR_START&&p<x44_ADDR_START)
	{
		tmp = p-x4_ADDR_START;
	}
	else if(p>=x44_ADDR_START&&p<xE_ADDR_START)
	{
		tmp = p-x44_ADDR_START;
	}
	else if(p>=xE_ADDR_START&&p<xF_ADDR_START)
	{
		tmp = p-xE_ADDR_START;
	}
	else if(p>=xF_ADDR_START&&p<xF_ADDR_START+13)
	{
		tmp = p-xF_ADDR_START;
	}
	else
		tmp = 0xffff;
	if(tmp != 0xffff)
	{
		for(i = 0;i<len;i++)
		{
			register_map.value[i+tmp] = reg[i];
		}
		result = 0;

	}
	else
		result = 1;
		return result;

}

unsigned char *getRegisterProc(unsigned char p)
{
   unsigned char *result;
	switch(p)
	{
		case auto_report_time:result = ReadRegister(reg_period_set);break;

	}

}

