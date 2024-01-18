#include "delay.h"
//#include <stdint.h>

void Delay_Us (uint32_t delay)
{
  uint8_t i=0;
  uint32_t j=0;
  for(i=0;i<delay;i++)
  {
    for(j=0;j<8;j++);
  }
}




void DelayMs(uint32_t x)
{
  unsigned int i;
  i=1000;
  for(;x>0;x--)
    for(i=1000;i>0;i--)     
      ;
}
//void HAL_Delay_nMS( uint32_t Delay )
//{
//    uint32_t tickstart = 0;
//    tickstart = HAL_GetTick( );
//    while( ( HAL_GetTick( ) - tickstart ) < Delay );
//}

