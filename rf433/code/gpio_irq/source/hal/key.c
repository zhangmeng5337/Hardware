#include "key.h"
#include "main.h"
#include "uartParse.h"
#include "led.h"

uint16_t UpdateKeyStatus=0;//按键状态，按下对应位位1
uint16_t *KeyStaus()//return key status 
{
  return &UpdateKeyStatus;
}


// gpio interrupt function whether key is low level trigger or rising trigger
void Gpio_IRQHandler(uint8_t u8Param)//key inttrupt call back function
{
  uint16_t tInchStatus;
  tInchStatus=system_params_get()->KEY_H8<<8+system_params_get()->KEY_L8;
  
  switch(u8Param)
  {
  case 0:
    {         

       if(Gpio_GetIrqStat(KEY_STOP_PORT, KEY_STOP_PIN))
      {
        UpdateKeyStatus = UpdateKeyStatus |KEY_STOP_BIT;					
       		  		 LED_blink(LED_RED,10); 
      }     
      if(Gpio_GetIrqStat(KEY_BREAK_PORT, KEY_BREAK_PIN))
      {
          UpdateKeyStatus = UpdateKeyStatus |KEY_BREAK_BIT;			  		 LED_blink(LED_RED,10);				      	
      }
      
      //*((uint32_t *)((uint32_t)&M0P_GPIO->P0ICLR + u8Param * 0x40)) = 0;	
      *(uint32_t *)(0x40020C00+0x210)=0;
    }break;
    
  case 1:
    { 

      //WEST key down
      if(Gpio_GetIrqStat(KEY_WEST_PORT, KEY_WEST_PIN))
      {
        UpdateKeyStatus = UpdateKeyStatus |KEY_WEST_BIT;		  		 LED_blink(LED_RED,10);		
      }  

      //*((uint32_t *)((uint32_t)&M0P_GPIO->P1ICLR + u8Param * 0x40)) = 0;	
       *(uint32_t *)(0x40020C00+0x250)=0;
    }break;
  case 2:
  	{

	//EAST key down
	if(Gpio_GetIrqStat(KEY_EAST_PORT, KEY_EAST_PIN))
	{
	  UpdateKeyStatus = UpdateKeyStatus |KEY_EAST_BIT;				  
			  LED_blink(LED_RED,10);
	    *(uint32_t *)(0x40020C00+0x290)=0;
	}

    }
  case 3:
    {	
          if(Gpio_GetIrqStat(KEY_DOWN_PORT, KEY_DOWN_PIN))
      {  
          UpdateKeyStatus = UpdateKeyStatus |KEY_DOWN_BIT;
		  		 LED_blink(LED_RED,10);
      }
      //NORTH key down
      if(Gpio_GetIrqStat(KEY_NORTH_PORT, KEY_NORTH_PIN))
      {
      	UpdateKeyStatus = UpdateKeyStatus |KEY_NORTH_BIT;			  		
				LED_blink(LED_RED,10);	
      }
      //UP key down   
      if(Gpio_GetIrqStat(KEY_UP_PORT, KEY_UP_PIN))
      {
        UpdateKeyStatus = UpdateKeyStatus |KEY_UP_BIT;			  		 
				LED_blink(LED_RED,10);	
      }    
      
      //SOUTH key down
      if(Gpio_GetIrqStat(KEY_SOUTH_PORT, KEY_SOUTH_PIN))
      {
        UpdateKeyStatus = UpdateKeyStatus |KEY_SOUTH_BIT;		  		 
				LED_blink(LED_RED,10);		
      }  
      
      

      //START key down
      if(Gpio_GetIrqStat(KEY_START_PORT, KEY_START_PIN))
      {
        UpdateKeyStatus = UpdateKeyStatus |KEY_START_BIT;		  		
				LED_blink(LED_RED,10);					
      }
//      
      
			  *(uint32_t *)(0x40020C00+0x2D0)=0;
    }break;
    
    
  }
  
  
}

/*******************************************************************************
** \brief   GPIO 初始化2 
** 
** \param   [in]  u8Port             IO Port口
** \param   [in]  u8Pin              IO Pin脚
** \param   [in]  enDir              IO 方向（输入或输出）
** \param   [in]  bPullup            上拉开关
** \param   [in]  bPulldown          下拉开关
** \param   [in]  bOdr               开漏开关
** \param   [in]  bDrive             驱动能力 
**                                   0 = 高
**                                   1 = 低
** \retval     Ok         设置成功
**             其他值     设置失败
******************************************************************************/
/*
判断对应按键设置为电平触发还是边沿触发
点动:电平触发
自锁:上升沿触发
*/
/*boolean_t key_interrupt_mode(uint16_t inch_status,uint16_t key_num)
{
  boolean_t EXTI_Trigger;
  
  if(inch_status&key_num)
    EXTI_Trigger = 1;//rising edge trigger
  else 
    EXTI_Trigger = 0; //low level trigger
  return EXTI_Trigger;
}*/

