#include "app.h"
#include "display.h"
#include "modbus.h"
#include "sensor.h"
#include "reg.h"
#include "key.h"


void app_init()
{  
	reg_init();
	adc_init();
}

void app_proc()
{
	adc_proc(); 
	display();
	KeyProc();
	modbus_recv_proc();
}
