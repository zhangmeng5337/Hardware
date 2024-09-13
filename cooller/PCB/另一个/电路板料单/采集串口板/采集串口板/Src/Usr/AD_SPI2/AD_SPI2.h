/*
*/
#ifndef __AD_SPI2_H
#define __AD_SPI2_H

#define AD_SPI2_CS_L       {GPIO_ResetBits(GPIOB, GPIO_Pin_12);}
#define AD_SPI2_CS_H       {GPIO_SetBits(GPIOB, GPIO_Pin_12);}



void ADSPI2Intial(void);
unsigned int ADSPI2collectionADSignal(unsigned char channel,unsigned char *erro);
#endif
//$
