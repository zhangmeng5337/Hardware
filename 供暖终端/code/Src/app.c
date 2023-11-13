#include "app.h"
#include "di.h"
#include "do.h"
<<<<<<< HEAD
=======
#include "modbus.h"
#include "lte_hal.h"
#include "lcd.h"

>>>>>>> aa4ae69f45d718490ac99e831aaa8d4fee09c114
//#include "ads1158.h"
#include "unit_tests.h"
stru_di_stru di_status;

void hardware_init()
{
<<<<<<< HEAD
ads1158_init();
	ads1158_config();
=======
    ads1158_init();
	ads1158_config();
	lte_init();
	
>>>>>>> aa4ae69f45d718490ac99e831aaa8d4fee09c114


	//ads_init();
}
<<<<<<< HEAD
=======
void system_reconfig()
{
	HAL_NVIC_SystemReset();

}
>>>>>>> aa4ae69f45d718490ac99e831aaa8d4fee09c114
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
<<<<<<< HEAD
	 adc_proc();
	HAL_Delay(1000);
 // ads_read_data();
   di_proc(0);
=======
    
   ai_proc();//2s采集一次
   di_proc(0);//采集一次
   lcd_proc();
   modbus_proc();
   lte_proc();
>>>>>>> aa4ae69f45d718490ac99e831aaa8d4fee09c114
   //do_ctrl_proc(0,1);
   
}
