
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ---------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ---------------------*/
/* --- Web: www.stcmcudata.com -------------------------------------*/
/* --- QQ:  800003751 ----------------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/



#ifndef	_STC8H_8C_H
#define	_STC8H_8C_H

#include <intrins.h>

/*  BYTE Registers  */
sfr P0    = 0x80;
sfr SP    = 0x81;
sfr DPL   = 0x82;
sfr DPH   = 0x83;
sfr	S4CON = 0x84;
sfr	S4BUF = 0x85;
sfr PCON  = 0x87;

sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0  = 0x8A;
sfr TL1  = 0x8B;
sfr TH0  = 0x8C;
sfr TH1  = 0x8D;
sfr	AUXR = 0x8E;
sfr INT_CLKO = 0x8F;
sfr INTCLKO  = 0x8F;

sfr P1    = 0x90;
sfr P1M1  = 0x91;	//P1M1.n,P1M0.n 	=00--->Standard,	01--->push-pull		ʵ����1T�Ķ�һ��
sfr P1M0  = 0x92;	//					=10--->pure input,	11--->open drain
sfr P0M1  = 0x93;	//P0M1.n,P0M0.n 	=00--->Standard,	01--->push-pull
sfr P0M0  = 0x94;	//					=10--->pure input,	11--->open drain
sfr P2M1  = 0x95;	//P2M1.n,P2M0.n 	=00--->Standard,	01--->push-pull
sfr P2M0  = 0x96;	//					=10--->pure input,	11--->open drain

sfr SCON  = 0x98;
sfr SBUF  = 0x99;
sfr S2CON = 0x9A;	//
sfr S2BUF = 0x9B;	//
sfr IRCBAND = 0x9D;	//IRCƵ��ѡ����
sfr LIRTRIM = 0x9E;	//IRCƵ��΢���Ĵ���
sfr IRTRIM  = 0x9F;	//IRCƵ�ʵ����Ĵ���

sfr P2    = 0xA0;
sfr BUS_SPEED = 0xA1;
sfr P_SW1 = 0xA2;	//����˿��л��Ĵ���1

sfr IE    = 0xA8;
sfr SADDR = 0xA9;
sfr WKTCL = 0xAA;	//���Ѷ�ʱ�����ֽ�
sfr WKTCH = 0xAB;	//���Ѷ�ʱ�����ֽ�
sfr	S3CON = 0xAC;
sfr S3BUF = 0xAD;
sfr TA    = 0xAE;	//DPTRʱ����ƼĴ���
sfr IE2   = 0xAF;	//�ж�����Ĵ���2

sfr P3    = 0xB0;
sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain
sfr P4M1  = 0xB3;	//P4M1.n,P4M0.n 	=00--->Standard,	01--->push-pull
sfr P4M0  = 0xB4;	//					=10--->pure input,	11--->open drain
sfr IP2   = 0xB5;	//
sfr IP2H  = 0xB6;	//
sfr IPH   = 0xB7;

sfr IP        = 0xB8;
sfr SADEN     = 0xB9;
sfr	P_SW2     = 0xBA;	//����˿��л��Ĵ���2
sfr ADC_CONTR = 0xBC;	//ADC���ƼĴ���
sfr ADC_RES   = 0xBD;	//ADC������ֽ�
sfr ADC_RESL  = 0xBE;	//ADC������ֽ�

sfr P4        = 0xC0;
sfr WDT_CONTR = 0xC1;
sfr IAP_DATA  = 0xC2;
sfr IAP_ADDRH = 0xC3;
sfr IAP_ADDRL = 0xC4;
sfr IAP_CMD   = 0xC5;
sfr IAP_TRIG  = 0xC6;
sfr IAP_CONTR = 0xC7;

sfr ISP_DATA  = 0xC2;
sfr ISP_ADDRH = 0xC3;
sfr ISP_ADDRL = 0xC4;
sfr ISP_CMD   = 0xC5;
sfr ISP_TRIG  = 0xC6;
sfr ISP_CONTR = 0xC7;

sfr P5     = 0xC8;	//
sfr P5M1   = 0xC9;	//	P5M1.n,P5M0.n 	=00--->Standard,	01--->push-pull
sfr P5M0   = 0xCA;	//					=10--->pure input,	11--->open drain
sfr P6M1   = 0xCB;	//	P6M1.n,P6M0.n 	=00--->Standard,	01--->push-pull
sfr P6M0   = 0xCC;	//					=10--->pure input,	11--->open drain
sfr SPSTAT = 0xCD;	//SPI״̬�Ĵ���
sfr SPCTL  = 0xCE;	//SPI���ƼĴ���
sfr SPDAT  = 0xCF;	//SPI���ݼĴ���

sfr PSW    = 0xD0;
sfr	T4T3M  = 0xD1;
sfr	T4H    = 0xD2;
sfr	T4L    = 0xD3;
sfr	T3H    = 0xD4;
sfr	T3L    = 0xD5;
sfr	T2H    = 0xD6;
sfr	T2L    = 0xD7;

sfr	TH4    = 0xD2;
sfr	TL4    = 0xD3;
sfr	TH3    = 0xD4;
sfr	TL3    = 0xD5;
sfr	TH2    = 0xD6;
sfr	TL2    = 0xD7;

sfr USBCLK = 0xDC;	//USBʱ�ӿ��ƼĴ�����
sfr	ADCCFG = 0xDE;	//ADC���üĴ���
sfr	IP3    = 0xDF;	//�ж����ȼ����ƼĴ���3

sfr ACC    = 0xE0;
sfr	P7M1   = 0xE1;	//	P7M1.n,P7M0.n 	=00--->Standard,	01--->push-pull
sfr	P7M0   = 0xE2;	//					=10--->pure input,	11--->open drain
sfr	DPS    = 0xE3;	//DPTRָ��ѡ��Ĵ���
sfr	DPL1   = 0xE4;	//��2������ָ����ֽ�
sfr	DPH1   = 0xE5;	//��2������ָ����ֽ�
sfr	CMPCR1 = 0xE6;	//�Ƚ������ƼĴ���1
sfr	CMPCR2 = 0xE7;	//�Ƚ������ƼĴ���2

sfr	P6     = 0xE8;
sfr USBDAT = 0xEC;	//USB���ݼĴ���
sfr	IP3H   = 0xEE;	//���ж����ȼ����ƼĴ���3
sfr	AUXINTIF = 0xEF;	//��չ�ⲿ�жϱ�־�Ĵ��� B6-INT4IF, B5-INT3IF, B4-INT2IF, B2-T4IF, B1-T3IF, B0-T2IF

sfr B        = 0xF0;
sfr USBCON   = 0xF4;	//USB���ƼĴ�����
sfr	IAP_TPS  = 0xf5;	//IAP�ȴ�ʱ����ƼĴ���

sfr	P7       = 0xF8;
sfr USBADR   = 0xFC;	//USB��ַ�Ĵ���
sfr	RSTCFG   = 0xFF;		//��λ���üĴ���

//=============================================================================================================
#define DMA_M2M_CFG     (*(unsigned char volatile xdata *)0xfa00)	/*  M2M_DMA���üĴ��� */
#define DMA_M2M_CR      (*(unsigned char volatile xdata *)0xfa01)	/*  M2M_DMA���ƼĴ��� */
#define DMA_M2M_STA     (*(unsigned char volatile xdata *)0xfa02)	/*  M2M_DMA״̬�Ĵ��� */
#define DMA_M2M_AMT     (*(unsigned char volatile xdata *)0xfa03)	/*  M2M_DMA�������ֽ��� */
#define DMA_M2M_DONE    (*(unsigned char volatile xdata *)0xfa04)	/*  M2M_DMA��������ֽ��� */
#define DMA_M2M_TXAH    (*(unsigned char volatile xdata *)0xfa05)	/*  M2M_DMA���͸ߵ�ַ */
#define DMA_M2M_TXAL    (*(unsigned char volatile xdata *)0xfa06)	/*  M2M_DMA���͵͵�ַ */
#define DMA_M2M_RXAH    (*(unsigned char volatile xdata *)0xfa07)	/*  M2M_DMA���ոߵ�ַ */
#define DMA_M2M_RXAL    (*(unsigned char volatile xdata *)0xfa08)	/*  M2M_DMA���յص�ַ */

#define DMA_ADC_CFG     (*(unsigned char volatile xdata *)0xfa10)	/*  ADC_DMA���üĴ��� */
#define DMA_ADC_CR      (*(unsigned char volatile xdata *)0xfa11)	/*  ADC_DMA���ƼĴ��� */
#define DMA_ADC_STA     (*(unsigned char volatile xdata *)0xfa12)	/*  ADC_DMA״̬�Ĵ��� */
#define DMA_ADC_RXAH    (*(unsigned char volatile xdata *)0xfa17)	/*  ADC_DMA���ոߵ�ַ */
#define DMA_ADC_RXAL    (*(unsigned char volatile xdata *)0xfa18)	/*  ADC_DMA���յص�ַ */
#define DMA_ADC_CFG2    (*(unsigned char volatile xdata *)0xfa19)	/*  ADC_DMA���üĴ���2 */
#define DMA_ADC_CHSW0   (*(unsigned char volatile xdata *)0xfa1a)	/*  ADC_DMAͨ��ʹ�ܼĴ���0 */
#define DMA_ADC_CHSW1   (*(unsigned char volatile xdata *)0xfa1b)	/*  ADC_DMAͨ��ʹ�ܼĴ���1 */

#define DMA_SPI_CFG     (*(unsigned char volatile xdata *)0xfa20)	/*  SPI_DMA���üĴ��� */
#define DMA_SPI_CR      (*(unsigned char volatile xdata *)0xfa21)	/*  SPI_DMA���ƼĴ��� */
#define DMA_SPI_STA     (*(unsigned char volatile xdata *)0xfa22)	/*  SPI_DMA״̬�Ĵ��� */
#define DMA_SPI_AMT     (*(unsigned char volatile xdata *)0xfa23)	/*  SPI_DMA�������ֽ��� */
#define DMA_SPI_DONE    (*(unsigned char volatile xdata *)0xfa24)	/*  SPI_DMA��������ֽ��� */
#define DMA_SPI_TXAH    (*(unsigned char volatile xdata *)0xfa25)	/*  SPI_DMA���͸ߵ�ַ */
#define DMA_SPI_TXAL    (*(unsigned char volatile xdata *)0xfa26)	/*  SPI_DMA���͵͵�ַ */
#define DMA_SPI_RXAH    (*(unsigned char volatile xdata *)0xfa27)	/*  SPI_DMA���ոߵ�ַ */
#define DMA_SPI_RXAL    (*(unsigned char volatile xdata *)0xfa28)	/*  SPI_DMA���յص�ַ */
#define DMA_SPI_CFG2    (*(unsigned char volatile xdata *)0xfa29)	/*  SPI_DMA���üĴ���2 */

#define DMA_UR1T_CFG     (*(unsigned char volatile xdata *)0xfa30)	/*  UR1T_DMA���üĴ��� */
#define DMA_UR1T_CR      (*(unsigned char volatile xdata *)0xfa31)	/*  UR1T_DMA���ƼĴ��� */
#define DMA_UR1T_STA     (*(unsigned char volatile xdata *)0xfa32)	/*  UR1T_DMA״̬�Ĵ��� */
#define DMA_UR1T_AMT     (*(unsigned char volatile xdata *)0xfa33)	/*  UR1T_DMA�������ֽ��� */
#define DMA_UR1T_DONE    (*(unsigned char volatile xdata *)0xfa34)	/*  UR1T_DMA��������ֽ��� */
#define DMA_UR1T_TXAH    (*(unsigned char volatile xdata *)0xfa35)	/*  UR1T_DMA���͸ߵ�ַ */
#define DMA_UR1T_TXAL    (*(unsigned char volatile xdata *)0xfa36)	/*  UR1T_DMA���͵͵�ַ */
#define DMA_UR1R_CFG     (*(unsigned char volatile xdata *)0xfa38)	/*  UR1R_DMA���üĴ��� */
#define DMA_UR1R_CR      (*(unsigned char volatile xdata *)0xfa39)	/*  UR1R_DMA���ƼĴ��� */
#define DMA_UR1R_STA     (*(unsigned char volatile xdata *)0xfa3a)	/*  UR1R_DMA״̬�Ĵ��� */
#define DMA_UR1R_AMT     (*(unsigned char volatile xdata *)0xfa3b)	/*  UR1R_DMA�������ֽ��� */
#define DMA_UR1R_DONE    (*(unsigned char volatile xdata *)0xfa3c)	/*  UR1R_DMA��������ֽ��� */
#define DMA_UR1R_TXAH    (*(unsigned char volatile xdata *)0xfa3d)	/*  UR1R_DMA���͸ߵ�ַ */
#define DMA_UR1R_TXAL    (*(unsigned char volatile xdata *)0xfa3e)	/*  UR1R_DMA���͵͵�ַ */

#define DMA_UR2T_CFG     (*(unsigned char volatile xdata *)0xfa40)	/*  UR2T_DMA���üĴ��� */
#define DMA_UR2T_CR      (*(unsigned char volatile xdata *)0xfa41)	/*  UR2T_DMA���ƼĴ��� */
#define DMA_UR2T_STA     (*(unsigned char volatile xdata *)0xfa42)	/*  UR2T_DMA״̬�Ĵ��� */
#define DMA_UR2T_AMT     (*(unsigned char volatile xdata *)0xfa43)	/*  UR2T_DMA�������ֽ��� */
#define DMA_UR2T_DONE    (*(unsigned char volatile xdata *)0xfa44)	/*  UR2T_DMA��������ֽ��� */
#define DMA_UR2T_TXAH    (*(unsigned char volatile xdata *)0xfa45)	/*  UR2T_DMA���͸ߵ�ַ */
#define DMA_UR2T_TXAL    (*(unsigned char volatile xdata *)0xfa46)	/*  UR2T_DMA���͵͵�ַ */
#define DMA_UR2R_CFG     (*(unsigned char volatile xdata *)0xfa48)	/*  UR2R_DMA���üĴ��� */
#define DMA_UR2R_CR      (*(unsigned char volatile xdata *)0xfa49)	/*  UR2R_DMA���ƼĴ��� */
#define DMA_UR2R_STA     (*(unsigned char volatile xdata *)0xfa4a)	/*  UR2R_DMA״̬�Ĵ��� */
#define DMA_UR2R_AMT     (*(unsigned char volatile xdata *)0xfa4b)	/*  UR2R_DMA�������ֽ��� */
#define DMA_UR2R_DONE    (*(unsigned char volatile xdata *)0xfa4c)	/*  UR2R_DMA��������ֽ��� */
#define DMA_UR2R_TXAH    (*(unsigned char volatile xdata *)0xfa4d)	/*  UR2R_DMA���͸ߵ�ַ */
#define DMA_UR2R_TXAL    (*(unsigned char volatile xdata *)0xfa4e)	/*  UR2R_DMA���͵͵�ַ */

#define DMA_UR3T_CFG     (*(unsigned char volatile xdata *)0xfa50)	/*  UR3T_DMA���üĴ��� */
#define DMA_UR3T_CR      (*(unsigned char volatile xdata *)0xfa51)	/*  UR3T_DMA���ƼĴ��� */
#define DMA_UR3T_STA     (*(unsigned char volatile xdata *)0xfa52)	/*  UR3T_DMA״̬�Ĵ��� */
#define DMA_UR3T_AMT     (*(unsigned char volatile xdata *)0xfa53)	/*  UR3T_DMA�������ֽ��� */
#define DMA_UR3T_DONE    (*(unsigned char volatile xdata *)0xfa54)	/*  UR3T_DMA��������ֽ��� */
#define DMA_UR3T_TXAH    (*(unsigned char volatile xdata *)0xfa55)	/*  UR3T_DMA���͸ߵ�ַ */
#define DMA_UR3T_TXAL    (*(unsigned char volatile xdata *)0xfa56)	/*  UR3T_DMA���͵͵�ַ */
#define DMA_UR3R_CFG     (*(unsigned char volatile xdata *)0xfa58)	/*  UR3R_DMA���üĴ��� */
#define DMA_UR3R_CR      (*(unsigned char volatile xdata *)0xfa59)	/*  UR3R_DMA���ƼĴ��� */
#define DMA_UR3R_STA     (*(unsigned char volatile xdata *)0xfa5a)	/*  UR3R_DMA״̬�Ĵ��� */
#define DMA_UR3R_AMT     (*(unsigned char volatile xdata *)0xfa5b)	/*  UR3R_DMA�������ֽ��� */
#define DMA_UR3R_DONE    (*(unsigned char volatile xdata *)0xfa5c)	/*  UR3R_DMA��������ֽ��� */
#define DMA_UR3R_TXAH    (*(unsigned char volatile xdata *)0xfa5d)	/*  UR3R_DMA���͸ߵ�ַ */
#define DMA_UR3R_TXAL    (*(unsigned char volatile xdata *)0xfa5e)	/*  UR3R_DMA���͵͵�ַ */

