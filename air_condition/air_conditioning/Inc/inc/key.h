#ifndef _KEY_H_
#define _KEY_H_
#include "main.h"
typedef struct{
 unsigned char key_status;
 unsigned char keynum;
 uint32_t keytime;
 unsigned char update;
}key_stru;
key_stru *GetKeyNum(void);
void SetKeyNum(void);
key_stru * key_process(void);

#endif
