/*
*/
#ifndef __AD_SPI1_H
#define __AD_SPI1_H


#define AD_SPI1_CS_L       {GPIO_ResetBits(GPIOC, GPIO_Pin_1);}
#define AD_SPI1_CS_H       {GPIO_SetBits(GPIOC, GPIO_Pin_1);}


void ADSPI1Intial(void);
unsigned int ADSPI1collectionADSignal(unsigned char channel,unsigned char *erro);
#endif
//$
