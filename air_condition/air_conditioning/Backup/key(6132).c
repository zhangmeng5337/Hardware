#include "key.h"
#include "main.h"
extern unsigned char KeyNum;
unsigned char KeyStatus;
extern uint32_t key_time;



void key_scan()
{
	if(KeyNum&0x03)
	{
		while(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)==GPIO_PIN_RESET)
		{
			if((HAL_GetTick()-key_time)>=LONG_HIT)
			{
				KeyNum = KeyNum |0x02;
				break;
			}
		}
		
		if(KeyNum == 0x01)
		{
			KeyStatus = 0x01;//key1 短按
			#if DEBUG_USER
			  printf("key1 is short hit\n");
	    #endif
		}
		else
		{
			KeyStatus = 0x02;	//key1 长按		
		  #if DEBUG_USER
			  printf("key1 is long hit\n");
	    #endif
		}
	}
	else if(KeyNum&0x0c)
	{
		while(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)==GPIO_PIN_RESET)
		{
			if((HAL_GetTick()-key_time)>=LONG_HIT)
			{
				KeyNum = KeyNum |0x08;
				break;
			}
		}
		if(KeyNum&0x08)
		{
			KeyStatus = 0x03;//key2 短按
		 #if DEBUG_USER
			  printf("key2 is short hit\n");
	    #endif
		}
			else
			{
				KeyStatus = 0x04;	
		   #if DEBUG_USER
			  printf("key2 is long hit\n");
	     #endif
			}				
	}
	else 	if(KeyNum&0x30)
	{
		while(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)==GPIO_PIN_RESET)
		{
			if((HAL_GetTick()-key_time)>=LONG_HIT)
			{
				KeyNum = KeyNum |0x20;
				break;
			}
		}
		if(KeyNum&0x20)
		{
			KeyStatus = 0x05;//key3 短按
		 #if DEBUG_USER
			  printf("key3 is short hit\n");
	    #endif
		}
		else
		{
			KeyStatus = 0x06;	
		   #if DEBUG_USER
			  printf("key3 is long hit\n");
	     #endif
		}			
	}
	else 	if(KeyNum&0xc0)
	{
		while(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin)==GPIO_PIN_RESET)
		{
			if((HAL_GetTick()-key_time)>=LONG_HIT)
			{
				KeyNum = KeyNum |0x80;
				break;
			}
		}
		if(KeyNum&0x80)
		{
			KeyStatus = 0x07;//key4 短按
		 #if DEBUG_USER
			  printf("key4 is short hit\n");
	    #endif
			
		}
		else
		{
			KeyStatus = 0x08;
		   #if DEBUG_USER
			  printf("key4 is long hit\n");
	     #endif
		}			
	}

	KeyNum = 0;
}

unsigned char GetKeyNum()
{
	return KeyStatus;
}
void SetKeyNum(unsigned char clr)
{
	KeyStatus = KeyStatus & clr;
	
}