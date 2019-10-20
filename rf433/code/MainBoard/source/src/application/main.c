#include "bsp_uart.h"
#include "bsp_spi.h"
#include "key.h"
#include "main.h"
#include "eeprom.h"
#include <CMT2300drive.h>
#include "uartParse.h"
#include "bsp_i2c_ee.h"
#include "command.h"
static boolean_t statetx = TRUE;  //  false为RX  true为TX
/**/	extern boolean_t  FixedPktLength;						//false: for contain packet length in Tx message, the same mean with variable lenth
//true : for doesn't include packet length in Tx message, the same mean with fixed length
/**/	extern word  PayloadLength;
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

//void Device_Init(void);
void CLK_Init(void);
void setup_Tx(void);
void setup_Rx(void);
void CMT2300_Init(void);
//void loop_Tx(void);
//void loop_Rx(void);

#define LEN 21

unsigned char str[LEN] = {'H','o','p','e','R','F',' ','R','F','M',' ','C','O','B','R','F','M','3','0','0','A'};
//byte getstr[LEN+1];



/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
* @brief  Main program.
* @param  None
* @retval None
*/


/*****此处通过RFPDK软件导出，然后更改每个数组中的参数 *****/
/************************************************************
频点:  433.92Mhz
速率:  2.4Kpbs
频偏:  +/-10Khz
带宽:  +/-100khz
数据包格式:
0xAAAAAAAAAAAAAAAA + 0x2DD4 +0x15 +"HopeRF RFM COBRFM300A" 

发射功率: 13dBm
**************************************************************/
word CMTBank[12] = {
  0x0000,
  0x0166,
  0x02EC,
  0x031C,
  0x04F0,
  0x0580,
  0x0614,
  0x0708,
  0x0891,
  0x0902,
  0x0A02,
  0x0BD0  //用于校准RSSI误差
};

word SystemBank[12] = {
  0x0CAE,
  0x0DE0,
  0x0E35,
  0x0F00,
  0x1000,
  0x11F4,
  0x1210,
  0x13E2,
  0x1442,
  0x1520,
  0x1600,
  0x1781				  
};

word FrequencyBank[8] = {  //CMT2300 频率设置
  0x1842,
  0x1971,
  0x1ACE,
  0x1B1C,
  0x1C42,
  0x1D5B,
  0x1E1C,
  0x1F1C					
};

word DataRateBank[24] = {
  0x2032,
  0x2118,
  0x2200,
  0x2399,
  0x24E0,
  0x2569,
  0x2619,
  0x2705,
  0x289F,
  0x2939,
  0x2A29,
  0x2B29,
  0x2CC0,
  0x2D51,
  0x2E2A,
  0x2F53,
  0x3000,
  0x3100,
  0x32B4,
  0x3300,
  0x3400,
  0x3501,
  0x3600,
  0x3700
};	   

word BasebandBank[29] = {
  0x3812, // 1byte RX_preamble  单位8bit
  0x3908, // 8byte TX_preamble  
  0x3A00,
  0x3BAA, //报文头内容为AA
  0x3C02,//同步字设置为1byte
  0x3D00,
  0x3E00,
  0x3F00,
  0x4000,
  0x4100,
  0x4200,//同步字00
  0x43D4,//同步字D4
  0x442D,//同步字2D
  0x4501, //可变包长
  0x461F, //31byte的负载
  0x4700, //Node ID 检测电路设置
  0x4800, //Node ID 缺省位
  0x4900, //Node ID 缺省位
  0x4A00, //Node ID 缺省位
  0x4B00, //Node ID 缺省位
  0x4C00,//CRC 禁止
  0x4D00,//CRC多项式初始值
  0x4E00,//CRC多项式初始值
  0x4F60, //CRC大小顺序
  0x50FF,//白化多项式使能
  0x5102,//PA打开 定义预发射内容，发1。
  0x5200,//重复发包的次数
  0x531F,//包与包的间隔 单位symbol
  0x5410 //发包完自动清除fifo	
};	

word TXBank[11] = {
  0x5550,
  0x564D,
  0x5706,
  0x5800,
  0x5942,
  0x5AB0,
  0x5B00,
  0x5C37,
  0x5D0A,
  0x5E3F,
  0x5F7F															
};			
extern uint16_t UpdateKeyStatus;
void Gpio_IRQHandler(uint8_t u8Param)
{
    *((uint32_t *)((uint32_t)&M0P_GPIO->P3ICLR + u8Param * 0x40)) = 0;
    
    //SK_LED_SET(0);
    //delay1ms(1000);
   // SK_LED_SET(1);
  //  delay1ms(1000);
}

