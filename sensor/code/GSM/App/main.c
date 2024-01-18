#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "GSM.h"
#include "Protocal.h"

extern unsigned char RtcWakeUp;
extern Uart_Types uart_str;
uint32_t tick = 0;


void main(void)
{
  
  HardwareInit();
  module_prams_init();
  //test();
  uart_str.receive_flag =0;
  RtcWakeUp = 1;
  
  while(1)
  {
    // delay_ms(1000);
    tick ++;
    
    if(RtcWakeUp == 1)
    {
      #if !DEGUG_SENSOR 
      SIMCOM_Register_Network();
      #endif
      module_process(); 
    }
    // GPIO_ToggleBits( PORT_KEY, PIN_KEY );
    //delay_ms(2);
  }
}
