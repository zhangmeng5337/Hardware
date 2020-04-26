#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "Protocal.h"
#include "gprs_app.h"
extern unsigned char RtcWakeUp;

uint32_t tick = 0;


void main(void)
{
  
  HardwareInit();
  module_prams_init();
  RtcWakeUp = 1;
  
  while(1)
  {
    if(RtcWakeUp == 1)
    {
      //#if !DEGUG_SENSOR 
     // SIMCOM_Register_Network();
      //parseGpsBuffer();
     // #endif
    //  module_process(); 
	 // FLOW_Ctrl(ON);
	//  FLOW_Ctrl(OFF);
    GPIO_ResetBits( PORT_FLOW, PIN_FLOW );
    GPIO_SetBits( PORT_FLOW, PIN_FLOW );  
	  LED_TOG();
	  delay_ms(2000);//不是很精确
    }

  }
}
