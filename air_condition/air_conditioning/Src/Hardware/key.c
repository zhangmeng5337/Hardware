#include "key.h"
extern unsigned char KeyNum;
unsigned char KeyStatus;
void key_scan()
{
	if(KeyNum&0x03)
	{
		if(KeyNum&0x02)
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