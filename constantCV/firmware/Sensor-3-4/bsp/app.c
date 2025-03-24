#include "app.h"

#include "modbus.h"
#include "sensor.h"
#include "reg.h"
#include "key.h"
#include "uart_hal.h"


void app_init()
{  
	reg_init();
	key_init();
	adc_init();
	uart_init();
	modbus_init();//
   // display_init();
}

void app_proc()
{
   /* uart_proc();*/ 
	adc_proc();
	display_menu();
	modbus_recv_proc();
    reg_proc();
	pwm_ctrl(1);
	 uart_proc();
}
