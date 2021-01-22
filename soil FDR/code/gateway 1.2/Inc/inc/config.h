#ifndef CONFIG_H_
#define CONFIG_H_
#include "main.h"

typedef struct{
unsigned char TcpServer[4];
unsigned int TcpServerPort;
unsigned char srcaddr[10];
unsigned char destaddr[10];
unsigned char channel;
unsigned char airrate;
}config_stru;
#endif
void configInit(void);

config_stru * getConfig(void);

