#ifndef __SPI_H
#define __SPI_H

#define SPI3_SPEED 20

#include "ddl.h"
//#include "BSP_cmt2300a.h"
//#include "Delay.h"

#ifndef	byte
typedef unsigned char byte;
#endif

#ifndef word
typedef unsigned int  word;
#endif













//#define cmt_spi3_csb_out()      GPIO_Init(SPI_PORT,CSB_PIN, GPIO_Mode_Out_PP_High_Slow)
//#define cmt_spi3_fcsb_out()     GPIO_Init(SPI_PORT,FCSB_PIN, GPIO_Mode_Out_PP_High_Slow)
//#define cmt_spi3_scl_out()      GPIO_Init(SPI_PORT,SCLK_PIN, GPIO_Mode_Out_PP_High_Slow)
//       
//          
//          
//          
//          
//          
//          
#define	SetCSB()	Gpio_SetIO(SPI_PORT, CSB_PIN, 1)
#define	ClrCSB()	Gpio_SetIO(SPI_PORT, CSB_PIN, 0)

#define	SetFCSB()	Gpio_SetIO(SPI_PORT, FCSB_PIN, 1)
#define	ClrFCSB()	Gpio_SetIO(SPI_PORT, FCSB_PIN, 0)

#define	SetSDCK()	Gpio_SetIO(SPI_PORT, SCLK_PIN, 1)
#define	ClrSDCK()	Gpio_SetIO(SPI_PORT, SCLK_PIN, 0)

#define	SetSDIO()	Gpio_SetIO(SPI_PORT, SDIO_PIN, 1)
#define	ClrSDIO()	Gpio_SetIO(SPI_PORT, SDIO_PIN, 0)

#define   InputSDIO()	Gpio_InitIOExt(SPI_PORT,SDIO_PIN, GpioDirIn, TRUE, FALSE, FALSE, 0)//SDIO 
#define	OutputSDIO()	Gpio_InitIOExt(SPI_PORT,SDIO_PIN, GpioDirOut, TRUE, FALSE, FALSE, 0)//SDIO   

#define	SDIO_H()	 Gpio_GetIO(SPI_PORT, SDIO_PIN) 
#define	SDIO_L()	 Gpio_GetIO(SPI_PORT, SDIO_PIN)  


  void vSpi3Init(void);				/** initialize software SPI-3 **/	
  void vSpi3Write(word dat);			/** SPI-3 send one word **/
  byte bSpi3Read(byte addr);			/** SPI-3 read one byte **/
  
  void vSpi3WriteFIFO(byte dat);		/** SPI-3 send one byte to FIFO **/
  byte bSpi3ReadFIFO(void);			/** SPI-3 read one byte from FIFO **/
  void vSpi3BurstWriteFIFO(byte ptr[], byte length);			/** SPI-3 burst send N byte to FIFO**/
  void vSpi3BurstReadFIFO(byte ptr[], byte length);			/** SPI-3 burst read N byte to FIFO**/

  void vSpi3WriteByte(byte dat);		/** SPI-3 send one byte **/
  byte bSpi3ReadByte(void);			/** SPI-3 read one byte **/
  



#else
#warning "spi.h have been defined!"

#endif 

