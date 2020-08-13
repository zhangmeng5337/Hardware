#ifndef SENSOR_H
#define SENSOR_H
#define COLLECT_DAT_SIZE                11
#define HEAD_SIZE                       2
#define ID_SIZE                         11
#define VER_DEV_SIZE                    11
#define SESSION_SIZE                    11
#define SUB_DAT_SIZE                    11
#define IMEI_SIZE                       11
#define ICCID_SIZE                      11
#define CSQ_SIZE                        11
#define LONG_SIZE                       11
#define LAT_SIZE                        11
#define VBAT_SIZE                       11
#define RSI_SIZE                        11
typedef struct{
  float air_humid;
  float air_temper;
  float soil_tempre;
  float soil_humid;
  float soil_ph;
  float sunshine;
  unsigned char Collect_Date[11];
  unsigned char error_staus;
  unsigned int amount;
  unsigned char header[12];
  float version;
  unsigned char id[12];
  unsigned char ver_dev[12];
  unsigned char session[64];
  unsigned char Sub_Date[11];
  unsigned char imei[12];
  unsigned char iccid[12];
  unsigned char csq[12]; 
  unsigned char longitude[12]; 
  unsigned char latitude[12]; 
  unsigned char battery[2]; 
  unsigned char rssi[2]; 
}Sensor_Stru;
#endif
