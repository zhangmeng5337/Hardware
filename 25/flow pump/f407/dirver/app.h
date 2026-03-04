#ifndef APP_H_
#define APP_H_
#include "main.h"
typedef struct
{
 uint32_t adcRefreshTime;
 uint32_t lcdRefreshTime;
 uint32_t flashRefreshTime;
 unsigned char rotation;
}config_stru;
void app(void);
#endif

