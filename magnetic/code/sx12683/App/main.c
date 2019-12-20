#include "stm8l15x_flash.h"
#include "bsp.h"
#include "stm8l15x_it.h"
//
#include "math.h"
#include "sx126x-board.h"
#include "gpio.h"
#include "delay.h"
#include "spi.h"
#include "radio.h"
#include "sx126x.h"
#include "sx126x-board.h"
#include "crc.h"
//
//#include "sx126x-board.h"


/**************************************************************************************************************************************
Demo 3ìDòá÷3ì  EnableMaster=true  ?a?÷?ú??￡??÷?ú??·￠?íò???"PING"êy?Yoó?D??μ??óê?￡?μè′y′ó?ú·μ??μ?ó|′e"PONG"êy?YLEDéá??

EnableMaster=FALSE ?a′ó?ú??￡?′ó?ú???óê?μ??÷?ú??·￠1yà′μ?"PING"êy?YoóLEDéá??2￠·￠?íò???"PONG"êy?Y×÷?aó|′e
***************************************************************************************************************************************/


#define USE_MODEM_LORA
#define REGION_CN779

#if defined( REGION_AS923 )

#define RF_FREQUENCY                                923000000 // Hz

#elif defined( REGION_AU915 )

#define RF_FREQUENCY                                915000000 // Hz

#elif defined( REGION_CN779 )

#define RF_FREQUENCY                                434000000 // Hz

#elif defined( REGION_EU868 )

#define RF_FREQUENCY                                868000000 // Hz

#elif defined( REGION_KR920 )

#define RF_FREQUENCY                                920000000 // Hz

#elif defined( REGION_IN865 )

#define RF_FREQUENCY                                865000000 // Hz

#elif defined( REGION_US915 )

#define RF_FREQUENCY                                915000000 // Hz

#elif defined( REGION_US915_HYBRID )

#define RF_FREQUENCY                                915000000 // Hz

#else

#error "Please define a frequency band in the compiler options."

#endif

#define TX_OUTPUT_POWER                             22        // 22 dBm

extern bool IrqFired;




//bool EnableMaster=FALSE;//?÷′ó????
bool EnableMaster=TRUE;//?÷′ó????
uint16_t  crc_value;
/*!
* Radio events function pointer
*/
static RadioEvents_t RadioEvents;

#if defined( USE_MODEM_LORA )

#define LORA_BANDWIDTH                              2         // [0: 125 kHz,
//  1: 250 kHz,
//  2: 500 kHz,
//  3: Reserved]
#define LORA_SPREADING_FACTOR                       12         // [SF7..SF12]
#define LORA_CODINGRATE                             2         // [1: 4/5,
//  2: 4/6,
//  3: 4/7,
//  4: 4/8]
#define LORA_PREAMBLE_LENGTH                        8         // Same for Tx and Rx
#define LORA_SYMBOL_TIMEOUT                         0         // Symbols
#define LORA_FIX_LENGTH_PAYLOAD_ON                  FALSE
#define LORA_IQ_INVERSION_ON                        FALSE

#elif defined( USE_MODEM_FSK )

#define FSK_FDEV                                    5e3      // Hz 
#define FSK_DATARATE                                2.4e3      // bps
#define FSK_BANDWIDTH                               20e3     // Hz >> DSB in sx126x
#define FSK_AFC_BANDWIDTH                           100e3     // Hz
#define FSK_PREAMBLE_LENGTH                         5         // Same for Tx and Rx
#define FSK_FIX_LENGTH_PAYLOAD_ON                   FALSE

#else
#error "Please define a modem in the compiler options."
#endif

typedef enum
{
  LOWPOWER,
  RX,
  RX_TIMEOUT,
  RX_ERROR,
  TX,
  TX_TIMEOUT,
}States_t;

#define RX_TIMEOUT_VALUE                            0xffff
#define BUFFER_SIZE                                 64 // Define the payload size here

const uint8_t PingMsg[] = "PING";
const uint8_t PongMsg[] = "PONG";

uint16_t BufferSize = BUFFER_SIZE;
uint8_t TX_Buffer[BUFFER_SIZE];
uint8_t RX_Buffer[BUFFER_SIZE];