#define DMA_UR4T_CFG     (*(unsigned char volatile xdata *)0xfa60)	/*  UR4T_DMA���üĴ��� */
#define DMA_UR4T_CR      (*(unsigned char volatile xdata *)0xfa61)	/*  UR4T_DMA���ƼĴ��� */
#define DMA_UR4T_STA     (*(unsigned char volatile xdata *)0xfa62)	/*  UR4T_DMA״̬�Ĵ��� */
#define DMA_UR4T_AMT     (*(unsigned char volatile xdata *)0xfa63)	/*  UR4T_DMA�������ֽ��� */
#define DMA_UR4T_DONE    (*(unsigned char volatile xdata *)0xfa64)	/*  UR4T_DMA��������ֽ��� */
#define DMA_UR4T_TXAH    (*(unsigned char volatile xdata *)0xfa65)	/*  UR4T_DMA���͸ߵ�ַ */
#define DMA_UR4T_TXAL    (*(unsigned char volatile xdata *)0xfa66)	/*  UR4T_DMA���͵͵�ַ */
#define DMA_UR4R_CFG     (*(unsigned char volatile xdata *)0xfa68)	/*  UR4R_DMA���üĴ��� */
#define DMA_UR4R_CR      (*(unsigned char volatile xdata *)0xfa69)	/*  UR4R_DMA���ƼĴ��� */
#define DMA_UR4R_STA     (*(unsigned char volatile xdata *)0xfa6a)	/*  UR4R_DMA״̬�Ĵ��� */
#define DMA_UR4R_AMT     (*(unsigned char volatile xdata *)0xfa6b)	/*  UR4R_DMA�������ֽ��� */
#define DMA_UR4R_DONE    (*(unsigned char volatile xdata *)0xfa6c)	/*  UR4R_DMA��������ֽ��� */
#define DMA_UR4R_TXAH    (*(unsigned char volatile xdata *)0xfa6d)	/*  UR4R_DMA���͸ߵ�ַ */
#define DMA_UR4R_TXAL    (*(unsigned char volatile xdata *)0xfa6e)	/*  UR4R_DMA���͵͵�ַ */

#define DMA_LCM_CFG     (*(unsigned char volatile xdata *)0xfa70)	/*  LCM_DMA���üĴ��� */
#define DMA_LCM_CR      (*(unsigned char volatile xdata *)0xfa71)	/*  LCM_DMA���ƼĴ��� */
#define DMA_LCM_STA     (*(unsigned char volatile xdata *)0xfa72)	/*  LCM_DMA״̬�Ĵ��� */
#define DMA_LCM_AMT     (*(unsigned char volatile xdata *)0xfa73)	/*  LCM_DMA�������ֽ��� */
#define DMA_LCM_DONE    (*(unsigned char volatile xdata *)0xfa74)	/*  LCM_DMA��������ֽ��� */
#define DMA_LCM_TXAH    (*(unsigned char volatile xdata *)0xfa75)	/*  LCM_DMA���͸ߵ�ַ */
#define DMA_LCM_TXAL    (*(unsigned char volatile xdata *)0xfa76)	/*  LCM_DMA���͵͵�ַ */
#define DMA_LCM_RXAH    (*(unsigned char volatile xdata *)0xfa77)	/*  LCM_DMA���ոߵ�ַ */
#define DMA_LCM_RXAL    (*(unsigned char volatile xdata *)0xfa78)	/*  LCM_DMA���յص�ַ */

#define COMEN     (*(unsigned char volatile xdata *)0xfb00)	/* COMʹ�ܼĴ��� */
#define SEGENL    (*(unsigned char volatile xdata *)0xfb01)	/* SEGʹ�ܼĴ��� */
#define SEGENH    (*(unsigned char volatile xdata *)0xfb02)	/* SEGʹ�ܼĴ��� */
#define LEDCTRL   (*(unsigned char volatile xdata *)0xfb03)	/* LED���ƼĴ��� */
#define LEDCKS    (*(unsigned char volatile xdata *)0xfb04)	/* LEDʱ�ӷ�Ƶ�Ĵ��� */

#define COM0_DAL  (*(unsigned char volatile xdata *)0xfb10)	/* ����ģʽ��ʾ���� */
#define COM1_DAL  (*(unsigned char volatile xdata *)0xfb11)	/* ����ģʽ��ʾ���� */
#define COM2_DAL  (*(unsigned char volatile xdata *)0xfb12)	/* ����ģʽ��ʾ���� */
#define COM3_DAL  (*(unsigned char volatile xdata *)0xfb13)	/* ����ģʽ��ʾ���� */
#define COM4_DAL  (*(unsigned char volatile xdata *)0xfb14)	/* ����ģʽ��ʾ���� */
#define COM5_DAL  (*(unsigned char volatile xdata *)0xfb15)	/* ����ģʽ��ʾ���� */
#define COM6_DAL  (*(unsigned char volatile xdata *)0xfb16)	/* ����ģʽ��ʾ���� */
#define COM7_DAL  (*(unsigned char volatile xdata *)0xfb17)	/* ����ģʽ��ʾ���� */
#define COM0_DAH  (*(unsigned char volatile xdata *)0xfb18)	/* ����ģʽ��ʾ���� */
#define COM1_DAH  (*(unsigned char volatile xdata *)0xfb19)	/* ����ģʽ��ʾ���� */
#define COM2_DAH  (*(unsigned char volatile xdata *)0xfb1a)	/* ����ģʽ��ʾ���� */
#define COM3_DAH  (*(unsigned char volatile xdata *)0xfb1b)	/* ����ģʽ��ʾ���� */
#define COM4_DAH  (*(unsigned char volatile xdata *)0xfb1c)	/* ����ģʽ��ʾ���� */
#define COM5_DAH  (*(unsigned char volatile xdata *)0xfb1d)	/* ����ģʽ��ʾ���� */
#define COM6_DAH  (*(unsigned char volatile xdata *)0xfb1e)	/* ����ģʽ��ʾ���� */
#define COM7_DAH  (*(unsigned char volatile xdata *)0xfb1f)	/* ����ģʽ��ʾ���� */

#define COM0_DCL  (*(unsigned char volatile xdata *)0xfb20)	/* ����ģʽ��ʾ���� */
#define COM1_DCL  (*(unsigned char volatile xdata *)0xfb21)	/* ����ģʽ��ʾ���� */
#define COM2_DCL  (*(unsigned char volatile xdata *)0xfb22)	/* ����ģʽ��ʾ���� */
#define COM3_DCL  (*(unsigned char volatile xdata *)0xfb23)	/* ����ģʽ��ʾ���� */
#define COM4_DCL  (*(unsigned char volatile xdata *)0xfb24)	/* ����ģʽ��ʾ���� */
#define COM5_DCL  (*(unsigned char volatile xdata *)0xfb25)	/* ����ģʽ��ʾ���� */
#define COM6_DCL  (*(unsigned char volatile xdata *)0xfb26)	/* ����ģʽ��ʾ���� */
#define COM7_DCL  (*(unsigned char volatile xdata *)0xfb27)	/* ����ģʽ��ʾ���� */
#define COM0_DCH  (*(unsigned char volatile xdata *)0xfb28)	/* ����ģʽ��ʾ���� */
#define COM1_DCH  (*(unsigned char volatile xdata *)0xfb29)	/* ����ģʽ��ʾ���� */
#define COM2_DCH  (*(unsigned char volatile xdata *)0xfb2a)	/* ����ģʽ��ʾ���� */
#define COM3_DCH  (*(unsigned char volatile xdata *)0xfb2b)	/* ����ģʽ��ʾ���� */
#define COM4_DCH  (*(unsigned char volatile xdata *)0xfb2c)	/* ����ģʽ��ʾ���� */
#define COM5_DCH  (*(unsigned char volatile xdata *)0xfb2d)	/* ����ģʽ��ʾ���� */
#define COM6_DCH  (*(unsigned char volatile xdata *)0xfb2e)	/* ����ģʽ��ʾ���� */
#define COM7_DCH  (*(unsigned char volatile xdata *)0xfb2f)	/* ����ģʽ��ʾ���� */

#define TSCHEN     (*(unsigned int  volatile xdata *)0xfb40)	/* ��������ʹ�ܼĴ��� */
#define TSCFG      (*(unsigned int  volatile xdata *)0xfb42)	/* �����������üĴ���1 */
#define TSSTA      (*(unsigned int  volatile xdata *)0xfb46)	/* ��������״̬�Ĵ���1 */
#define TSDAT      (*(unsigned int  volatile xdata *)0xfb49)	/* ������������ */
#define TSTH00     (*(unsigned int  volatile xdata *)0xfb50)	/* ��������0�ż�ֵ���ֽ� */
#define TSTH01     (*(unsigned int  volatile xdata *)0xfb52)	/* ��������1�ż�ֵ���ֽ� */
#define TSTH02     (*(unsigned int  volatile xdata *)0xfb54)	/* ��������2�ż�ֵ���ֽ� */
#define TSTH03     (*(unsigned int  volatile xdata *)0xfb56)	/* ��������3�ż�ֵ���ֽ� */
#define TSTH04     (*(unsigned int  volatile xdata *)0xfb58)	/* ��������4�ż�ֵ���ֽ� */
#define TSTH05     (*(unsigned int  volatile xdata *)0xfb5a)	/* ��������5�ż�ֵ���ֽ� */
#define TSTH06     (*(unsigned int  volatile xdata *)0xfb5c)	/* ��������6�ż�ֵ���ֽ� */
#define TSTH07     (*(unsigned int  volatile xdata *)0xfb5e)	/* ��������7�ż�ֵ���ֽ� */
#define TSTH08     (*(unsigned int  volatile xdata *)0xfb60)	/* ��������8�ż�ֵ���ֽ� */
#define TSTH09     (*(unsigned int  volatile xdata *)0xfb62)	/* ��������9�ż�ֵ���ֽ� */
#define TSTH10     (*(unsigned int  volatile xdata *)0xfb64)	/* ��������10�ż�ֵ���ֽ� */
#define TSTH11     (*(unsigned int  volatile xdata *)0xfb66)	/* ��������11�ż�ֵ���ֽ� */
#define TSTH12     (*(unsigned int  volatile xdata *)0xfb68)	/* ��������12�ż�ֵ���ֽ� */
#define TSTH13     (*(unsigned int  volatile xdata *)0xfb6a)	/* ��������13�ż�ֵ���ֽ� */
#define TSTH14     (*(unsigned int  volatile xdata *)0xfb6c)	/* ��������14�ż�ֵ���ֽ� */
#define TSTH15     (*(unsigned int  volatile xdata *)0xfb6e)	/* ��������15�ż�ֵ���ֽ� */

#define TSCHEN1    (*(unsigned char volatile xdata *)0xfb40)	/* ��������ʹ�ܼĴ���1 */
#define TSCHEN2    (*(unsigned char volatile xdata *)0xfb41)	/* ��������ʹ�ܼĴ���2 */
#define TSCFG1     (*(unsigned char volatile xdata *)0xfb42)	/* �����������üĴ���1 */
#define TSCFG2     (*(unsigned char volatile xdata *)0xfb43)	/* �����������üĴ���2 */
#define TSWUTC     (*(unsigned char volatile xdata *)0xfb44)	/* �����������ѿ��ƼĴ��� */
#define TSCTRL     (*(unsigned char volatile xdata *)0xfb45)	/* �����������ƼĴ���  */
#define TSSTA1     (*(unsigned char volatile xdata *)0xfb46)	/* ��������״̬�Ĵ���1 */
#define TSSTA2     (*(unsigned char volatile xdata *)0xfb47)	/* ��������״̬�Ĵ���2 */

#define TSRT       (*(unsigned char volatile xdata *)0xfb48)	/* ��������ʱ����ƼĴ��� */
#define TSDATH     (*(unsigned char volatile xdata *)0xfb49)	/* �����������ݸ��ֽ� */
#define TSDATL     (*(unsigned char volatile xdata *)0xfb4a)	/* �����������ݵ��ֽ� */

#define TSTH00H    (*(unsigned char volatile xdata *)0xfb50)	/* ��������0�ż�ֵ���ֽ� */
#define TSTH00L    (*(unsigned char volatile xdata *)0xfb51)	/* ��������0�ż�ֵ���ֽ� */
#define TSTH01H    (*(unsigned char volatile xdata *)0xfb52)	/* ��������1�ż�ֵ���ֽ� */
#define TSTH01L    (*(unsigned char volatile xdata *)0xfb53)	/* ��������1�ż�ֵ���ֽ� */
#define TSTH02H    (*(unsigned char volatile xdata *)0xfb54)	/* ��������2�ż�ֵ���ֽ� */
#define TSTH02L    (*(unsigned char volatile xdata *)0xfb55)	/* ��������2�ż�ֵ���ֽ� */
#define TSTH03H    (*(unsigned char volatile xdata *)0xfb56)	/* ��������3�ż�ֵ���ֽ� */
#define TSTH03L    (*(unsigned char volatile xdata *)0xfb57)	/* ��������3�ż�ֵ���ֽ� */
#define TSTH04H    (*(unsigned char volatile xdata *)0xfb58)	/* ��������4�ż�ֵ���ֽ� */
#define TSTH04L    (*(unsigned char volatile xdata *)0xfb59)	/* ��������4�ż�ֵ���ֽ� */
#define TSTH05H    (*(unsigned char volatile xdata *)0xfb5a)	/* ��������5�ż�ֵ���ֽ� */
#define TSTH05L    (*(unsigned char volatile xdata *)0xfb5b)	/* ��������5�ż�ֵ���ֽ� */
#define TSTH06H    (*(unsigned char volatile xdata *)0xfb5c)	/* ��������6�ż�ֵ���ֽ� */
#define TSTH06L    (*(unsigned char volatile xdata *)0xfb5d)	/* ��������6�ż�ֵ���ֽ� */
#define TSTH07H    (*(unsigned char volatile xdata *)0xfb5e)	/* ��������7�ż�ֵ���ֽ� */
#define TSTH07L    (*(unsigned char volatile xdata *)0xfb5f)	/* ��������7�ż�ֵ���ֽ� */
#define TSTH08H    (*(unsigned char volatile xdata *)0xfb60)	/* ��������8�ż�ֵ���ֽ� */
#define TSTH08L    (*(unsigned char volatile xdata *)0xfb61)	/* ��������8�ż�ֵ���ֽ� */
#define TSTH09H    (*(unsigned char volatile xdata *)0xfb62)	/* ��������9�ż�ֵ���ֽ� */
#define TSTH09L    (*(unsigned char volatile xdata *)0xfb63)	/* ��������9�ż�ֵ���ֽ� */
#define TSTH10H    (*(unsigned char volatile xdata *)0xfb64)	/* ��������10�ż�ֵ���ֽ� */
#define TSTH10L    (*(unsigned char volatile xdata *)0xfb65)	/* ��������10�ż�ֵ���ֽ� */
#define TSTH11H    (*(unsigned char volatile xdata *)0xfb66)	/* ��������11�ż�ֵ���ֽ� */
#define TSTH11L    (*(unsigned char volatile xdata *)0xfb67)	/* ��������11�ż�ֵ���ֽ� */
#define TSTH12H    (*(unsigned char volatile xdata *)0xfb68)	/* ��������12�ż�ֵ���ֽ� */
#define TSTH12L    (*(unsigned char volatile xdata *)0xfb69)	/* ��������12�ż�ֵ���ֽ� */
#define TSTH13H    (*(unsigned char volatile xdata *)0xfb6a)	/* ��������13�ż�ֵ���ֽ� */
#define TSTH13L    (*(unsigned char volatile xdata *)0xfb6b)	/* ��������13�ż�ֵ���ֽ� */
#define TSTH14H    (*(unsigned char volatile xdata *)0xfb6c)	/* ��������14�ż�ֵ���ֽ� */
#define TSTH14L    (*(unsigned char volatile xdata *)0xfb6d)	/* ��������14�ż�ֵ���ֽ� */
#define TSTH15H    (*(unsigned char volatile xdata *)0xfb6e)	/* ��������15�ż�ֵ���ֽ� */
#define TSTH15L    (*(unsigned char volatile xdata *)0xfb6f)	/* ��������15�ż�ֵ���ֽ� */

