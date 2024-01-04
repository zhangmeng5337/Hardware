#include "modbus.h"
#include "uart.h"
#include "tm7780.h"
#include "hardware.h"
#include "app.h"


void hardware_init(void)
{
     mcu_init();
	 tm7780_init();
	 UartInit();
	 Timer1_Init();		//100微秒@12.000MHz

}
#define TM7780_CAF   	P35
#define TM7780_CF    	P36

void app_proc(void)
{
   unsigned char result = 1;
   unsigned char status ;
  
   status = 0;

   tm7780_proc();
   status = rs485_recv();
   
   if(status !=0)
   	{
		if(status == 1)
		{
			modbus_proc(POWER_REQUEST);//获取数据
		}
		else if(status == 2)
		{
			modbus_proc(POWER_CALI);//配置校准，未实现
		}
   }
   else 
   	{
   		if(get_power()->time_1s>=1000)
   		{
			modbus_proc(POWER_REQUEST);//获取数据
			get_power()->time_1s = 0;
		}

   }


}



