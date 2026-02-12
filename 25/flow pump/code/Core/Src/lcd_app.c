#include "lcd_app.h"
#include "key.h"
#include "sys.h"
#include "draw.h"
#include "lcd_string.h"
#include "pump_config.h"

uint8_t taskIndex = 0;  //任务调度序号
menu_key_stru menu_key_usr;

void l1_main_page_Interface(void);
void l2_main_page_info_Interface(void);
void l2_main_page_warn_Interface(void);
void l2_main_page_set_Interface(void);
void l3_main_page_info_Interface(void);



/**
  * @brief  菜单界面函数
  * @param  无
  * @retval 无
  */
void l1_main_page_Interface(void)
{
	LCD12864_Write_SuperString(0, 0, "运行");
	// LCD_Clear_GDRAM();
	LCD12864_Fixpicture(1, 4, 0, 2, 16, (unsigned char *)INFO_B);
	LCD12864_Fixpicture(1, 5, 0, 2, 16, (unsigned char *)WARN_B);
	LCD12864_Fixpicture(1, 6, 0, 2, 16, (unsigned char *)SET2_B);
	LCD12864_Fixpicture(1, 7, 0, 2, 16, (unsigned char *)BACK_B);
	LCD12864_Fixpicture(2, 1, 0, 2, 16, (unsigned char *)STOP_B);
	LCD12864_Write_Numberf(2, 1, get_pump_params()->flow_real);
	if(get_pump_params()->flow_real<=100)
		get_pump_params()->flow_real++;
	else
		get_pump_params()->flow_real = 0;
	if(get_pump_params()->unit == 0)
		LCD12864_Write_SuperString(2, 4, "ml/h");
	else
		LCD12864_Write_SuperString(2, 4, "L/h");
	LCD12864_Write_SuperString(3, 0, "手动");
	LCD12864_Write_Numberf(3, 3, get_pump_params()->set_flow);


	LCD12864_Fixpicture(4, 1, 0, 2, 16, (unsigned char *)FLOW_CTRL_B);
	LCD12864_Fixpicture(4, 7, 0, 2, 16, (unsigned char *)EMPTY_B);
	//LCD12864_Write_Numberf(2,1,float number)

}

void l2_main_page_info_Interface(void)
{
}

void l2_main_page_warn_Interface(void)
{
}
void l2_main_page_set_Interface(void)
{
}

void l3_main_page_infoDATE_Interface(void)
{
}

void l3_main_page_infoMAINTAIN_Interface(void)
{
}
void l3_main_page_infoSER_Interface(void)
{
}
void l3_main_page_infoRESTORE_Interface(void)
{
}
void l3_main_page_infoMOTOR_Interface(void)
{
}
void l3_main_page_infoHARDWARE_Interface(void)
{
}
void l3_main_page_infoSERIAL_Interface(void)
{
}
void l3_main_page_infoPRODUCT_Interface(void)
{
}
void l2_page_infoCOUNTER_Interface(void)
{
}



