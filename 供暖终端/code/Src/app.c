#include "app.h"
#include "di.h"
#include "do.h"
//#include "ads1158.h"
#include "unit_tests.h"
stru_di_stru di_status;

void hardware_init()
{
ads1158_init();
	ads1158_config();


	//ads_init();
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
	 adc_proc();
	HAL_Delay(1000);
 // ads_read_data();
   di_proc(0);
   //do_ctrl_proc(0,1);
   
}