#define LCDCFG     (*(unsigned char volatile xdata *)0xfb80)	/* LCD���üĴ��� */
#define LCDCFG2    (*(unsigned char volatile xdata *)0xfb81)	/* LCD���üĴ���2 */
#define DBLEN      (*(unsigned char volatile xdata *)0xfb82)	/* ����ʱ�䳤�����üĴ��� */
#define COMLENL    (*(unsigned char volatile xdata *)0xfb83)	/* COMʱ�䳤�����õ�λ */
#define COMLENM    (*(unsigned char volatile xdata *)0xfb84)	/* COMʱ�䳤��������λ */
#define COMLENH    (*(unsigned char volatile xdata *)0xfb85)	/* COMʱ�䳤�����ø�λ */
#define BLINKRATE  (*(unsigned char volatile xdata *)0xfb86)	/* ��˸�����üĴ��� */
#define LCDCR      (*(unsigned char volatile xdata *)0xfb87)	/* LCD���ƼĴ��� */
#define COMON      (*(unsigned char volatile xdata *)0xfb88)	/* COMʹ�ܼĴ��� */
#define SEGON1     (*(unsigned char volatile xdata *)0xfb8a)	/* SEG��ʹ�ܼĴ���1 */
#define SEGON2     (*(unsigned char volatile xdata *)0xfb8b)	/* SEG��ʹ�ܼĴ���2 */
#define SEGON3     (*(unsigned char volatile xdata *)0xfb8c)	/* SEG��ʹ�ܼĴ���3 */
#define SEGON4     (*(unsigned char volatile xdata *)0xfb8d)	/* SEG��ʹ�ܼĴ���4 */
#define SEGON5     (*(unsigned char volatile xdata *)0xfb8e)	/* SEG��ʹ�ܼĴ���5 */
#define C0SEGV0    (*(unsigned char volatile xdata *)0xfb90)	/* C0SEG  7~0���ݼĴ���  */
#define C0SEGV1    (*(unsigned char volatile xdata *)0xfb91)	/* C0SEG 15~8���ݼĴ���  */
#define C0SEGV2    (*(unsigned char volatile xdata *)0xfb92)	/* C0SEG 23~16���ݼĴ��� */
#define C0SEGV3    (*(unsigned char volatile xdata *)0xfb93)	/* C0SEG 31~24���ݼĴ��� */
#define C0SEGV4    (*(unsigned char volatile xdata *)0xfb94)	/* C0SEG 39~32���ݼĴ��� */
#define C1SEGV0    (*(unsigned char volatile xdata *)0xfb98)	/* C1SEG  7~0���ݼĴ���  */
#define C1SEGV1    (*(unsigned char volatile xdata *)0xfb99)	/* C1SEG 15~8���ݼĴ���  */
#define C1SEGV2    (*(unsigned char volatile xdata *)0xfb9a)	/* C1SEG 23~16���ݼĴ��� */
#define C1SEGV3    (*(unsigned char volatile xdata *)0xfb9b)	/* C1SEG 31~24���ݼĴ��� */
#define C1SEGV4    (*(unsigned char volatile xdata *)0xfb9c)	/* C1SEG 39~32���ݼĴ��� */
#define C2SEGV0    (*(unsigned char volatile xdata *)0xfba0)	/* C2SEG  7~0���ݼĴ���  */
#define C2SEGV1    (*(unsigned char volatile xdata *)0xfba1)	/* C2SEG 15~8���ݼĴ���  */
#define C2SEGV2    (*(unsigned char volatile xdata *)0xfba2)	/* C2SEG 23~16���ݼĴ��� */
#define C2SEGV3    (*(unsigned char volatile xdata *)0xfba3)	/* C2SEG 31~24���ݼĴ��� */
#define C2SEGV4    (*(unsigned char volatile xdata *)0xfba4)	/* C2SEG 39~32���ݼĴ��� */
#define C3SEGV0    (*(unsigned char volatile xdata *)0xfba8)	/* C3SEG  7~0���ݼĴ���  */
#define C3SEGV1    (*(unsigned char volatile xdata *)0xfba9)	/* C3SEG 15~8���ݼĴ���  */
#define C3SEGV2    (*(unsigned char volatile xdata *)0xfbaa)	/* C3SEG 23~16���ݼĴ��� */
#define C3SEGV3    (*(unsigned char volatile xdata *)0xfbab)	/* C3SEG 31~24���ݼĴ��� */
#define C3SEGV4    (*(unsigned char volatile xdata *)0xfbac)	/* C3SEG 39~32���ݼĴ��� */

#define MD3U32    (*(unsigned long volatile xdata *)0xfcf0)	/* MDU���ݼĴ��� */
#define MD3U16    (*(unsigned int  volatile xdata *)0xfcf0)	/* MDU���ݼĴ��� */
#define MD1U16    (*(unsigned int  volatile xdata *)0xfcf2)	/* MDU���ݼĴ��� */
#define MD5U16    (*(unsigned int  volatile xdata *)0xfcf4)	/* MDU���ݼĴ��� */

#define MD3       (*(unsigned char volatile xdata *)0xfcf0)	/* MDU���ݼĴ��� */
#define MD2       (*(unsigned char volatile xdata *)0xfcf1)	/* MDU���ݼĴ��� */
#define MD1       (*(unsigned char volatile xdata *)0xfcf2)	/* MDU���ݼĴ��� */
#define MD0       (*(unsigned char volatile xdata *)0xfcf3)	/* MDU���ݼĴ��� */
#define MD5       (*(unsigned char volatile xdata *)0xfcf4)	/* MDU���ݼĴ��� */
#define MD4       (*(unsigned char volatile xdata *)0xfcf5)	/* MDU���ݼĴ��� */
#define ARCON     (*(unsigned char volatile xdata *)0xfcf6)	/* MDUģʽ���ƼĴ��� */
#define OPCON     (*(unsigned char volatile xdata *)0xfcf7)	/* MDU�������ƼĴ��� */

#define P0INTE      (*(unsigned char volatile xdata *)0xfd00)	/* P0���ж�ʹ�ܼĴ��� */
#define P1INTE      (*(unsigned char volatile xdata *)0xfd01)	/* P1���ж�ʹ�ܼĴ��� */
#define P2INTE      (*(unsigned char volatile xdata *)0xfd02)	/* P2���ж�ʹ�ܼĴ��� */
#define P3INTE      (*(unsigned char volatile xdata *)0xfd03)	/* P3���ж�ʹ�ܼĴ��� */
#define P4INTE      (*(unsigned char volatile xdata *)0xfd04)	/* P4���ж�ʹ�ܼĴ��� */
#define P5INTE      (*(unsigned char volatile xdata *)0xfd05)	/* P5���ж�ʹ�ܼĴ��� */
#define P6INTE      (*(unsigned char volatile xdata *)0xfd06)	/* P6���ж�ʹ�ܼĴ��� */
#define P7INTE      (*(unsigned char volatile xdata *)0xfd07)	/* P7���ж�ʹ�ܼĴ��� */

#define P0INTF      (*(unsigned char volatile xdata *)0xfd10)	/* P0���жϱ�־�Ĵ��� */
#define P1INTF      (*(unsigned char volatile xdata *)0xfd11)	/* P1���жϱ�־�Ĵ��� */
#define P2INTF      (*(unsigned char volatile xdata *)0xfd12)	/* P2���жϱ�־�Ĵ��� */
#define P3INTF      (*(unsigned char volatile xdata *)0xfd13)	/* P3���жϱ�־�Ĵ��� */
#define P4INTF      (*(unsigned char volatile xdata *)0xfd14)	/* P4���жϱ�־�Ĵ��� */
#define P5INTF      (*(unsigned char volatile xdata *)0xfd15)	/* P5���жϱ�־�Ĵ��� */
#define P6INTF      (*(unsigned char volatile xdata *)0xfd16)	/* P6���жϱ�־�Ĵ��� */
#define P7INTF      (*(unsigned char volatile xdata *)0xfd17)	/* P7���жϱ�־�Ĵ��� */

#define P0IM0       (*(unsigned char volatile xdata *)0xfd20)	/* P0���ж�ģʽ�Ĵ���0 */
#define P1IM0       (*(unsigned char volatile xdata *)0xfd21)	/* P1���ж�ģʽ�Ĵ���0 */
#define P2IM0       (*(unsigned char volatile xdata *)0xfd22)	/* P2���ж�ģʽ�Ĵ���0 */
#define P3IM0       (*(unsigned char volatile xdata *)0xfd23)	/* P3���ж�ģʽ�Ĵ���0 */
#define P4IM0       (*(unsigned char volatile xdata *)0xfd24)	/* P4���ж�ģʽ�Ĵ���0 */
#define P5IM0       (*(unsigned char volatile xdata *)0xfd25)	/* P5���ж�ģʽ�Ĵ���0 */
#define P6IM0       (*(unsigned char volatile xdata *)0xfd26)	/* P6���ж�ģʽ�Ĵ���0 */
#define P7IM0       (*(unsigned char volatile xdata *)0xfd27)	/* P7���ж�ģʽ�Ĵ���0 */

#define P0IM1       (*(unsigned char volatile xdata *)0xfd30)	/* P0���ж�ģʽ�Ĵ���1 */
#define P1IM1       (*(unsigned char volatile xdata *)0xfd31)	/* P1���ж�ģʽ�Ĵ���1 */
#define P2IM1       (*(unsigned char volatile xdata *)0xfd32)	/* P2���ж�ģʽ�Ĵ���1 */
#define P3IM1       (*(unsigned char volatile xdata *)0xfd33)	/* P3���ж�ģʽ�Ĵ���1 */
#define P4IM1       (*(unsigned char volatile xdata *)0xfd34)	/* P4���ж�ģʽ�Ĵ���1 */
#define P5IM1       (*(unsigned char volatile xdata *)0xfd35)	/* P5���ж�ģʽ�Ĵ���1 */
#define P6IM1       (*(unsigned char volatile xdata *)0xfd36)	/* P6���ж�ģʽ�Ĵ���1 */
#define P7IM1       (*(unsigned char volatile xdata *)0xfd37)	/* P7���ж�ģʽ�Ĵ���1 */

#define INTE_0       0x01	/* ʹ��Px.0���жϹ��� */
#define INTE_1       0x02	/* ʹ��Px.1���жϹ��� */
#define INTE_2       0x04	/* ʹ��Px.2���жϹ��� */
#define INTE_3       0x08	/* ʹ��Px.3���жϹ��� */
#define INTE_4       0x10	/* ʹ��Px.4���жϹ��� */
#define INTE_5       0x20	/* ʹ��Px.5���жϹ��� */
#define INTE_6       0x40	/* ʹ��Px.6���жϹ��� */
#define INTE_7       0x80	/* ʹ��Px.7���жϹ��� */

#define INTF_0       0x01	/* ʹ��Px.0���жϱ�־ */
#define INTF_1       0x02	/* ʹ��Px.1���жϱ�־ */
#define INTF_2       0x04	/* ʹ��Px.2���жϱ�־ */
#define INTF_3       0x08	/* ʹ��Px.3���жϱ�־ */
#define INTF_4       0x10	/* ʹ��Px.4���жϱ�־ */
#define INTF_5       0x20	/* ʹ��Px.5���жϱ�־ */
#define INTF_6       0x40	/* ʹ��Px.6���жϱ�־ */
#define INTF_7       0x80	/* ʹ��Px.7���жϱ�־ */

#define	INT_FALL	0		/* �½����ж�   */
#define	INT_RISE	1		/* �������ж�   */
#define	INT_LOW		2		/* �͵�ƽ�ж�   */
#define	INT_HIGH	3		/* �ߵ�ƽ�ж�   */

#define P0WKUE      (*(unsigned char volatile xdata *)0xfd40)	/* P0���жϻ���ʹ�ܼĴ��� */
#define P1WKUE      (*(unsigned char volatile xdata *)0xfd41)	/* P1���жϻ���ʹ�ܼĴ��� */
#define P2WKUE      (*(unsigned char volatile xdata *)0xfd42)	/* P2���жϻ���ʹ�ܼĴ��� */
#define P3WKUE      (*(unsigned char volatile xdata *)0xfd43)	/* P3���жϻ���ʹ�ܼĴ��� */
#define P4WKUE      (*(unsigned char volatile xdata *)0xfd44)	/* P4���жϻ���ʹ�ܼĴ��� */
#define P5WKUE      (*(unsigned char volatile xdata *)0xfd45)	/* P5���жϻ���ʹ�ܼĴ��� */
#define P6WKUE      (*(unsigned char volatile xdata *)0xfd46)	/* P6���жϻ���ʹ�ܼĴ��� */
#define P7WKUE      (*(unsigned char volatile xdata *)0xfd47)	/* P7���жϻ���ʹ�ܼĴ��� */
#define PINIPL      (*(unsigned char volatile xdata *)0xfd60)	/* IO���ж����ȼ��ͼĴ��� */
#define PINIPH      (*(unsigned char volatile xdata *)0xfd61)	/* IO���ж����ȼ��߼Ĵ��� */


#define CLKSEL      (*(unsigned char volatile xdata *)0xfe00)	/* ��ʱ��Դѡ��Ĵ���    */
#define	CKSEL		(*(unsigned char volatile xdata *)0xfe00)	/* ��ʱ��Դѡ��Ĵ���    */
#define	CLKDIV		(*(unsigned char volatile xdata *)0xfe01)	/* ��ʱ�ӷ�Ƶ�Ĵ���      */
#define	CKDIV		(*(unsigned char volatile xdata *)0xfe01)	/* ��ʱ�ӷ�Ƶ�Ĵ���      */
#define	HIRCCR		(*(unsigned char volatile xdata *)0xfe02)	/* �ڲ������������ƼĴ��� */
#define	XOSCCR		(*(unsigned char volatile xdata *)0xfe03)	/* �ⲿ���پ����������ƼĴ���    */
#define	IRC32KCR	(*(unsigned char volatile xdata *)0xfe04)	/* �ڲ�32K����IRCʱ�ӿ��ƼĴ���   */
#define	MCLKOCR		(*(unsigned char volatile xdata *)0xfe05)	/* ��ʱ��������ƼĴ���  */
#define	IRCDB		(*(unsigned char volatile xdata *)0xfe06)	/* �ڲ���������ȥ�����ƼĴ���   */
#define	IRC48MCR	(*(unsigned char volatile xdata *)0xfe07)	/* �ڲ�48M�������ƼĴ���  */
#define	SPFUNC		(*(unsigned char volatile xdata *)0xfe08)	/* ���⹦�ܿ��ƼĴ���   */
#define	X32KCR		(*(unsigned char volatile xdata *)0xfe08)	/* �ⲿ32K�����������ƼĴ���   */
#define	RSTFLAG		(*(unsigned char volatile xdata *)0xfe09)	/* ��λ��־�Ĵ���  */

#define	P0PU		(*(unsigned char volatile xdata *)0xfe10)	/* P0���ڲ�3.7K��������, 0: ��ֹ(Ĭ��), 1: ����  */
#define	P1PU		(*(unsigned char volatile xdata *)0xfe11)	/* P1���ڲ�3.7K��������  */
#define	P2PU		(*(unsigned char volatile xdata *)0xfe12)	/* P2���ڲ�3.7K��������  */
#define	P3PU		(*(unsigned char volatile xdata *)0xfe13)	/* P3���ڲ�3.7K��������  */
#define	P4PU		(*(unsigned char volatile xdata *)0xfe14)	/* P4���ڲ�3.7K��������  */
#define	P5PU		(*(unsigned char volatile xdata *)0xfe15)	/* P5���ڲ�3.7K��������  */
#define	P6PU		(*(unsigned char volatile xdata *)0xfe16)	/* P6���ڲ�3.7K��������  */
#define	P7PU		(*(unsigned char volatile xdata *)0xfe17)	/* P7���ڲ�3.7K��������  */

