// 	Demo ��������  ��     SI4463, 
// 	RF ģ��Ĳ�������Ϊ�� +/-10PPM
//	ʹ��AFC��ʹ��CRC�� PH + FIFO ģʽ
// 	ÿ���ӷ���Ĺ̶���������Ϊ�� 0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x6d, 
//  chksum:  0x6d = (0x41 +0x42 +0x43 +0x44 +0x45 +0x46 +0x47 + 0x48 +0x49)
// 	MCU : RENSAS high performance 8 bit MCU :R5f211B4, internal 8MHz
// 	src_field, ��ָ�ĸ�field ���� ������Ϣ��������Ϣ�����field �����һ������2���ֽ�
// 	dst-field ��ָ������Ǹ�field��ָ��������ݡ�
// 	���磺 Ҫ��  1,2,3,4,5,6,7,8,9 ��9���ɱ䳤�ȵ����ݷŵ�field 4,��ô��������:src-field = 1,field 1 ����������Ϣ��
//	�����field�����һ���ֽھ��ǳ�����Ϣ��Ҳ����˵���һ���ֽ� �������� 9�����ȣ�,Dst-field = 3;����ζ�� field 3�������� 1��2�� ��������


#include "main.h"
#include "radio_config_si4463.h"
#include "radio_config_user.h"
#include "si4463.h"
#include "uart.h"

extern SPI_HandleTypeDef hspi1;



FlagType	Flag;

unsigned int	count_1hz, rf_timeout;
unsigned char spi_read_buf[20];   // ���ڶ�ȡ SPI����ķ�����������
unsigned char rx_buf[25];

unsigned char mode;

// add joyce
const unsigned char Tab_DispCode[17]={0x77,0x24,0x6b,0x6d,0x3c,0x5d,0x5f,0x64,0x7f,0x7d,0x7e,0x1f,0x53,0x2f,0x5b,0x5a,0x00};
unsigned char 	dis_ram[28] = {00};

unsigned char	key_value;
unsigned char	have_set;
unsigned char 	set;		// ����ģʽ
unsigned char 	old_mode;
unsigned char 	mode;		 
unsigned char 	freq3;	
unsigned char 	freq2;	
unsigned char 	freq1;		// �ز�Ƶ��
unsigned char 	power;		// ���书��
unsigned char 	rssi;		// �����ź�ǿ��
unsigned char 	rate;		// ����

unsigned char	cnt_10s;

unsigned long   tx_cnt = 0,rx_cnt = 0; //�շ�����
unsigned char 	reset_mode;

unsigned char 	reach_15hz = 0,reach_flash = 0,reach_1s = 0;
unsigned char 	count_15hz,count_flash = 0,count_1s = 0,nirq_cnt = 0;
void GPIO_SET(unsigned char data);
unsigned char check_cts(void);
void spi_write(unsigned char tx_length, unsigned char *p);
void spi_fast_read( unsigned char api_command);
void spi_write_fifo(void);
void spi_read_fifo(void);
void sdn_reset(void);
void clr_interrupt(void);
void fifo_reset(void);
void enable_tx_interrupt(void);
void enable_rx_interrupt(void);
void tx_start(void);
void rx_start(void);
void rx_init(void);
void tx_const_aa(void);
void rx_start_test(void);
void spi_read(unsigned char data_length, unsigned char api_command );
void rf_init_freq(void);

/*
void tx_data(void);
void SI4463_init(void);
void sysclk_cfg(void);
void port_init(void);
void timerx_init(void);
void delay_1ms(unsigned int delay_time);
unsigned char spi_byte(unsigned char data);


// joyce add
void GPIO_SET(unsigned char data);
void rf_standby(void);



void delay_10ms(void);*/


void delay_1ms(unsigned int delay_time)
{
	//unsigned int i;
	HAL_Delay(delay_time);
}	

void delay_10ms(void)
{
	HAL_Delay(10);

}

