#include <string.h>
#include "mystring.h"

uint8_t *uart_Check_Cmd(uint8_t *pb,uint8_t *str)
{
    char *strx ;
    strx = strstr((const char *)(pb), (const char *)str); //Ѱ���ı�(��Ѱ�ң���Ѱ��
    return (uint8_t *)strx;

}	char *pcBegin = NULL;
	char *pcEnd = NULL;
int Find_string(char *pcBuf,char *left,char *right, char *pcRes)
{

	pcBegin = strstr(pcBuf, left);//??????��?��?
	pcEnd = strstr(pcBegin+strlen(left), right);//??????
	if(pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd)
	{
		//printf("string name not found!\n");
		return 0;
	}
	else
	{
		pcBegin += strlen(left);
		memcpy(pcRes, pcBegin, pcEnd-pcBegin);
		return 1;
	}
}