#define	P0NCS		(*(unsigned char volatile xdata *)0xfe18)	/* P0 Non Schmit Trigger  0: ʹ�ܶ˿�ʩ���ش�������(Ĭ��), 1: ��ֹ  */
#define	P1NCS		(*(unsigned char volatile xdata *)0xfe19)	/* P1 Non Schmit Trigger  */
#define	P2NCS		(*(unsigned char volatile xdata *)0xfe1a)	/* P2 Non Schmit Trigger  */
#define	P3NCS		(*(unsigned char volatile xdata *)0xfe1b)	/* P3 Non Schmit Trigger  */
#define	P4NCS		(*(unsigned char volatile xdata *)0xfe1c)	/* P4 Non Schmit Trigger  */
#define	P5NCS		(*(unsigned char volatile xdata *)0xfe1d)	/* P5 Non Schmit Trigger  */
#define	P6NCS		(*(unsigned char volatile xdata *)0xfe1e)	/* P6 Non Schmit Trigger  */
#define	P7NCS		(*(unsigned char volatile xdata *)0xfe1f)	/* P7 Non Schmit Trigger  */

#define	P0SR		(*(unsigned char volatile xdata *)0xfe20)	/* P0�ڵ�ƽת�����ʼĴ��� Ĭ��1: ��ͨģʽ, 0:����ģʽ */
#define	P1SR		(*(unsigned char volatile xdata *)0xfe21)	/* P1�ڵ�ƽת�����ʼĴ���  */
#define	P2SR		(*(unsigned char volatile xdata *)0xfe22)	/* P2�ڵ�ƽת�����ʼĴ���  */
#define	P3SR		(*(unsigned char volatile xdata *)0xfe23)	/* P3�ڵ�ƽת�����ʼĴ���  */
#define	P4SR		(*(unsigned char volatile xdata *)0xfe24)	/* P4�ڵ�ƽת�����ʼĴ���  */
#define	P5SR		(*(unsigned char volatile xdata *)0xfe25)	/* P5�ڵ�ƽת�����ʼĴ���  */
#define	P6SR		(*(unsigned char volatile xdata *)0xfe26)	/* P6�ڵ�ƽת�����ʼĴ���  */
#define	P7SR		(*(unsigned char volatile xdata *)0xfe27)	/* P7�ڵ�ƽת�����ʼĴ���  */

#define	P0DR		(*(unsigned char volatile xdata *)0xfe28)	/* P0�������������ƼĴ��� Ĭ��1: ��ͨģʽ, 0:�����ģʽ*/
#define	P1DR		(*(unsigned char volatile xdata *)0xfe29)	/* P1�������������ƼĴ��� */
#define	P2DR		(*(unsigned char volatile xdata *)0xfe2a)	/* P2�������������ƼĴ��� */
#define	P3DR		(*(unsigned char volatile xdata *)0xfe2b)	/* P3�������������ƼĴ��� */
#define	P4DR		(*(unsigned char volatile xdata *)0xfe2c)	/* P4�������������ƼĴ��� */
#define	P5DR		(*(unsigned char volatile xdata *)0xfe2d)	/* P5�������������ƼĴ��� */
#define	P6DR		(*(unsigned char volatile xdata *)0xfe2e)	/* P6�������������ƼĴ��� */
#define	P7DR		(*(unsigned char volatile xdata *)0xfe2f)	/* P7�������������ƼĴ��� */

#define	P0IE		(*(unsigned char volatile xdata *)0xfe30)	/* P0������ʹ�ܿ��ƼĴ���, 1����(Ĭ��), 0��ֹ */
#define	P1IE		(*(unsigned char volatile xdata *)0xfe31)	/* P1������ʹ�ܿ��ƼĴ��� */
#define	P2IE		(*(unsigned char volatile xdata *)0xfe32)	/* P2������ʹ�ܿ��ƼĴ��� */
#define	P3IE		(*(unsigned char volatile xdata *)0xfe33)	/* P3������ʹ�ܿ��ƼĴ��� */
#define	P4IE		(*(unsigned char volatile xdata *)0xfe34)	/* P4������ʹ�ܿ��ƼĴ��� */
#define	P5IE		(*(unsigned char volatile xdata *)0xfe35)	/* P5������ʹ�ܿ��ƼĴ��� */
#define	P6IE		(*(unsigned char volatile xdata *)0xfe36)	/* P6������ʹ�ܿ��ƼĴ��� */
#define	P7IE		(*(unsigned char volatile xdata *)0xfe37)	/* P7������ʹ�ܿ��ƼĴ��� */

#define	LCMIFCFG	(*(unsigned char volatile xdata *)0xfe50)	/* LCM�ӿ����üĴ��� */
#define	LCMIFCFG2	(*(unsigned char volatile xdata *)0xfe51)	/* LCM�ӿ����üĴ���2 */
#define	LCMIFCR		(*(unsigned char volatile xdata *)0xfe52)	/* LCM�ӿڿ��ƼĴ��� */
#define	LCMIFSTA	(*(unsigned char volatile xdata *)0xfe53)	/* LCM�ӿ�״̬�Ĵ��� */
#define	LCMIDDATL	(*(unsigned char volatile xdata *)0xfe54)	/* LCM�ӿڵ��ֽ����� */
#define	LCMIDDATH	(*(unsigned char volatile xdata *)0xfe55)	/* LCM�ӿڸ��ֽ����� */

#define	RTCCR		(*(unsigned char volatile xdata *)0xfe60)	/* RTC���ƼĴ��� */
#define	RTCCFG		(*(unsigned char volatile xdata *)0xfe61)	/* RTC���üĴ��� */
#define	RTCIEN		(*(unsigned char volatile xdata *)0xfe62)	/* RTC�ж�ʹ�ܼĴ��� */
#define	RTCIF		(*(unsigned char volatile xdata *)0xfe63)	/* RTC�ж�����Ĵ��� */
#define	ALAHOUR		(*(unsigned char volatile xdata *)0xfe64)	/* RTC���ӵ�Сʱֵ */
#define	ALAMIN		(*(unsigned char volatile xdata *)0xfe65)	/* RTC���ӵķ���ֵ */
#define	ALASEC		(*(unsigned char volatile xdata *)0xfe66)	/* RTC���ӵ���ֵ */
#define	ALASSEC		(*(unsigned char volatile xdata *)0xfe67)	/* RTC���ӵ�1/128��ֵ */
#define	INIYEAR		(*(unsigned char volatile xdata *)0xfe68)	/* RTC���ʼ�� */
#define	INIMONTH	(*(unsigned char volatile xdata *)0xfe69)	/* RTC�³�ʼ�� */
#define	INIDAY		(*(unsigned char volatile xdata *)0xfe6a)	/* RTC�ճ�ʼ�� */
#define	INIHOUR		(*(unsigned char volatile xdata *)0xfe6b)	/* RTCʱ��ʼ�� */
#define	INIMIN		(*(unsigned char volatile xdata *)0xfe6c)	/* RTC�ֳ�ʼ�� */
#define	INISEC		(*(unsigned char volatile xdata *)0xfe6d)	/* RTC���ʼ�� */
#define	INISSEC		(*(unsigned char volatile xdata *)0xfe6e)	/* RTC1/128���ʼ�� */
#define	RTC_YEAR	(*(unsigned char volatile xdata *)0xfe70)	/* RTC�������ֵ */
#define	RTC_MONTH	(*(unsigned char volatile xdata *)0xfe71)	/* RTC���¼���ֵ */
#define	RTC_DAY		(*(unsigned char volatile xdata *)0xfe72)	/* RTC���ռ���ֵ */
#define	RTC_HOUR	(*(unsigned char volatile xdata *)0xfe73)	/* RTC��ʱ����ֵ */
#define	RTC_MIN		(*(unsigned char volatile xdata *)0xfe74)	/* RTC�ķּ���ֵ */
#define	RTC_SEC		(*(unsigned char volatile xdata *)0xfe75)	/* RTC�������ֵ */
#define	RTC_SSEC	(*(unsigned char volatile xdata *)0xfe76)	/* RTC��1/128�����ֵ */

#define	I2CCFG		(*(unsigned char volatile xdata *)0xfe80)	/* I2C���üĴ���  */
#define	I2CMSCR		(*(unsigned char volatile xdata *)0xfe81)	/* I2C�������ƼĴ���  */
#define	I2CMSST		(*(unsigned char volatile xdata *)0xfe82)	/* I2C����״̬�Ĵ���  */
#define	I2CSLCR		(*(unsigned char volatile xdata *)0xfe83)	/* I2C�ӻ����ƼĴ���  */
#define	I2CSLST		(*(unsigned char volatile xdata *)0xfe84)	/* I2C�ӻ�״̬�Ĵ���  */
#define	I2CSLADR	(*(unsigned char volatile xdata *)0xfe85)	/* I2C�ӻ���ַ�Ĵ���  */
#define	I2CTXD		(*(unsigned char volatile xdata *)0xfe86)	/* I2C���ݷ��ͼĴ���  */
#define	I2CRXD		(*(unsigned char volatile xdata *)0xfe87)	/* I2C���ݽ��ռĴ���  */
#define	I2CMSAUX	(*(unsigned char volatile xdata *)0xfe88)	/* I2C�����������ƼĴ���  */

#define	TM2PS		(*(unsigned char volatile xdata *)0xfea2)	/* ��ʱ��2ʱ��Ԥ��Ƶ�Ĵ���  */
#define	TM3PS		(*(unsigned char volatile xdata *)0xfea3)	/* ��ʱ��3ʱ��Ԥ��Ƶ�Ĵ���  */
#define	TM4PS		(*(unsigned char volatile xdata *)0xfea4)	/* ��ʱ��4ʱ��Ԥ��Ƶ�Ĵ���  */
#define	ADCTIM		(*(unsigned char volatile xdata *)0xfea8)	/* ADCʱ����ƼĴ���  */
#define	T3T4PIN		(*(unsigned char volatile xdata *)0xfeac)	/* T3/T4ѡ��Ĵ���  */
#define	CMPEXCFG	(*(unsigned char volatile xdata *)0xfeae)	/* �Ƚ�����չ���üĴ���  */

#define	PWMA_ETRPS	(*(unsigned char volatile xdata *)0xfeb0)	/* PWMA��ETRѡ��Ĵ���  */
#define	PWMA_ENO	(*(unsigned char volatile xdata *)0xfeb1)	/* PWMA���ʹ�ܼĴ���   */
#define	PWMA_PS		(*(unsigned char volatile xdata *)0xfeb2)	/* PWMA�����ѡ��Ĵ��� */
#define	PWMA_IOAUX	(*(unsigned char volatile xdata *)0xfeb3)	/* PWMA�����Ĵ���       */
#define	PWMB_ETRPS	(*(unsigned char volatile xdata *)0xfeb4)	/* PWMB��ETRѡ��Ĵ���  */
#define	PWMB_ENO	(*(unsigned char volatile xdata *)0xfeb5)	/* PWMB���ʹ�ܼĴ���   */
#define	PWMB_PS		(*(unsigned char volatile xdata *)0xfeb6)	/* PWMB�����ѡ��Ĵ��� */
#define	PWMB_IOAUX	(*(unsigned char volatile xdata *)0xfeb7)	/* PWMB�����Ĵ���       */

#define	PWMA_CR1	(*(unsigned char volatile xdata *)0xfec0)	/* PWMA���ƼĴ���1  */
#define	PWMA_CR2	(*(unsigned char volatile xdata *)0xfec1)	/* PWMA���ƼĴ���2  */
#define	PWMA_SMCR	(*(unsigned char volatile xdata *)0xfec2)	/* PWMA��ģʽ���ƼĴ���  */
#define	PWMA_ETR	(*(unsigned char volatile xdata *)0xfec3)	/* PWMA�ⲿ�����Ĵ���  */
#define	PWMA_IER	(*(unsigned char volatile xdata *)0xfec4)	/* PWMA�ж�ʹ�ܼĴ���  */
#define	PWMA_SR1	(*(unsigned char volatile xdata *)0xfec5)	/* PWMA״̬�Ĵ���1  */
#define	PWMA_SR2	(*(unsigned char volatile xdata *)0xfec6)	/* PWMA״̬�Ĵ���2  */
#define	PWMA_EGR	(*(unsigned char volatile xdata *)0xfec7)	/* PWMA�¼������Ĵ���  */

#define	PWMA_CCMR1	(*(unsigned char volatile xdata *)0xfec8)	/* PWMA����/�Ƚ�ģʽ�Ĵ���1  */
#define	PWMA_CCMR2	(*(unsigned char volatile xdata *)0xfec9)	/* PWMA����/�Ƚ�ģʽ�Ĵ���2  */
#define	PWMA_CCMR3	(*(unsigned char volatile xdata *)0xfeca)	/* PWMA����/�Ƚ�ģʽ�Ĵ���3  */
#define	PWMA_CCMR4	(*(unsigned char volatile xdata *)0xfecb)	/* PWMA����/�Ƚ�ģʽ�Ĵ���4  */
#define	PWMA_CCER1	(*(unsigned char volatile xdata *)0xfecc)	/* PWMA����Ƚ�ʹ�ܼĴ���1  */
#define	PWMA_CCER2	(*(unsigned char volatile xdata *)0xfecd)	/* PWMA����Ƚ�ʹ�ܼĴ���2  */
#define	PWMA_CNTRH	(*(unsigned char volatile xdata *)0xfece)	/* PWMA���������ֽ�   */
#define	PWMA_CNTRL	(*(unsigned char volatile xdata *)0xfecf)	/* PWMA���������ֽ�   */

#define PWMA_PSCRH  (*(unsigned char volatile xdata *)0xfed0)	/* PWMAԤ��Ƶ�����ֽ� */
#define PWMA_PSCRL  (*(unsigned char volatile xdata *)0xfed1)	/* PWMAԤ��Ƶ�����ֽ� */
#define PWMA_ARRH   (*(unsigned char volatile xdata *)0xfed2)	/* PWMA�Զ���װ�Ĵ������ֽ� */
#define PWMA_ARRL   (*(unsigned char volatile xdata *)0xfed3)	/* PWMA�Զ���װ�Ĵ������ֽ� */
#define PWMA_RCR    (*(unsigned char volatile xdata *)0xfed4)	/* PWMA�ظ������Ĵ��� */
#define PWMA_CCR1H  (*(unsigned char volatile xdata *)0xfed5)	/* PWMA�Ƚϲ���Ĵ���1���ֽ� */
#define PWMA_CCR1L  (*(unsigned char volatile xdata *)0xfed6)	/* PWMA�Ƚϲ���Ĵ���1���ֽ� */
#define PWMA_CCR2H  (*(unsigned char volatile xdata *)0xfed7)	/* PWMA�Ƚϲ���Ĵ���2���ֽ� */
#define PWMA_CCR2L  (*(unsigned char volatile xdata *)0xfed8)	/* PWMA�Ƚϲ���Ĵ���2���ֽ� */
#define PWMA_CCR3H  (*(unsigned char volatile xdata *)0xfed9)	/* PWMA�Ƚϲ���Ĵ���3���ֽ� */
#define PWMA_CCR3L  (*(unsigned char volatile xdata *)0xfeda)	/* PWMA�Ƚϲ���Ĵ���3���ֽ� */
#define PWMA_CCR4H  (*(unsigned char volatile xdata *)0xfedb)	/* PWMA�Ƚϲ���Ĵ���4���ֽ� */
#define PWMA_CCR4L  (*(unsigned char volatile xdata *)0xfedc)	/* PWMA�Ƚϲ���Ĵ���4���ֽ� */
#define PWMA_BKR    (*(unsigned char volatile xdata *)0xfedd)	/* PWMAɲ���Ĵ��� */
#define PWMA_DTR    (*(unsigned char volatile xdata *)0xfede)	/* PWMA�������ƼĴ��� */
#define PWMA_OISR   (*(unsigned char volatile xdata *)0xfedf)	/* PWMA�������״̬�Ĵ��� */

#define	PWMB_CR1	(*(unsigned char volatile xdata *)0xfee0)	/* PWMB���ƼĴ���1  */
#define	PWMB_CR2	(*(unsigned char volatile xdata *)0xfee1)	/* PWMB���ƼĴ���2  */
#define	PWMB_SMCR	(*(unsigned char volatile xdata *)0xfee2)	/* PWMB��ģʽ���ƼĴ���  */
#define	PWMB_ETR	(*(unsigned char volatile xdata *)0xfee3)	/* PWMB�ⲿ�����Ĵ���  */
#define	PWMB_IER	(*(unsigned char volatile xdata *)0xfee4)	/* PWMB�ж�ʹ�ܼĴ���  */
#define	PWMB_SR1	(*(unsigned char volatile xdata *)0xfee5)	/* PWMB״̬�Ĵ���1  */
#define	PWMB_SR2	(*(unsigned char volatile xdata *)0xfee6)	/* PWMB״̬�Ĵ���2  */
#define	PWMB_EGR	(*(unsigned char volatile xdata *)0xfee7)	/* PWMB�¼������Ĵ���  */