void SI4463_init(void)
{	
	unsigned char app_command_buf[20],i;
	
	for(i=4;i<16;i++) 
		RF_MODEM_MOD_TYPE_12_data[i] = RF_MODEM_MOD_TYPE_12[rate][i-4];
	if(freq3<8)
	{
		for(i=4;i<12;i++)	
			RF_MODEM_TX_RAMP_DELAY_8_data[i] = RF_MODEM_TX_RAMP_DELAY_8_433[rate][i-4];
		for(i=4;i<11;i++)
			RF_MODEM_AFC_GEAR_7_data[i] = RF_MODEM_AFC_GEAR_7_433[rate][i-4];	
		for(i=4;i<15;i++)
			RF_MODEM_OOK_CNT1_11_data[i] =RF_MODEM_OOK_CNT1_11_433[rate][i-4];
		for(i=4;i<16;i++)
			RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12_data[i] =RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12_433[rate][i-4];
		for(i=4;i<16;i++)
			RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12_data[i] = RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12_433[rate][i-4];
		for(i=4;i<16;i++)
			RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12_data[i] = RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12_433[rate][i-4];	
		for(i=4;i<13;i++)
			RF_MODEM_AGC_WINDOW_SIZE_9_data[i] = RF_MODEM_AGC_WINDOW_SIZE_9_433[rate][i-4];
		for(i=4;i<13;i++)	
			RF_MODEM_BCR_OSR_1_9_data[i] = RF_MODEM_BCR_OSR_1_9_433[rate][i-4];	
	}
	else
	{
		for(i=4;i<12;i++)	
			RF_MODEM_TX_RAMP_DELAY_8_data[i] = RF_MODEM_TX_RAMP_DELAY_8_850[rate][i-4];
		for(i=4;i<11;i++)
			RF_MODEM_AFC_GEAR_7_data[i] = RF_MODEM_AFC_GEAR_7_850[rate][i-4];		
		for(i=4;i<15;i++)
			RF_MODEM_OOK_CNT1_11_data[i] =RF_MODEM_OOK_CNT1_11_850[rate][i-4];
		for(i=4;i<16;i++)
			RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12_data[i] =RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12_850[rate][i-4];
		for(i=4;i<16;i++)
			RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12_data[i] = RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12_850[rate][i-4];
		for(i=4;i<16;i++)
			RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12_data[i] = RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12_850[rate][i-4];	
		for(i=4;i<13;i++)
			RF_MODEM_AGC_WINDOW_SIZE_9_data[i] = RF_MODEM_AGC_WINDOW_SIZE_9_850[rate][i-4];	
		for(i=4;i<13;i++)	
			RF_MODEM_BCR_OSR_1_9_data[i] = RF_MODEM_BCR_OSR_1_9_850[rate][i-4];	
	}		
	for(i=4;i<11;i++)
		RF_SYNTH_PFDCP_CPFF_7_data[i] = RF_SYNTH_PFDCP_CPFF_7[rate][i-4];
	
	GPIO_SET(mode);
	
	// spi_write(0x05, RF_GLOBAL_XO_TUNE_1_data);   //  Ƶ�ʵ���
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x00;    // 0x0000 
	app_command_buf[2]  = 0x01;    // �� 1������
	app_command_buf[3]  = 0x00;   // 0x0000
	app_command_buf[4]  = 98;  // freq  adjustment
	spi_write(5, app_command_buf);

	// spi_write(0x05, RF_GLOBAL_CONFIG_1_data);  //  *******************************************************
  	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x00;    // 0x0003 
	app_command_buf[2]  = 0x01;    // �� 1������
	app_command_buf[3]  = 0x03;   // 0x0003
	app_command_buf[4]  = 0x40;  // tx = rx = 64 byte, ��ͨ PH��high performance mode  ????????????????????? 
	spi_write(5, app_command_buf); 
    
    // *****************************************************************************    
    spi_write(0x08, RF_FRR_CTL_A_MODE_4_data);    // disable all fast response register
     
    // spi_write(0x0D, RF_PREAMBLE_TX_LENGTH_9_data); ǰ��������*********************************************   
 	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x10;    // 0x1000 
	app_command_buf[2]  = 0x09;    // �� 9������
	app_command_buf[3]  = 0x00;   // 0x1000
	
//	if(mode == tx_test_mode)
//	{
//		app_command_buf[4]  = 0xff;   //  ��Ҫ����255���ֽڵ�Preamble
//	}
//	else
	{
		app_command_buf[4]  = 0x08;   //  ��Ҫ����8���ֽڵ�Preamble
	}	
	
	//app_command_buf[4]  = 0x08;   //  ��Ҫ����8���ֽڵ�Preamble
	app_command_buf[5]  = 0x14;   //Ҫ���20 bit ͬ���֣�
	app_command_buf[6]  = 0x00;   // �Ǳ�׼��preamble �趨��û����
	app_command_buf[7]  = 0x0f;   // ǰ����Timeout��ʱ��   ????????????????????????????????????????????????????????????
	app_command_buf[8]  = 0x31;  // ǰ����ĳ����� byte ���㣬 1st = 1��û��manchest  ���룬ʹ�ñ�׼�� 1010.����
	app_command_buf[9]  = 0x0;  	//  �Ǳ�׼��Preamble�� Patten 4th byte
	app_command_buf[10]  = 0x00;   //  �Ǳ�׼��Preamble�� Patten 3rd byte
	app_command_buf[11]  = 0x00;  //  �Ǳ�׼��Preamble�� Patten 2nd byte
	app_command_buf[12]  = 0x00;  //  �Ǳ�׼��Preamble�� Patten 1st byte
	spi_write(13, app_command_buf);  // 	
	
	//  RF_SYNC_CONFIG_5_data,  ͬ���ֵ����� *********************************************
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x11;    // command = 0x1100
	app_command_buf[2]  = 0x05;    // �� 5������
	app_command_buf[3]  = 0x00;   // command = 0x1100
	app_command_buf[4]  = 0x01;   //  ͬ������ length field ���岢���ͣ�ͬ���ֲ����д����� 4FSK�� ����manchest ���룬ֻ�� 2���ֽ�
	
//	if(mode == tx_test_mode)
//	{
//		app_command_buf[5]  = 0x55;   //ͬ���� 3
//		app_command_buf[6]  = 0x55;   // ͬ����2 
//	}	
//	else
	{
		app_command_buf[5]  = 0x2d;   //ͬ���� 3
		app_command_buf[6]  = 0xd4;   // ͬ����2 
	}
	
	app_command_buf[7]  = 0x00;   // ͬ����1
	app_command_buf[8]  = 0x00;  // ͬ���� 0
    spi_write(9, app_command_buf);  //
        
	//  packet crc ���� *******************************************************************        
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // command = 0x1200
	app_command_buf[2]  = 0x01;    // �� 1������
	app_command_buf[3]  = 0x00;   // command = 0x1200
	app_command_buf[4]  = 0x81; //   1��cRC ���ӣ�CRC = itu-c, enable crc
    spi_write(5, app_command_buf);  
        
	// packet   gernale configuration        
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // command = 0x1206
	app_command_buf[2]  = 0x01;    // �� 1������
	app_command_buf[3]  = 0x06;   // command = 0x1206
	app_command_buf[4]  = 0x02;   //  tx = rx = 120d--1220 (tx packet��ph enable, not 4fsk, �յ�һ�����ݺ�RX off,CRC ����ת ��CRC MSB�� data MSB
    spi_write(5, app_command_buf);  
        
  	// spi_write(0x07, RF_PKT_LEN_3_data);   //  �̶����ȣ�����һ���ֽڣ�������Ϣû����Payload����
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // command = 0x1208
	app_command_buf[2]  = 0x03;    // �� 3������
	app_command_buf[3]  = 0x08;   // command = 0x1208
	app_command_buf[4]  = 0x00;   //  Length Field = LSB,  length ֻ��1���ֽڣ�length ������ FiFo���棬�̶�����ģʽ
	app_command_buf[5]  = 0x00;   //���ڿɱ����ģʽ�������ĸ� Field ������ length Filed
	app_command_buf[6]  = 0x00;   // ���ڿɱ����ģʽ�������ɱ������ length
    spi_write(7, app_command_buf); 
    
        
	// length Field 1 -- 4 ������ 
	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // 0x120d 
	app_command_buf[2]  = 0x0c;    // �� 12������
	app_command_buf[3]  = 0x0d;   // 0x120d
	app_command_buf[4]  = 0x00;   //  Field 1 �ĳ��� ����4λ��
	app_command_buf[5]  = payload_length;   //  field 1 �ĳ��ȣ� ���� 8λ), �ܹ�14���ֽ�
	app_command_buf[6]  = 0x04;   // field 1 ���� 4FSK��manchest, whiting, PN9,   
	app_command_buf[7]  = 0xaa;   // field 1 crc enble, check enbale, ����Ҳ��CRC��cRC��Seed ��CRC_seed�����һ��  
	app_command_buf[8]  = 0x00;  //  field2 �ĳ��ȣ���4λ��
	app_command_buf[9]  = 0x00;  	//  field 2 �ĳ��ȣ� ���� 8λ)�� ���� = 0 ��ʾ���field û��ʹ��
	app_command_buf[10]  = 0x00;   // field 2 ���� 4FSK��manchest, whiting, PN9
	app_command_buf[11]  = 0x00;  //  field 2 ��CRC����
	app_command_buf[12]  = 0x00;  //  field 3 �ĳ��ȣ� ���� 8λ)�� ���� = 0 ��ʾ���field û��ʹ��
	app_command_buf[13]  = 0x00;  	//  field 3 �ĳ��ȣ� ���� 8λ)�� ���� = 0 ��ʾ���field û��ʹ��
	app_command_buf[14]  = 0x00;   //  field 3 ���� 4FSK��manchest, whiting, PN9
	app_command_buf[15]  = 0x00;  //  field 3 ��CRC����
	spi_write(16, app_command_buf);  // 
  
    // spi_write(0x0C, RF_PKT_FIELD_4_LENGTH_12_8_8_data);  **************************************
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // 0x1219 
	app_command_buf[2]  = 0x08;    // �� 8������
	app_command_buf[3]  = 0x19;   // 0x1219
	app_command_buf[4]  = 0x00;   //  field4 �ĳ��ȣ���4λ��
	app_command_buf[5]  = 0x00;   //  field 4 �ĳ��ȣ� ���� 8λ)�� ���� = 0 ��ʾ���field û��ʹ��
	app_command_buf[6]  = 0x00;   //   field 4 ���� 4FSK��manchest, whiting, PN9
	app_command_buf[7]  = 0x00;   // field 4 ��CRC����
	app_command_buf[8]  = 0x00;  //  field5 �ĳ��ȣ���4λ��
	app_command_buf[9]  = 0x00;  	//   field 5 �ĳ��ȣ� ���� 8λ)�� ���� = 0 ��ʾ���field û��ʹ��
	app_command_buf[10]  = 0x00;   //  field 5 ���� 4FSK��manchest, whiting, PN9
	app_command_buf[11]  = 0x00;   // field 5 ��CRC����
    spi_write(12, app_command_buf);  //
    
    // ********************************************************************************
    spi_write(0x10, RF_MODEM_MOD_TYPE_12_data);   // //  2FSK ,  module source = PH fifo, disable manchest, tx, rx ����ת�� deviation ����ת
        
	//spi_write(0x05, RF_MODEM_FREQ_DEV_0_1_data); **************************************
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x20;    // 0x200c                                                         
	app_command_buf[2]  = 0x01;    // �� 1������                                                     
	app_command_buf[3]  = 0x0c;   // 0x200c                                                          
	//app_command_buf[4]  = 0x4f; // frequency deviation ��Bit 0--7, ����WDS����
	if(freq3<8)
	{
		if(rate>=dr_256k)
			app_command_buf[4]  = 0x4f;
		else if(rate>=dr_19p2)
			app_command_buf[4]  = 0x5e;
		else
		    app_command_buf[4]  = 0xd2;   
	}
	else
	{
		if(rate>=dr_115p2)
			app_command_buf[4]  = 0x8f;	// 15k
		else if(rate>=dr_19p2)
			app_command_buf[4]  = 0x5e;	// 10k
		else
		    app_command_buf[4]  = 0x18; // 8k
	}	  
    spi_write(5, app_command_buf);  
        
    // ********************************************************************************    
    spi_write(0x0C, RF_MODEM_TX_RAMP_DELAY_8_data);    	// û���������������Ҫ
    spi_write(0x0D, RF_MODEM_BCR_OSR_1_9_data);			// û���������������Ҫ
    spi_write(0x0B, RF_MODEM_AFC_GEAR_7_data);			// û���������������Ҫ
    spi_write(0x05, RF_MODEM_AGC_CONTROL_1_data);		// û���������������Ҫ
    spi_write(0x0D, RF_MODEM_AGC_WINDOW_SIZE_9_data);	// û���������������Ҫ
    spi_write(0x0F, RF_MODEM_OOK_CNT1_11_data);			// û���������������Ҫ   
    
	// spi_write(0x05, RF_MODEM_RSSI_COMP_1_data);	**************************************************
	app_command_buf[0] = 0x11;  
	app_command_buf[1] = 0x20;    // 0x204e                                                         
	app_command_buf[2] = 0x01;    // �� 1������                                                     
	app_command_buf[3] = 0x4e;   // 0x204e 
	if(rate==dr_500)
		app_command_buf[4]  = 0x3a; 
	else                                                         
		app_command_buf[4]  = 0x40;  //  rssi ����ƫ�����ʵֵ�Ĳ�ֵ
    spi_write(5, app_command_buf);
            	     
    // ********************************************************************************    
    spi_write(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12_data);  // û���������������Ҫ
    spi_write(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12_data);   // û���������������Ҫ
    spi_write(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12_data);   // û���������������Ҫ        
        
	// RF_PA **************************************************************************
	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x22;    // 0x2200                                                         
	app_command_buf[2]  = 0x04;    // �� 4������                                                     
	app_command_buf[3]  = 0x00;   // 0x2200                                                          
	app_command_buf[4]  = 0x08;  //0x10;   //   PA mode  = default , ʹ��Class E ģʽ�������� Switch Current ģʽ  ????????
	if(power==7)	
		app_command_buf[5]  = 127;   //  ����Ϊ�����
	else if(power==6)
		app_command_buf[5]  = 50;
	else if(power==5)
		app_command_buf[5]  = 30;
	else if(power==4)
		app_command_buf[5]  = 20;
	else if(power==3)
		app_command_buf[5]  = 15;
	else if(power==2)
		app_command_buf[5]  = 10;
	else if(power==1)
		app_command_buf[5]  = 7;					
	else	
		app_command_buf[5]  = 4;		
	app_command_buf[6]  =0x00; //???????? 0x00;   // CLK duty = 50%�� ���� = Default
	if((rate<=dr_115p2)||(rate==dr_500))
		app_command_buf[7]  = 0x3d;  // ???????? 0x5d;   // PA ramp time = default 
    else
    	app_command_buf[7]  = 0x5d;    
    spi_write(8, app_command_buf);        
    
    // ********************************************************************************     
	spi_write(0x0B, RF_SYNTH_PFDCP_CPFF_7_data);      // û���������������Ҫ
        
   	// header match ******************************************************************
   	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x30;    // 0x3000                                                         
	app_command_buf[2]  = 0x0c;    // �� 12������                                                     
	app_command_buf[3]  = 0x00;   // 0x3000                                                          
	app_command_buf[4]  = 's';    //0x00;   //  match 1 ��ֵ
	app_command_buf[5]  = 0xff;   //  match 1 mask
	app_command_buf[6]  = 0x40;   // eable match 1, match 1��ֵ��ͬ���ֵ����ľ���, 0 = match 
	app_command_buf[7]  = 'w';    //  match 2 ��ֵ                                        
	app_command_buf[8]  = 0xff;   //  match 2 mask                                        
	app_command_buf[9]  = 0x01;   // enable match 2, match 2��ֵ��ͬ���ֵ����ľ���  , and function   
	app_command_buf[10] = 'w';   //     match 3 ��ֵ                                        
	app_command_buf[11]  =0xff;;    //  match 3 mask                                        
	app_command_buf[12]  =0x02;;    // enable match 3, match 3��ֵ��ͬ���ֵ����ľ���    and function 
	app_command_buf[13]  = 'x';    //   match 4 ��ֵ                                        
	app_command_buf[14]  = 0xff;   //   match 4 mask                                        
	app_command_buf[15]  = 0x03;   //  enable match 4, match 4��ֵ��ͬ���ֵ����ľ���    and function 
        spi_write(16, app_command_buf); 
        
	rf_init_freq();
}        