//void Gpio_IRQHandler(uint8_t u8Param)//key inttrupt call back function
//{
//  uint16_t tInchStatus;
//  tInchStatus=system_params_get()->KEY_H8<<8+system_params_get()->KEY_L8;
//  
//  switch(u8Param)
//  {
//  case 0:
//    {         
//      if(Gpio_GetIrqStat(KEY_DOWN_PORT, KEY_DOWN_PIN))
//      {  
//          UpdateKeyStatus = UpdateKeyStatus |KEY_DOWN_BIT;	
//      }
//      
//      if(Gpio_GetIrqStat(KEY_BREAK_PORT, KEY_BREAK_PIN))
//      {
//          UpdateKeyStatus = UpdateKeyStatus |KEY_BREAK_BIT;					      	
//      }
//      
//      *((uint32_t *)((uint32_t)&M0P_GPIO->P0ICLR + u8Param * 0x40)) = 0;	
//      
//    }break;
//    
//  case 1:
//    { 
//      if(Gpio_GetIrqStat(KEY_STOP_PORT, KEY_STOP_PIN))
//      {
//        UpdateKeyStatus = UpdateKeyStatus |KEY_STOP_BIT;					
//        
//      }
//      
//      *((uint32_t *)((uint32_t)&M0P_GPIO->P1ICLR + u8Param * 0x40)) = 0;	
//      
//    }break;
//  case 3:
//    {		
//      //NORTH key down
//      if(Gpio_GetIrqStat(KEY_NORTH_PORT, KEY_NORTH_PIN))
//      {
//      	UpdateKeyStatus = UpdateKeyStatus |KEY_NORTH_BIT;		
//      }
//      //UP key down   
//      if(Gpio_GetIrqStat(KEY_UP_PORT, KEY_UP_PIN))
//      {
//        UpdateKeyStatus = UpdateKeyStatus |KEY_UP_BIT;		
//      }    
//      
//      //SOUTH key down
//      if(Gpio_GetIrqStat(KEY_SOUTH_PORT, KEY_SOUTH_PIN))
//      {
//        UpdateKeyStatus = UpdateKeyStatus |KEY_SOUTH_BIT;		
//      }  
//      
//      
//      //WEST key down
//      if(Gpio_GetIrqStat(KEY_WEST_PORT, KEY_WEST_PIN))
//      {
//        UpdateKeyStatus = UpdateKeyStatus |KEY_WEST_BIT;		
//      }  
//      
//      
//      //EAST key down
//      if(Gpio_GetIrqStat(KEY_EAST_PORT, KEY_EAST_PIN))
//      {
//        UpdateKeyStatus = UpdateKeyStatus |KEY_EAST_BIT;		
//      }  
//      
//      //START key down
//      if(Gpio_GetIrqStat(KEY_START_PORT, KEY_START_PIN))
//      {
//        
//        UpdateKeyStatus = UpdateKeyStatus |KEY_START_BIT;					
//      }
//      
//      *((uint32_t *)((uint32_t)&M0P_GPIO->P3ICLR + u8Param * 0x40)) = 0;	
//    }break;
//    
//    
//  }
//  
//  
//}
void main(void)
{
 //SystemClock_Init();
  //CLK_Init();

   delay1ms(5000);
   KEY_Init();
	    //GPIO初始化，配置P03为输出，外接LED
    Gpio_InitIO(3, 3, GpioDirOut);
    Gpio_SetIO(3, 3, TRUE);

    //初始化外部IO P33
    Gpio_InitIOExt(3, 3, GpioDirIn, TRUE, FALSE, FALSE, 0);
    
    //开启GPIO中断
    Gpio_ClearIrq(3, 3);
    Gpio_EnableIrq(3, 3, GpioIrqFalling);
    EnableNvic(PORT3_IRQn, DDL_IRQ_LEVEL_DEFAULT, TRUE);
  //CFG->GCR |= CFG_GCR_SWD;
 
 // ee_Test();

  
//    if(FALSE == statetx )
//    {
//      
      while(1)
      {
       ; // command_process();
      }
//    }
//    else
//    {
//      
//      setup_Tx(); 
//      while (1)
//      {
//        loop_Tx(str,LEN);
//      }
//    }
    

}


/**
* @brief Device Initialize configuration
* @param None
* @retval None
*/

void CMT2300_Init()
{
  /**********基础设置初始化一次即可*******/
  FixedPktLength    = FALSE;				
  PayloadLength     = LEN;	
  vInit();
  vCfgBank(CMTBank, 12);
  vCfgBank(SystemBank, 12);
  vCfgBank(FrequencyBank, 8);
  vCfgBank(DataRateBank, 24);
  vCfgBank(BasebandBank, 29);
  vCfgBank(TXBank, 11);
  vEnablePLLcheck();
  bGoSleep();  				//让配置生效
  /**************************************/
  
}

void setup_Tx(void)
{
  
  bGoStandby();   //进入配置模式
  
  vEnableAntSwitch(0);  //设置天线切换_IO口切换
  vGpioFuncCfg(GPIO1_INT1+GPIO2_INT2+GPIO3_DOUT); //IO口的映射
  vIntSrcCfg(INT_FIFO_NMTY_TX, INT_TX_DONE);    //IO口中断的映射
  vIntSrcEnable(TX_DONE_EN);           //中断使能        
  
  vClearFIFO();  //清除FIFO
  bGoSleep();    //进入睡眠，让配置生效
  
  
}

void setup_Rx(void)
{
  
  bGoStandby();   //进入配置模式
  vEnableAntSwitch(0); //为 1 时 GPIO1 和 GPIO2 不可用
  vGpioFuncCfg(GPIO1_INT1+GPIO2_INT2+GPIO3_DOUT);  //IO口的功能映射
  
  //vIntSrcCfg(INT_RSSI_VALID, INT_CRC_PASS);   //GPO3映射成CRC_pass中断，此处如果要用该中断，RFPDK需要配置CRC
  vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_PKT_DONE);  //GPO3映射成PKT_DONE中断 //IO口中断的映射
  vIntSrcEnable(PKT_DONE_EN + CRC_PASS_EN);          //中断使能 
  
  vClearFIFO();
  bGoSleep();           //让配置生效
  bGoRx();              //for Rx
  
}


void loop_Tx(unsigned char *str,unsigned char len)
{
  bSendMessage(str, len);
  while(GPO2_L());   // 判断GPIO中断 为低等 为高运行下面代码
  bIntSrcFlagClr();
  vClearFIFO(); 
  delay1ms(200);
}

unsigned char loop_Rx(byte *getstr)
{
  byte tmp;
   tmp = 0;
  if(GPO2_H())
  {
    bGoStandby();
    tmp = bGetMessage(getstr);  //仿真到此能看到getstr收到的数据包
    bIntSrcFlagClr();
    vClearFIFO(); 
    bGoRx();
  }
  return tmp;
}
