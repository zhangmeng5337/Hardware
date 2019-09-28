 
#include "tea.h"
unsigned char TEA_key[16]=
{
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
    0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10
};
 
unsigned char TX_buffer[32];
unsigned char RX_buffer[32]; 
 
#define MX                (z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[p&3^e]^z)
#define DELTA             0x9e3779b9
#define S_LOOPTIME        1        //5
#define BLOCK_SIZE        31       //PAGE_SIZE 
/*
*key  maybe 128bit =16 Bytes.
*buf  maybe BLOCK_SIZE
*/
<<<<<<< HEAD
#include <stdio.h>
#include "tea.h"

/*******************
  TEA加密解密算法
*******************/

//TEA密钥
unsigned char TEA_key[16]=
{ 
    0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
    0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10
};

//数据缓冲区




#define MX                (z>>5^y<<2)+(y>>3^z<<4)^(sum^y)+(k[p&3^e]^z)
#define DELTA             0x9e3779b9
#define S_LOOPTIME        1        //5
#define BLOCK_SIZE        31       //PAGE_SIZE,根据你所要加密的数据包长度修改此参数(单位:字节)

/* 
*key  maybe 128bit =16 Bytes.
*buf  maybe BLOCK_SIZE
*/

void btea_encrypt( unsigned char* buf, unsigned char* key ,unsigned char len)
{
    unsigned char n=len/4;
=======
 
void btea_encrypt( unsigned char* buf, unsigned char* key )
{
    unsigned char n=BLOCK_SIZE/4;
>>>>>>> 634390e9e47a62b5fc22d79312c43b9dfb8b2958
    unsigned long *v=(unsigned long *)buf;
    unsigned long *k=(unsigned long *)key;
    unsigned long z = v[n - 1],y = v[0],sum = 0,e ;
    unsigned char p,q ;
<<<<<<< HEAD
    // Coding Part 
    
=======
    // Coding Part
     
>>>>>>> 634390e9e47a62b5fc22d79312c43b9dfb8b2958
    q = S_LOOPTIME + 52 / n ;
    while ( q-- > 0 )
    {
        sum += DELTA ;
        e = sum >> 2 & 3 ;
        for ( p = 0 ; p < n - 1 ; p++ )
          y = v[p + 1],
          z = v[p] += MX;
          y = v[0] ;
        z = v[n - 1] += MX;
    }
}
<<<<<<< HEAD


=======
 
 
>>>>>>> 634390e9e47a62b5fc22d79312c43b9dfb8b2958
/*
*key  maybe 128bit =16Bytes.
*buf  maybe BLOCK_SIZE
inbuf == outbuf == buf
*/
<<<<<<< HEAD

void btea_decrpyt( unsigned char* buf, unsigned char* key ,unsigned char len)
{
    unsigned char n=len/4;
=======
 
void btea_decrpyt( unsigned char* buf, unsigned char* key )
{
    unsigned char n=BLOCK_SIZE/4;
>>>>>>> 634390e9e47a62b5fc22d79312c43b9dfb8b2958
    unsigned long *v=(unsigned long *)buf;
    unsigned long *k=(unsigned long *)key;
    unsigned long z = v[n - 1],y = v[0],sum = 0,e ;
    unsigned char  p,q ;
<<<<<<< HEAD
    
=======
     
>>>>>>> 634390e9e47a62b5fc22d79312c43b9dfb8b2958
    //Decoding Part...
    q = S_LOOPTIME + 52 / n ;
    sum = q * DELTA ;
    while ( sum != 0 )
    {
        e = sum >> 2 & 3 ;
        for ( p = n - 1 ; p > 0 ; p-- )
            z = v[p - 1],
            y = v[p] -= MX;
            z = v[n - 1] ;
        y = v[0] -= MX;
        sum -= DELTA ;
<<<<<<< HEAD
    }
}

=======
    }}
>>>>>>> 634390e9e47a62b5fc22d79312c43b9dfb8b2958
