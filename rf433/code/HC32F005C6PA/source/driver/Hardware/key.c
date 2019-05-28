#include "key.h"
#include "main.h"
#include "uartParse.h"
void Gpio_IRQHandler(uint8_t u8Param)
{
  *((uint32_t *)((uint32_t)&M0P_GPIO->P3ICLR + u8Param * 0x40)) = 0;
  
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
/*en_result_t Gpio_InitIOExt(uint8_t u8Port, uint8_t u8Pin,
en_gpio_dir_t enDir,
boolean_t bPullup,
boolean_t bPulldown,
boolean_t bOdr,
boolean_t bDrive)
*/

void key_intterupt_config( en_gpio_irqtype_t EXTI_Trigger)
{
  
  
  //初始化外部IO P33
  Gpio_InitIOExt(KEY_UP_PORT,KEY_UP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//up
  Gpio_ClearIrq(KEY_UP_PORT, KEY_UP_PIN);
  Gpio_EnableIrq(KEY_UP_PORT, KEY_UP_PIN, EXTI_Trigger);
  EnableNvic(KEY_UP_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); 
  
  Gpio_InitIOExt(KEY_DOWN_PORT,KEY_DOWN_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//DOWN
  Gpio_ClearIrq(KEY_DOWN_PORT, KEY_DOWN_PIN);
  Gpio_EnableIrq(KEY_DOWN_PORT, KEY_DOWN_PIN, EXTI_Trigger);
  EnableNvic(KEY_DOWN_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   
  
  Gpio_InitIOExt(KEY_EAST_PORT,KEY_EAST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//EAST
  Gpio_ClearIrq(KEY_EAST_PORT, KEY_EAST_PIN);
  Gpio_EnableIrq(KEY_EAST_PORT, KEY_EAST_PIN, EXTI_Trigger);
  EnableNvic(KEY_EAST_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); 
  
  Gpio_InitIOExt(KEY_WEST_PORT,KEY_WEST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//WEST
  Gpio_ClearIrq(KEY_WEST_PORT, KEY_WEST_PIN);
  Gpio_EnableIrq(KEY_WEST_PORT, KEY_WEST_PIN, EXTI_Trigger);
  EnableNvic(KEY_WEST_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   
  
  Gpio_InitIOExt(KEY_SOUTH_PORT,KEY_SOUTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//SOUTH
  Gpio_ClearIrq(KEY_SOUTH_PORT, KEY_SOUTH_PIN);
  Gpio_EnableIrq(KEY_SOUTH_PORT, KEY_SOUTH_PIN, EXTI_Trigger);
  EnableNvic(KEY_SOUTH_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   
  
  Gpio_InitIOExt(KEY_START_PORT,KEY_START_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//START
  Gpio_ClearIrq(KEY_START_PORT, KEY_START_PIN);
  Gpio_EnableIrq(KEY_START_PORT, KEY_START_PIN, EXTI_Trigger);
  EnableNvic(KEY_START_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);  
  
  Gpio_InitIOExt(KEY_NORTH_PORT,KEY_NORTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//NORTH
  Gpio_ClearIrq(KEY_NORTH_PORT, KEY_NORTH_PIN);
  Gpio_EnableIrq(KEY_NORTH_PORT, KEY_NORTH_PIN, EXTI_Trigger);
  EnableNvic(KEY_NORTH_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE);   
  
  Gpio_InitIOExt(KEY_BREAK_PORT,KEY_BREAK_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//BREAK
  Gpio_ClearIrq(KEY_BREAK_PORT, KEY_BREAK_PIN);
  Gpio_EnableIrq(KEY_BREAK_PORT, KEY_BREAK_PIN, EXTI_Trigger);
  EnableNvic(KEY_STOP_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); 
  
  Gpio_InitIOExt(KEY_STOP_PORT,KEY_STOP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//STOP
  Gpio_ClearIrq(KEY_STOP_PORT, KEY_STOP_PIN);
  Gpio_EnableIrq(KEY_STOP_PORT, KEY_STOP_PIN, EXTI_Trigger);
  EnableNvic(KEY_BREAK_PORT_IRQ, DDL_IRQ_LEVEL_DEFAULT, TRUE); 
  
}
void KEY_Init()
{
  params_typedef params;
  en_gpio_irqtype_t EXTI_Trigger;
  //初始化外部IO P33
  Gpio_InitIOExt(KEY_UP_PORT,KEY_UP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//up
  Gpio_InitIOExt(KEY_DOWN_PORT,KEY_DOWN_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//DOWN
  Gpio_InitIOExt(KEY_EAST_PORT,KEY_EAST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//EAST
  Gpio_InitIOExt(KEY_WEST_PORT,KEY_WEST_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//WEST
  Gpio_InitIOExt(KEY_SOUTH_PORT,KEY_SOUTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//SOUTH
  Gpio_InitIOExt(KEY_START_PORT,KEY_START_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//START
  Gpio_InitIOExt(KEY_NORTH_PORT,KEY_NORTH_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//NORTH
  Gpio_InitIOExt(KEY_BREAK_PORT,KEY_BREAK_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//BREAK 
  Gpio_InitIOExt(KEY_STOP_PORT,KEY_STOP_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0);//STOP
  
  
  /*
#define INCH					0X01   //点动
#define SELFLOCK				0X02 //自锁
#define INHIBITION			0X03//相互拟制
#define NOINHIBITION		0X04//非拟制
  */
  //params = system_params_get();
  switch(params.KEY_H8 )
  {
  case INCH:			EXTI_Trigger = GpioIrqLow; break;
  case SELFLOCK:		EXTI_Trigger = GpioIrqRising; break;
  case INHIBITION:		EXTI_Trigger = GpioIrqFalling; break;
  case NOINHIBITION:	        EXTI_Trigger = GpioIrqFalling; break;
  default:			EXTI_Trigger = GpioIrqFalling; break;	
  }
  key_intterupt_config(EXTI_Trigger);
}




