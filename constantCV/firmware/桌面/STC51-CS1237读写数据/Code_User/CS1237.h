#ifndef __CS1237_H__
#define __CS1237_H__
#include<STC15W.h>
#include<intrins.h>

//����CS1237оƬ
void Con_CS1237(void);
//��ȡоƬ����������
unsigned char Read_CON(void);
//��ȡADC����
long Read_CS1237(void);

#endif