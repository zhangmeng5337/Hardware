#ifndef DI_H_
#define DI_H_
#include "main.h"
#define DI_SIZE  16
//*****di status*********************



typedef struct
{
	uint32_t di_status;//di31...0 ---->di0..7  wdi 0..7 pnp 0...7 npn0...7
} stru_di_stru;

uint32_t di_input_read(void);
void di_proc(void);


#endif

