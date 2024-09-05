#include "lt2470.h"
extern SPI_HandleTypeDef hspi2;


ai_stru ai_usr;
void ltc2470_init()
{
   
}
void ltc2470_TransmitReceive(uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
//		pRxData[i] = SPI1_ReadWriteByte(pTxData[i+1]);
    HAL_SPI_TransmitReceive(&hspi2, pTxData, pRxData,Size, 2000);

}

void spiSendReceiveArrays(uint8_t DataTx[], uint8_t DataRx[], uint8_t byteLength)
{
    
    ltc2470_TransmitReceive(DataTx, DataRx, byteLength);

}
unsigned char ai_read()
{
	uint8_t DataTx[2];
	uint8_t DataRx[2];
	unsigned int adc_result;
	unsigned char result;
	result = 0;
    DataTx[0] =  EN1|EN2|SPD_RATE1|NAP_MODE;
    DataTx[1] =  0;  
	spiSendReceiveArrays(DataTx, DataRx, 2);
	adc_result = DataRx[0];
	adc_result = adc_result <<8;
	adc_result = adc_result | DataRx[1];

	if(adc_result >=65534)
		result = 1;//over range high
	else if(adc_result >=65535)
		result = 2;//over range medi
	else 
	{
		result = 3;
		ai_usr.adc_value = adc_result;
		ai_usr.current = adc_result/65535.0;
		ai_usr.current = ai_usr.current*VREF;
		ai_usr.current = ai_usr.current/VR;	
	}
	return result;

}

