#ifndef MODBUS_PARA_H
#define MODBUS_PARA_H
#include "main.h"
/* 私有函数原形 --------------------------------------------------------------*/`
// CRC校验计算
static uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _RegNum);
// 判断操作的数量是否符合协议范围
static uint8_t MB_JudgeNum(uint16_t _RegNum,uint8_t _FunCode,uint16_t _ByteNum);
// 功能码
static uint16_t MB_RSP_01H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t _CoilNum );
static uint16_t MB_RSP_02H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t _CoilNum);
static uint8_t MB_RSP_03H(uint16_t _TxCount,uint16_t *_AddrOffset,uint16_t _RegNum );
static uint8_t MB_RSP_04H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _RegNum );
static uint8_t MB_RSP_05H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t _RegDATA);
static uint8_t MB_RSP_06H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t _RegNum ,uint16_t *_AddrAbs);
static uint8_t MB_RSP_10H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _RegNum ,uint16_t *_AddrAbs ,uint8_t* _Datebuf);```
// 全局变量和结构已定义
extern uint8_t Rx_Buf[256]; // 接收缓冲区
extern uint8_t Tx_Buf[256]; // 发送缓冲区
extern PDUData_TypeDef PduData; // Modbus PDU 数据结构

#define MB_SLAVEADDR 		0x01 // 从站地址
#define FUN_CODE_03H            0x03  //功能码03H

#define IS_NOT_FUNCODE(code)  (!((code == FUN_CODE_01H)||\
			                     (code == FUN_CODE_02H)||\
                       		  (code == FUN_CODE_03H)||\
                                 (code == FUN_CODE_04H)||\
// ... 其他必要的定义 ...

/* 函数声明 ------------------------------------------------------------------*/
// CRC校验计算
uint16_t CRC16_MODBUS(uint8_t *_pbuf, uint8_t _uslen);
// 提取数据帧，进行解析数据帧
void MB_Parse_Data(void);
// 对接收到的信息进行分析并执行
uint8_t MB_Analyze_Execute(void );
// 判断操作的数量是否符合协议范围
uint8_t MB_JudgeNum(uint16_t _Num,uint8_t _FunCode,uint16_t ByteNum);
// 判断地址是否符合协议范围
uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _Num);
// 异常响应
void MB_Exception_RSP(uint8_t _FunCode,uint8_t _ExCode);
// 正常响应
void MB_RSP(uint8_t _FunCode);

#endif
