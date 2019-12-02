

#include "stm8l15x_flash.h"
#include "sx1276.h"
#include "bsp.h"
#include "stm8l15x_it.h"

#include "math.h"
#include "RF.h"


#define TX              1       // 发送模式
#define RX              0       // 接收模式

//参数配置

//extern u8 CheckBit;
////Flash 存储
//extern unsigned char Flash_Data[5] ;
//extern unsigned char Flash_Write_Done;
////stm8控制线以及工作状态线
//extern unsigned char LoraM0Flag;
//extern unsigned char LoraM1Flag;
//extern unsigned char AUX ;
///*参数1*/
//extern unsigned char ADDH ;
///*参数2*/
//extern unsigned char ADDL;
///*参数3*/
//extern u32 SerialRate ;
//extern unsigned char AirRate ;   //空口速度 默认2.4k
//
//extern u8 ConfigureDone ;
//extern u8 CurrentMode ;
extern unsigned int pream_long[6] ;
//
//extern u8 LastMode ;
///*参数4*/
//extern unsigned char Channel ; //默认通道为23通道 433MHZ
///*参数5*/
//extern  unsigned char TranMode;
//extern  unsigned int WakeupTime ;

//#define ACK_LENGTH      10      // 应答信号长度
//#define SEND_LENGTH     10      // 发送数据每包的长度
//INT8U   SendBuffer[SEND_LENGTH] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//INT8U   SendFlag = 1;           // =1，发送无线数据，=0不处理
//INT16U  SendCnt = 0;            // 计数发送的数据包数
//INT16U  SendTime = 1;           // 计数数据发送间隔时间
//INT16U  RecvWaitTime = 0;       // 接收等待时间
//#define RECV_TIMEOUT    800     // 接收超时
////需要应答的数据
//INT8U   AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
//INT16U  RecvCnt = 0;            // 计数接收的数据包数





void DelayMs(INT8U x)
{
  unsigned int i;
  i=1000;
  for(;x>0;x--)
    for(i=1000;i>0;i--)     
      ;
}





void delay_ms(u16 num)//不是很精确
{
  u8 i = 0;
  while(num--)
  {
    for (i=0; i<100; i++);
  }
}






extern unsigned char  ExitInterFlag ;
int error_temp = 0;
void main(void)
{

  HardwareInit();
  RF_Initial( );
  while(1)
  {
   // if(ExitInterFlag == 0)
    //{SX1276_SendPacket(UsartReceiveData, 3);
   // SX1276_SetPreambleSize((pream_long[GetModuleParams()->AirRate]*(GetModuleParams()->WakeupTime/250)));   //前导码为972 BW 500kHZ SF为7 则空中传输时间250ms
   // SX1276_SetRxMode();      
    //moduleconfig();
   lora_process();
   // }
    //SX1276_SetRxMode(); 
  }
}
