#include "led.h"
#include "ddl.h"
#include "main.h"
void LED_Init(void)
{
  Gpio_InitIOExt(LED_PORT1,LED_PIN1, GpioDirOut, TRUE, FALSE, FALSE, 0);//led1
  Gpio_InitIOExt(LED_PORT2,LED_PIN2, GpioDirOut, TRUE, FALSE, FALSE, 0);//led2
}
void LED_blink(unsigned char led_sel,uint16_t DelayCount)
{
  switch(led_sel)
  {
  case LED_RED:
    {
      Gpio_SetIO(LED_PORT1, LED_PIN1, !Gpio_GetIO(LED_PORT1, LED_PIN1));        
      delay1ms(DelayCount);		
    }break;
  case LED_GREEN:
    {
      Gpio_SetIO(LED_PORT2, LED_PIN2, !Gpio_GetIO(LED_PORT2, LED_PIN2));  
      delay1ms(DelayCount);			
    }break; 	
  default :
    {
      Gpio_SetIO(LED_PORT1, LED_PIN1, !Gpio_GetIO(LED_PORT1, LED_PIN1)); 
      delay1ms(DelayCount);		
    }break;
  }
}

void LED_ctrl(unsigned char led_sel,boolean_t status)
{
  switch(led_sel)
  {
  case LED_RED:
    { 
      if(status == ON)
    	Gpio_SetIO(LED_PORT1, LED_PIN1, 1); 
      else
    	Gpio_SetIO(LED_PORT1, LED_PIN1, 0); 	   	
    }break;
  case LED_GREEN:
    { 
      if(status == ON)
        Gpio_SetIO(LED_PORT2, LED_PIN2, 1);
      else
        Gpio_SetIO(LED_PORT2, LED_PIN2, 0);	   	
    }break;	
  default :
    { 
      if(status == ON)
    	Gpio_SetIO(LED_PORT1, LED_PIN1, 1); 
      else
    	Gpio_SetIO(LED_PORT1, LED_PIN1, 0); 	   	
    }break;
  }
}
