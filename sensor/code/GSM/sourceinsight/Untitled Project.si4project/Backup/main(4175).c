

#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "GSM.h"









void main(void)
{

  HardwareInit();
  while(1)
  {
	  SIMCOM_Register_Network();

  }
}
