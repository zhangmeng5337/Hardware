#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"
#define RH_ADDR	  0x0002
#define TE_ADDR	  0x0003
#define DEV_ADDR	0x0100
#define RATE_ADDR	0x0101

#define MODBUS_BSIZE	1024
typedef struct{
u32 RS485_Baudrate;//通讯波特率
u8 RS485_Parity;//0无校验；1奇校验；2偶校验
u8 RS485_Addr;//从机地址
u16 RS485_Frame_Distance;//数据帧最小间隔（ms),超过此时间则认为是下一帧
u8  RS485_TX_EN;
u8 RS485_RX_BUFF[MODBUS_BSIZE];//接收缓冲区2048字节
u16 RS485_RX_CNT;//接收计数器
u8 RS485_FrameFlag;//帧结束标记
u8 RS485_TX_BUFF[MODBUS_BSIZE];//发送缓冲区
u16 RS485_TX_CNT;//发送计数器
}modbus_stru;

void RS485_Init(void);
void RS485_Service(void);
void timCallback(void);
#endif
