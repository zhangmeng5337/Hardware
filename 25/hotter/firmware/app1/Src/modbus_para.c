#include "modbus_para.h"
/**
 * 函数功能：提取数据帧，进行解析数据帧
 * 输入参数：无
 * 返 回 值：无
 * 说    明：无
 */
void MB_Parse_Data()
{
  PduData.Code = Rx_Buf[1];                                         // 功能码
  PduData.Addr = ((Rx_Buf[2] << 8) | Rx_Buf[3]);                    // 寄存器起始地址
  PduData.Num = ((Rx_Buf[4] << 8) | Rx_Buf[5]);                     // 数量(Coil,Input,Holding Reg,Input Reg)
  PduData._CRC = CRC16_MODBUS((uint8_t *)&Rx_Buf, RxCount - 2);     // CRC校验码
  PduData.byteNums = Rx_Buf[6];                                     // 获得字节数
  PduData.ValueReg = (uint8_t *)&Rx_Buf[7];                         // 写多个寄存器起始地址
  PduData.PtrHoldingOffset = PduData.PtrHoldingbase + PduData.Addr; // 保持寄存器的起始地址
}
/**
 * 函数功能：对接收到的信息进行分析并执行
 * 输入参数：无
 * 返 回 值：异常码0x00
 * 说    明：判断功能码，验证地址，数据是否溢出，数据没错误发送响应信号
 */
uint8_t MB_Analyze_Execute(void){
  uint16_t ExCode = EX_CODE_NONE; // 0x00
  /* 校验码功能 */
  if (IS_NOT_FUNCODE(PduData.Code)) // 不支持的功能码
  {
    /* MODBUS异常响应 */
    ExCode = EX_CODE_01H;
    return ExCode;
  }
  /* 根据功能码分别判断 */
  switch (PduData.Code){
    /* ---- 01H  02H 读离散输入寄存器(Coil Input)----------- */
    
    /* ---- 03H  04H 读保持/读输入寄存器---------------------- */
    case FUN_CODE_03H:
    case FUN_CODE_04H:
      /* 判断寄存器数量 */
      ExCode = MB_JudgeNum(PduData.Num, PduData.Code, PduData.byteNums);
      if (ExCode != EX_CODE_NONE)
      {
        return ExCode;
      }
      /* 判断地址*/
      ExCode = MB_JudgeAddr(PduData.Addr, PduData.Num);
      if (ExCode != EX_CODE_NONE)
      {
        return ExCode;
      }
      break;

      /* ---- 05H 写单个线圈---------------------- */

      /* ---- 06H 写单个保持 ---------------------- */
  }
  /* 数据帧没有异常 */
  return ExCode; //   EX_CODE_NONE
}
/**
 * 函数功能：正常响应
 * 输入参数：_FunCode 功能码
 * 返 回 值：无
 * 说    明：当通讯数据帧没有异常时并且成功执行后，发送响应数据帧
 */
void MB_RSP(uint8_t _FunCode)
{
  uint16_t TxCount = 0;
  uint16_t crc = 0;
  Tx_Buf[TxCount++] = MB_SLAVEADDR; /* 从站地址 */
  Tx_Buf[TxCount++] = _FunCode;     /* 功能码   */
  switch (_FunCode)
  {
  case FUN_CODE_01H:
    /* 读取线圈状态 */
    TxCount = MB_RSP_01H(TxCount, PduData.Addr, PduData.Num);
    break;
  case FUN_CODE_02H:
    /* 读取离散输入 */
    TxCount = MB_RSP_02H(TxCount, PduData.Addr, PduData.Num);
    break;
  case FUN_CODE_03H:
    /* 读取保持寄存器 */
    TxCount = MB_RSP_03H(TxCount, (uint16_t *)PduData.PtrHoldingOffset, PduData.Num);
    break;
  case FUN_CODE_04H:
    /* 读取输入寄存器 */
    TxCount = MB_RSP_04H(TxCount, PduData.Addr, PduData.Num);
    break;
  case FUN_CODE_05H:
    /* 写单个线圈 */
    TxCount = MB_RSP_05H(TxCount, PduData.Addr, PduData.Num);
    break;
  case FUN_CODE_06H:
    /* 写单个保持寄存器 */
    TxCount = MB_RSP_06H(TxCount, PduData.Addr, PduData.Num, (uint16_t *)PduData.PtrHoldingOffset);
    break;
  case FUN_CODE_10H:
    /* 写多个保持寄存器 */
    TxCount = MB_RSP_10H(TxCount, PduData.Addr, PduData.Num, (uint16_t *)PduData.PtrHoldingOffset, (uint8_t *)PduData.ValueReg);
    break;
  }
  // 计算CRC校验码
  crc = CRC16_MODBUS((uint8_t *)&Tx_Buf, TxCount);

  // 将CRC校验码添加到发送缓冲区
  Tx_Buf[TxCount++] = crc & 0xFF;      // CRC低字节
  Tx_Buf[TxCount++] = (crc >> 8) & 0xFF; // CRC高字节

  // 发送响应
  UART_Tx((uint8_t *)Tx_Buf, TxCount);
}
void processModbusRequest() {
    // 假设 Rx_Buf 已经接收到数据

    // 解析请求数据
    MB_Parse_Data();

    // 分析请求数据并执行相应操作
    uint8_t ExCode = MB_Analyze_Execute();
    if (ExCode != EX_CODE_NONE) {
        // 如果有异常，可以在这里处理，例如发送异常响应
        // MB_Exception_RSP(PduData.Code, ExCode);
        return;
    }

    // 准备并发送响应数据
    MB_RSP(PduData.Code);
}




