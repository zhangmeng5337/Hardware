#include "lora.h"

ModuleParams_Str         ModuleParams;
ModuleOperation_Str     ModuleOperation;


void ModuleSetting()
{
	switch(ModuleOperation.mode)
	{
		case  NORMAL_MODE: 
			ModuleMode(NORMAL_MODE);
			break;	
		case  WAKE_MODE: 
			ModuleMode(WAKE_MODE);
			break;	
		case  SLEEP_MODE: 
			ModuleMode(SLEEP_MODE);
			break;	
		case  POWER_SAVING_MODE: 
			ModuleMode(POWER_SAVING_MODE);
			break;	
	}
}
void ModuleWrite()
{
	unsigned char wparams[15];
	unsigned char i;
	i = 0;

	ModuleOperation.mode = NORMAL_MODE;
	ModuleSetting();
	
	wparams[i++] = 0xff;
	wparams[i++] = 0x56;
	wparams[i++] = 0xae;
	wparams[i++] = 0x35;
	wparams[i++] = 0xa9;
	wparams[i++] = 0x55;
	wparams[i++] = 0x90;	
	wparams[i++] = ModuleParams.Frequence;
	wparams[i++] = ModuleParams.Frequence;
	wparams[i++] = ModuleParams.Frequence;
	wparams[i++] = ModuleParams.AirRate;
	wparams[i++] = ModuleParams.Power;
	wparams[i++] = ModuleParams.UartBurdRate;
	wparams[i++] = ModuleParams.Check;
	wparams[i++] = ModuleParams.WakeUpPeriod;	
	wparams[i++] = ModuleParams.RfId;	
	wparams[i++] = ModuleParams.RfId;	
}

void ModuleRead()
{
	unsigned char wparams[15];
	unsigned char i;
	i = 0;
	ModuleOperation.mode = NORMAL_MODE;
	ModuleSetting();
	
	wparams[i++] = 0xff;
	wparams[i++] = 0x56;
	wparams[i++] = 0xae;
	wparams[i++] = 0x35;
	wparams[i++] = 0xa9;
	wparams[i++] = 0x55;
	wparams[i++] = 0xf0;	
	
}

