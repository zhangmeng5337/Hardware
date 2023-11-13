#ifndef _LTE_HAL_H
#define _LTE_HAL_H
#include "main.h"

uint8_t* lte_Check_Cmd(uint8_t *str);
uint8_t lte_Send_Cmd(uint8_t *cmd, uint8_t *ack, unsigned int WaitTime);

uint8_t lte_Info_Show(void);
void lte_CallNum(uint8_t *Num);
void lte_CmdShowOff(void);
void lte_SendEN_SMS(uint8_t *phone,uint8_t *text);
void lte_proc(void);
void lte_init(void);

#endif


