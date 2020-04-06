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
      #if !DEGUG_SENSOR 
      SIMCOM_Register_Network();
      #endif
      module_process(); 
    }

  }
}
