#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "main.h"
#include "stdio.h"




/* 启动的步骤 */
#define Startup_Normal 0xFF	//正常启动
#define Startup_Update 0xAA	//升级再启动
#define OTA_NUM_ADDR   0x801fff0
unsigned int Read_Start_Mode(void);
void  MoveCode(uint32_t src_addr, uint32_t des_addr, uint32_t byte_size);
void IAP_ExecuteApp (uint32_t App_Addr);
void Start_BootLoader(void);
void test(void);
#endif
