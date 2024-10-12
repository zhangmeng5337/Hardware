#include "app.h"
#include "di.h"
#include "do.h"
#include "modbus.h"
#include "lte_hal.h"
#include "lcd.h"
#include "sys.h"
#include "config.h"
#include "ai_proc.h"
#include "uart.h"
#include "equipment_ctrl.h"
#include "water_ctrl.h"
#include "mqtt_analy.h"
#include "pid.h"
#include "machine_ctrl.h"



stru_di_stru di_status;


void hardware_init()
{
  do_off();
  ads1158_init();
	ads1158_config(1);
	ads1158_config(2);
	config_init();
	uart_init();
	CAT1_Init();
    mqtt_init();
	pid_init();
	 modbus_init();


	//ads_init();
}
void system_reconfig()
{
	HAL_NVIC_SystemReset();

}


void module_test()
{
   unsigned char i;
	di_proc(0);

	for(i=0;i<15;i++)
	{
		do_ctrl_proc(i,1);
		HAL_Delay(300);
		//do_ctrl_proc(i,0);
		//HAL_Delay(1000);
	}
	for(i=0;i<15;i++)
	{
		//do_ctrl_proc(i,1);
		//HAL_Delay(1000);
		do_ctrl_proc(i,0);
		HAL_Delay(300);
	}


}
void control_proc()
{
	
	if(equipment_control()==0)//no fault control water
		water_temperature_control();
}
void app_proc()
{
   TickPro(); //sys timer proc
   ai_proc();//pressure temperature sample interval 2s 
   di_proc(0);//di detection
   mqtt_proc();//lte ,mqtt and ota
   modbus_proc();//machine control
   dev_ctrl();
  //modbus_test();
  // control_proc();
   //config_save();
   //do_ctrl_proc(0,1);//do control
  // module_test();
}