States_t State = LOWPOWER;

int8_t RssiValue = 0;
int8_t SnrValue = 0;
void OnTxDone( void );
void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr );
void OnTxTimeout( void );
void OnRxTimeout( void );
void OnRxError( void );


//void SysClock_48()
//{ 
//   RCC_PLLCmd(DISABLE);
//   RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_12);//48M
//   RCC_PLLCmd(ENABLE);
//   while(!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));
//   RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
//}
//
//void Tick_Configration()
//{
//    /* Setup SysTick Timer for 1ms interrupts ( not too often to save power ) */
//    if( SysTick_Config( SystemCoreClock / 1000 ) )
//    { 
//        /* Capture error */ 
//        while (1);
//    }
//}

//void RCC_Configuration()
//{   
//  /* Enable GPIO clock */
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA|RCC_AHBPeriph_GPIOB|RCC_AHBPeriph_GPIOC|RCC_AHBPeriph_GPIOF, ENABLE);
//  
//  /* Enable peripheral Clock */
//  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2|RCC_APB1Periph_PWR, ENABLE);  
//  
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_SYSCFG, ENABLE);
//}

//void HW_int(void)
//{
//    SysClock_48(); 
//    Tick_Configration();
//    RCC_Configuration();
//    GPIO_int();
//    SPI2_Int();
//}


void LedToggle(void)
{
  GPIO_ToggleBits(PORT_LED, PIN_LED);   
}


void HW_int(void)
{
  disableInterrupts(); 
  
  SystemClock_Init();     // 系统时钟初始化
  CLK_LSEConfig(CLK_LSE_OFF);
  //SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC, DISABLE);
  // SYSCFG_REMAPPinConfig(REMAP_Pin_SPI1Full, DISABLE);
  // SYSCFG_REMAPPinConfig(REMAP_Pin_TIM2Channel1, DISABLE);  
  // SYSCFG_REMAPPinConfig(REMAP_Pin_TIM2Channel2, DISABLE);   
  // Tick_Configration();
  //RCC_Configuration();
  GPIO_int();
  SPI_Int();
  enableInterrupts();
  //LED_Init();             //调试LED初始化 
}
int main( void )
{   
  //    uint8_t data[2]={0x12,0x34};
  //    static uint8_t test[2];
  
  HW_int();//MCUía?§×ê?′3?ê??ˉ
  DelayMs(1000);
  
  // Radio initialization
  RadioEvents.TxDone = OnTxDone;
  RadioEvents.RxDone = OnRxDone;
  RadioEvents.TxTimeout = OnTxTimeout;
  RadioEvents.RxTimeout = OnRxTimeout;
  RadioEvents.RxError = OnRxError;
  LedToggle();
  
  Radio.Init( &RadioEvents );
  Radio.SetChannel( RF_FREQUENCY );
  
  //    Radio.WriteBuffer(0x06C0,data,2);
  //    Radio.ReadBuffer(0x06C0,test,2);
  
//#if defined( USE_MODEM_LORA )
  
  Radio.SetTxConfig( MODEM_LORA, TX_OUTPUT_POWER, 0, LORA_BANDWIDTH,
                    LORA_SPREADING_FACTOR, LORA_CODINGRATE,
                    LORA_PREAMBLE_LENGTH, LORA_FIX_LENGTH_PAYLOAD_ON,
                    true, 0, 0, LORA_IQ_INVERSION_ON, 3000 );
  
  Radio.SetRxConfig( MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                    LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                    LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                    0, true, 0, 0, LORA_IQ_INVERSION_ON, FALSE );
  
//#elif defined( USE_MODEM_FSK )
//  
//  Radio.SetTxConfig( MODEM_FSK, TX_OUTPUT_POWER, FSK_FDEV, 0,
//                    FSK_DATARATE, 0,
//                    FSK_PREAMBLE_LENGTH, FSK_FIX_LENGTH_PAYLOAD_ON,
//                    true, 0, 0, 0, 3000 );
//  
//  Radio.SetRxConfig( MODEM_FSK, FSK_BANDWIDTH, FSK_DATARATE,
//                    0, FSK_AFC_BANDWIDTH, FSK_PREAMBLE_LENGTH,
//                    0, FSK_FIX_LENGTH_PAYLOAD_ON, 0, true,
//                    0, 0,FALSE, true );
////#else
////#error "Please define a frequency band in the compiler options."
//#endif
  
  
  if(EnableMaster)
  {
    TX_Buffer[0] = 'P';
    TX_Buffer[1] = 'I';
    TX_Buffer[2] = 'N';
    TX_Buffer[3] = 'G'; 
    
    crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????μ?3?òa·￠?íêy?Y°üCRC?μ
    TX_Buffer[4]=crc_value>>8;
    TX_Buffer[5]=crc_value;
    Radio.Send( TX_Buffer, 6);
  }
  else
  {
    Radio.Rx( RX_TIMEOUT_VALUE ); 
  }
  
  while( 1 )
  {
    extern  uint8_t tx_complete;
//    if(EnableMaster&&tx_complete)
//    {
//      DelayMs(1000);
//      tx_complete =0;
//      TX_Buffer[0] = 'P';
//      TX_Buffer[1] = 'I';
//      TX_Buffer[2] = 'N';
//      TX_Buffer[3] = 'G'; 
//      
//      crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????μ?3?òa·￠?íêy?Y°üCRC?μ
//      TX_Buffer[4]=crc_value>>8;
//      TX_Buffer[5]=crc_value;
//      Radio.Send( TX_Buffer, 6);
//      LedToggle();
//    }      
    Radio.IrqProcess( ); // Process Radio IRQ
  }
}

