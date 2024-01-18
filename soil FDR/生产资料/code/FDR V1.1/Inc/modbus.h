#ifndef MODBUS_H
#define MODBUS_H
#include "main.h"
#define ROLE  1    //1:为校准程序 0为标准程序
#define RH_ADDR	  0x0002
#define TE_ADDR	  0x0003
#define DEV_ADDR	0x0100
#define RATE_ADDR	0x0101

#define CALB_ADDR		0xfe
#define MODBUS_BSIZE	512
typedef struct{
u32 RS485_Baudrate;//~{M(Q62(LXBJ~}
u8 RS485_Parity;//0~{N^P#Qi#;~}1~{FfP#Qi#;~}2~{E<P#Qi~}
u8 RS485_Addr;//~{4S;z5XV7~}
u16 RS485_Frame_Distance;//~{J}>]V!WnP!<d8t#(~}ms),~{3,9}4KJ1<dTrHON*JGOBR;V!~}
u8  RS485_TX_EN;
u8 RS485_RX_BUFF[MODBUS_BSIZE];//~{=SJU;:3eGx~}2048~{WV=Z~}
u16 RS485_RX_CNT;//~{=SJU<FJ}Fw~}
u8 RS485_FrameFlag;//~{V!=aJx1j<G~}
u8 RS485_TX_BUFF[MODBUS_BSIZE];//~{7"KM;:3eGx~}
u16 RS485_TX_CNT;//~{7"KM<FJ}Fw~}
}modbus_stru;


void RS485_Init(void);
void RS485_Service(void);
void timCallback(void);
#endif