void GPIO_SET(unsigned char data)
{
	unsigned char app_command_buf[7];
	
	app_command_buf[0] = 0x13;     // gpio ����
	
	switch(data)
	{
		case tx_normal:
		case rx_normal:
			app_command_buf[1]  = 3; 		// GPIO0=1
			app_command_buf[2]  = 2;		// GPIO1=0
		break;
		
//		case tx_test:
//			app_command_buf[1]  = 3; 		// GPIO0=1
//			app_command_buf[2]  = 3;		// GPIO1=1
//		break;
//		
//		case rx_test:
//			app_command_buf[1]  = 2; 		// GPIO0=0
//			app_command_buf[2]  = 20;		// GPIO1=RX DATA
//		break;
//		
//		case rf_off:
//			app_command_buf[1]  = 2; 		// GPIO0=0
//			app_command_buf[2]  = 2;		// GPIO1=0
//		break;			 
	}
	
	app_command_buf[3]  = 0x21;  //0x20;   //  gpio2, h = tx mode
	app_command_buf[4]  = 0x20; // 0x14;  //   gpio3 
	app_command_buf[5]  = 0x27;   // nIRQ
	app_command_buf[6]  = 0x0b;  // sdo
	spi_write(7, app_command_buf);	
}

void fifo_reset(void)
{
	unsigned char p[2];
	
	p[0] = FIFO_INFO;
	p[1] = 0x03;   // reset tx ,rx fifo
	spi_write(2,p);
}	