en_gpio_irqtype_t key_intterupt_LevelConfig(uint16_t inch_status,uint16_t key_num)
{
  en_gpio_irqtype_t EXTI_Trigger;
  
  if(inch_status&key_num)
    EXTI_Trigger = GpioIrqRising;
  else 
    EXTI_Trigger = GpioIrqLow; 
  return EXTI_Trigger;
}

void key_intterupt_config( uint16_t inch_status)
{
  en_gpio_irqtype_t EXTI_Trigger;
  inch_status=system_params_get()->KEY_H8<<8+
  	                system_params_get()->KEY_L8;//获取按键工作方式，点动、自锁
  EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_UP_BIT); 	 	
  //初始化外部IO P33

  Gpio_InitIOExt(KEY_UP_PORT,KEY_UP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//up
  Gpio_ClearIrq(KEY_UP_PORT, KEY_UP_PIN);
  Gpio_EnableIrq(KEY_UP_PORT, KEY_UP_PIN, GpioIrqFalling);
  EnableNvic(KEY_UP_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);
  	Clk_SetPeripheralGate(ClkPeripheralGpio,1);
  Clk_SetFunc(ClkFuncSwdPinIOEn, 1);
	M0P_GPIO->P27_SEL = 0;
	M0P_GPIO->P31_SEL = 0;	
 EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_DOWN_BIT);  
  Gpio_InitIOExt(KEY_DOWN_PORT,KEY_DOWN_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//DOWN
  Gpio_ClearIrq(KEY_DOWN_PORT, KEY_DOWN_PIN);
  Gpio_EnableIrq(KEY_DOWN_PORT, KEY_DOWN_PIN, GpioIrqFalling);
  EnableNvic(KEY_DOWN_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   
  
  
 EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_EAST_BIT);     
  Gpio_InitIOExt(KEY_EAST_PORT,KEY_EAST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//EAST
  Gpio_ClearIrq(KEY_EAST_PORT, KEY_EAST_PIN);
  Gpio_EnableIrq(KEY_EAST_PORT, KEY_EAST_PIN, GpioIrqFalling);
  EnableNvic(KEY_EAST_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); 
  
  EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_WEST_BIT); 
  Gpio_InitIOExt(KEY_WEST_PORT,KEY_WEST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//WEST
  Gpio_ClearIrq(KEY_WEST_PORT, KEY_WEST_PIN);
  Gpio_EnableIrq(KEY_WEST_PORT, KEY_WEST_PIN, GpioIrqFalling);
  EnableNvic(KEY_WEST_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   
  
  
  EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_SOUTH_BIT);  
  Gpio_InitIOExt(KEY_SOUTH_PORT,KEY_SOUTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//SOUTH
  Gpio_ClearIrq(KEY_SOUTH_PORT, KEY_SOUTH_PIN);
  Gpio_EnableIrq(KEY_SOUTH_PORT, KEY_SOUTH_PIN, GpioIrqFalling);
  EnableNvic(KEY_SOUTH_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   
  
  
  EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_START_BIT); 
  Gpio_InitIOExt(KEY_START_PORT,KEY_START_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//START
  Gpio_ClearIrq(KEY_START_PORT, KEY_START_PIN);
  Gpio_EnableIrq(KEY_START_PORT, KEY_START_PIN, GpioIrqFalling);
  EnableNvic(KEY_START_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);  
  
  EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_NORTH_BIT);   
  Gpio_InitIOExt(KEY_NORTH_PORT,KEY_NORTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//NORTH
  Gpio_ClearIrq(KEY_NORTH_PORT, KEY_NORTH_PIN);
  Gpio_EnableIrq(KEY_NORTH_PORT, KEY_NORTH_PIN, GpioIrqFalling);
  EnableNvic(KEY_NORTH_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   
  
  
 /*   EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_BREAK_BIT);    
  Gpio_InitIOExt(KEY_BREAK_PORT,KEY_BREAK_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//BREAK
  Gpio_ClearIrq(KEY_BREAK_PORT, KEY_BREAK_PIN);
  Gpio_EnableIrq(KEY_BREAK_PORT, KEY_BREAK_PIN, EXTI_Trigger);
  EnableNvic(KEY_STOP_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); */ 
  
  
  EXTI_Trigger=key_intterupt_LevelConfig(inch_status,KEY_STOP_BIT); 
  Gpio_InitIOExt(KEY_STOP_PORT,KEY_STOP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//STOP
  Gpio_ClearIrq(KEY_STOP_PORT, KEY_STOP_PIN);
  Gpio_EnableIrq(KEY_STOP_PORT, KEY_STOP_PIN, GpioIrqFalling);
  EnableNvic(KEY_BREAK_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);
  
}
void KEY_Init()
{
  params_typedef *params;
  uint16_t EXTI_Trigger;
  /*
#define INCH					0X01   //点动
#define SELFLOCK				0X02 //自锁
#define INHIBITION			0X03//相互拟制
#define NOINHIBITION		0X04//非拟制
  */
  key_intterupt_config(EXTI_Trigger);//配置按键中断模式
  
}




