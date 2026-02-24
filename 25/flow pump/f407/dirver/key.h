#ifndef KEY_H_
#define KEY_H_
#define ENCODER_KEY_INDEX 0
typedef struct
{
	unsigned key_dir;
	unsigned char keyStatus;
	unsigned char keyEnter;
}key_stru;
key_stru *getEncoderKey(void);
#endif

