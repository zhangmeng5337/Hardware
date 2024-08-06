#ifndef MODBUS_PARA_H
#define MODBUS_PARA_H
#include "main.h"
/* ˽�к���ԭ�� --------------------------------------------------------------*/`
// CRCУ�����
static uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _RegNum);
// �жϲ����������Ƿ����Э�鷶Χ
static uint8_t MB_JudgeNum(uint16_t _RegNum,uint8_t _FunCode,uint16_t _ByteNum);
// ������
static uint16_t MB_RSP_01H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t _CoilNum );
static uint16_t MB_RSP_02H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t _CoilNum);
static uint8_t MB_RSP_03H(uint16_t _TxCount,uint16_t *_AddrOffset,uint16_t _RegNum );
static uint8_t MB_RSP_04H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _RegNum );
static uint8_t MB_RSP_05H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t _RegDATA);
static uint8_t MB_RSP_06H(uint16_t _TxCount,uint16_t _AddrOffset ,uint16_t _RegNum ,uint16_t *_AddrAbs);
static uint8_t MB_RSP_10H(uint16_t _TxCount,uint16_t _AddrOffset,uint16_t _RegNum ,uint16_t *_AddrAbs ,uint8_t* _Datebuf);```
// ȫ�ֱ����ͽṹ�Ѷ���
extern uint8_t Rx_Buf[256]; // ���ջ�����
extern uint8_t Tx_Buf[256]; // ���ͻ�����
extern PDUData_TypeDef PduData; // Modbus PDU ���ݽṹ

#define MB_SLAVEADDR 		0x01 // ��վ��ַ
#define FUN_CODE_03H            0x03  //������03H

#define IS_NOT_FUNCODE(code)  (!((code == FUN_CODE_01H)||\
			                     (code == FUN_CODE_02H)||\
                       		  (code == FUN_CODE_03H)||\
                                 (code == FUN_CODE_04H)||\
// ... ������Ҫ�Ķ��� ...

/* �������� ------------------------------------------------------------------*/
// CRCУ�����
uint16_t CRC16_MODBUS(uint8_t *_pbuf, uint8_t _uslen);
// ��ȡ����֡�����н�������֡
void MB_Parse_Data(void);
// �Խ��յ�����Ϣ���з�����ִ��
uint8_t MB_Analyze_Execute(void );
// �жϲ����������Ƿ����Э�鷶Χ
uint8_t MB_JudgeNum(uint16_t _Num,uint8_t _FunCode,uint16_t ByteNum);
// �жϵ�ַ�Ƿ����Э�鷶Χ
uint8_t MB_JudgeAddr(uint16_t _Addr,uint16_t _Num);
// �쳣��Ӧ
void MB_Exception_RSP(uint8_t _FunCode,uint8_t _ExCode);
// ������Ӧ
void MB_RSP(uint8_t _FunCode);

#endif