void OnTxDone( void )
{   
  Radio.Standby();
  Radio.Rx( RX_TIMEOUT_VALUE ); //??è??óê?
  
}

void OnRxDone( uint8_t *payload, uint16_t size, int16_t rssi, int8_t snr )
{
  BufferSize = size;
  memcpy( RX_Buffer, payload, BufferSize );
  RssiValue = rssi;
  SnrValue = snr;
  
  Radio.Standby();
  
  if(EnableMaster)
  {
    if(memcmp(RX_Buffer,PongMsg,4)==0)
    {
      LedToggle();//LEDéá??
      
    }
    
    TX_Buffer[0] = 'P';
    TX_Buffer[1] = 'I';
    TX_Buffer[2] = 'N';
    TX_Buffer[3] = 'G'; 
    
    crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????μ?3?òa·￠?íêy?Y°üCRC?μ
    TX_Buffer[4]=crc_value>>8;
    TX_Buffer[5]=crc_value;
    Radio.Send( TX_Buffer, 6);
  }
  else
  {
    if(memcmp(RX_Buffer,PingMsg,4)==0)
    {
      LedToggle();//LEDéá??
      
      TX_Buffer[0] = 'P';
      TX_Buffer[1] = 'O';
      TX_Buffer[2] = 'N';
      TX_Buffer[3] = 'G'; 
      
      crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????μ?3?òa·￠?íêy?Y°üCRC?μ
      TX_Buffer[4]=crc_value>>8;
      TX_Buffer[5]=crc_value;
      Radio.Send( TX_Buffer, 6);
    }
    else
    {
      Radio.Rx( RX_TIMEOUT_VALUE ); 
    }   
  }
}

void OnTxTimeout( void )
{
  
}

void OnRxTimeout( void )
{
  Radio.Standby();
  if(EnableMaster)
  {
    TX_Buffer[0] = 'P';
    TX_Buffer[1] = 'I';
    TX_Buffer[2] = 'N';
    TX_Buffer[3] = 'G'; 
    
    crc_value=RadioComputeCRC(TX_Buffer,4,CRC_TYPE_IBM);//????μ?3?òa·￠?íêy?Y°üCRC?μ
    TX_Buffer[4]=crc_value>>8;
    TX_Buffer[5]=crc_value;
    Radio.Send( TX_Buffer, 6);
  }
  else
  {
    Radio.Rx( RX_TIMEOUT_VALUE ); 
  }
}

void OnRxError( void )
{
  
  
}



