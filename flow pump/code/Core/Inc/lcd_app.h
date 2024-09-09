#ifndef LCD_APP_H_
#define LCD_APP_H_
#include "main.h"
typedef struct
{
	uint8_t CurrentNum;	//当前索引序号:页码
	uint8_t Enter;		//确认键
	uint8_t Next;		//下一个
	uint8_t Return;		//返回键
	void (*Current_Operation)(void);//当前操作(函数指针)
}Menu_table_t;


#endif

