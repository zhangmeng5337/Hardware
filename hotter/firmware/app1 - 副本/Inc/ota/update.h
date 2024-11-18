#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "main.h"
#include "stdio.h"
//#include "cat1.h"



/* 启动的步骤 */
#define Startup_Normal 0xFF	//正常启动
#define Startup_Update 0xAA	//升级再启动

unsigned int Read_Start_Mode(void);
void MoveCode(unsigned int src_addr, unsigned int des_addr, unsigned int byte_size);
void IAP_ExecuteApp (uint32_t App_Addr);
void Start_BootLoader(void);
void test(void);
void boot_jump(void);

#endif