#define	PWMB_CCMR1	(*(unsigned char volatile xdata *)0xfee8)	/* PWMB����/�Ƚ�ģʽ�Ĵ���1  */
#define	PWMB_CCMR2	(*(unsigned char volatile xdata *)0xfee9)	/* PWMB����/�Ƚ�ģʽ�Ĵ���2  */
#define	PWMB_CCMR3	(*(unsigned char volatile xdata *)0xfeea)	/* PWMB����/�Ƚ�ģʽ�Ĵ���3  */
#define	PWMB_CCMR4	(*(unsigned char volatile xdata *)0xfeeb)	/* PWMB����/�Ƚ�ģʽ�Ĵ���4  */
#define	PWMB_CCER1	(*(unsigned char volatile xdata *)0xfeec)	/* PWMB����Ƚ�ʹ�ܼĴ���1  */
#define	PWMB_CCER2	(*(unsigned char volatile xdata *)0xfeed)	/* PWMB����Ƚ�ʹ�ܼĴ���2  */
#define	PWMB_CNTRH	(*(unsigned char volatile xdata *)0xfeee)	/* PWMB���������ֽ�   */
#define	PWMB_CNTRL	(*(unsigned char volatile xdata *)0xfeef)	/* PWMB���������ֽ�   */

#define PWMB_PSCRH  (*(unsigned char volatile xdata *)0xfef0)	/* PWMBԤ��Ƶ�����ֽ� */
#define PWMB_PSCRL  (*(unsigned char volatile xdata *)0xfef1)	/* PWMBԤ��Ƶ�����ֽ� */
#define PWMB_ARRH   (*(unsigned char volatile xdata *)0xfef2)	/* PWMB�Զ���װ�Ĵ������ֽ� */
#define PWMB_ARRL   (*(unsigned char volatile xdata *)0xfef3)	/* PWMB�Զ���װ�Ĵ������ֽ� */
#define PWMB_RCR    (*(unsigned char volatile xdata *)0xfef4)	/* PWMB�ظ������Ĵ��� */
#define PWMB_CCR1H  (*(unsigned char volatile xdata *)0xfef5)	/* PWMB�Ƚϲ���Ĵ���1���ֽ� */
#define PWMB_CCR1L  (*(unsigned char volatile xdata *)0xfef6)	/* PWMB�Ƚϲ���Ĵ���1���ֽ� */
#define PWMB_CCR2H  (*(unsigned char volatile xdata *)0xfef7)	/* PWMB�Ƚϲ���Ĵ���2���ֽ� */
#define PWMB_CCR2L  (*(unsigned char volatile xdata *)0xfef8)	/* PWMB�Ƚϲ���Ĵ���2���ֽ� */
#define PWMB_CCR3H  (*(unsigned char volatile xdata *)0xfef9)	/* PWMB�Ƚϲ���Ĵ���3���ֽ� */
#define PWMB_CCR3L  (*(unsigned char volatile xdata *)0xfefa)	/* PWMB�Ƚϲ���Ĵ���3���ֽ� */
#define PWMB_CCR4H  (*(unsigned char volatile xdata *)0xfefb)	/* PWMB�Ƚϲ���Ĵ���4���ֽ� */
#define PWMB_CCR4L  (*(unsigned char volatile xdata *)0xfefc)	/* PWMB�Ƚϲ���Ĵ���4���ֽ� */
#define PWMB_BKR    (*(unsigned char volatile xdata *)0xfefd)	/* PWMBɲ���Ĵ��� */
#define PWMB_DTR    (*(unsigned char volatile xdata *)0xfefe)	/* PWMB�������ƼĴ��� */
#define PWMB_OISR   (*(unsigned char volatile xdata *)0xfeff)	/* PWMB�������״̬�Ĵ��� */


#define PWMA_CNTR   (*(unsigned int  volatile xdata *)0xfece)	/* PWMA������16λ���� */
#define PWMA_PSCR   (*(unsigned int  volatile xdata *)0xfed0)	/* PWMAԤ��Ƶ��16λ���� */
#define PWMA_ARR    (*(unsigned int  volatile xdata *)0xfed2)	/* PWMA�Զ���װ�Ĵ���16λ���� */
#define PWMA_CCR1   (*(unsigned int  volatile xdata *)0xfed5)	/* PWMA�Ƚϲ���Ĵ���1 16λ���� */
#define PWMA_CCR2   (*(unsigned int  volatile xdata *)0xfed7)	/* PWMA�Ƚϲ���Ĵ���2 16λ���� */
#define PWMA_CCR3   (*(unsigned int  volatile xdata *)0xfed9)	/* PWMA�Ƚϲ���Ĵ���3 16λ���� */
#define PWMA_CCR4   (*(unsigned int  volatile xdata *)0xfedB)	/* PWMA�Ƚϲ���Ĵ���4 16λ���� */

#define PWMB_CNTR   (*(unsigned int  volatile xdata *)0xfeee)	/* PWMA������16λ���� */
#define PWMB_PSCR   (*(unsigned int  volatile xdata *)0xfef0)	/* PWMAԤ��Ƶ��16λ���� */
#define PWMB_ARR    (*(unsigned int  volatile xdata *)0xfef2)	/* PWMA�Զ���װ�Ĵ���16λ���� */
#define PWMB_CCR1   (*(unsigned int  volatile xdata *)0xfef5)	/* PWMA�Ƚϲ���Ĵ���1 16λ���� */
#define PWMB_CCR2   (*(unsigned int  volatile xdata *)0xfef7)	/* PWMA�Ƚϲ���Ĵ���2 16λ���� */
#define PWMB_CCR3   (*(unsigned int  volatile xdata *)0xfef9)	/* PWMA�Ƚϲ���Ĵ���3 16λ���� */
#define PWMB_CCR4   (*(unsigned int  volatile xdata *)0xfefB)	/* PWMA�Ƚϲ���Ĵ���4 16λ���� */

//=============================================================================================================

#define	EAXSFR()		P_SW2 |=  0x80		/* MOVX A,@DPTR/MOVX @DPTR,Aָ��Ĳ�������Ϊ��չSFR(XSFR) */
#define	EAXRAM()		P_SW2 &= ~0x80		/* MOVX A,@DPTR/MOVX @DPTR,Aָ��Ĳ�������Ϊ��չRAM(XRAM) */

#define	I2C_USE_P15P14()	P_SW2 &= ~0x30						/* ��I2C�л���P1.5(SCL) P1.4(SDA)(�ϵ�Ĭ��).*/
#define	I2C_USE_P25P24()	P_SW2  = (P_SW2 & ~0x30) | 0x10		/* ��I2C�л���P2.5(SCL) P2.4(SDA).*/
#define	I2C_USE_P32P33()	P_SW2 |= 0x30						/* ��I2C�л���P3.2(SCL) P3.3(SDA).*/

#define	MainFosc_IRC24M()	CKSEL = (CKSEL & ~0x03)			/* ѡ���ڲ�24MHZʱ�� */
#define	MainFosc_XTAL()		CKSEL = (CKSEL & ~0x03) | 0x01	/* ѡ���ⲿ�����ʱ�� */
#define	EXT_CLOCK()			XOSCCR = 0x80					/* ѡ���ⲿʱ�� */
#define	EXT_CRYSTAL()		XOSCCR = 0xC0					/* ѡ���ⲿ���� */
#define	MainFosc_IRC32K()	CKSEL =  CKSEL | 0x03			/* ѡ���ڲ�32Kʱ�� */
#define	MCLKO_None()		MCLKOCR = 0						/* ��ʱ�Ӳ���� 	    */
#define	MCLKO54_DIV(n)		MCLKOCR = n						/* ��ʱ�Ӵ�P5.4��Ƶ��� */
#define	MCLKO16_DIV(n)		MCLKOCR = n | 0x80				/* ��ʱ�Ӵ�P1.6��Ƶ��� */


/*  BIT Registers  */
/*  PSW   */
sbit CY   = PSW^7;
sbit AC   = PSW^6;
sbit F0   = PSW^5;
sbit RS1  = PSW^4;
sbit RS0  = PSW^3;
sbit OV   = PSW^2;
sbit F1   = PSW^1;
sbit P    = PSW^0;

/*  TCON  */
sbit TF1  = TCON^7;	//��ʱ��1����жϱ�־λ
sbit TR1  = TCON^6;	//��ʱ��1���п���λ
sbit TF0  = TCON^5;	//��ʱ��0����жϱ�־λ
sbit TR0  = TCON^4;	//��ʱ��0���п���λ
sbit IE1  = TCON^3;	//���ж�1��־λ
sbit IT1  = TCON^2;	//���ж�1�źŷ�ʽ����λ��1���½����жϣ�0�������½����жϡ�
sbit IE0  = TCON^1;	//���ж�0��־λ
sbit IT0  = TCON^0;	//���ж�0�źŷ�ʽ����λ��1���½����жϣ�0�������½����жϡ�

/*  P0  */
sbit  P00 = P0^0;
sbit  P01 = P0^1;
sbit  P02 = P0^2;
sbit  P03 = P0^3;
sbit  P04 = P0^4;
sbit  P05 = P0^5;
sbit  P06 = P0^6;
sbit  P07 = P0^7;

/*  P1  */
sbit  P10 = P1^0;
sbit  P11 = P1^1;
sbit  P12 = P1^2;
sbit  P13 = P1^3;
sbit  P14 = P1^4;
sbit  P15 = P1^5;
sbit  P16 = P1^6;
sbit  P17 = P1^7;

/*  P2  */
sbit  P20 = P2^0;
sbit  P21 = P2^1;
sbit  P22 = P2^2;
sbit  P23 = P2^3;
sbit  P24 = P2^4;
sbit  P25 = P2^5;
sbit  P26 = P2^6;
sbit  P27 = P2^7;

/*  P3  */
sbit  P30 = P3^0;
sbit  P31 = P3^1;
sbit  P32 = P3^2;
sbit  P33 = P3^3;
sbit  P34 = P3^4;
sbit  P35 = P3^5;
sbit  P36 = P3^6;
sbit  P37 = P3^7;

/*  P4  */
sbit  P40 = P4^0;
sbit  P41 = P4^1;
sbit  P42 = P4^2;
sbit  P43 = P4^3;
sbit  P44 = P4^4;
sbit  P45 = P4^5;
sbit  P46 = P4^6;
sbit  P47 = P4^7;

/*  P5  */
sbit  P50 = P5^0;
sbit  P51 = P5^1;
sbit  P52 = P5^2;
sbit  P53 = P5^3;
sbit  P54 = P5^4;
sbit  P55 = P5^5;
sbit  P56 = P5^6;
sbit  P57 = P5^7;

/*  P6  */
sbit  P60 = P6^0;
sbit  P61 = P6^1;
sbit  P62 = P6^2;
sbit  P63 = P6^3;
sbit  P64 = P6^4;
sbit  P65 = P6^5;
sbit  P66 = P6^6;
sbit  P67 = P6^7;

/*  P7  */
sbit  P70 = P7^0;
sbit  P71 = P7^1;
sbit  P72 = P7^2;
sbit  P73 = P7^3;
sbit  P74 = P7^4;
sbit  P75 = P7^5;
sbit  P76 = P7^6;
sbit  P77 = P7^7;

sbit  RXD2      = P1^0;
sbit  TXD2      = P1^1;
sbit  ECI       = P1^2;
sbit  CCP0      = P1^1;
sbit  CCP1      = P1^0;
sbit  CCP2      = P3^7;
sbit  ECI_2     = P3^4;
sbit  CCP0_2    = P3^5;
sbit  CCP1_2    = P3^6;
sbit  CCP2_2    = P3^7;

sbit  SPI_SS    = P1^2;
sbit  SPI_MOSI  = P1^3;
sbit  SPI_MISO  = P1^4;
sbit  SPI_SCLK  = P1^5;

sbit  SPI_SS_2    = P2^2;
sbit  SPI_MOSI_2  = P2^3;
sbit  SPI_MISO_2  = P2^4;
sbit  SPI_SCLK_2  = P2^5;

sbit  SPI_SS_4    = P3^5;
sbit  SPI_MOSI_4  = P3^4;
sbit  SPI_MISO_4  = P3^3;
sbit  SPI_SCLK_4  = P3^2;

sbit RXD  = P3^0;
sbit TXD  = P3^1;
sbit INT0 = P3^2;
sbit INT1 = P3^3;
sbit T0   = P3^4;
sbit T1   = P3^5;

sbit INT2 = P3^6;
sbit INT3 = P3^7;
sbit INT4 = P3^0;

sbit CLKOUT0   = P3^5;
sbit CLKOUT1   = P3^4;


/*  SCON  */
sbit SM0  = SCON^7;	//SM0/FE		SM0 SM1 = 00 ~ 11: ��ʽ0~3
sbit SM1  = SCON^6;	//
sbit SM2  = SCON^5;	//���ͨѶ
sbit REN  = SCON^4;	//��������
sbit TB8  = SCON^3;	//�������ݵ�8λ
sbit RB8  = SCON^2;	//�������ݵ�8λ
sbit TI   = SCON^1;	//�����жϱ�־λ
sbit RI   = SCON^0;	//�����жϱ�־λ

/*  IE   */
sbit EA   = IE^7;	//�ж������ܿ���λ
sbit ELVD = IE^6;	//��ѹ����ж�����λ
sbit EADC = IE^5;	//ADC �ж� ����λ
sbit ES   = IE^4;	//�����ж� �������λ
sbit ET1  = IE^3;	//��ʱ�ж�1�������λ
sbit EX1  = IE^2;	//�ⲿ�ж�1�������λ
sbit ET0  = IE^1;	//��ʱ�ж�0�������λ
sbit EX0  = IE^0;	//�ⲿ�ж�0�������λ

sbit ACC0 = ACC^0;
sbit ACC1 = ACC^1;
sbit ACC2 = ACC^2;
sbit ACC3 = ACC^3;
sbit ACC4 = ACC^4;
sbit ACC5 = ACC^5;
sbit ACC6 = ACC^6;
sbit ACC7 = ACC^7;

sbit B0 = B^0;
sbit B1 = B^1;
sbit B2 = B^2;
sbit B3 = B^3;
sbit B4 = B^4;
sbit B5 = B^5;
sbit B6 = B^6;
sbit B7 = B^7;

//                                     7    6    5    4    3     2       1       0      Reset Value
//INT_CLKO:  �ж���ʱ��������ƼĴ���  -   EX4  EX3  EX2   -   T2CLKO  T1CLKO  T0CLKO    0000,0000
#define	INT4_Enable()	INT_CLKO |= (1 << 6)
#define	INT3_Enable()	INT_CLKO |= (1 << 5)
#define	INT2_Enable()	INT_CLKO |= (1 << 4)
#define	INT1_Enable()	EX1 = 1
#define	INT0_Enable()	EX0 = 1

#define	INT4_Disable()	INT_CLKO &= ~(1 << 6)
#define	INT3_Disable()	INT_CLKO &= ~(1 << 5)
#define	INT2_Disable()	INT_CLKO &= ~(1 << 4)
#define	INT1_Disable()	EX1 = 0
#define	INT0_Disable()	EX0 = 0

//                               7    6       5       4      3    2     1     0      Reset Value
//AUXINTIF:  �����жϱ�־�Ĵ���  -  INT4IF  INT3IF  INT2IF   -   T4IF  T3IF  T2IF    0000,0000
#define	INT4IF	0x40
#define	INT3IF	0x20
#define	INT2IF	0x10
#define	T4IF	0x04
#define	T3IF	0x02
#define	T2IF	0x01

#define	INT4_Clear()	AUXINTIF &= ~INT4IF		/* ������ж�4��־λ */
#define	INT3_Clear()	AUXINTIF &= ~INT3IF		/* ������ж�3��־λ */
#define	INT2_Clear()	AUXINTIF &= ~INT2IF		/* ������ж�2��־λ */
#define	INT1_Clear()	IE1 = 0					/* ������ж�1��־λ */
#define	INT0_Clear()	IE0 = 0					/* ������ж�0��־λ */

#define	INT0_Fall()		IT0 = 1		/* INT0 �½����ж�           */
#define	INT0_RiseFall()	IT0 = 0		/* INT0 �½��������ؾ��ж�   */
#define	INT1_Fall()		IT1 = 1		/* INT1 �½����ж�           */
#define	INT1_RiseFall()	IT0 = 0		/* INT1 �½��������ؾ��ж�   */