void clr_interrupt(void)
{
	unsigned char p[4];
	
	p[0] = GET_INT_STATUS;
	p[1] = 0;   // clr  PH pending
	p[2] = 0;   // clr modem_pending
	p[3] = 0;   // clr chip pending
	spi_write(4,p);
	spi_read(9,GET_INT_STATUS); 
}
	
void enable_rx_interrupt(void)
{
	unsigned char p[7];

	p[0] = 0x11;
	p[1] = 0x01;  // 0x0100
	p[2] = 0x03;  // 3������
	p[3] = 0x00;   // 0100
	p[4] = 0x03;   // ph, modem int
	p[5] = 0x18; // 0x10;   // Pack received int
	p[6] = 0x00;   //preamble int, sync int setting	
	spi_write(0x07, p);  // enable  packet receive interrupt	
}	

void enable_tx_interrupt(void)
{	
	unsigned char p[6];

	p[0] = 0x11;
	p[1] = 0x01;  // 0x0100
	p[2] = 0x02;   // 2������
	p[3] = 0x00;  // 0x0100
	p[4] = 0x01;   // Ph  int
	p[5] = 0x20; //  enable  packet sent interrupt	
	spi_write(0x06, p);  // enable  packet receive interrupt	
}	

void rf_standby(void)
{
	unsigned char p[2];
	
	p[0] = CHANGE_STATE ; 	// CHANGE_STATE ����
	p[1] = 0x01 ; 			// sleep mode
	spi_write(2, p);  
} 

