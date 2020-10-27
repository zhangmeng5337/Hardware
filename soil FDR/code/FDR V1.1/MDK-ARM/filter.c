#include "filter.h"
unsigned int *value_buf;
extern  unsigned int adcBuf_ref[N];	
extern	unsigned int adcBuf_humid[N];	
extern	unsigned int adcBuf_ta[N];
extern	unsigned int adcBuf_tb[N];

/****************************************
¾ùÖµÂË²¨
*****************************************/
uint32_t filter(uint32_t seq)  
{  
   unsigned char count;  
     
   uint32_t  sum=0;  
			switch(seq)
			{
				case 0: value_buf=adcBuf_ref;break;
				case 1: value_buf=adcBuf_humid;break;
				case 2: value_buf=adcBuf_ta;break;
				case 3: value_buf=adcBuf_tb;break;				
			}
//   for (j=0;j<N-1;j++)  
//   {  
//      for (i=0;i<N-j;i++)  
//      {  
//         if ( value_buf[i]>value_buf[i+1] )  
//         {  
//            temp = value_buf[i];  
//            value_buf[i] = value_buf[i+1];   
//             value_buf[i+1] = temp;  
//         }  
//      }  
//   }  
   for(count=0;count<N;count++)  
      sum += value_buf[count];  
   return (sum/(N));  
}  