//							7     6     5    4    3    2    1     0    Reset Value
//sfr IE2       = 0xAF;		-     -     -    -    -    -   ESPI  ES2   0000,0000B	//Auxiliary Interrupt
#define		SPI_INT_ENABLE()		IE2 |=  2	/* ����SPI�ж�		*/
#define		SPI_INT_DISABLE()		IE2 &= ~2	/* ����SPI�ж�		*/
#define		UART2_INT_ENABLE()		IE2 |=  1	/* ������2�ж�	*/
#define		UART2_INT_DISABLE()		IE2 &= ~1	/* ������2�ж�	*/

//                                          7     6     5    4    3    2    1    0    Reset Value
//sfr IP      = 0xB8; //�ж����ȼ���λ      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000
//--------
sbit PPCA	= IP^7;	//PCA ģ���ж����ȼ�
sbit PLVD	= IP^6;	//��ѹ����ж����ȼ�
sbit PADC	= IP^5;	//ADC �ж����ȼ�
sbit PS   	= IP^4;	//�����ж�0���ȼ��趨λ
sbit PT1	= IP^3;	//��ʱ�ж�1���ȼ��趨λ
sbit PX1	= IP^2;	//�ⲿ�ж�1���ȼ��趨λ
sbit PT0	= IP^1;	//��ʱ�ж�0���ȼ��趨λ
sbit PX0	= IP^0;	//�ⲿ�ж�0���ȼ��趨λ

//                                           7      6      5     4     3     2    1     0        Reset Value
//sfr IPH   = 0xB7; //�ж����ȼ���λ       PPCAH  PLVDH  PADCH  PSH  PT1H  PX1H  PT0H  PX0H   0000,0000
//sfr IP2   = 0xB5; //                       -      -      -     -     -     -   PSPI   PS2   xxxx,xx00
//sfr IP2H  = 0xB6; //                       -      -      -     -     -     -   PSPIH  PS2H  xxxx,xx00
#define		PPCAH	0x80
#define		PLVDH	0x40
#define		PADCH	0x20
#define		PSH		0x10
#define		PT1H	0x08
#define		PX1H	0x04
#define		PT0H	0x02
#define		PX0H	0x01

#define		PCA_InterruptFirst()	PPCA = 1
#define		LVD_InterruptFirst()	PLVD = 1
#define		ADC_InterruptFirst()	PADC = 1
#define		UART1_InterruptFirst()	PS   = 1
#define		Timer1_InterruptFirst()	PT1  = 1
#define		INT1_InterruptFirst()	PX1  = 1
#define		Timer0_InterruptFirst()	PT0  = 1
#define		INT0_InterruptFirst()	PX0  = 1


/*************************************************************************************************/


//                       7      6     5    4    3    2     1      0        Reset Value
//sfr CMPCR1 = 0xE6;   CMPEN  CMPIF  PIE  NIE  PIS  NIS  CMPOE  CMPRES      00000000B
#define	CMPEN	0x80	//1: ����Ƚ���, 0: ��ֹ,�رձȽ�����Դ
#define	CMPIF	0x40	//�Ƚ����жϱ�־, ���������ػ��½����ж�, �����0
#define	PIE		0x20	//1: �ȽϽ����0��1, �����������ж�
#define	NIE		0x10	//1: �ȽϽ����1��0, �����½����ж�
#define	PIS		0x08	//����������ѡ��, 0: ѡ���ڲ�P3.7��������,           1: ��ADC_CHS[3:0]��ѡ���ADC�������������.
#define	NIS		0x04	//���븺����ѡ��, 0: ѡ���ڲ�BandGap��ѹBGv��������, 1: ѡ���ⲿP3.6������.
#define	CMPOE	0x02	//1: ����ȽϽ�����, 0: ��ֹ.
#define	CMPRES	0x01	//�ȽϽ��, 1: CMP+��ƽ����CMP-,  0: CMP+��ƽ����CMP-,  ֻ��

#define	CMP_P_P37	0x00	//����������ѡ��, 0: ѡ���ڲ�P3.7��������
#define	CMP_P_ADC	0x08	//����������ѡ��, 1: ��ADC_CHS[3:0]��ѡ���ADC�������������.
#define	CMP_N_GAP	0x00	//���븺����ѡ��, 0: ѡ���ڲ�BandGap��ѹBGv��������.
#define	CMP_N_P36	0x04	//���븺����ѡ��, 1: ѡ���ⲿP3.6������.

#define	CMPO_P34()	P_SW2 &= ~0x08	//��������P3.4.

//                       7        6       5  4  3  2  1  0    Reset Value
//sfr CMPCR2 = 0xE7;   INVCMPO  DISFLT       LCDTY[5:0]       00001001B
#define	INVCMPO	0x80	//1: �Ƚ������IOȡ��,  0: ��ȡ��
#define	DISFLT	0x40	//1: �ر�0.1uF�˲�,   	0: ����
#define	LCDTY	0x00	//0~63, �ȽϽ���仯��ʱ������



/*************************************************************************************************/
//                     7     6     5    4    3    2   1   0       Reset Value
//sfr SCON  = 0x98;   SM0   SM1   SM2  REN  TB8  RB8  TI  RI      00000000B		 //S1 Control

#define		S1_DoubleRate()		PCON  |=  0x80
#define		S1_SHIFT()			SCON  &=  0x3f

#define		S1_8bit()			SCON   =  (SCON & 0x3f) | 0x40
#define		S1_9bit()			SCON   =  (SCON & 0x3f) | 0xc0
#define		S1_RX_Enable()		SCON  |=  0x10
#define		S1_RX_Disable()		SCON  &= ~0x10
#define		TI1					TI					/* �ж�TI1�Ƿ������								 */
#define		RI1					RI					/* �ж�RI1�Ƿ�������								 */
#define		SET_TI1()			TI = 1				/* ����TI1(�����ж�)								 */
#define		CLR_TI1()			TI = 0				/* ���TI1											 */
#define		CLR_RI1()			RI = 0				/* ���RI1											 */
#define		S1TB8_SET()			TB8 = 1				/* ����TB8											 */
#define		S1TB8_CLR()			TB8 = 0				/* ���TB8											 */
#define		S1_Int_Enable()		ES     =  1			/* ����1�����ж�									 */
#define		S1_Int_Disable()	ES     =  0			/* ����1��ֹ�ж�									 */
#define 	S1_BRT_UseTimer1()	AUXR  &= ~1
#define 	S1_BRT_UseTimer2()	AUXR  |=  1
#define		S1_USE_P30P31()		P_SW1 &= ~0xc0						//UART1 ʹ��P30 P31��	Ĭ��
#define		S1_USE_P36P37()		P_SW1  =  (P_SW1 & ~0xc0) | 0x40	//UART1 ʹ��P36 P37��
#define		S1_USE_P16P17()		P_SW1  =  (P_SW1 & ~0xc0) | 0x80	//UART1 ʹ��P16 P17��
#define		S1_USE_P43P44()		P_SW1  =  (P_SW1 & ~0xc0) | 0xc0	//UART1 ʹ��P4.3 P4.4��

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr S2CON = 0x9A;		S2SM0    -    S2SM2  S2REN  S2TB8  S2RB8  S2TI  S2RI      00000000B		 //S2 Control

#define		S2_MODE0()			S2CON &= ~(1<<7)	/* ����2ģʽ0��8λUART�������� = ��ʱ��2������� / 4 */
#define		S2_MODE1()			S2CON |=  (1<<7)	/* ����2ģʽ1��9λUART�������� = ��ʱ��2������� / 4 */
#define		S2_8bit()			S2CON &= ~(1<<7)	/* ����2ģʽ0��8λUART�������� = ��ʱ��2������� / 4 */
#define		S2_9bit()			S2CON |=  (1<<7)	/* ����2ģʽ1��9λUART�������� = ��ʱ��2������� / 4 */
#define		S2_RX_Enable()		S2CON |=  (1<<4)	/* ����2����										 */
#define		S2_RX_Disable()		S2CON &= ~(1<<4)	/* ��ֹ��2����										 */
#define		TI2					(S2CON & 2) 		/* �ж�TI2�Ƿ������								 */
#define		RI2					(S2CON & 1) 		/* �ж�RI2�Ƿ�������								 */
#define		SET_TI2()			S2CON |=  (1<<1)	/* ����TI2(�����ж�)								 */
#define		CLR_TI2()			S2CON &= ~(1<<1)	/* ���TI2											 */
#define		CLR_RI2()			S2CON &= ~1			/* ���RI2											 */
#define		S2TB8_SET()			S2CON |=  (1<<3)	/* ����TB8											 */
#define		S2TB8_CLR()			S2CON &= ~(1<<3)	/* ���TB8											 */
#define		S2_Int_Enable()		IE2   |=  1			/* ����2�����ж�									 */
#define		S2_Int_Disable()	IE2   &= ~1			/* ����2��ֹ�ж�									 */
#define		S2_USE_P10P11()		P_SW2 &= ~1			/* UART2 ʹ��P1��	Ĭ��							 */
#define		S2_USE_P46P47()		P_SW2 |=  1			/* UART2 ʹ��P4.6 P4.7��							 */

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr S3CON = 0xAC;		S3SM0  S3ST3  S3SM2  S3REN  S3TB8  S3RB8  S3TI  S3RI      00000000B		 //S3 Control

#define		S3_MODE0()			S3CON &= ~(1<<7)	/* ����3ģʽ0��8λUART�������� = ��ʱ��������� / 4  */
#define		S3_MODE1()			S3CON |=  (1<<7)	/* ����3ģʽ1��9λUART�������� = ��ʱ��������� / 4  */
#define		S3_8bit()			S3CON &= ~(1<<7)	/* ����3ģʽ0��8λUART�������� = ��ʱ��������� / 4  */
#define		S3_9bit()			S3CON |=  (1<<7)	/* ����3ģʽ1��9λUART�������� = ��ʱ��������� / 4  */
#define		S3_RX_Enable()		S3CON |=  (1<<4)	/* ����3����									     */
#define		S3_RX_Disable()		S3CON &= ~(1<<4)	/* ��ֹ��3����									     */
#define		TI3					(S3CON & 2) != 0	/* �ж�TI3�Ƿ������								 */
#define		RI3					(S3CON & 1) != 0	/* �ж�RI3�Ƿ�������								 */
#define		SET_TI3()			S3CON |=  (1<<1)	/* ����TI3(�����ж�)								 */
#define		CLR_TI3()			S3CON &= ~(1<<1)	/* ���TI3											 */
#define		CLR_RI3()			S3CON &= ~1			/* ���RI3											 */
#define		S3TB8_SET()			S3CON |=  (1<<3)	/* ����TB8											 */
#define		S3TB8_CLR()			S3CON &= ~(1<<3)	/* ���TB8											 */
#define		S3_Int_Enable()		IE2   |=  (1<<3)	/* ����3�����ж�								     */
#define		S3_Int_Disable()	IE2   &= ~(1<<3)	/* ����3��ֹ�ж�								     */
#define 	S3_BRT_UseTimer3()	S3CON |=  (1<<6)	/* BRT select Timer3								 */
#define 	S3_BRT_UseTimer2()	S3CON &= ~(1<<6)	/* BRT select Timer2								 */
#define		S3_USE_P00P01()		P_SW2 &= ~2			/* UART3 ʹ��P0.0 P0.1��	Ĭ��				     */
#define		S3_USE_P50P51()		P_SW2 |=  2			/* UART3 ʹ��P5.0 P5.1��						     */

//						  7      6      5      4      3      2     1     0        Reset Value
//sfr S4CON = 0x84;		S4SM0  S4ST4  S4SM2  S4REN  S4TB8  S4RB8  S4TI  S4RI      00000000B		 //S4 Control

#define		S4_MODE0()			S4CON &= ~(1<<7)	/* ����4ģʽ0��8λUART�������� = ��ʱ��������� / 4  */
#define		S4_MODE1()			S4CON |=  (1<<7)	/* ����4ģʽ1��9λUART�������� = ��ʱ��������� / 4  */
#define		S4_8bit()			S4CON &= ~(1<<7)	/* ����4ģʽ0��8λUART�������� = ��ʱ��������� / 4  */
#define		S4_9bit()			S4CON |=  (1<<7)	/* ����4ģʽ1��9λUART�������� = ��ʱ��������� / 4  */
#define		S4_RX_Enable()		S4CON |=  (1<<4)	/* ����4����									     */
#define		S4_RX_Disable()		S4CON &= ~(1<<4)	/* ��ֹ��4����									     */
#define		TI4					(S4CON & 2) != 0	/* �ж�TI3�Ƿ������							     */
#define		RI4					(S4CON & 1) != 0	/* �ж�RI3�Ƿ�������							     */
#define		SET_TI4()			S4CON |=  2			/* ����TI3(�����ж�)							     */
#define		CLR_TI4()			S4CON &= ~2			/* ���TI3										     */
#define		CLR_RI4()			S4CON &= ~1			/* ���RI3										     */
#define		S4TB8_SET()			S4CON |=  8			/* ����TB8										     */
#define		S4TB8_CLR()			S4CON &= ~8			/* ���TB8										     */
#define		S4_Int_Enable()		IE2   |=  (1<<4)	/* ����4�����ж�								     */
#define		S4_Int_Disable()	IE2   &= ~(1<<4)	/* ����4��ֹ�ж�								     */
#define 	S4_BRT_UseTimer4()	S4CON |=  (1<<6)	/* BRT select Timer4								 */
#define 	S4_BRT_UseTimer2()	S4CON &= ~(1<<6)	/* BRT select Timer2								 */
#define		S4_USE_P02P03()		P_SW2 &= ~4			/* UART4 ʹ��P0.2 P0.3��	Ĭ��				     */
#define		S4_USE_P52P53()		P_SW2 |=  4			/* UART4 ʹ��P5.2 P5.3��						     */


/**********************************************************/
//						   7     6       5      4     3      2      1      0    Reset Value
//sfr AUXR  = 0x8E;		T0x12 T1x12 UART_M0x6  T2R  T2_C/T T2x12 EXTRAM  S1ST2  0000,0000	//Auxiliary Register

#define 	InternalXdata_Disable()		AUXR |=  2		/* ��ֹʹ���ڲ�xdata, ���з���xdata���Ƿ����ⲿxdata  */
#define 	InternalXdata_Enable()		AUXR &= ~2		/* ����ʹ���ڲ�xdata, �����ʵĵ�ַ���ڲ�xdata��Χʱ, �����ڲ���xadta, ����ַ�����ڲ�xdataʱ, �����ⲿxdata  */
#define		S1_M0x6()					AUXR |=  (1<<5)	/* UART Mode0 Speed is 6x Standard       */
#define		S1_M0x1()					AUXR &= ~(1<<5)	/* default,	UART Mode0 Speed is Standard */

//====================================
#define		Timer0_16bitAutoReload()	TMOD &= ~0x03					/* 16λ�Զ���װ	*/
#define		Timer0_16bit()				TMOD  = (TMOD & ~0x03) | 0x01	/* 16λ         */
#define		Timer0_8bitAutoReload()		TMOD  = (TMOD & ~0x03) | 0x02	/* 8λ�Զ���װ	*/
#define		Timer0_16bitAutoRL_NoMask()	TMOD |=  0x03		/* 16λ�Զ���װ���������ж�	*/
#define 	Timer0_Run()	 			TR0 = 1				/* ����ʱ��0����			*/
#define 	Timer0_Stop()	 			TR0 = 0				/* ��ֹ��ʱ��0����			*/
#define		Timer0_Gate_INT0_P32()		TMOD |=  (1<<3)		/* ʱ��0���ⲿINT0�ߵ�ƽ����ʱ���� */
#define		Timer0_AsTimer()			TMOD &= ~(1<<2)		/* ʱ��0������ʱ��	*/
#define		Timer0_AsCounter()			TMOD |=  (1<<2)		/* ʱ��0����������	*/
#define		Timer0_AsCounterP34()		TMOD |=  (1<<2)		/* ʱ��0����������	*/
#define 	Timer0_1T()					AUXR |=  (1<<7)		/* Timer0 clodk = fo	*/
#define 	Timer0_12T()				AUXR &= ~(1<<7)		/* Timer0 clodk = fo/12	12��Ƶ,	default	*/
#define		Timer0_CLKO_Enable()		INT_CLKO |=  1			/* ���� T0 ���������T0(P3.5)�������Fck0 = 1/2 T0 ����ʣ�T0����1T��12T��	*/
#define		Timer0_CLKO_Disable()		INT_CLKO &= ~1
#define		Timer0_CLKO_Enable_P35()	INT_CLKO |=  1			/* ���� T0 ���������T0(P3.5)�������Fck0 = 1/2 T0 ����ʣ�T0����1T��12T��	*/
#define		Timer0_CLKO_Disable_P35()	INT_CLKO &= ~1
#define 	Timer0_InterruptEnable()	ET0 = 1				/* ����Timer1�ж�.*/
#define 	Timer0_InterruptDisable()	ET0 = 0				/* ��ֹTimer1�ж�.*/

