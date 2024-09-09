#include "lcd_app.h"
uint8_t taskIndex = 0;	//任务调度序号
//任务调度表
Menu_table_t taskTable[] =
{
    //菜单界面函数 -- 一级界面
    {0, 1, 0, 0, Menu_Interface}, 
    //功能界面函数 -- 二级界面
    {1, 4, 2, 0, Function_Interface1},
    {2, 5, 3, 0, Function_Interface2},
    {3, 6, 1, 0, Function_Interface3},
	//功能设置界面函数 -- 三级界面
	{4, 4, 4, 1, Function_Interface4},
	{5, 5, 5, 2, Function_Interface5},
	{6, 6, 6, 3, Function_Interface6},
};

 
/**
  * @brief  菜单界面函数
  * @param  无
  * @retval 无
  */
void Menu_Interface(void)
{
	char buff[50];
	
	//RTC_Get_StdTime(RTC_GetCounter());
	
	//sprintf(buff,"%0.2d:%0.2d:%0.2d",RTC_CLOCK.hour, RTC_CLOCK.min, RTC_CLOCK.sec);
	//OLED_ShowString(4, 10,  24, buff);
	
	//sprintf(buff,"Date:%0.4d-%0.2d-%0.2d",RTC_CLOCK.year,RTC_CLOCK.mon,RTC_CLOCK.day);
	//OLED_ShowString(1, 6,  16, buff);
}

