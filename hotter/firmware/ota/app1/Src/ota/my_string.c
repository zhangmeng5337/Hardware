#include "my_string.h"

#define POLY	0x1021  

/***********************************************************
  函数名称：Find_string(char *pcBuf,char*left,char*right, char *pcRes)
  函数功能：寻找特定字符串
  入口参数：
           char *pcBuf 为传入的字符串
           char*left   为搜索字符的左边标识符  例如："["
           char*right  为搜索字符的右边标识符  例如："]"
					 char *pcRes 为输出转存的字符串数组
  返回值：用来校验是否成功，无所谓的。
  备注： left字符需要唯一，right字符从left后面开始唯一即可
***********************************************************/
int Find_string(char *pcBuf,char *left,char *right, char *pcRes)
{
	char *pcBegin = NULL;
	char *pcEnd = NULL;
	pcBegin = strstr(pcBuf, left);//找到第一次出现的位置
	pcEnd = strstr(pcBegin+strlen(left), right);//找到右边标识
	if(pcBegin == NULL || pcEnd == NULL || pcBegin > pcEnd)
	{
		printf("string name not found!\n");
		return 0;
	}
	else
	{
		pcBegin += strlen(left);
		memcpy(pcRes, pcBegin, pcEnd-pcBegin);
		return 1;
	}
}
/**
 * @bieaf CRC-16 校验
 *
 * @param addr 开始地址
 * @param num   长度
 * @param num   CRC
 * @return crc  返回CRC的值
 */
uint16_t crc16(unsigned char *addr, int num, uint16_t crc)
{  
    int i;  
    for (; num > 0; num--)					/* Step through bytes in memory */  
    {  
        crc = crc ^ (*addr++ << 8);			/* Fetch byte from memory, XOR into CRC top byte*/  
        for (i = 0; i < 8; i++)				/* Prepare to rotate 8 bits */  
        {
            if (crc & 0x8000)				/* b15 is set... */  
                crc = (crc << 1) ^ POLY;  	/* rotate and XOR with polynomic */  
            else                          	/* b15 is clear... */  
                crc <<= 1;					/* just rotate */  
        }									/* Loop for 8 bits */  
        crc &= 0xFFFF;						/* Ensure CRC remains 16-bit value */  
    }										/* Loop until num=0 */  
    return(crc);							/* Return updated CRC */  
}


/**从字符串的中间截取n个字符
	 n为长度，m为位置
	*/
char * mid(char *dst,char *src, int n,int m)
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    /*从第m个到最后*/
    if(m<0) m=0;    /*从第一个开始*/
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*有必要吗？很有必要*/
    return dst;
}
