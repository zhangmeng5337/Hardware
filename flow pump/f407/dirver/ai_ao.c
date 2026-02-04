#include "aiAo.h"
#include "ads1118.h"
#include "dac8551.h"
void aiAoInit(void)
{
	ADS1118_Init();
	dacInit();
}
void aiAoProc(void)
{
	adcProc();
}
