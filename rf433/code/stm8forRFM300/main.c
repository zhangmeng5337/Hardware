/**
  ******************************************************************************
  * @file     TIM4_TimeBase\main.c
  * @author   Xu xucheng        
  * @version  V1.0.0
  * @date     4-Jan-2017
  * @brief    This file contains the main function for chuangweilock
  ******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s.h"
#include "main.h"
#include "CMT2300drive.h"

static bool statetx = true;  //  falseΪRX  trueΪTX

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

//void Device_Init(void);
void CLK_Init(void);
void setup_Tx(void);
void setup_Rx(void);
void CMT2300_Init();
void loop_Tx(void);
void loop_Rx(void);

#define LEN 21

byte str[LEN] = {'H','o','p','e','R','F',' ','R','F','M',' ','C','O','B','R','F','M','3','0','0','A'};
byte getstr[LEN+1];

cmt2300aEasy radio;

/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */


/*****�˴�ͨ��RFPDK���������Ȼ�����ÿ�������еĲ��� *****/
/************************************************************
Ƶ��:  433.92Mhz
����:  2.4Kpbs
Ƶƫ:  +/-10Khz
����:  +/-100khz
���ݰ���ʽ:
		0xAAAAAAAAAAAAAAAA + 0x2DD4 +0x15 +"HopeRF RFM COBRFM300A" 

���书��: 13dBm
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
					0x0BD0
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

word FrequencyBank[8] = {
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
					0x3812,
					0x3908,
					0x3A00,
					0x3BAA,
					0x3C02,
					0x3D00,
					0x3E00,
					0x3F00,
					0x4000,
					0x4100,
					0x4200,
					0x43D4,
					0x442D,
					0x4501,
					0x461F,
					0x4700,
					0x4800,
					0x4900,
					0x4A00,
					0x4B00,
					0x4C00,
					0x4D00,
					0x4E00,
					0x4F60,
					0x50FF,
					0x5102,
					0x5200,
					0x531F,
					0x5410	
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
		

void main(void)
{
  
  CLK_Init();
  CMT2300_Init();
  if(false == statetx )
  	{
  		setup_Rx();
		while(1)
	    {
          loop_Rx();
	    }
  	}
  else
  	{
  		
        setup_Tx(); 
		while (1)
		{
		   loop_Tx();
		}
  	}
  
}



/**
  * @brief Device Initialize configuration
  * @param None
  * @retval None
  */

void CMT2300_Init()
{
	/**********�������ó�ʼ��һ�μ���*******/
	radio.FixedPktLength    = false;				
	radio.PayloadLength     = LEN;	
	radio.vInit();
	radio.vCfgBank(CMTBank, 12);
	radio.vCfgBank(SystemBank, 12);
	radio.vCfgBank(FrequencyBank, 8);
	radio.vCfgBank(DataRateBank, 24);
	radio.vCfgBank(BasebandBank, 29);
	radio.vCfgBank(TXBank, 11);
	radio.vEnablePLLcheck();
	radio.bGoSleep();  				//��������Ч
	/**************************************/

}

void setup_Tx(void)
{

	radio.bGoStandby();   //��������ģʽ
	
	radio.vEnableAntSwitch(0);  //���������л�_IO���л�
	radio.vGpioFuncCfg(GPIO1_INT1+GPIO2_INT2+GPIO3_INT2); //IO�ڵ�ӳ��
	radio.vIntSrcCfg(INT_FIFO_NMTY_TX, INT_TX_DONE);    //IO���жϵ�ӳ��
	radio.vIntSrcEnable(TX_DONE_EN);           //�ж�ʹ��        
	
	radio.vClearFIFO();  //���FIFO
	radio.bGoSleep();    //����˯�ߣ���������Ч

 
}

void setup_Rx(void)
{

	radio.bGoStandby();   //��������ģʽ
	radio.vEnableAntSwitch(0); //Ϊ 1 ʱ GPIO1 �� GPIO2 ������
	radio.vGpioFuncCfg(GPIO1_INT1+GPIO2_Dout+GPIO3_INT2);  //IO�ڵĹ���ӳ��

	//radio.vIntSrcCfg(INT_RSSI_VALID, INT_CRC_PASS);   //GPO3ӳ���CRC_pass�жϣ��˴����Ҫ�ø��жϣ�RFPDK��Ҫ����CRC
	radio.vIntSrcCfg(INT_FIFO_WBYTE_RX, INT_PKT_DONE);  //GPO3ӳ���PKT_DONE�ж� //IO���жϵ�ӳ��
	radio.vIntSrcEnable(PKT_DONE_EN + CRC_PASS_EN);          //�ж�ʹ�� 
	
	radio.vClearFIFO();
	radio.bGoSleep();           //��������Ч
	radio.bGoRx();              //for Rx

}


void loop_Tx()
{
	radio.bSendMessage(str, LEN);
	while(GPO3_L());   // �ж�GPIO�ж� Ϊ�͵� Ϊ�������������
	radio.bIntSrcFlagClr();
	radio.vClearFIFO(); 
	Delay_ms(200);
}

void loop_Rx()
{
	byte tmp;
	if(GPO3_H())
		{
		  radio.bGoStandby();
		  tmp = radio.bGetMessage(getstr);  //���浽���ܿ���getstr�յ������ݰ�
		  radio.bIntSrcFlagClr();
		  radio.vClearFIFO(); 
		  radio.bGoRx();
		}	
}

/**
  * @brief CLOCK Initialize configuration
  * @param None
  * @retval None
  */
void CLK_Init(void)
{
  /* Deinitializes the CLK peripheral registers to their default reset */
  CLK_DeInit();
  /* Clock divider to HSI/1 */
  CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);
}

/**
  * @brief  Configure TIM4 to generate an update interrupt each 1ms 
  * @param  None
  * @retval None
  */
 


#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
