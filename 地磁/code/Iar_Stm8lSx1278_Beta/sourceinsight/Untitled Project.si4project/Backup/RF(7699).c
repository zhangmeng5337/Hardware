#include "stm8l15x.h"
#include "sysclock.h"
#include "uart1.h"
#include "stm8l15x_clk.h"
#include "sx1276.h"
#include "stm8l15x_spi.h"
#include "bsp.h"
#include "stm8l15x_it.h"
#include "stm8l15x_tim3.h"
#include "stm8l15x_rtc.h"
#include "stm8l15x_flash.h"
#include "RF.h"
#include "stm8l15x_flash.h"
Module_mode_stru Module_mode;
Module_Params_stru Module_Params;
//Flash 存储
unsigned char Flash_Data[5] = {0};
unsigned char Flash_Write_Done = 1;
//stm8控制线以及工作状态线
unsigned char LoraM0Flag = 0;
unsigned char LoraM1Flag = 0;
unsigned char AUX = 0;
/*参数1*/
unsigned char ADDH = 0x00;
/*参数2*/
unsigned char ADDL = 0x00;
/*参数3*/
u32 SerialRate = 9600;
unsigned char AirRate = 2;   //空口速度 默认2.4k
/*参数4*/
unsigned char Channel = 0x17; //默认通道为23通道 433MHZ
/*参数5*/
unsigned char TranMode = 0x01;
unsigned int WakeupTime = 250;

unsigned int pream_long[ ] = {7, 30, 61, 244, 488, 976};

u8 CheckBit = 0;
u8 CurrentMode = 0;
u8 LastMode = 0;
u8 ConfigureDone = 0;
POWER_MODE_Stru POWER_MODE;
uint32_t rx_length;  //payload count in debug mode
INT8U recv_buffer[128]={ 0 }; //receive data buffer
extern unsigned char AirRate ;   //空口速度 默认2.4k
/*参数4*/
extern unsigned char Channel ; //默认通道为23通道 433MHZ
void RF_Initial( )
{
    FLASH_SetProgrammingTime(FLASH_ProgramTime_TProg);  //Flash 初始化
    
    if(FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 5)) == 1)  //判断Flash中是否有数据
    {
        Flash_Data[0] = FLASH_ReadByte(FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS);
        Flash_Data[1] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 1));
        Flash_Data[2] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 2));
        Flash_Data[3] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 3));
        Flash_Data[4] = FLASH_ReadByte((FLASH_DATA_EEPROM_START_PHYSICAL_ADDRESS + 4));
        ADDH = Flash_Data[0];
                                    
        ADDL = Flash_Data[1];

        if (Flash_Data[2] >> 6 == 0x01)
            CheckBit = 0x06;
        else if (Flash_Data[2] >> 6 == 0x02)
            CheckBit = 0x04;
        else CheckBit = 0x00;
        if (((Flash_Data[2]>>3) & 0x7) < 6)
            SerialRate = 1200 * (unsigned int)(pow(2, ((Flash_Data[2]>>3) & 0x7)));
        else if (((Flash_Data[2]>>3) & 0x7) == 6)
            SerialRate = 57600;
        else SerialRate = 115200;
        
        AirRate = Flash_Data[2]& 0x7;
                                    
        Channel =  Flash_Data[3]&0x1F;

        TranMode = Flash_Data[4]>>7;
        WakeupTime = 250*(((Flash_Data[4]>>3) & 0x7) + 1);
    }  
  
    SX1276_Init(MODE_LORA);         //SX127X 模块复位并初始化为LoRa模式
    SX1276_LoRa_SetDataRate(AirRate);
    SX1276_SetFreq(Channel);				//配置信道为23，即433Hz
    SX1276_SetPower(15, 1);         //配置RFO PIN输出，功率20dBm
    SX1276_SetRxMode();  
}

POWER_MODE_Stru GetPowerMode()
{
  return POWER_MODE;
}

void lora_rx_process()

{
  if(GetPowerMode().status  == 1)
  {
    POWER_MODE.length = SX1276_ReceivePacket(recv_buffer);			 // 妫�娴嬫槸鍚︽敹鍒颁竴涓暟鎹寘   
    if( POWER_MODE.length )
    {
      #if debug == 1
        POWER_MODE.RxLengthSum =  POWER_MODE.RxLengthSum +  POWER_MODE.length +4;
      #endif
      POWER_MODE.length_tmp = POWER_MODE.length+POWER_MODE.length_tmp;
      POWER_MODE.length = 0; 
    }  
    
  }
  
}

