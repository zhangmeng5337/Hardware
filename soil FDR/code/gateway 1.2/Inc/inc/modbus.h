#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"
#define RH_ADDR	  0x0002
#define TE_ADDR	  0x0003
#define DEV_ADDR	0x0100
#define RATE_ADDR	0x0101

#define MODBUS_BSIZE	1024
typedef struct{
u32 RS485_Baudrate;//ͨѶ������
u8 RS485_Parity;//0��У�飻1��У�飻2żУ��
u8 RS485_Addr;//�ӻ���ַ
u16 RS485_Frame_Distance;//����֡��С�����ms),������ʱ������Ϊ����һ֡
u8  RS485_TX_EN;
u8 RS485_RX_BUFF[MODBUS_BSIZE];//���ջ�����2048�ֽ�
u16 RS485_RX_CNT;//���ռ�����
u8 RS485_FrameFlag;//֡�������
u8 RS485_TX_BUFF[MODBUS_BSIZE];//���ͻ�����
u16 RS485_TX_CNT;//���ͼ�����
}modbus_stru;
typedef struct{

u8 RS485_Addr;//�ӻ���ַ
u8 func;
u16 startaddr;
u16 regnum;
u16 crc;
}modbus_pack_stru;
void Modbus_Pack(modbus_pack_stru p);
modbus_stru *modbusRxResult(void);
void RS485_RxCpltCallback(void);

void RS485_Init(void);
void RS485_Service(void);
void timCallback(void);
void sensorModbusRate(unsigned int p,unsigned char num);
unsigned char *modbusBuffer(void);
unsigned char *modbusFrameStatus(void);
void RS485_SendData(u8 *buff,u8 len,u8 flag);

#endif
