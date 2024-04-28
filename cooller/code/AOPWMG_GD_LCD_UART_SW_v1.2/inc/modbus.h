#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"
#define ORP_ADDR  1
#define PH_ADDR   2


#define MIN_ADDR  ORP_ADDR
#define MAX_ADDR  M4_ADDR



#define M1_ADDR   3
#define M2_ADDR   4
#define M3_ADDR   5
#define M4_ADDR   6



#define UINT_TYPE		0
#define FLOAT_TYPE		1

#define FUNC_WRITE	0x06

#define FUNC_READ	0x03

#define MODBUS_BSIZE	128
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
u16 datcount;
u16 crc;
u8 datType;
u8 ack;
u8 modbus_data[4];//���ջ�����2048�ֽ�
u8 modbus_txdata[32];//���ջ�����2048�ֽ�

}modbus_pack_stru;


void RS485_Init(void);
void RS485_RxCpltCallback(void);
unsigned char  RS485_Service(void);
modbus_pack_stru *GetModbusDat(void);
void Modbus_Pack(modbus_pack_stru p);
modbus_stru *getModbusPayload(void);
void recovery_ORP_PH(unsigned char addr);
void setAddr_ORP_PH(unsigned char addr);
void Modbus_Pack_cali(modbus_pack_stru p);

#endif

