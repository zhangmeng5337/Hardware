#include "key.h"
#include "main.h"
extern unsigned char KeyNum;
unsigned char KeyStatus;
extern uint32_t key_time;
key_stru key;


void key_scan()
{
    if(key.keynum != 0)
    {
		key.update = 1;
	}
	if(key.keynum &0x03)
	{
		while(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)==GPIO_PIN_RESET)
		{
			if((HAL_GetTick()-key.keytime)>=LONG_HIT)
			{
				key.keynum = key.keynum |0x02;
				break;
			}
		}
		
		if(key.keynum == 0x01)
		{  
		    key.keynum = 0;
			key.key_status = 0x01;//key1 短按 
			#if DEBUG_USER
			  printf("key1 is short hit\n");
	    #endif
		}
		else
		{    key.keynum = 0;
			key.key_status = 0x02;	//key1 长按		
		  #if DEBUG_USER
			  printf("key1 is long hit\n");
	    #endif
		}
	}
	else if(key.keynum&0x0c)
	{
		while(HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)==GPIO_PIN_RESET)
		{
			if((HAL_GetTick()-key.keytime)>=LONG_HIT)
			{  
				key.keynum = key.keynum |0x08;
				break;
			}
		}
		if(key.keynum==0x04)
		{ key.keynum = 0;
			key.key_status = 0x03;//key2 短按
		 #if DEBUG_USER
			  printf("key2 is short hit\n");
	    #endif
		}
			else
			{ key.keynum = 0;
				key.key_status = 0x04;	
		   #if DEBUG_USER
			  printf("key2 is long hit\n");
	     #endif
			}				
	}
	else 	if(key.keynum&0x30)
	{
		while(HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)==GPIO_PIN_RESET)
		{
			if((HAL_GetTick()-key.keytime)>=LONG_HIT)
			{
				key.keynum = key.keynum |0x20;
				break;
			}
		}
		if(key.keynum==0x10)
		{ key.keynum = 0;
			key.key_status = 0x05;//key3 短按
		 #if DEBUG_USER
			  printf("key3 is short hit\n");
	    #endif
		}
		else
		{ key.keynum = 0;
			key.key_status = 0x06;	
		   #if DEBUG_USER
			  printf("key3 is long hit\n");
	     #endif
		}			
	}
	else 	if(key.keynum&0xc0)
	{
		while(HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin)==GPIO_PIN_RESET)
		{
			if((HAL_GetTick()-key.keytime)>=LONG_HIT)
			{
				key.keynum = key.keynum |0x80;
				break;
			}
		}
		if(key.keynum==0x40)
		{ key.keynum = 0;
			key.key_status = 0x07;//key4 短按
		 #if DEBUG_USER
			  printf("key4 is short hit\n");
	    #endif
			
		}
		else
		{ key.keynum = 0;
			key.key_status = 0x08;
		   #if DEBUG_USER
			  printf("key4 is long hit\n");
	     #endif
		}			
	}

	
}

key_stru *GetKeyNum()
{
	return &key;
}
void SetKeyNum()
{
	key.key_status = 0;
	
}
key_stru * key_process()
{
    key_stru *KeyStatus2;
	key_scan();
	return &key;
}
GPIO_PinState get_key_level(unsigned char keyNo)
{
  if(keyNo == 1)
  	return HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin);
  else if(keyNo == 2)
   return HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin);
  else if(keyNo == 3)
   return HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin);
  else if(keyNo == 4)
   return HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin);
}