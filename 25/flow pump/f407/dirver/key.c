#include "key.h"
#include "bsp_encoder.h"
key_stru  keyTable[] = 
{
	{0,0,0}
};
key_stru *getEncoderKey()
{
	return &keyTable[ENCODER_KEY_INDEX];
}
void keyProc(void)
{
	if(get_button()->status == 1)
	{
	    keyTable[ENCODER_KEY_INDEX].keyStatus = 1;
		keyTable[ENCODER_KEY_INDEX].key_dir = get_button()->dir;
		 
	}
	keyTable[ENCODER_KEY_INDEX].keyEnter = get_button()->enter;
}
