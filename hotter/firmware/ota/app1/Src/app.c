#include "app.h"
#include "di.h"
#include "do.h"
#include "modbus.h"
#include "lte_hal.h"
#include "lcd.h"

//#include "ads1158.h"
#include "unit_tests.h"
stru_di_stru di_status;

void hardware_init()
{
    ads1158_init();
	ads1158_config(1);
	ads1158_config(2);



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
	for(i=0;i<8;i++)
	{
		di_proc(i);
		HAL_Delay(1000);
	}	
	for(i=0;i<20;i++)
	{
		do_ctrl_proc(i,1);
		HAL_Delay(1000);
		do_ctrl_proc(i,0);
		HAL_Delay(1000);
	}


}
void app_proc()
{
   TickPro(); 
   ai_proc();//2s采集一次
   di_proc(0);//采集一次
   lcd_proc();
   modbus_proc();
   lte_proc();
   //do_ctrl_proc(0,1);
   
}
