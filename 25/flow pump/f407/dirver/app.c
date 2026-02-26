#include "app.h"
#include "aiAo.h"
#include "di_do.h"
#include "bldc_ctronl.h"
#include "dac8551.h"
#include "pid.h"
void init(void)
{
 aiAoInit();
 doInit();
 dacInit();
 PID_param_init();
 bldcm_init();
 
}
float cur_set=0;
void app(void)
{
 aiAoProc();
 diDoProc();
 DAC8552_Out_Put_Voltage(cur_set);
}
