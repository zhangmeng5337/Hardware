#ifndef RF_H
#define RF_H
#include "stm8l15x.h"
#include "mytypedef.h"
//����ģʽ



//POWER_MODE_Stru *GetPowerMode(void);
//Module_Params_stru *GetModuleParams(void);
//Module_mode_stru *GetModuleMode(void);
void RF_Initial(void);
void moduleconfig(void);
void lora_process(void);
void ModuleInit(void);
#endif

