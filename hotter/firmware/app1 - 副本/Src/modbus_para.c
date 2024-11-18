#include "modbus_para.h"
/**
 * �������ܣ���ȡ����֡�����н�������֡
 * �����������
 * �� �� ֵ����
 * ˵    ������
 */
void MB_Parse_Data()
{
  PduData.Code = Rx_Buf[1];                                         // ������
  PduData.Addr = ((Rx_Buf[2] << 8) | Rx_Buf[3]);                    // �Ĵ�����ʼ��ַ
  PduData.Num = ((Rx_Buf[4] << 8) | Rx_Buf[5]);                     // ����(Coil,Input,Holding Reg,Input Reg)
  PduData._CRC = CRC16_MODBUS((uint8_t *)&Rx_Buf, RxCount - 2);     // CRCУ����
  PduData.byteNums = Rx_Buf[6];                                     // ����ֽ���
  PduData.ValueReg = (uint8_t *)&Rx_Buf[7];                         // д����Ĵ�����ʼ��ַ
  PduData.PtrHoldingOffset = PduData.PtrHoldingbase + PduData.Addr; // ���ּĴ�������ʼ��ַ
}
/**
 * �������ܣ��Խ��յ�����Ϣ���з�����ִ��
 * �����������
 * �� �� ֵ���쳣��0x00
 * ˵    �����жϹ����룬��֤��ַ�������Ƿ����������û��������Ӧ�ź�
 */
uint8_t MB_Analyze_Execute(void){
  uint16_t ExCode = EX_CODE_NONE; // 0x00
  /* У���빦�� */
  if (IS_NOT_FUNCODE(PduData.Code)) // ��֧�ֵĹ�����
  {
    /* MODBUS�쳣��Ӧ */
    ExCode = EX_CODE_01H;
    return ExCode;
  }
  /* ���ݹ�����ֱ��ж� */
  switch (PduData.Code){
    /* ---- 01H  02H ����ɢ����Ĵ���(Coil Input)----------- */
    
    /* ---- 03H  04H ������/������Ĵ���---------------------- */
    case FUN_CODE_03H:
    case FUN_CODE_04H:
      /* �жϼĴ������� */
      ExCode = MB_JudgeNum(PduData.Num, PduData.Code, PduData.byteNums);
      if (ExCode != EX_CODE_NONE)
      {
        return ExCode;
      }
      /* �жϵ�ַ*/
      ExCode = MB_JudgeAddr(PduData.Addr, PduData.Num);
      if (ExCode != EX_CODE_NONE)
      {
        return ExCode;
      }
      break;

      /* ---- 05H д������Ȧ---------------------- */

      /* ---- 06H д�������� ---------------------- */
  }
  /* ����֡û���쳣 */
  return ExCode; //   EX_CODE_NONE
}
/**
 * �������ܣ�������Ӧ
 * ���������_FunCode ������
 * �� �� ֵ����
 * ˵    ������ͨѶ����֡û���쳣ʱ���ҳɹ�ִ�к󣬷�����Ӧ����֡
 */
void MB_RSP(uint8_t _FunCode)
{
  uint16_t TxCount = 0;
  uint16_t crc = 0;
  Tx_Buf[TxCount++] = MB_SLAVEADDR; /* ��վ��ַ */
  Tx_Buf[TxCount++] = _FunCode;     /* ������   */
  switch (_FunCode)
  {
  case FUN_CODE_01H:
    /* ��ȡ��Ȧ״̬ */
    TxCount = MB_RSP_01H(TxCount, PduData.Addr, PduData.Num);
    break;
  case FUN_CODE_02H:
    /* ��ȡ��ɢ���� */
    TxCount = MB_RSP_02H(TxCount, PduData.Addr, PduData.Num);
    break;
  case FUN_CODE_03H:
    /* ��ȡ���ּĴ��� */
    TxCount = MB_RSP_03H(TxCount, (uint16_t *)PduData.PtrHoldingOffset, PduData.Num);
    break;
  case FUN_CODE_04H:
    /* ��ȡ����Ĵ��� */
    TxCount = MB_RSP_04H(TxCount, PduData.Addr, PduData.Num);
    break;
  case FUN_CODE_05H:
    /* д������Ȧ */
    TxCount = MB_RSP_05H(TxCount, PduData.Addr, PduData.Num);
    break;
  case FUN_CODE_06H:
    /* д�������ּĴ��� */
    TxCount = MB_RSP_06H(TxCount, PduData.Addr, PduData.Num, (uint16_t *)PduData.PtrHoldingOffset);
    break;
  case FUN_CODE_10H:
    /* д������ּĴ��� */
    TxCount = MB_RSP_10H(TxCount, PduData.Addr, PduData.Num, (uint16_t *)PduData.PtrHoldingOffset, (uint8_t *)PduData.ValueReg);
    break;
  }
  // ����CRCУ����
  crc = CRC16_MODBUS((uint8_t *)&Tx_Buf, TxCount);

  // ��CRCУ������ӵ����ͻ�����
  Tx_Buf[TxCount++] = crc & 0xFF;      // CRC���ֽ�
  Tx_Buf[TxCount++] = (crc >> 8) & 0xFF; // CRC���ֽ�

  // ������Ӧ
  UART_Tx((uint8_t *)Tx_Buf, TxCount);
}
void processModbusRequest() {
    // ���� Rx_Buf �Ѿ����յ�����

    // ������������
    MB_Parse_Data();

    // �����������ݲ�ִ����Ӧ����
    uint8_t ExCode = MB_Analyze_Execute();
    if (ExCode != EX_CODE_NONE) {
        // ������쳣�����������ﴦ�����緢���쳣��Ӧ
        // MB_Exception_RSP(PduData.Code, ExCode);
        return;
    }

    // ׼����������Ӧ����
    MB_RSP(PduData.Code);
}




