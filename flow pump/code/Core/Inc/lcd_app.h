#ifndef LCD_APP_H_
#define LCD_APP_H_
#include "main.h"


//*****************page define*************
#define MAIN_PAGE 1
#define MAIN_PAGE_RUN 2
#define MAIN_PAGE_INFO 3
#define MAIN_PAGE_WARN 4
#define MAIN_PAGE_SET  5
#define PAGE_INFO_COUNTER  6


//************************MAIN_PAGE icon define
#define MAIN_PAGE_ICON_COUNT   5
#define RUN_ICON            	1
#define INFOR_ICON          	2
#define WARN_ICON           	3
#define SETTING_ICON        	4
#define RETURN_ICON         	5
#define MAIN_PAGE_MANUAL_ICON   6
#define MAIN_PAGE_PULSE_ICON    7
#define MAIN_PAGE_CURR0_ICON    8
#define MAIN_PAGE_CURR4_ICON    9
#define MAIN_PAGE_PRODUC_ICON   10



#define INFO_DATE_ICON      	6
#define INFO_COUNTER_ICON       7
#define INFO_MAINTAIN_ICON      8
#define INFO_SER_PACK_ICON      9
#define INFO_RESTORE_ICON       10
#define INFO_MOTOR_CTRL_ICON    11
#define INFO_HARDWARE_ICON      12
#define INFO_SERIAL_ICON    	13
#define INFO_PRODUCT_ICON 		14




typedef struct
{
    // unsigned char mode;
    uint8_t CurrentNum; //当前索引序号:页码
    unsigned char current_icon_num;//当前icon
    unsigned char next;

    unsigned char max_icon_num;
    void (*Current_Operation)(void);//当前操作(函数指针)
    unsigned char next_status; //1需要跳转
    unsigned char mode;//1：选中
} Menu_table_t;
typedef struct
{

    //unsigned char current_icon_num;//当前icon
    //uint8_t CurrentNum;   //当前索引序号:页码
    uint8_t enter_status;       //同一页面下，确认键,按一下选中再按一下取消
    uint8_t next_status;        //同一页面下，下一个图标跳转
    uint8_t return_status;      //同一页面下，返回键
    uint32_t tick;
} menu_key_stru;

void lcd_proc(void);
void menu_init(void);

#endif

