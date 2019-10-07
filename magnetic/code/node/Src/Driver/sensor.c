#include "main.h"
#include "bsp.h"
#include "rm3100.h"
#include "sensor.h"

		static BOOL flag;
unsigned int temptime;

float elapsed_time;

/** Temporization functions */
float get_time (void)
{


    // Colocar em fun??o
    if(flag == 0)
    {
        flag = 1;
		temptime = HAL_GetTick(); // Reads time elapsed
       // elapsed_time = HAL_GetTick() -temptime;// tempo = valortimer*overflow*prescaler*4/fosc ;
	}
	else
		elapsed_time = HAL_GetTick() -temptime;// tempo = valortimer*overflow*prescaler*4/fosc ;
    return elapsed_time;
}

void reset_timer (void)
{
   flag = 0;
   temptime = HAL_GetTick();
}
 // short int ManeticBuffer[3];


