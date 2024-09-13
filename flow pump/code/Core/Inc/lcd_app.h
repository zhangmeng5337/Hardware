#ifndef LCD_APP_H_
#define LCD_APP_H_
#include "main.h"

#define FRESH  200
//*****************page define*************
#define MAIN_PAGE 1
#define MAIN_PAGE_RUN 2
#define MAIN_PAGE_INFO 3
#define MAIN_PAGE_WARN 4
#define MAIN_PAGE_SET  5


//************************MAIN_PAGE icon define 
#define RUN_ICON		1
#define INFOR_ICON  	2	
#define WARN_ICON   	3
#define SETTING_ICON 	4
#define RETURN_ICON  	5
typedef struct
{
   // unsigned char mode;
    uint8_t CurrentNum;	//��ǰ�������:ҳ��
	unsigned char current_icon_num;//��ǰicon
	unsigned char next;
	unsigned char mode;//1��ѡ��
    unsigned char max_icon_num;
	void (*Current_Operation)(void);//��ǰ����(����ָ��)
}Menu_table_t;
typedef struct
{
    
	//unsigned char current_icon_num;//��ǰicon
	//uint8_t CurrentNum;	//��ǰ�������:ҳ��
	uint8_t enter_status;		//ͬһҳ���£�ȷ�ϼ�,��һ��ѡ���ٰ�һ��ȡ��
	uint8_t next_status;		//ͬһҳ���£���һ��ͼ����ת
	uint8_t return_status;		//ͬһҳ���£����ؼ�
	uint32_t tick;
}menu_key_stru;

void lcd_proc(void);

#endif

