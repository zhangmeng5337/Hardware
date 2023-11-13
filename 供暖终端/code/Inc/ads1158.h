#ifndef _ADS1158_H_
#define _ADS1158_H_
 
#include "main.h"
 #define AI_SIZE   16
typedef  struct
{
	uint8_t data[4];
	unsigned char  chip_No;
	unsigned char ads1_update;
	unsigned char ads2_update;	
	unsigned char ads1_start;
	unsigned char ads2_start;
	uint32_t value;
	uint32_t adsvolt[AI_SIZE];
	float    ads_ma[AI_SIZE];	
	//uint32_t adsvolt2[AI_SIZE];
	uint32_t ADIref;
	uint32_t ADIgain;
	uint32_t ADItemp;
	uint32_t ADIvcc;
	uint32_t ADIoffset;
	float ft;
	float vcc;
	float gain;
	float offset;
	float temp;

    float wendu;	
}structChan;
 



		  
extern uint32_t adsvolt[20];
 
extern structChan channelData[20];

#define SINGLE
#define MV_PER_LSB 0.16276f // 5000/30720 = 0.16276f
#define MVLOT_ACC_MID_THROR 900.0f // Mid Voltage mv
#define MPERS2_MV  0.0980665f // 100mv/1g =  9.80665f m/s2/100mv = 0.0980665f m/s2/mv 
 
#define MVLOT_GYRO_MID_THROR 2500.0f // Mid Voltage mv
#define ANGPERS_MV 0.142857f // 7mv -> 1ang/s = 1/7 = 0.142857f  

 
#define ADS1158_CS1_LOW      HAL_GPIO_WritePin(MIBSPI1MCS1_GPIO_Port, MIBSPI1MCS1_Pin,GPIO_PIN_RESET);
#define ADS1158_CS1_HIGH	 HAL_GPIO_WritePin(MIBSPI1MCS1_GPIO_Port, MIBSPI1MCS1_Pin,GPIO_PIN_SET);

#define ADS1158_CS2_LOW      HAL_GPIO_WritePin(MIBSPI1MCS2_GPIO_Port, MIBSPI1MCS2_Pin,GPIO_PIN_RESET);
#define ADS1158_CS2_HIGH	 HAL_GPIO_WritePin(MIBSPI1MCS2_GPIO_Port, MIBSPI1MCS2_Pin,GPIO_PIN_SET);
 


 
//#define ADS1158_START_ON	 HAL_GPIO_WritePin(MSTART_MCU_GPIO_Port, MSTART_MCU_Pin);
 
#ifdef TEST_BORAD //≤‚ ‘
#define DATA_READY       HAL_GPIO_ReadPin(MDRDY_PT1_MCU_GPIO_Port,MDRDY_PT1_MCU_Pin)
#define DATA_READY2       HAL_GPIO_ReadPin(MDRDY_PT1_MCU_GPIO_Port,MDRDY_MCU_Pin)

#define START_H          HAL_GPIO_WritePin(MSTART_MCU_GPIO_Port,MSTART_MCU_Pin,GPIO_PIN_RESET);
#define START_L          HAL_GPIO_WritePin(MSTART_MCU_GPIO_Port,MSTART_MCU_Pin,GPIO_PIN_SET);
#else
#define DATA_READY       HAL_GPIO_ReadPin(MDRDY_PT1_MCU_GPIO_Port,MDRDY_PT1_MCU_Pin)
#define DATA_READY2       HAL_GPIO_ReadPin(MDRDY_PT1_MCU_GPIO_Port,MDRDY_MCU_Pin)
#define START_H          HAL_GPIO_WritePin(MSTART_MCU_GPIO_Port,MSTART_MCU_Pin,GPIO_PIN_RESET);
#define START_L          HAL_GPIO_WritePin(MSTART_MCU_GPIO_Port,MSTART_MCU_Pin,GPIO_PIN_SET);

#endif
 
 
#define BASE 0.06
 
 
#define SENDDATALEN 1000
 
extern unsigned char ChannelSendData[8]; 
extern unsigned char ChannelData[1000][29];//[19];// [12]; 
extern unsigned int ptrWr;
extern unsigned int ptrRd;
 
extern float ft;
 

float *get_ads_volt(void);
unsigned char REG_readdata_u(unsigned char adc_NO);
void ads1158_init(void);
unsigned char *get_chip_NO(void);
void ads_read_data(void);





#endif

