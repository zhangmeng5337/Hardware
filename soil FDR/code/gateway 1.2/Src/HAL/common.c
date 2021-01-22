#include "common.h"

unsigned char *byteTostring(unsigned char *p,len)
{
    unsigned char i;
	unsigned char q[3];
	sprintf(q,"%d",p[0]);
    return q;
}
