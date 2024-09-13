#include "lcd_app.h"
#include "key.h"

uint8_t taskIndex = 0;  //任务调度序号
menu_key_stru menu_key_usr;

void Menu_Interface(void);




Menu_table_t taskTable[] =
{
    //菜单界面函数 -- 一级界面
    {0, 0, 0, 0, 0, Menu_Interface}
//    //功能界面函数 -- 二级界面
//    {1, 4, 2, 0, Function_Interface1},
//    {2, 5, 3, 0, Function_Interface2},
//    {3, 6, 1, 0, Function_Interface3},
//    //功能设置界面函数 -- 三级界面
//    {4, 4, 4, 1, Function_Interface4},
//    {5, 5, 5, 2, Function_Interface5},
//    {6, 6, 6, 3, Function_Interface6},
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
void menu_init()
{
    taskIndex = 0;

}
void Menu_Interface_manage()
{
    if (taskTable[taskIndex].CurrentNum == MAIN_PAGE)
    {
        if (taskTable[taskIndex].mode == 1)
        {
            menu_key_usr.next_status = 1;
            if (taskTable[taskIndex].current_icon_num == RUN_ICON)
            {
                taskTable[taskIndex].next = MAIN_PAGE_RUN;
                ///taskTable[taskIndex].CurrentNum = MAIN_PAGE_RUN;
            }
            else if (taskTable[taskIndex].current_icon_num == INFOR_ICON)
            {
                taskTable[taskIndex].next = MAIN_PAGE_INFO;
                //taskTable[taskIndex].CurrentNum = MAIN_PAGE_INFO;
            }
            else if (taskTable[taskIndex].current_icon_num == WARN_ICON)
            {
                taskTable[taskIndex].next = MAIN_PAGE_WARN;
                //taskTable[taskIndex].CurrentNum = MAIN_PAGE_WARN;
            }
            else if (taskTable[taskIndex].current_icon_num == SETTING_ICON)
            {
                taskTable[taskIndex].next = MAIN_PAGE_SET;
                //taskTable[taskIndex].CurrentNum = MAIN_PAGE_SET;
            }
            taskTable[taskIndex].mode =  0;

        }

    }
    else
    {
        if (taskTable[taskIndex].mode == 1)
        {
            if (taskTable[taskIndex].CurrentNum == MAIN_PAGE_RUN)//run information
            {
                if (taskTable[taskIndex].current_icon_num == INFOR_ICON)
                {
                    taskTable[taskIndex].next = MAIN_PAGE_INFO;
                    //taskTable[taskIndex].CurrentNum = MAIN_PAGE_INFO;
                }
                else if (taskTable[taskIndex].current_icon_num == WARN_ICON)
                {
                    taskTable[taskIndex].next = MAIN_PAGE_WARN;
                    //taskTable[taskIndex].CurrentNum = MAIN_PAGE_WARN;
                }
                else if (taskTable[taskIndex].current_icon_num == SETTING_ICON)
                {
                    taskTable[taskIndex].next = MAIN_PAGE_SET;
                    //taskTable[taskIndex].CurrentNum = MAIN_PAGE_SET;
                }
                else if (taskTable[taskIndex].current_icon_num == RETURN_ICON)
                {
                    taskTable[taskIndex].next = MAIN_PAGE_RUN;
                    //taskTable[taskIndex].CurrentNum = MAIN_PAGE_SET;
                }

            }

        }


    }


}
void menu_change_proc()
{

}
void menu_manage()
{
    if (menu_key_usr.next_status == 1) //切换画面
    {
        //taskIndex = taskTable[taskIndex].next;
        taskTable[taskIndex].current_icon_num = 0;
        taskTable[taskIndex].mode = 0;
        menu_key_usr.next_status = 0;


        taskIndex = taskTable[taskIndex].next;
        taskTable[taskIndex].current_icon_num = 0;
        taskTable[taskIndex].mode = 0;
    }

    if (get_key_state()->encode_update == 1)//forward
    {
        if (taskTable[taskIndex].current_icon_num < taskTable[taskIndex].max_icon_num)
            taskTable[taskIndex].current_icon_num =
                taskTable[taskIndex].current_icon_num + 1;

    }
    else if (get_key_state()->encode_update == 2)//backward
    {
        if (taskTable[taskIndex].current_icon_num > 0)
            taskTable[taskIndex].current_icon_num =
                taskTable[taskIndex].current_icon_num - 1;

    }
    else if (get_key_state()->encode_update == 3)//ok
    {
        if (taskTable[taskIndex].mode == 0)
        {
            taskTable[taskIndex].mode = 1;//选中
        }

        else
            taskTable[taskIndex].mode = 0;

    }
    else
    {

    }

    menu_change_proc();

}

void lcd_proc()
{
    static uint32_t tick;

    menu_manage();
    if ((HAL_GetTick() - tick) >= FRESH)
    {
        tick = HAL_GetTick();
        taskTable[taskIndex].Current_Operation();//执行函数
    }
}

