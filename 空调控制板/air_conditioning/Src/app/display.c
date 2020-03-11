#include "74hc595.h"
#include "display.h"
const unsigned char seg_table[16]={0};
void display(float dat,unsigned char seg_pos)
{
	unsigned char buf[4],i;
	unsigned char status;
	uint32_t tmp,tmp2;
	i = 0;
	status = 0;
	if(dat<10)
	{
		tmp=(uint32_t)(dat*1000);
		
		tmp2 = (unsigned char)(tmp/1000);
		buf[3] = seg_table[tmp2];
    if(tmp2>0)
			status = status | 0x08;
		else
			status = status &  (~0x08);
		
		tmp2 = (unsigned char)(tmp%1000/100);
		buf[2] = seg_table[tmp2];	
    if(tmp2>0)
			status = status | 0x04;
		else
			status = status & (~0x04);
		
		tmp2 = (unsigned char)(tmp%100/10);
		buf[1] = seg_table[tmp2];	
    if(tmp2>0)
			status = status | 0x02;
		else
			status = status &  (~0x02);
		
		tmp2 = (unsigned char)(tmp%10);
		buf[0] = seg_table[tmp2];
    if(tmp2>0)
			status = status | 0x01;
		else
			status = status &  (~0x01);
	}
	switch(status)
	{
		case 0x0e:
    {
			buf[0] = buf[1];
			buf[1] = buf[2];
			buf[2] = buf[3]+DOT;
		}break;
		case 0x0c:
    {
			buf[0] = buf[2];
			buf[1] = buf[3]+DOT;
		}break;
		case 0x08:
    {
			buf[0] = buf[3];
		}break;
		case 0x00: 
    {
			buf[3] = buf[3]+DOT;
		}break;			
	}
	
	if(dat>=10&&dat<100)
	{
		tmp=(uint32_t)(dat*100);
		
		tmp2 = (unsigned char)(tmp/1000);
		buf[3] = seg_table[tmp2];
    if(tmp2>0)
			status = status | 0x08;
		else
			status = status &  (~0x08);
		
		tmp2 = (unsigned char)(tmp%1000/100);
		buf[2] = seg_table[tmp2];	
    if(tmp2>0)
			status = status | 0x04;
		else
			status = status & (~0x04);
		
		tmp2 = (unsigned char)(tmp%100/10);
		buf[1] = seg_table[tmp2];	
    if(tmp2>0)
			status = status | 0x02;
		else
			status = status &  (~0x02);
		
		tmp2 = (unsigned char)(tmp%10);
		buf[0] = seg_table[tmp2];
    if(tmp2>0)
			status = status | 0x01;
		else
			status = status &  (~0x01);
	}
	switch(status)
	{
		case 0x0e:
    {
			buf[0] = buf[1];
			buf[1] = buf[2]+DOT;
			buf[2] = buf[3];
		}break;
		case 0x0c:
    {
			buf[0] = buf[2];
			buf[1] = buf[3];
		}break;
		case 0x08:
    {
			buf[0] = buf[3];
		}break;
		default: 
    {
			buf[2] = buf[2]+DOT;
		}break;			
	}	
	
	
	if(dat>=100&&dat<1000)
	{
		tmp=(uint32_t)(dat*10);
		
		tmp2 = (unsigned char)(tmp/1000);
		buf[3] = seg_table[tmp2];
    if(tmp2>0)
			status = status | 0x08;
		else
			status = status &  (~0x08);
		
		tmp2 = (unsigned char)(tmp%1000/100);
		buf[2] = seg_table[tmp2];	
    if(tmp2>0)
			status = status | 0x04;
		else
			status = status & (~0x04);
		
		tmp2 = (unsigned char)(tmp%100/10);
		buf[1] = seg_table[tmp2];	
    if(tmp2>0)
			status = status | 0x02;
		else
			status = status &  (~0x02);
		
		tmp2 = (unsigned char)(tmp%10);
		buf[0] = seg_table[tmp2];
    if(tmp2>0)
			status = status | 0x01;
		else
			status = status &  (~0x01);
	}
	switch(status)
	{
		case 0x0e:
    {
			buf[0] = buf[1];
			buf[1] = buf[2];
			buf[2] = buf[3];
		}break;
		case 0x0c:
    {

		}break;
		case 0x08:
    {

		}break;
		default: 
    {
			buf[1] = buf[1]+DOT;
		}break;			
	}		
	if(dat>=1000&&dat<10000)
	{
		tmp=(uint32_t)(dat*1);
		
		tmp2 = (unsigned char)(tmp/1000);
		buf[3] = seg_table[tmp2];
    if(tmp2>0)
			status = status | 0x08;
		else
			status = status &  (~0x08);
		
		tmp2 = (unsigned char)(tmp%1000/100);
		buf[2] = seg_table[tmp2];	
    if(tmp2>0)
			status = status | 0x04;
		else
			status = status & (~0x04);
		
		tmp2 = (unsigned char)(tmp%100/10);
		buf[1] = seg_table[tmp2];	
    if(tmp2>0)
			status = status | 0x02;
		else
			status = status &  (~0x02);
		
		tmp2 = (unsigned char)(tmp%10);
		buf[0] = seg_table[tmp2];
    if(tmp2>0)
			status = status | 0x01;
		else
			status = status &  (~0x01);
	}

}