#define		T0_Load(n)					TH0 = (n) / 256,	TL0 = (n) % 256
#define		T0_Load_us_1T(n)			Timer0_AsTimer(),Timer0_1T(), Timer0_16bitAutoReload(),TH0=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, TL0=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T0_Load_us_12T(n)			Timer0_AsTimer(),Timer0_12T(),Timer0_16bitAutoReload(),TH0=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,TL0=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T0_Frequency_1T_P35(n)		ET0=0,Timer0_AsTimer(),Timer0_1T(),Timer0_16bitAutoReload(),TH0=(65536-(n/2+MAIN_Fosc/2)/(n))/256,TL0=(65536-(n/2+MAIN_Fosc/2)/(n))%256,INT_CLKO |= 1,TR0=1		/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T0_Frequency_12T_P35(n)		ET0=0,Timer0_AsTimer(),Timer0_12T(),Timer0_16bitAutoReload(),TH0=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL0=(65536-(n/2+MAIN_Fosc/24)/(n))%256,INT_CLKO |= 1,TR0=1	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//====================================
#define		Timer1_16bitAutoReload()	TMOD &= ~0x30					/* 16λ�Զ���װ	*/
#define		Timer1_16bit()				TMOD  = (TMOD & ~0x30) | 0x10	/* 16λ			*/
#define		Timer1_8bitAutoReload()		TMOD  = (TMOD & ~0x30) | 0x20	/* 8λ�Զ���װ	*/
#define 	Timer1_Run()	 			TR1 = 1				/* ����ʱ��1����			*/
#define 	Timer1_Stop()	 			TR1 = 0				/* ��ֹ��ʱ��1����			*/
#define		Timer1_Gate_INT1_P33()		TMOD |=  (1<<7)		/* ʱ��1���ⲿINT1�ߵ�ƽ����ʱ����	*/
#define		Timer1_AsTimer()			TMOD &= ~(1<<6)		/* ʱ��1������ʱ��			*/
#define		Timer1_AsCounter()			TMOD |=  (1<<6)		/* ʱ��1����������			*/
#define		Timer1_AsCounterP35()		TMOD |=  (1<<6)		/* ʱ��1����������			*/
#define 	Timer1_1T()					AUXR |=  (1<<6)		/* Timer1 clodk = fo		*/
#define 	Timer1_12T()				AUXR &= ~(1<<6)		/* Timer1 clodk = fo/12	12��Ƶ,	default	*/
#define		Timer1_CLKO_Enable()		INT_CLKO |=  2			/* ���� T1 ���������T1(P3.4)�������Fck1 = 1/2 T1 ����ʣ�T1����1T��12T��	*/
#define		Timer1_CLKO_Disable()		INT_CLKO &= ~2
#define		Timer1_CLKO_Enable_P34()	INT_CLKO |=  2			/* ���� T1 ���������T1(P3.4)�������Fck1 = 1/2 T1 ����ʣ�T1����1T��12T��	*/
#define		Timer1_CLKO_Disable_P34()	INT_CLKO &= ~2
#define 	Timer1_InterruptEnable()	ET1 = 1				/* ����Timer1�ж�.	*/
#define 	Timer1_InterruptDisable()	ET1 = 0				/* ��ֹTimer1�ж�.	*/

#define		T1_Load(n)					TH1 = (n) / 256,	TL1 = (n) % 256
#define		T1_Load_us_1T(n)			Timer1_AsTimer(),Timer1_1T(), Timer1_16bitAutoReload(),TH1=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, TL1=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T1_Load_us_12T(n)			Timer1_AsTimer(),Timer1_12T(),Timer1_16bitAutoReload(),TH1=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,TL1=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T1_Frequency_1T_P34(n)		ET1=0,Timer1_AsTimer(),Timer1_1T(),Timer1_16bitAutoReload(),TH1=(65536-(n/2+MAIN_Fosc/2)/(n))/256,TL1=(65536-(n/2+MAIN_Fosc/2)/(n))%256,INT_CLKO |= 2,TR1=1		/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T1_Frequency_12T_P34(n)		ET1=0,Timer1_AsTimer(),Timer1_12T(),Timer1_16bitAutoReload(),TH1=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL1=(65536-(n/2+MAIN_Fosc/24)/(n))%256,INT_CLKO |= 2,TR1=1	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//====================================
#define 	Timer2_Run()	 			AUXR |=  (1<<4)	/* ����ʱ��2����	*/
#define 	Timer2_Stop()	 			AUXR &= ~(1<<4)	/* ��ֹ��ʱ��2����	*/
#define		Timer2_AsTimer()			AUXR &= ~(1<<3)	/* ʱ��2������ʱ��	*/
#define		Timer2_AsCounter()			AUXR |=  (1<<3)	/* ʱ��2����������	*/
#define		Timer2_AsCounterP31()		AUXR |=  (1<<3)	/* ʱ��2����������	*/
#define 	Timer2_1T()					AUXR |=  (1<<2)	/* Timer0 clock = fo	*/
#define 	Timer2_12T()				AUXR &= ~(1<<2)	/* Timer0 clock = fo/12	12��Ƶ,	default	*/
#define		Timer2_CLKO_Enable()		INT_CLKO |=  4		/* ���� T2 ���������P1.3�������Fck2 = 1/2 T2 ����ʣ�T2����1T��12T��	*/
#define		Timer2_CLKO_Disable()		INT_CLKO &= ~4
#define		Timer2_CLKO_Enable_P13()	INT_CLKO |=  4		/* ���� T2 ���������P1.3�������Fck2 = 1/2 T2 ����ʣ�T2����1T��12T��	*/
#define		Timer2_CLKO_Disable_P13()	INT_CLKO &= ~4
#define 	Timer2_InterruptEnable()	IE2  |=  (1<<2)	/* ����Timer2�ж�.	*/
#define 	Timer2_InterruptDisable()	IE2  &= ~(1<<2)	/* ��ֹTimer2�ж�.	*/

#define		T2_Load(n)					TH2 = (n) / 256,	TL2 = (n) % 256
#define		T2_Load_us_1T(n)			Timer2_AsTimer(),Timer2_1T(), TH2=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, TL2=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T2_Load_us_12T(n)			Timer2_AsTimer(),Timer2_12T(),TH2=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,TL2=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T2_Frequency_1T_P13(n)		Timer2_InterruptDisable(),Timer2_AsTimer(),Timer2_1T(), TH2=(65536-(n/2+MAIN_Fosc/2)/(n))/256, TL2=(65536-(n/2+MAIN_Fosc/2)/(n))%256, Timer2_CLKO_Enable_P13(),Timer2_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T2_Frequency_12T_P13(n)		Timer2_InterruptDisable(),Timer2_AsTimer(),Timer2_12T(),TH2=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL2=(65536-(n/2+MAIN_Fosc/24)/(n))%256,Timer2_CLKO_Enable_P13(),Timer2_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//====================================
#define 	Timer3_Run()	 			T4T3M |=  (1<<3)	/* ����ʱ��3����	*/
#define 	Timer3_Stop()	 			T4T3M &= ~(1<<3)	/* ��ֹ��ʱ��3����	*/
#define		Timer3_AsTimer()			T4T3M &= ~(1<<2)	/* ʱ��3������ʱ��	*/
#define		Timer3_AsCounter()			T4T3M |=  (1<<2)	/* ʱ��3����������, P0.5Ϊ�ⲿ����	*/
#define		Timer3_AsCounterP05()		T4T3M |=  (1<<2)	/* ʱ��3����������, P0.5Ϊ�ⲿ����	*/
#define 	Timer3_1T()					T4T3M |=  (1<<1)	/* 1Tģʽ	*/
#define 	Timer3_12T()				T4T3M &= ~(1<<1)	/* 12Tģʽ,	default	*/
#define		Timer3_CLKO_Enable()		T4T3M |=  1			/* ����T3���������T3(P0.5)�������Fck = 1/2 T2 ����ʣ�T2����1T��12T��	*/
#define		Timer3_CLKO_Disable()		T4T3M &= ~1			/* ��ֹT3���������T3(P0.5)�����	*/
#define 	Timer3_InterruptEnable()	IE2  |=  (1<<5)		/* ����Timer3�ж�.	*/
#define 	Timer3_InterruptDisable()	IE2  &= ~(1<<5)		/* ��ֹTimer3�ж�.	*/

#define		T3_Load(n)					TH3 = (n) / 256,	TL3 = (n) % 256
#define		T3_Load_us_1T(n)			Timer3_AsTimer(),Timer3_1T(), TH3=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, TL3=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T3_Load_us_12T(n)			Timer3_AsTimer(),Timer3_12T(),TH3=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,TL3=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T3_Frequency_1T_P01(n)		P_SW2 |= 0x80,Timer3_InterruptDisable(),Timer3_AsTimer(),Timer3_1T(), TH3=(65536-(n/2+MAIN_Fosc/2)/(n))/256, TL3=(65536-(n/2+MAIN_Fosc/2)/(n))%256, T3T4PIN = 0x01,Timer3_CLKO_Enable(),Timer3_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T3_Frequency_12T_P01(n)		P_SW2 |= 0x80,Timer3_InterruptDisable(),Timer3_AsTimer(),Timer3_12T(),TH3=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL3=(65536-(n/2+MAIN_Fosc/24)/(n))%256,T3T4PIN = 0x01,Timer3_CLKO_Enable(),Timer3_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T3_Frequency_1T_P05(n)		P_SW2 |= 0x80,Timer3_InterruptDisable(),Timer3_AsTimer(),Timer3_1T(), TH3=(65536-(n/2+MAIN_Fosc/2)/(n))/256, TL3=(65536-(n/2+MAIN_Fosc/2)/(n))%256, T3T4PIN = 0x00,Timer3_CLKO_Enable(),Timer3_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T3_Frequency_12T_P05(n)		P_SW2 |= 0x80,Timer3_InterruptDisable(),Timer3_AsTimer(),Timer3_12T(),TH3=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL3=(65536-(n/2+MAIN_Fosc/24)/(n))%256,T3T4PIN = 0x00,Timer3_CLKO_Enable(),Timer3_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */

//====================================
#define 	Timer4_Run()	 			T4T3M |=  (1<<7)	/* ����ʱ��4����	*/
#define 	Timer4_Stop()	 			T4T3M &= ~(1<<7)	/* ��ֹ��ʱ��4����	*/
#define		Timer4_AsTimer()			T4T3M &= ~(1<<6)	/* ʱ��4������ʱ��  */
#define		Timer4_AsCounter()			T4T3M |=  (1<<6)	/* ʱ��4����������, P0.7Ϊ�ⲿ����	*/
#define		Timer4_AsCounterP07()		T4T3M |=  (1<<6)	/* ʱ��4����������, P0.7Ϊ�ⲿ����	*/
#define 	Timer4_1T()					T4T3M |=  (1<<5)	/* 1Tģʽ	*/
#define 	Timer4_12T()				T4T3M &= ~(1<<5)	/* 12Tģʽ,	default	*/
#define		Timer4_CLKO_Enable()		T4T3M |=  (1<<4)	/* ����T4���������T4(P0.7)�������Fck = 1/2 T2 ����ʣ�T2����1T��12T��	*/
#define		Timer4_CLKO_Disable()		T4T3M &= ~(1<<4)	/* ��ֹT4���������T4(P0.7)�����	*/
#define 	Timer4_InterruptEnable()	IE2  |=  (1<<6)		/* ����Timer4�ж�.	*/
#define 	Timer4_InterruptDisable()	IE2  &= ~(1<<6)		/* ��ֹTimer4�ж�.	*/

#define		T4_Load(n)					TH4 = (n) / 256,	TL4 = (n) % 256
#define		T4_Load_us_1T(n)			Timer4_AsTimer(),Timer4_1T(), TH4=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)/256, TL4=(65536-((MAIN_Fosc/1000)*(n)+500)/1000)%256
#define		T4_Load_us_12T(n)			Timer4_AsTimer(),Timer4_12T(),TH4=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)/256,TL4=(65536-((MAIN_Fosc/12000)*(n)+500)/1000)%256
#define		T4_Frequency_1T_P03(n)		P_SW2 |= 0x80,Timer4_InterruptDisable(),Timer4_AsTimer(),Timer4_1T(), TH4=(65536-(n/2+MAIN_Fosc/2)/(n))/256, TL4=(65536-(n/2+MAIN_Fosc/2)/(n))%256, T3T4PIN = 0x01,Timer4_CLKO_Enable(),Timer4_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T4_Frequency_12T_P03(n)		P_SW2 |= 0x80,Timer4_InterruptDisable(),Timer4_AsTimer(),Timer4_12T(),TH4=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL4=(65536-(n/2+MAIN_Fosc/24)/(n))%256,T3T4PIN = 0x01,Timer4_CLKO_Enable(),Timer4_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T4_Frequency_1T_P07(n)		P_SW2 |= 0x80,Timer4_InterruptDisable(),Timer4_AsTimer(),Timer4_1T(), TH4=(65536-(n/2+MAIN_Fosc/2)/(n))/256, TL4=(65536-(n/2+MAIN_Fosc/2)/(n))%256, T3T4PIN = 0x00,Timer4_CLKO_Enable(),Timer4_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define		T4_Frequency_12T_P07(n)		P_SW2 |= 0x80,Timer4_InterruptDisable(),Timer4_AsTimer(),Timer4_12T(),TH4=(65536-(n/2+MAIN_Fosc/24)/(n))/256,TL4=(65536-(n/2+MAIN_Fosc/24)/(n))%256,T3T4PIN = 0x00,Timer4_CLKO_Enable(),Timer4_Run()	/* fx=fosc/(2*M)/n,  M=1 or M=12 */
//====================================================================================================================

//sfr WDT_CONTR = 0xC1; //Watch-Dog-Timer Control register
//                                      7     6     5      4       3      2   1   0     Reset Value
//                                  WDT_FLAG  -  EN_WDT CLR_WDT IDLE_WDT PS2 PS1 PS0    xx00,0000
#define D_WDT_FLAG			(1<<7)
#define D_EN_WDT			(1<<5)
#define D_CLR_WDT			(1<<4)	/* auto clear	*/
#define D_IDLE_WDT			(1<<3)	/* WDT counter when Idle	*/
#define D_WDT_SCALE_2		0
#define D_WDT_SCALE_4		1
#define D_WDT_SCALE_8		2		/* T=393216*N/fo	*/
#define D_WDT_SCALE_16		3
#define D_WDT_SCALE_32		4
#define D_WDT_SCALE_64		5
#define D_WDT_SCALE_128		6
#define D_WDT_SCALE_256		7

#define	WDT_reset(n)	WDT_CONTR = D_EN_WDT + D_CLR_WDT + D_IDLE_WDT + (n)		/* ��ʼ��WDT��ι�� */


//						  7     6      5    4     3      2    1     0     Reset Value
//sfr PCON   = 0x87;	SMOD  SMOD0  LVDF  POF   GF1    GF0   PD   IDL    0001,0000	 //Power Control
//SMOD		//����˫����
//SMOD0
#define		LVDF		(1<<5)	/* P4.6��ѹ����־ */
//POF
//GF1
//GF0
//#define 	D_PD		2		/* set 1, power down mode */
//#define 	D_IDLE		1		/* set 1, idle mode */
#define		MCU_IDLE()			PCON |= 1	/* MCU ���� IDLE ģʽ */
#define		MCU_POWER_DOWN()	PCON |= 2	/* MCU ���� ˯�� ģʽ */


//sfr ISP_CMD   = 0xC5;
#define		ISP_STANDBY()	ISP_CMD = 0		/* ISP���������ֹ��*/
#define		ISP_READ()		ISP_CMD = 1		/* ISP��������		*/
#define		ISP_WRITE()		ISP_CMD = 2		/* ISPд������		*/
#define		ISP_ERASE()		ISP_CMD = 3		/* ISP��������		*/

