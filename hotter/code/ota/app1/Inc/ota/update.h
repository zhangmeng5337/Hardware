#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "main.h"
#include "stdio.h"
#include "usart.h"
#include "stmflash.h"
#include "cat1.h"

/* 启动的步骤 */
#define Startup_Normal 0xFF	//正常启动
#define Startup_Update 0xAA	//升级再启动

void Update_Success(void);
void OTA_Task(void);

#endif