void tx_start(void)
{
	unsigned char p[5];
	
	p[0] = START_TX ; // start ����
	p[1] = freq_channel ; // channel 0
	p[2] = 0x30; // �������ص� Readyģʽ�� ���ط�����������
	p[3] = 0;
	p[4] = 0; //payload_length; // �ܹ�Ҫ����10���ֽ�	
	spi_write(5, p);  
} 

void rx_start(void)
{
	unsigned char p[8];
	
	p[0] = START_RX ; // start ����
	p[1] = freq_channel ; // channel 0
	p[2] = 0x00; // �����������ģʽ
	p[3] = 0;
	p[4] = 0;// payload_length; // �ܹ�Ҫ����10���ֽ�
	p[5] = 0;  // unchanged after preamble timeout 
	//p[6] = 0x03;  // ready after valid packet received
	//p[7] = 0x0;  // unchanged after invalid packet received 	
	p[6] = 0x08;  
	p[7] = 0x08;   	
	spi_write(8, p);  
}

void rx_start_test(void)
{
	unsigned char p[8];
	
	//LED_GREEN = 1;
	
	p[0] = START_RX ; // start ����
	p[1] = freq_channel ; // channel 0
	p[2] = 0x00; // �����������ģʽ
	p[3] = 0;
	//p[4] = payload_length; // �ܹ�Ҫ����10���ֽ�
	p[4] = 0;
	p[5] = 0;  // unchanged after preamble timeout 
	//p[6] = 0x0;  // unchanged mode if valid packet received
	//p[7] = 0x0;  // unchanged mode if unvalid packet received	
	p[6] = 0x08;  
	p[7] = 0x08;  
	spi_write(8, p);  
}
	
