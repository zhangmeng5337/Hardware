#ifndef LCD_APP_H_
#define LCD_APP_H_
#include "main.h"
typedef struct
{
	uint8_t CurrentNum;	//��ǰ�������:ҳ��
	uint8_t Enter;		//ȷ�ϼ�
	uint8_t Next;		//��һ��
	uint8_t Return;		//���ؼ�
	void (*Current_Operation)(void);//��ǰ����(����ָ��)
}Menu_table_t;


#endif