Menu_table_t taskTable[] =
{
    //菜单界面函数 -- 一级界面

    //*************main page *****************************************
    {MAIN_PAGE, RUN_ICON, MAIN_PAGE_RUN, 0, l1_main_page_Interface, 0, 0},
    {MAIN_PAGE, INFOR_ICON, MAIN_PAGE_INFO, 0, l2_main_page_info_Interface, 0, 0},
    {MAIN_PAGE, WARN_ICON, MAIN_PAGE_WARN, 0, l2_main_page_warn_Interface, 0, 0},
    {MAIN_PAGE, SETTING_ICON, MAIN_PAGE_SET, 0, l2_main_page_set_Interface, 0, 0},

//    //功能界面函数 -- 二级界面
    //*************MAIN_PAGE_RUN *****************************************

    {MAIN_PAGE_RUN, INFOR_ICON, MAIN_PAGE_INFO, 0, l2_main_page_info_Interface, 0, 0},
    {MAIN_PAGE_RUN, WARN_ICON, MAIN_PAGE_WARN, 0, l2_main_page_warn_Interface, 0, 0},
    {MAIN_PAGE_RUN, SETTING_ICON, MAIN_PAGE_SET, 0, l2_main_page_set_Interface, 0, 0},
    {MAIN_PAGE_RUN, RETURN_ICON, MAIN_PAGE_RUN, 0, l1_main_page_Interface, 0, 0},
    //{MAIN_PAGE_RUN, RETURN_ICON, MAIN_PAGE_RUN, 0, l1_main_page_Interface, 0, 0},
    {MAIN_PAGE_RUN, MAIN_PAGE_MANUAL_ICON, MAIN_PAGE_RUN, 0, l1_main_page_Interface, 0, 0},
    {MAIN_PAGE_RUN, MAIN_PAGE_PULSE_ICON, MAIN_PAGE_RUN, 0, l1_main_page_Interface, 0, 0},
    {MAIN_PAGE_RUN, MAIN_PAGE_CURR0_ICON, MAIN_PAGE_RUN, 0, l1_main_page_Interface, 0, 0},
    {MAIN_PAGE_RUN, MAIN_PAGE_CURR4_ICON, MAIN_PAGE_RUN, 0, l1_main_page_Interface, 0, 0},
    {MAIN_PAGE_RUN, MAIN_PAGE_PRODUC_ICON, MAIN_PAGE_RUN, 0, l1_main_page_Interface, 0, 0},





    //*************MAIN_PAGE_INFO *****************************************

    {MAIN_PAGE_INFO, RUN_ICON, MAIN_PAGE_RUN, 0, l2_main_page_info_Interface, 0, 0},
    {MAIN_PAGE_INFO, WARN_ICON, MAIN_PAGE_WARN, 0, l2_main_page_info_Interface, 0, 0},
    {MAIN_PAGE_INFO, SETTING_ICON, MAIN_PAGE_SET, 0, l2_main_page_info_Interface, 0, 0},
    {MAIN_PAGE_INFO, RETURN_ICON, MAIN_PAGE_RUN, 0, l2_main_page_info_Interface, 0, 0},


//*************MAIN_PAGE_WARN *****************************************

    {MAIN_PAGE_WARN, RUN_ICON, MAIN_PAGE_RUN, 0, l2_main_page_warn_Interface, 0, 0},
    {MAIN_PAGE_WARN, INFOR_ICON, MAIN_PAGE_INFO, 0, l2_main_page_warn_Interface, 0, 0},
    {MAIN_PAGE_WARN, SETTING_ICON, MAIN_PAGE_SET, 0, l2_main_page_warn_Interface, 0, 0},
    {MAIN_PAGE_WARN, RETURN_ICON, MAIN_PAGE_RUN, 0, l2_main_page_warn_Interface, 0, 0},
//*************MAIN_PAGE_SET *****************************************

    {MAIN_PAGE_SET, RUN_ICON, MAIN_PAGE_RUN, 0, l2_main_page_set_Interface, 0, 0},
    {MAIN_PAGE_SET, INFOR_ICON, MAIN_PAGE_INFO, 0, l2_main_page_set_Interface, 0, 0},
    {MAIN_PAGE_SET, SETTING_ICON, MAIN_PAGE_SET, 0, l2_main_page_set_Interface, 0, 0},
    {MAIN_PAGE_SET, RETURN_ICON, MAIN_PAGE_RUN, 0, l2_main_page_set_Interface, 0, 0},


//    //功能设置界面函数 -- 三级界面
//*************MAIN_PAGE_INFO *****************************************
    {MAIN_PAGE_INFO, INFO_DATE_ICON, MAIN_PAGE_INFO, 0, l3_main_page_infoDATE_Interface, 0, 0},
    // {MAIN_PAGE_INFO, INFO_COUNTER_ICON, MAIN_PAGE_INFO, 0, main_page_info_Interface, 0, 0},
    {MAIN_PAGE_INFO, INFO_MAINTAIN_ICON, MAIN_PAGE_INFO, 0, l3_main_page_infoMAINTAIN_Interface, 0, 0},
    {MAIN_PAGE_INFO, INFO_SER_PACK_ICON, MAIN_PAGE_INFO, 0, l3_main_page_infoSER_Interface, 0, 0},
    {MAIN_PAGE_INFO, INFO_RESTORE_ICON, MAIN_PAGE_INFO, 0, l3_main_page_infoRESTORE_Interface, 0, 0},
    {MAIN_PAGE_INFO, INFO_MOTOR_CTRL_ICON, MAIN_PAGE_INFO, 0, l3_main_page_infoMOTOR_Interface, 0, 0},
    {MAIN_PAGE_INFO, INFO_HARDWARE_ICON, MAIN_PAGE_INFO, 0, l3_main_page_infoHARDWARE_Interface, 0, 0},
    {MAIN_PAGE_INFO, INFO_SERIAL_ICON, MAIN_PAGE_INFO, 0, l3_main_page_infoSERIAL_Interface, 0, 0},
    {MAIN_PAGE_INFO, INFO_PRODUCT_ICON, MAIN_PAGE_INFO, 0, l3_main_page_infoPRODUCT_Interface, 0, 0},
    {MAIN_PAGE_INFO, INFO_COUNTER_ICON, PAGE_INFO_COUNTER, 0, l2_page_infoCOUNTER_Interface, 0, 0},
};




void menu_init()
{
    taskIndex = 0;
	 lcd_init();

}
void Menu_Interface_manage()
{
    if (get_key_state()->ok_press_update == 1)
    {
        if (taskTable[taskIndex].mode == 1)
            taskTable[taskIndex].mode = 0;
        else
            taskTable[taskIndex].mode = 1;
        get_key_state()->ok_press_update = 0;
    }
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
    Menu_Interface_manage();
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

  

}

void lcd_proc()
{
    static uint32_t tick;

    
    registerTick(LCD_FUCN_SW_TICK,FRESH_TIME);
	if(GetTickResult(LCD_FUCN_SW_TICK) == 1)
	{
	    menu_manage();
		taskTable[taskIndex].Current_Operation();//执行函数
		reset_registerTick(LCD_FUCN_SW_TICK);
//	    lcd_proc_test();
	}
	

}

