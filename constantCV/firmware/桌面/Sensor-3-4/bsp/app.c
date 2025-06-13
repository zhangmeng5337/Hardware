#include "app.h"
#include "cs1237_32.h"

#include "modbus.h"
#include "sensor.h"
#include "reg.h"
#include "key.h"
#include "uart_hal.h"
#include "display.h"


void app_init()
{
    reg_init();
    key_init();
    adc_init();
   //system_setup();
    uart_init();
    modbus_init();//
    // display_init();
     
}
uint32_t time_cal = 0;
void app_proc()
{
    /* uart_proc();*/
    static uint32_t tick_tmp;
	//if (getuart()->recv_update == 0)

    if (GetTick() - tick_tmp >= 5)
    {

       // tick_tmp = GetTick();
        adc_proc();
    }
	   pwm_ctrl(1);
	   KeyProc();
	   display_menu();


	modbus_recv_proc();
	reg_proc();
	modbus_init();
	uart_proc();


}
