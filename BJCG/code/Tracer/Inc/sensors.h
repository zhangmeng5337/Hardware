#ifndef SENSOR_H
#define SENSOR_H
#define GPS_COUNT       60
#define SENSORS_COUNT   12
#define SUM_COUNT   SENSORS_COUNT*4 + GPS_COUNT
#define PAYLOAD_COUNT	9
#define SN		SENSORS_COUNT
void sensors_Init(void);
void snesors_process(void);
float *GetSensorsData(void);
unsigned char *getGPS(void);//获取gps位置信息

void SensorsPowerDown(void);

#endif

