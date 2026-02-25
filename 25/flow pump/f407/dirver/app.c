#include "app.h"
#include "aiAo.h"
#include "di_do.h"
#include "bldc_ctronl.h"
#include "dac8551.h"
void init(void)
{
 aiAoInit();
 doInit();
 dacInit();
}
void app(void)
{
aiAoProc();
 DAC8552_Out_Put_Voltage(10);
}
