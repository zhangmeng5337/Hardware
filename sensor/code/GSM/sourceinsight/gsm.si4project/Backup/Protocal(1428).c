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
  Data_usr.id[0] = 0x01;  //id
  Data_usr.id[1] = 0x02;	//id
  Data_usr.len = 0x07;		//data pack length not include header and id
  Data_usr.checksum = 0x00;
  Data_usr.deepth [0] = 0x01;
  Data_usr.deepth[1] = 0x02;	
  Data_usr.deepth_percent = 0;
  Data_usr.vbat[0] = 0;
  Data_usr.vbat[1] = 0;
  Data_usr.status = 0;
  Data_usr.deepth_calibration = 1;
  Data_usr.Warn_Thres = 0x25;
}
unsigned char xorCheck(unsigned char *pbuffer,unsigned char len)
{
    unsigned char result;
    unsigned char i;
	result = 0;
   for(i=0;i<len;i++)
   {
	result = result^pbuffer[i];
   }
  return result;
}
  uint32_t adc_tmp;
unsigned char p[16];
void module_process()
{
 
  unsigned char len,i;
i=0;
  //p=malloc(12);
  if(Get_Network_status()==SIMCOM_NET_OK)
  {
    adc_tmp = (adcGet(ADC_BAT_CHANNEL));
    Data_usr.vbat[0] = (unsigned char)(adc_tmp/1000/1000)*2;
    Data_usr.vbat[1] = (unsigned char)(adc_tmp/1000/100%10)*2;
     adc_tmp = (adcGet(ADC_SENSOR_CHANNEL));
	 if(adc_tmp>Data_usr.Warn_Thres)
	 	Data_usr.status = 1;
	 else
	 	Data_usr.status = 0;
    Data_usr.deepth[0] = (unsigned char)(adc_tmp/Data_usr.deepth_calibration );
    Data_usr.deepth[0] = (unsigned char)(adc_tmp/Data_usr.deepth_calibration%256 );
    p[0] = NODE_TO_SERVERH;
    p[1] = NODE_TO_SERVERL;
    len = 2;
    memcpy(p+len,Data_usr.id,2);
    len = len + 2;
    Data_usr.len = 7;
    memcpy(p+len,&Data_usr.len ,1);
      
    

    len = len + 1;
    Data_usr.checksum = 0;
    memcpy(p+len,&Data_usr.checksum ,1);
    len = len + 1;
    memcpy(p+len,Data_usr.deepth  ,2);
    len = len + 2;
    memcpy(p+len,&Data_usr.deepth_percent  ,1);
    len = len + 1;
    memcpy(p+len,Data_usr.vbat  ,2);
    len = len + 2;
    memcpy(p+len,&Data_usr.status  ,1);
    len = len + 1;
	
	Data_usr.checksum = xorCheck(p+2,len-2);
    memcpy(p+CHECKSUM_INDEX,&Data_usr.checksum ,1);

    while(len--)
      UART1_SendByte(p[i++]);
    
  }
  
  free(p);
  
}