void rx_init(void)
{
	Flag.is_tx = 0;
	fifo_reset();  // ���������			
	enable_rx_interrupt();	
	clr_interrupt();  // ���ж�Factor	
	
//	if(mode == rx_test_mode)
//		rx_start_test();
//	else
		rx_start();    // �������ģʽ,���յ����ݺ󣬻ص�Readyģʽ
}	

void tx_data(void)
{	
	//unsigned char i;
	
	Flag.is_tx = 1;
	//LED_RED ^= 1;
	fifo_reset();  // ���������	
	spi_write_fifo();  // ��������������			
	enable_tx_interrupt();	
	clr_interrupt();  // ���ж�Factor	
	tx_start();    // ���뷢��ģʽ, ��ʼ����	
	rf_timeout = 0;
	Flag.rf_reach_timeout = 0;
	Flag.start_time  = HAL_GetTick();
	
	while(nIRQ)		// �ȴ��ж�
	{
		//wdtr = 0;
		//wdtr = 0xff;		
		if((HAL_GetTick()-Flag.start_time)  >=1000 )
		{
			sdn_reset();
 			SI4463_init();  // RF ģ���ʼ��
			break;		// ��ǿ������
		}			
	}
	
	tx_cnt++;
  	rx_init();		//rf ������ɣ��������ģʽ
}

//void tx_const_aa(void)
//{
//	//unsigned char i;
//	
//	//LED_RED = 1;
//	fifo_reset();  // ���������	
//	spi_write_fifo();  // ��������������			
//	enable_tx_interrupt();	
//	clr_interrupt();  // ���ж�Factor	
//	tx_start();    // ���뷢��ģʽ, ��ʼ����	
//	rf_timeout = 0;
//	Flag.rf_reach_timeout = 0;
//	
//	while(nIRQ)		// �ȴ��ж�
//	{
//	//	wdtr = 0;
//		//wdtr = 0xff;		
//	/*	if(Flag.rf_reach_timeout)
//		{
//			sdn_reset();
// 			SI4463_init();  // RF ģ���ʼ��
//			break;		// ��ǿ������
//		}		
//	}	
//}*/	

