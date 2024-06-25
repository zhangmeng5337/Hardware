#include "my_string.h"
#include "my_string.h"

#define POLY	0x1021  

/***********************************************************
  �������ƣ�Find_string(char *pcBuf,char*left,char*right, char *pcRes)
  �������ܣ�Ѱ���ض��ַ���
  ��ڲ�����
           char *pcBuf Ϊ������ַ���
           char*left   Ϊ�����ַ�����߱�ʶ��  ���磺"["
           char*right  Ϊ�����ַ����ұ߱�ʶ��  ���磺"]"
					 char *pcRes Ϊ���ת����ַ�������
  ����ֵ������У���Ƿ�ɹ�������ν�ġ�
  ��ע�� left�ַ���ҪΨһ��right�ַ���left���濪ʼΨһ����
***********************************************************/
int Find_string(char *pcBuf,char *left,char *right, char *pcRes)
{
	char *pcBegin = NULL;
	char *pcEnd = NULL;
	pcBegin = strstr(pcBuf, left);//�ҵ���һ�γ��ֵ�λ��
	pcEnd = strstr(pcBegin+strlen(left), right);//�ҵ��ұ߱�ʶ
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
 * @bieaf CRC-16 У��
 *
 * @param addr ��ʼ��ַ
 * @param num   ����
 * @param num   CRC
 * @return crc  ����CRC��ֵ
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


/**���ַ������м��ȡn���ַ�
	 nΪ���ȣ�mΪλ��
	*/
char * mid(char *dst,char *src, int n,int m)
{
    char *p = src;
    char *q = dst;
    int len = strlen(src);
    if(n>len) n = len-m;    /*�ӵ�m�������*/
    if(m<0) m=0;    /*�ӵ�һ����ʼ*/
    if(m>len) return NULL;
    p += m;
    while(n--) *(q++) = *(p++);
    *(q++)='\0'; /*�б�Ҫ�𣿺��б�Ҫ*/
    return dst;
}
