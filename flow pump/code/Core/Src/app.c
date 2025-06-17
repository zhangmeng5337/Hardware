#include "app.h"
#include "motor.h"
#include "sys.h"
#include "di.h"
#include "lcd_app.h"
#include "key.h"

void app_init()
{
      key_init();
	  menu_init();
	 Motor_Init();

}

void app_proc()
{
	 TickPro();
	 Di_Do_proc();

	 motor_ctrl_proc();
	 lcd_proc();

}