//sfr ISP_TRIG  = 0xC6;
#define 	ISP_TRIG()	ISP_TRIG = 0x5A,	ISP_TRIG = 0xA5		/* ISP�������� */

//							  7    6    5      4    3    2    1     0    Reset Value
//sfr IAP_CONTR = 0xC7;		IAPEN SWBS SWRST CFAIL  -    -    -     -    0000,x000	//IAP Control Register
#define ISP_EN			(1<<7)
#define ISP_SWBS		(1<<6)
#define ISP_SWRST		(1<<5)
#define ISP_CMD_FAIL	(1<<4)


/* ADC Register */
//								7       6      5       4         3      2    1    0   Reset Value
//sfr ADC_CONTR = 0xBC;		ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000	/* AD ת�����ƼĴ��� */
//sfr ADC_RES  = 0xBD;		ADCV.9 ADCV.8 ADCV.7 ADCV.6 ADCV.5 ADCV.4 ADCV.3 ADCV.2	  0000,0000	/* A/D ת�������8λ */
//sfr ADC_RESL = 0xBE;												  ADCV.1 ADCV.0	  0000,0000	/* A/D ת�������2λ */
//sfr ADC_CONTR  = 0xBC;	//ֱ����MOV��������Ҫ�����


//sfr SPCTL  = 0xCE;	SPI���ƼĴ���
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00

#define	SPI_SSIG_None()		SPCTL |=  (1<<7)		/* 1: ����SS��	*/
#define	SPI_SSIG_Enable()	SPCTL &= ~(1<<7)		/* 0: SS�����ھ������ӻ�	*/
#define	SPI_Enable()		SPCTL |=  (1<<6)		/* 1: ����SPI	*/
#define	SPI_Disable()		SPCTL &= ~(1<<6)		/* 0: ��ֹSPI	*/
#define	SPI_LSB_First()		SPCTL |=  (1<<5)		/* 1: LSB�ȷ�	*/
#define	SPI_MSB_First()		SPCTL &= ~(1<<5)		/* 0: MSB�ȷ�	*/
#define	SPI_Master()		SPCTL |=  (1<<4)		/* 1: ��Ϊ����	*/
#define	SPI_Slave()			SPCTL &= ~(1<<4)		/* 0: ��Ϊ�ӻ�	*/
#define	SPI_SCLK_NormalH()	SPCTL |=  (1<<3)		/* 1: ����ʱSCLKΪ�ߵ�ƽ	*/
#define	SPI_SCLK_NormalL()	SPCTL &= ~(1<<3)		/* 0: ����ʱSCLKΪ�͵�ƽ	*/
#define	SPI_PhaseH()		SPCTL |=  (1<<2)		/* 1: 	*/
#define	SPI_PhaseL()		SPCTL &= ~(1<<2)		/* 0: 	*/
#define	SPI_Speed(n)		SPCTL = (SPCTL & ~3) | (n)	/*�����ٶ�, 0 -- fosc/4, 1 -- fosc/8, 2 -- fosc/16, 3 -- fosc/32	*/

//sfr SPDAT  = 0xCF; //SPI Data Register                                                     0000,0000
//sfr SPSTAT  = 0xCD;	//SPI״̬�Ĵ���
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define	SPIF	0x80		/* SPI������ɱ�־��д��1��0��*/
#define	WCOL	0x40		/* SPIд��ͻ��־��д��1��0��  */

#define		SPI_USE_P12P13P14P15()	P_SW1 &= ~0x0c					/* ��SPI�л���P12(SS) P13(MOSI) P14(MISO) P15(SCLK)(�ϵ�Ĭ��)��*/
#define		SPI_USE_P22P23P24P25()	P_SW1 = (P_SW1 & ~0x0c) | 0x04	/* ��SPI�л���P22(SS) P23(MOSI) P24(MISO) P25(SCLK)��*/
#define		SPI_USE_P35P34P33P32()	P_SW1 =  P_SW1 | 0x0C			/* ��SPI�л���P35(SS) P34(MOSI) P33(MISO) P32(SCLK)��*/


/*
;PCA_PWMn:    7       6     5   4   3   2     1       0
;			EBSn_1	EBSn_0	-	-	-	-	EPCnH	EPCnL
;B5-B2:		����
;B1(EPCnH):	��PWMģʽ�£���CCAPnH���9λ����
;B0(EPCnL):	��PWMģʽ�£���CCAPnL���9λ����
*/
#define		PWM0_NORMAL()	PCA_PWM0 &= ~3					/* PWM0�������(Ĭ��)	*/
#define		PWM0_OUT_0()	PCA_PWM0 |=  3, CCAP0H = 0xff	/* PWM0һֱ���0		*/
#define		PWM0_OUT_1()	PCA_PWM0 &= 0xc0, CCAP0H = 0	/* PWM0һֱ���1		*/

#define		PWMA_NORMAL()	PCA_PWMA &= ~3					/* PWMA�������(Ĭ��)	*/
#define		PWMA_OUT_0()	PCA_PWMA |=  3, CCAP1H = 0xff	/* PWMAһֱ���0		*/
#define		PWMA_OUT_1()	PCA_PWMA &= 0xc0, CCAP1H = 0	/* PWMAһֱ���1		*/

#define		PWMB_NORMAL()	PCA_PWMB &= ~3					/* PWMB�������(Ĭ��)	*/
#define		PWMB_OUT_0()	PCA_PWMB |=  3, CCAP2H = 0xff	/* PWMBһֱ���0		*/
#define		PWMB_OUT_1()	PCA_PWMB &= 0xc0, CCAP2H = 0	/* PWMBһֱ���1		*/


/**********************************************************/
typedef 	unsigned char	u8;
typedef 	unsigned int	u16;
typedef 	unsigned long	u32;

/**********************************************************/
#define NOP1()  _nop_()
#define NOP2()  NOP1(),NOP1()
#define NOP3()  NOP2(),NOP1()
#define NOP4()  NOP3(),NOP1()
#define NOP5()  NOP4(),NOP1()
#define NOP6()  NOP5(),NOP1()
#define NOP7()  NOP6(),NOP1()
#define NOP8()  NOP7(),NOP1()
#define NOP9()  NOP8(),NOP1()
#define NOP10() NOP9(),NOP1()
#define NOP11() NOP10(),NOP1()
#define NOP12() NOP11(),NOP1()
#define NOP13() NOP12(),NOP1()
#define NOP14() NOP13(),NOP1()
#define NOP15() NOP14(),NOP1()
#define NOP16() NOP15(),NOP1()
#define NOP17() NOP16(),NOP1()
#define NOP18() NOP17(),NOP1()
#define NOP19() NOP18(),NOP1()
#define NOP20() NOP19(),NOP1()
#define NOP21() NOP20(),NOP1()
#define NOP22() NOP21(),NOP1()
#define NOP23() NOP22(),NOP1()
#define NOP24() NOP23(),NOP1()
#define NOP25() NOP24(),NOP1()
#define NOP26() NOP25(),NOP1()
#define NOP27() NOP26(),NOP1()
#define NOP28() NOP27(),NOP1()
#define NOP29() NOP28(),NOP1()
#define NOP30() NOP29(),NOP1()
#define NOP31() NOP30(),NOP1()
#define NOP32() NOP31(),NOP1()
#define NOP33() NOP32(),NOP1()
#define NOP34() NOP33(),NOP1()
#define NOP35() NOP34(),NOP1()
#define NOP36() NOP35(),NOP1()
#define NOP37() NOP36(),NOP1()
#define NOP38() NOP37(),NOP1()
#define NOP39() NOP38(),NOP1()
#define NOP40() NOP39(),NOP1()
#define NOP(N)  NOP##N()


/**********************************************/
#define	Pin0		0x01	//IO���� Px.0
#define	Pin1		0x02	//IO���� Px.1
#define	Pin2		0x04	//IO���� Px.2
#define	Pin3		0x08	//IO���� Px.3
#define	Pin4		0x10	//IO���� Px.4
#define	Pin5		0x20	//IO���� Px.5
#define	Pin6		0x40	//IO���� Px.6
#define	Pin7		0x80	//IO���� Px.7
#define	PinAll		0xFF	//IO��������

#define	GPIO_Pin_0		0x01	//IO���� Px.0
#define	GPIO_Pin_1		0x02	//IO���� Px.1
#define	GPIO_Pin_2		0x04	//IO���� Px.2
#define	GPIO_Pin_3		0x08	//IO���� Px.3
#define	GPIO_Pin_4		0x10	//IO���� Px.4
#define	GPIO_Pin_5		0x20	//IO���� Px.5
#define	GPIO_Pin_6		0x40	//IO���� Px.6
#define	GPIO_Pin_7		0x80	//IO���� Px.7
#define	GPIO_Pin_All	0xFF	//IO��������

#define	GPIO_PullUp		0	//����׼˫���
#define	GPIO_HighZ		1	//��������
#define	GPIO_OUT_OD		2	//��©���
#define	GPIO_OUT_PP		3	//�������

#define	GPIO_P0			0		//
#define	GPIO_P1			1
#define	GPIO_P2			2
#define	GPIO_P3			3
#define	GPIO_P4			4
#define	GPIO_P5			5
#define	GPIO_P6			6
#define	GPIO_P7			7

#define P0n_standard(bitn)			P0M1 &= ~(bitn),	P0M0 &= ~(bitn)	/* 00  */
#define P0n_push_pull(bitn)			P0M1 &= ~(bitn),	P0M0 |=  (bitn)	/* 01  */
#define P0n_pure_input(bitn)		P0M1 |=  (bitn),	P0M0 &= ~(bitn)	/* 10  */
#define P0n_open_drain(bitn)		P0M1 |=  (bitn),	P0M0 |=  (bitn)	/* 11  */

#define P1n_standard(bitn)			P1M1 &= ~(bitn),	P1M0 &= ~(bitn)
#define P1n_push_pull(bitn)			P1M1 &= ~(bitn),	P1M0 |=  (bitn)
#define P1n_pure_input(bitn)		P1M1 |=  (bitn),	P1M0 &= ~(bitn)
#define P1n_open_drain(bitn)		P1M1 |=  (bitn),	P1M0 |=  (bitn)

#define P2n_standard(bitn)			P2M1 &= ~(bitn),	P2M0 &= ~(bitn)
#define P2n_push_pull(bitn)			P2M1 &= ~(bitn),	P2M0 |=  (bitn)
#define P2n_pure_input(bitn)		P2M1 |=  (bitn),	P2M0 &= ~(bitn)
#define P2n_open_drain(bitn)		P2M1 |=  (bitn),	P2M0 |=  (bitn)

#define P3n_standard(bitn)			P3M1 &= ~(bitn),	P3M0 &= ~(bitn)
#define P3n_push_pull(bitn)			P3M1 &= ~(bitn),	P3M0 |=  (bitn)
#define P3n_pure_input(bitn)		P3M1 |=  (bitn),	P3M0 &= ~(bitn)
#define P3n_open_drain(bitn)		P3M1 |=  (bitn),	P3M0 |=  (bitn)

#define P4n_standard(bitn)			P4M1 &= ~(bitn),	P4M0 &= ~(bitn)
#define P4n_push_pull(bitn)			P4M1 &= ~(bitn),	P4M0 |=  (bitn)
#define P4n_pure_input(bitn)		P4M1 |=  (bitn),	P4M0 &= ~(bitn)
#define P4n_open_drain(bitn)		P4M1 |=  (bitn),	P4M0 |=  (bitn)

#define P5n_standard(bitn)			P5M1 &= ~(bitn),	P5M0 &= ~(bitn)
#define P5n_push_pull(bitn)			P5M1 &= ~(bitn),	P5M0 |=  (bitn)
#define P5n_pure_input(bitn)		P5M1 |=  (bitn),	P5M0 &= ~(bitn)
#define P5n_open_drain(bitn)		P5M1 |=  (bitn),	P5M0 |=  (bitn)

#define P6n_standard(bitn)			P6M1 &= ~(bitn),	P6M0 &= ~(bitn)
#define P6n_push_pull(bitn)			P6M1 &= ~(bitn),	P6M0 |=  (bitn)
#define P6n_pure_input(bitn)		P6M1 |=  (bitn),	P6M0 &= ~(bitn)
#define P6n_open_drain(bitn)		P6M1 |=  (bitn),	P6M0 |=  (bitn)

#define P7n_standard(bitn)			P7M1 &= ~(bitn),	P7M0 &= ~(bitn)
#define P7n_push_pull(bitn)			P7M1 &= ~(bitn),	P7M0 |=  (bitn)
#define P7n_pure_input(bitn)		P7M1 |=  (bitn),	P7M0 &= ~(bitn)
#define P7n_open_drain(bitn)		P7M1 |=  (bitn),	P7M0 |=  (bitn)


/****************************************************************/


//sfr INT_CLKO = 0x8F;	//���ӵ� SFR WAKE_CLKO (��ַ��0x8F)
/*
    7   6    5    4   3     2        1       0         Reset Value
    -  EX4  EX3  EX2  -   T2CLKO   T1CLKO  T0CLKO      0000,0000B
b6 -  EX4      : ���ж�INT4����
b5 -  EX3      : ���ж�INT3����
b4 -  EX2      : ���ж�INT2����
b2 - T1CLKO    : ���� T2 ���������P3.0�������Fck1 = 1/2 T1 �����
b1 - T1CLKO    : ���� T1 ���������P3.4�������Fck1 = 1/2 T1 �����
b0 - T0CLKO    : ���� T0 ���������P3.5�������Fck0 = 1/2 T0 �����
*/

#define		LVD_InterruptEnable()		ELVD = 1
#define		LVD_InterruptDisable()		ELVD = 0


//sfr WKTCL = 0xAA;	//STC11F\10��STC15ϵ�� ���Ѷ�ʱ�����ֽ�
//sfr WKTCH = 0xAB;	//STC11F\10��STC15ϵ�� ���Ѷ�ʱ�����ֽ�
//	B7		B6	B5	B4	B3	B2	B1	B0		B7	B6	B5	B4	B3	B2	B1	B0
//	WKTEN				S11	S10	S9	S8		S7	S6	S5	S4	S3	S2	S1	S0	n * 560us
#define		WakeTimerDisable()		WKTCH &= 0x7f	/* WKTEN = 0		��ֹ˯�߻��Ѷ�ʱ�� */
#define		WakeTimerSet(scale)		WKTCL = (scale) % 256,WKTCH = (scale) / 256 | 0x80	/* WKTEN = 1	����˯�߻��Ѷ�ʱ�� */



/*   interrupt vector */
#define		INT0_VECTOR		0
#define		TIMER0_VECTOR	1
#define		INT1_VECTOR		2
#define		TIMER1_VECTOR	3
#define		UART1_VECTOR	4
#define		ADC_VECTOR		5
#define		LVD_VECTOR		6
#define		PCA_VECTOR		7
#define		UART2_VECTOR	8
#define		SPI_VECTOR		9
#define		INT2_VECTOR		10
#define		INT3_VECTOR		11
#define		TIMER2_VECTOR	12
#define		INT4_VECTOR		16
#define		UART3_VECTOR	17
#define		UART4_VECTOR	18
#define		TIMER3_VECTOR	19
#define		TIMER4_VECTOR	20
#define		CMP_VECTOR		21
#define		I2C_VECTOR		24
#define		USB_VECTOR		25
#define		PWMA_VECTOR		26	/* �߼�PWMA */
#define		PWMB_VECTOR		27	/* �߼�PWMB */
#define		TKSU_VECTOR		35	/* ���������ж� */
#define		RTC_VECTOR		36	/* RTC�ж� */
#define		P0INT_VECTOR	37	/* P0.0~P0.7�ж� */
#define		P1INT_VECTOR	38	/* P1.0~P1.7�ж� */
#define		P2INT_VECTOR	39	/* P2.0~P2.7�ж� */
#define		P3INT_VECTOR	40	/* P3.0~P3.7�ж� */
#define		P4INT_VECTOR	41	/* P4.0~P4.7�ж� */
#define		P5INT_VECTOR	42	/* P5.0~P5.4�ж� */
#define		P6INT_VECTOR	43	/* P6.0~P6.7�ж� */
#define		P7INT_VECTOR	44	/* P7.0~P7.7�ж� */

//=============================================================

#define	TRUE	1
#define	FALSE	0

#define	PolityLow			0	//�����ȼ��ж�
#define	PolityHigh			1	//�����ȼ��ж�

#define		ENABLE		1
#define		DISABLE		0

#endif
