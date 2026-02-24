#include "verifier.h"
#include "config.h"
#include "adc.h"
#include "debug.h"
#include "uart.h"
#include "flash.h"

#include "applInterface.h"
#include "9252_HW.h"


void app_init(void)
{
	  uart_init();
	  config_init();
	  calibration_init();
	  adc_init();
	  flash_init();
	  
	  verifier_init();
	  set_led(0xffffffff);

}
void app_proc()
{
//     valve_test();
	 config_proc();
	 verifier_proc();
   	rs485_state_conv();  
}
