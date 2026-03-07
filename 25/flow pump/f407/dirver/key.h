#ifndef KEY_H_
#define KEY_H_
#define ENCODER_KEY_INDEX 	0
#define SW1_KEY_INDEX 		1
#define SW2_KEY_INDEX 		2
typedef struct
{
	unsigned key_dir;
	unsigned char keyStatus;
	unsigned char keyEnter;
}key_stru;
key_stru *getEncoderKey(unsigned char index);
#endif

