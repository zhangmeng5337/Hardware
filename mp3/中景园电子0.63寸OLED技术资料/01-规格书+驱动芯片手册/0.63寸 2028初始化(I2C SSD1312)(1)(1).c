/* N063-2028TSWIG02-H14 */
/* SSD1312 IIC  */

#include "io51.h"

    
void ssd1312()
{
    RES=0;
    delay(1000);
    RES=1;
    delay(1000);
    
       write_i(0xAE);    /*display off*/

       write_i(0x08);    /*set lower column address*/       
       write_i(0x10);    /*set higher column address*/
       
       write_i(0xA8);    /*multiplex ratio*/
       write_i(0x1b);    /*duty = 1/28*/

       write_i(0xad);    /*External or Internal IREF Selection */
       write_i(0x40);

       write_i(0xD3);    /*set display offset*/
       write_i(0x12);

       write_i(0xA0);    /*set segment remap*/

       write_i(0xC8);    /*Com scan direction*/
      
       write_i(0xA6);    /*normal / reverse*/
       
       write_i(0x40);    /*set display start line*/
      
       write_i(0xA4);  

       write_i(0x81);    /*contract control*/
       write_i(0x30);    /*  30  */  

       write_i(0xD9);    /*set pre-charge period*/
       write_i(0x22);
       
       write_i(0xD5);    /*set osc division*/
       write_i(0x41); 

       write_i(0xDA);    /*set COM pins*/
       write_i(0x10);

       write_i(0x20);   /*Set Memory Addressing Mode */
       write_i(0x02);   /*Page Addressing Mode*/
       
       write_i(0xdb);    /*set vcomh*/
       write_i(0x30);
       
       write_i(0x8d);    /*set charge pump enable*/
       write_i(0x52);     /*  52-8.0V  12-7.5V */

       write_i(0xAF);    /*display ON*/                           

}

void write_w(unsigned char dat)
{
   unsigned char m,da;
   unsigned char j;
   da=dat;
   for(j=0;j<8;j++)
   {
     m=da;
     SCL=0;     
     m=m&0x80;
     if(m==0x80)
        {
      	  SDA=1;
        }
     else
        {
      	  SDA=0;
        }     
     da=da<<1;
     SCL=1;
    }
    SCL=0;
    SCL=1;
}
      
void write_i(unsigned char ins)
{
   start();
   write_w(0x78);
   write_w(0x00);
   write_w(ins);
   stop();
}

void write_d(unsigned char dat)
{
   start();
   write_w(0x78);
   write_w(0x40);
   write_w(dat);
   stop();
}          

 void  start()
{
  SCL=1;
  SDA=1;
  SDA=0;
  SCL=0;
}

void  stop()
{
  SCL=0;
  SDA=0;
  SDA=1;
  SCL=1;
}

void delay(unsigned int t)
{
     while(t>0)
       {
       	  t--;
       }
}

