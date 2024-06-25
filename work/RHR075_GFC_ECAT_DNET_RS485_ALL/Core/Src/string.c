#include <string.h>

uint8_t *uart_Check_Cmd(uint8_t pb,uint8_t *str)
{
    char *strx ;
    strx = strstr((const char *)(pb), (const char *)str); //Ñ°ÕÒÎÄ±¾(±»Ñ°ÕÒ£¬ÓûÑ°³Ğ
    return (uint8_t *)strx;

}

