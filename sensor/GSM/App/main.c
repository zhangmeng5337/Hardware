#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "GSM.h"
#include "Protocal.h"
extern  unsigned char RtcWakeUp;
extern Uart_Types uart_str;
uint32_t tick = 0;

void main(void)
{
  RtcWakeUp =1;
  HardwareInit();
  module_prams_init();
  
  //test();
  uart_str.receive_flag =0;
  while(1)
  {

    if(RtcWakeUp == 1)
    {
      SIMCOM_Register_Network();
      module_process(); 
    }

  }
}
