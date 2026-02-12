/*
*/
#ifndef __AD_SPI3_H
#define __AD_SPI3_H
//........................................................................
#define AD_SPI3_CS_L       {GPIO_ResetBits(GPIOA, GPIO_Pin_8);}
#define AD_SPI3_CS_H       {GPIO_SetBits(GPIOA, GPIO_Pin_8);}
//........................................................................
#define AD_SPI3_NUMBRR      10
//........................................................................
void ADSPI3Intial(void);
unsigned int ADSPI3collectionADSignal(unsigned char channel,unsigned char *erro);
#endif
//$
