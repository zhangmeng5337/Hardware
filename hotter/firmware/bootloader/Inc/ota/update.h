#ifndef __UPDATE_H__
#define __UPDATE_H__

#include "main.h"
#include "stdio.h"




/* �����Ĳ��� */
#define Startup_Normal 0xFF	//��������
#define Startup_Update 0xAA	//����������

unsigned int Read_Start_Mode(void);
void  MoveCode(uint32_t src_addr, uint32_t des_addr, uint32_t byte_size);
void IAP_ExecuteApp (uint32_t App_Addr);
void Start_BootLoader(void);
void test(void);
#endif
