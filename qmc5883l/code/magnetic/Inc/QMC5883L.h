#ifndef QMC5883L_H
#define QMC5883L_H
#define	QMC5883_ADDR   0x1a	 //定义器件在IIC总线中的从地址  read

#define CALIBTATE_DATA_NUM    10
typedef unsigned char u8;
void QMC5883_GetData(float *Magnet);
uint8_t QMC5883_InitConfig(void);
#endif
