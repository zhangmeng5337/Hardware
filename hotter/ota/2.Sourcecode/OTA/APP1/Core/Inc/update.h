#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "main.h"
#include "stdio.h"
#include "usart.h"
#include "stmflash.h"
#include "cat1.h"

/* �����Ĳ��� */
#define Startup_Normal 0xFF	//��������
#define Startup_Update 0xAA	//����������

void Update_Success(void);
void OTA_Task(void);

#endif