unsigned char spi_byte(unsigned char data)
{
	unsigned char res;
	HAL_SPI_TransmitReceive(&hspi1, &data, &res, 1, 100);
	/*for (i = 0; i < 8; i++)		// ����SCK �� SDI������һ���ֽڵ����ͬʱ��ȡ1���ֽڵ�����
	{				// û�а���nSEL�Ŀ���
		if (data & 0x80)
			SDI = 1;
		else
			SDI = 0;
			
		data <<= 1;
		SCK = 1;
		
		if (SDO)
			data |= 0x01;
		else
			data &= 0xfe;
			
		
	}	*/
	return (res);	
}
unsigned char check_cts(void)			// ����������������棬��ȡresponse
{
	unsigned char i;
   
	nSEL_H();   
	
	nSEL_L();
	spi_byte(0x44);
	i = spi_byte(0);
	nSEL_H();
	return (i);
}	

unsigned char check_id(unsigned char *p)			// ����������������棬��ȡresponse
{
	unsigned char i;
   
	nSEL_H();   
	
	nSEL_L();
	spi_byte(0x01);
	for(i=0;i<4;i++)
	p[i] = spi_byte(0);
	nSEL_H();
	return (i);
}	


void spi_write(unsigned char tx_length, unsigned char *p)
{
	unsigned char i,j;
	
	i = 0;
	while(i!=0xff)
		i = check_cts();
	
	//
	nSEL_L();
	
	for (i = 0; i < tx_length; i++) 
	{
		j = *(p+i);
		spi_byte(j);
	}
	
	nSEL_H();	
}




void spi_read(unsigned char data_length, unsigned char api_command )
{
	unsigned char i;
	
	unsigned char p[1];
	p[0] = api_command;
	i = 0;
	while(i!=0xff)
		i = check_cts();		// ����Ƿ���Է�����
		
	spi_write(1, p);    // ������
	
	i = 0;
	while(i!=0xff)
		i = check_cts();	//����Ƿ���Զ�������
		
	nSEL_H();   
	
	nSEL_L();
	spi_byte(0x44);
	for (i = 0; i< data_length; i++)	// ������
		spi_read_buf[i] = spi_byte(0xff);
	nSEL_H();
}	

void spi_fast_read( unsigned char api_command)
{
	unsigned char i,p[1];
	
	p[0] = api_command;
	i = 0;
	while(i!=0xff)
		i = check_cts();		// ����Ƿ���Է�����
		
	nSEL_H();   
	//
	nSEL_L();
	spi_byte(api_command);    // ������
	for (i = 0; i< 4; i++)
		spi_read_buf[i] = spi_byte(0xff);   // ��4���Ĵ�����ֵȫ����������
	nSEL_H();
}		

void spi_write_fifo(void)
{
	unsigned char i;
	
	i = 0;
	while(i!=0xff)
		i = check_cts();		// ����Ƿ���Է�����
	nSEL_H();   
	//
	nSEL_L();
	spi_byte(WRITE_TX_FIFO);    
	
//	if(mode == tx_test_mode)
//	{
//		for (i = 0; i< payload_length; i++)
//		{
//			spi_byte(tx_test_aa_data[i]);   // ����10����������
//		}
//	}	
//	else
	{
		for (i = 0; i< payload_length; i++)
		{
			spi_byte(tx_ph_data[i]);
		}	
	}
	nSEL_H();
}	

void spi_read_fifo(void)
{
	unsigned char i;
	
	i = 0;
	while(i!=0xff)
		i = check_cts();		// ����Ƿ���Է�����
		
	nSEL_H();   
	//
	nSEL_L();
	spi_byte(READ_RX_FIFO);    
	for (i = 0; i< payload_length; i++)
		rx_buf[i] = spi_byte(0xff);   // ����10����������
	nSEL_H();
}	

void sdn_reset(void)
{	
	unsigned char i;
	
 	SDN_H();
 	delay_1ms(2);	// RF ģ�鸴λ 	
 	SDN_L();
 	delay_1ms(10);	// ��ʱ10ms RF ģ����빤��״̬
 
 	nSEL_H();   
	//
	nSEL_L();
	for (i = 0; i< 7; i++)
		spi_byte(RF_POWER_UP_data[i]);   // ����power up ����
	nSEL_H();
 
 	delay_1ms(20);	// ��ʱ20ms RF ģ����빤��״̬, ���Ǻ��滹��Ҫ�ж� CTS, ����ʱ����ȥ�� 	
}	
// song end 




