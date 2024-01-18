/**********
���ߣ� Suncity
�绰�� 13686405081

V4.0 : �����յ���������standby���ơ�
*********/

#include <CMT2300A_EasyDriver.h> 

#define LEN 21

byte str[LEN] = {'H','o','p','e','R','F',' ','R','F','M',' ','C','O','B','R','F','M','3','0','0','A'};
byte getstr[LEN+1];

cmt2300aEasy radio;
uartClass uart;  //uart ��  ��ֲʱɾ��

/*****�˴�ͨ��RFPDK���������Ȼ�����ÿ�������еĲ��� *****/
/************************************************************
Ƶ��:  433.92Mhz
����:  2.4Kpbs
Ƶƫ:  +/-10Khz
����:  +/-50khz
���ݰ���ʽ:
		0xAAAAAAAAAAAAAAAA + 0x2DD4 + 0x15 +"HopeRF RFM COBRFM300A" 

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


void CMT2300_Init()
{
	/**********�������ó�ʼ��һ�μ���*******/
	radio.FixedPktLength	 = false;			   
	radio.PayloadLength	 = LEN;    
	radio.vInit();
	radio.vCfgBank(CMTBank, 12);
	radio.vCfgBank(SystemBank, 12);
	radio.vCfgBank(FrequencyBank, 8);
	radio.vCfgBank(DataRateBank, 24);
	radio.vCfgBank(BasebandBank, 29);
	radio.vCfgBank(TXBank, 11);
	radio.bGoSleep();     //��������Ч
	/**************************************/
}
void setup_Rx()
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
	uart.vUartInit(9600, _8N1); // ���ڳ�ʼ��
}

void loop_Rx()
{
 byte tmp;
 if(GPO3_H())
      {
       radio.bGoStandby();     //��סfifo�����͹���
       tmp = radio.bGetMessage(getstr);
       radio.bIntSrcFlagClr();
       radio.vClearFIFO(); 
       radio.bReadStatus();
	   
       uart.vUartPutNByte(getstr, tmp);  //���ڴ�ӡ����
       uart.vUartNewLine();             //���ڴ�ӡ����
       radio.bGoRx();      
      }
}
void main(void)
{
	CMT2300_Init();

	setup_Rx();
	while(1)
	{
	   loop_Rx();
	}
}
					
					
