#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "main.h"
#include "stdio.h"
//#include "cat1.h"



/* �����Ĳ��� */
#define Startup_Normal 0xFF	//��������
#define Startup_Update 0xAA	//����������

unsigned int Read_Start_Mode(void);
void MoveCode(unsigned int src_addr, unsigned int des_addr, unsigned int byte_size);
void IAP_ExecuteApp (uint32_t App_Addr);
void Start_BootLoader(void);
void test(void);
void boot_jump(void);

#endif