// joyce add


void rf_init_freq(void)
{	
	unsigned char OUTDIV,DIV,VCO,INTE,BAND;
	unsigned char FRAC_2,FRAC_1,FRAC_0;
	unsigned long FRAC,data,frequency;
	unsigned char app_command_buf[20];
	
	frequency = (freq3 * 100) + (freq2 * 10) + freq1;
	frequency = 4330000;
	
	if(frequency>=7600000)					// 760~1050
	{
		OUTDIV = 4;
		BAND = 0;
		VCO = 0xff;	
	}	
	else if(frequency>=5460000)				// 546~759.9
	{
		OUTDIV = 6;
		BAND = 1;
		VCO = 0xfe;	
	}
	else if(frequency>=3850000)				// 385~545.9
	{
		OUTDIV = 8;
		BAND = 2;
		VCO = 0xfe;	
	}
	else if(frequency>=2730000)				// 273~384.9
	{
		OUTDIV = 12;
		BAND = 3;
		VCO = 0xfd;	
	}
	else if(frequency>=1940000)				// 194~272.9
	{
		OUTDIV = 16;
		BAND = 4;
		VCO = 0xfc;	
	}
	else									// 142~193.9
	{
		OUTDIV = 24;
		BAND = 5;
		VCO = 0xfa;
	}
	
	DIV = OUTDIV/2;
	
	data = (frequency*DIV)/3;
	INTE = (data/100000)-1;
	FRAC = (data-(INTE+1)*100000)*16384/3125;
	FRAC = FRAC+0x80000;
	
	FRAC_0 = FRAC;
	FRAC>>=8;
	FRAC_1 = FRAC;
	FRAC>>=8;
	FRAC_2 = FRAC;
	
	// send freq cmd	
	
	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x20;    // 0x2051                                                         
	app_command_buf[2]  = 0x01;    // �� 1������                                                     
	app_command_buf[3]  = 0x51;   // 0x2051                                                          
	app_command_buf[4]  = 0x08|BAND;  //  high performance mode , clk outpu = osc /4
    spi_write(5, app_command_buf); 	
	
	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x40;    // 0x4000                                                         
	app_command_buf[2]  = 0x08;    // �� 8������                                                     
	app_command_buf[3]  = 0x00;   // 0x4000                                                          
	app_command_buf[4]  = INTE;   //  default value 
	app_command_buf[5]  = FRAC_2;   //  defaul value 
	app_command_buf[6]  = FRAC_1;   //  default value 
	app_command_buf[7]  = FRAC_0;   // frac ,from WDS
	app_command_buf[8]  = step_500K_step1;   // channel step1  from wds 
	app_command_buf[9]  = step_500K_step0;   // channel step0  from wds 
	app_command_buf[10] = 0x20;  //  from wds û�����ֵ   
	app_command_buf[11]  = VCO;    // from wds û�����ֵ 
    spi_write(12, app_command_buf); 
} 





void rf_init()
{
	freq3 = 1;
	power = 7;
	rate = 9;
	mode = tx_normal;
	rssi = 0;
 	sdn_reset();
	SI4463_init();  // RF ģ���ʼ��
 	tx_cnt=0;
 	rx_cnt=0;

}

void rf_proc()
{ 
	unsigned char  i,chksum;
	while(1) 	
	{ 	


	 	tx_data();		// ÿ���һ��ʱ�䣬����һ�����ݣ������� Acknowledge �ź�
		if(!Flag.is_tx)
		{
			if(!nIRQ)
			{ 
				clr_interrupt();   // ���ж�,�������жϱ�־	

				if((spi_read_buf[4] &0x08) == 0)  // crc error check
				{
					spi_read_fifo();
					fifo_reset();
                    
					chksum = 0;
					for(i=4;i<payload_length-1;i++)		// ����Checksum
					chksum += rx_buf[i];          	 		
                    
					if(( chksum == rx_buf[payload_length-1] )&&( rx_buf[4] == 0x41 ))  // ǰ��4���ֽ���Header
					{
						//LED_GREEN ^= 1;					// �����յ���		     					
						rx_cnt++;
						

//						if((mode == slave_mode)&&(set==0))
//						{
//							delay_1ms(100);  
//							tx_data();
//						}	     						 
					} 
					else
						rx_init();     // �յ������ݲ��ԣ���Ҫ������ʼ����
				}
				else
				{
					sdn_reset();
					SI4463_init();  // RF ģ���ʼ��
					rx_init();   // crc error
				}			
			}	
		}	
		
	}
}




