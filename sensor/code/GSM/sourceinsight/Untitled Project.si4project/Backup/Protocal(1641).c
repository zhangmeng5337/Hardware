#include "Protocal.h"
#include "GSM.h"
#include "stdlib.h"
#include "bsp.h"
Data_Stru Data_usr;
//typedef struct{
//  unsigned char id[2];
//
//  unsigned char len;
//  unsigned char checksum;
//  unsigned char deepth[2];
//  unsigned char deepth_percent;
//  unsigned char vbat[2];
//  unsigned char status;
//  unsigned int deepth_calibration;
//}Data_Stru;

void module_prams_init()
{
  Data_usr.id[0] = 0x01;
  Data_usr.id[1] = 0x02;	
  Data_usr.len = 0x07;		
  Data_usr.checksum = 0x00;
  Data_usr.deepth [0] = 0x01;
  Data_usr.deepth[1] = 0x02;	
  Data_usr.deepth_percent = 0;
  Data_usr.vbat[0] = 0;
  Data_usr.vbat[1] = 0;
  Data_usr.status = 0;
  Data_usr.deepth_calibration = 1;
}
void module_process()
{
  unsigned char *p;
  unsigned char len;
  uint32_t adc_tmp;
  p=malloc(12);
  if(Get_Network_status()==SIMCOM_NET_OK)
  {
    adc_tmp = (adcGet(ADC_BAT_CHANNEL));
    Data_usr.vbat[0] = (unsigned char)(adc_tmp/1000/100);
    Data_usr.vbat[0] = (unsigned char)(adc_tmp/1000/100);
     adc_tmp = (adcGet(ADC_SENSOR_CHANNEL));
    Data_usr.deepth[0] = (unsigned char)(adc_tmp/Data_usr.deepth_calibration );
    Data_usr.deepth[0] = (unsigned char)(adc_tmp/Data_usr.deepth_calibration%256 );
    *p = NODE_TO_SERVERH;
    *(p+1) = NODE_TO_SERVERL;
    len = 2;
    Data_usr.len = 7;
    memcopy(p+len,Data_usr.id,2);
    len = len + 2;
    memcopy(p+len,Data_usr.len ,1);
    len = len + 1;
    memcopy(p+len,Data_usr.checksum ,1);
    len = len + 1;
    memcopy(p+len,Data_usr.deepth  ,2);
    len = len + 2;
    memcopy(p+len,Data_usr.deepth_percent  ,1);
    len = len + 1;
    memcopy(p+len,Data_usr.vbat  ,2);
    len = len + 2;
    memcopy(p+len,Data_usr.status  ,1);
    len = len + 1;
    while(len--)
      UART1_SendByte(*p++);
    
  }
  
  free(p);
  
}
