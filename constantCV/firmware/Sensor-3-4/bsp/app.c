#include "app.h"

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
    uart_init();
    modbus_init();//
    // display_init();
}

void app_proc()
{
    /* uart_proc();*/
    static uint32_t tick_tmp;
//    if (GetTick() - tick_tmp >= 10)
//    {   
//        tick_tmp = GetTick();
//       
//
//    }
	 if (getKey()->update == 0)
	 {
	 	adc_proc();

	 }
		


        KeyProc();
        display_menu();
		pwm_ctrl(1);
        modbus_recv_proc();
        reg_proc();
        modbus_init();
        uart_proc();

        


    // display();
    //dis_test();


}
