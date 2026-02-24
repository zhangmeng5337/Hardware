#include "my_string.h"
#include "my_string.h"
#include "sys.h"
#define POLY	0x1021  

/***********************************************************
  函数名称：Find_string(char *pcBuf,char*left,char*right, char *pcRes)
  函数功能：寻找特定字符串
  入口参数�?
           char *pcBuf 为传入的字符�?
           char*left   为搜索字符的左边标识�? 例如�?["
           char*right  为搜索字符的右边标识�? 例如�?]"
					 char *pcRes 为输出转存的字符串数�?
  返回值：用来校验是否成功，无所谓的�?
  备注�?left字符需要唯一，right字符从left后面开始唯一即可
***********************************************************/
int Find_string(char *pcBuf,char *left,char *right, char *pcRes)
{
	char *pcBegin = NULL;
	char *pcEnd = NULL;
	pcBegin = strstr(pcBuf, left);//找到第一次出现的位置
	pcEnd = strstr(pcBegin+strlen(left), right);//找到右边标识
	if(pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd)
	{
		;//printf("string name not found!\n");
		return 0;
	}
	else
	{
		pcBegin += strlen(left);
		memcpy(pcRes, pcBegin, pcEnd-pcBegin);
		return 1;
	}
}

unsigned char Find_string_left(char *pcBuf,char *left)
{
	char *pcBegin = NULL;
	char *pcEnd = NULL;
	pcBegin = strstr(pcBuf, left);//找到第一次出现的位置
	//pcEnd = strstr(pcBegin+strlen(left), right);//找到右边标识
	if(pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd)
	{
		;//printf("string name not found!\n");
		return 1;
	}
	else
	{
		//pcBegin += strlen(left);
		//memcpy(pcRes, pcBegin, pcEnd-pcBegin);
		return 0;
	}
}

/**
 * @bieaf CRC-16 校验
 *
 * @param addr 开始地址
 * @param num   长度
 * @param num   CRC
 * @return crc  返回CRC的�?
 */



/**从字符串的中间截取n个字�?
	 n为长度，m为位�?
	*/
char * mid(char *dst,char *src, int n,int m)
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    /*从第m个到最�?*/
    if(m<0) m=0;    /*从第一个开�?*/
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*有必要吗？很有必�?*/
    return dst;
}
