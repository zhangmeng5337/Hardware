#include "stm8l15x_flash.h"
#include "sx1276.h"
#include "bsp.h"
#include "stm8l15x_it.h"

#include "math.h"
#include "RF.h"


//#define TX              1       // ����ģʽ
//#define RX              0       // ����ģʽ

//��������

//extern u8 CheckBit;
////Flash �洢
//extern unsigned char Flash_Data[5] ;
//extern unsigned char Flash_Write_Done;
////stm8�������Լ�����״̬��
//extern unsigned char LoraM0Flag;
//extern unsigned char LoraM1Flag;
//extern unsigned char AUX ;
///*����1*/
//extern unsigned char ADDH ;
///*����2*/
//extern unsigned char ADDL;
///*����3*/
//extern u32 SerialRate ;
//extern unsigned char AirRate ;   //�տ��ٶ� Ĭ��2.4k
//
//extern u8 ConfigureDone ;
//extern u8 CurrentMode ;
//extern unsigned int pream_long[6] ;
//
//extern u8 LastMode ;
///*����4*/
//extern unsigned char Channel ; //Ĭ��ͨ��Ϊ23ͨ�� 433MHZ
///*����5*/
//extern  unsigned char TranMode;
//extern  unsigned int WakeupTime ;

//#define ACK_LENGTH      10      // Ӧ���źų���
//#define SEND_LENGTH     10      // ��������ÿ���ĳ���
//INT8U   SendBuffer[SEND_LENGTH] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
//INT8U   SendFlag = 1;           // =1�������������ݣ�=0������
//INT16U  SendCnt = 0;            // �������͵����ݰ���
//INT16U  SendTime = 1;           // �������ݷ��ͼ��ʱ��
//INT16U  RecvWaitTime = 0;       // ���յȴ�ʱ��
//#define RECV_TIMEOUT    800     // ���ճ�ʱ
////��ҪӦ�������
//INT8U   AckBuffer[ACK_LENGTH] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19 };
//INT16U  RecvCnt = 0;            // �������յ����ݰ���





void DelayMs(INT8U x)
{
  unsigned int i;
  i=1000;
  for(;x>0;x--)
    for(i=1000;i>0;i--)     
      ;
}










//extern unsigned char  ExitInterFlag ;
//int error_temp = 0;
void main(void)
{
  
   HardwareInit();
  RF_Initial( );
  IWDG_Config();
  while(1)
  {
    // if(ExitInterFlag == 0)
    //{SX1276_SendPacket(UsartReceiveData, 3);
    // SX1276_SetPreambleSize((pream_long[GetModuleParams()->AirRate]*(GetModuleParams()->WakeupTime/250)));   //ǰ����Ϊ972 BW 500kHZ SFΪ7 ����д���ʱ��250ms
    // SX1276_SetRxMode();      
    // moduleconfig();
    //RF_Initial( );
    lora_process();
    IWDG_ReloadCounter();
    //USART_SendData8(USART1, 0x01);
    // }
    //SX1276_SetRxMode(); 
  }
}