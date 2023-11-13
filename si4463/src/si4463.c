// 	Demo 程序适用  ：     SI4463, 
// 	RF 模块的参数设置为： +/-10PPM
//	使能AFC，使能CRC， PH + FIFO 模式
// 	每秒钟发射的固定测试数据为： 0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x6d, 
//  chksum:  0x6d = (0x41 +0x42 +0x43 +0x44 +0x45 +0x46 +0x47 + 0x48 +0x49)
// 	MCU : RENSAS high performance 8 bit MCU :R5f211B4, internal 8MHz
// 	src_field, 是指哪个field 包括 长度信息，长度信息在这个field 的最后一个或者2个字节
// 	dst-field 是指具体的那个field，指具体的内容。
// 	例如： 要将  1,2,3,4,5,6,7,8,9 这9个可变长度的数据放到field 4,那么可以设置:src-field = 1,field 1 包括长度信息，
//	在这个field的最后一个字节就是长度信息，也就是说最后一个字节 的内容是 9（长度）,Dst-field = 3;则意味着 field 3的内容是 1，2， 。。。。


#include "main.h"
#include "radio_config_si4463.h"
#include "radio_config_user.h"
#include "si4463.h"
#include "uart.h"

extern SPI_HandleTypeDef hspi1;



FlagType	Flag;

unsigned int	count_1hz, rf_timeout;
unsigned char spi_read_buf[20];   // 用于读取 SPI命令的返回数据内容
unsigned char rx_buf[25];

unsigned char mode;

// add joyce
const unsigned char Tab_DispCode[17]={0x77,0x24,0x6b,0x6d,0x3c,0x5d,0x5f,0x64,0x7f,0x7d,0x7e,0x1f,0x53,0x2f,0x5b,0x5a,0x00};
unsigned char 	dis_ram[28] = {00};

unsigned char	key_value;
unsigned char	have_set;
unsigned char 	set;		// 设置模式
unsigned char 	old_mode;
unsigned char 	mode;		 
unsigned char 	freq3;	
unsigned char 	freq2;	
unsigned char 	freq1;		// 载波频率
unsigned char 	power;		// 发射功率
unsigned char 	rssi;		// 接受信号强度
unsigned char 	rate;		// 速率

unsigned char	cnt_10s;

unsigned long   tx_cnt = 0,rx_cnt = 0; //收发次数
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
	
	// spi_write(0x05, RF_GLOBAL_XO_TUNE_1_data);   //  频率调整
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x00;    // 0x0000 
	app_command_buf[2]  = 0x01;    // 共 1个参数
	app_command_buf[3]  = 0x00;   // 0x0000
	app_command_buf[4]  = 98;  // freq  adjustment
	spi_write(5, app_command_buf);

	// spi_write(0x05, RF_GLOBAL_CONFIG_1_data);  //  *******************************************************
  	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x00;    // 0x0003 
	app_command_buf[2]  = 0x01;    // 共 1个参数
	app_command_buf[3]  = 0x03;   // 0x0003
	app_command_buf[4]  = 0x40;  // tx = rx = 64 byte, 普通 PH，high performance mode  ????????????????????? 
	spi_write(5, app_command_buf); 
    
    // *****************************************************************************    
    spi_write(0x08, RF_FRR_CTL_A_MODE_4_data);    // disable all fast response register
     
    // spi_write(0x0D, RF_PREAMBLE_TX_LENGTH_9_data); 前导码设置*********************************************   
 	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x10;    // 0x1000 
	app_command_buf[2]  = 0x09;    // 共 9个参数
	app_command_buf[3]  = 0x00;   // 0x1000
	
//	if(mode == tx_test_mode)
//	{
//		app_command_buf[4]  = 0xff;   //  需要发送255个字节的Preamble
//	}
//	else
	{
		app_command_buf[4]  = 0x08;   //  需要发送8个字节的Preamble
	}	
	
	//app_command_buf[4]  = 0x08;   //  需要发送8个字节的Preamble
	app_command_buf[5]  = 0x14;   //要检测20 bit 同步字，
	app_command_buf[6]  = 0x00;   // 非标准的preamble 设定，没有用
	app_command_buf[7]  = 0x0f;   // 前导码Timeout的时间   ????????????????????????????????????????????????????????????
	app_command_buf[8]  = 0x31;  // 前导码的长度以 byte 计算， 1st = 1，没有manchest  编码，使用标准的 1010.。。
	app_command_buf[9]  = 0x0;  	//  非标准的Preamble的 Patten 4th byte
	app_command_buf[10]  = 0x00;   //  非标准的Preamble的 Patten 3rd byte
	app_command_buf[11]  = 0x00;  //  非标准的Preamble的 Patten 2nd byte
	app_command_buf[12]  = 0x00;  //  非标准的Preamble的 Patten 1st byte
	spi_write(13, app_command_buf);  // 	
	
	//  RF_SYNC_CONFIG_5_data,  同步字的设置 *********************************************
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x11;    // command = 0x1100
	app_command_buf[2]  = 0x05;    // 共 5个参数
	app_command_buf[3]  = 0x00;   // command = 0x1100
	app_command_buf[4]  = 0x01;   //  同步字在 length field 定义并发送，同步字不能有错，不是 4FSK， 不是manchest 编码，只有 2个字节
	
//	if(mode == tx_test_mode)
//	{
//		app_command_buf[5]  = 0x55;   //同步字 3
//		app_command_buf[6]  = 0x55;   // 同步字2 
//	}	
//	else
	{
		app_command_buf[5]  = 0x2d;   //同步字 3
		app_command_buf[6]  = 0xd4;   // 同步字2 
	}
	
	app_command_buf[7]  = 0x00;   // 同步字1
	app_command_buf[8]  = 0x00;  // 同步字 0
    spi_write(9, app_command_buf);  //
        
	//  packet crc 设置 *******************************************************************        
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // command = 0x1200
	app_command_buf[2]  = 0x01;    // 共 1个参数
	app_command_buf[3]  = 0x00;   // command = 0x1200
	app_command_buf[4]  = 0x81; //   1是cRC 种子，CRC = itu-c, enable crc
    spi_write(5, app_command_buf);  
        
	// packet   gernale configuration        
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // command = 0x1206
	app_command_buf[2]  = 0x01;    // 共 1个参数
	app_command_buf[3]  = 0x06;   // command = 0x1206
	app_command_buf[4]  = 0x02;   //  tx = rx = 120d--1220 (tx packet，ph enable, not 4fsk, 收到一包数据后，RX off,CRC 不翻转 ，CRC MSB， data MSB
    spi_write(5, app_command_buf);  
        
  	// spi_write(0x07, RF_PKT_LEN_3_data);   //  固定长度，长度一个字节，长度信息没有在Payload里面
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // command = 0x1208
	app_command_buf[2]  = 0x03;    // 共 3个参数
	app_command_buf[3]  = 0x08;   // command = 0x1208
	app_command_buf[4]  = 0x00;   //  Length Field = LSB,  length 只有1个字节，length 不放在 FiFo里面，固定包长模式
	app_command_buf[5]  = 0x00;   //用于可变包长模式，定义哪个 Field 包含了 length Filed
	app_command_buf[6]  = 0x00;   // 用于可变包长模式，调整可变包长的 length
    spi_write(7, app_command_buf); 
    
        
	// length Field 1 -- 4 的设置 
	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // 0x120d 
	app_command_buf[2]  = 0x0c;    // 共 12个参数
	app_command_buf[3]  = 0x0d;   // 0x120d
	app_command_buf[4]  = 0x00;   //  Field 1 的长度 （高4位）
	app_command_buf[5]  = payload_length;   //  field 1 的长度， （低 8位), 总共14个字节
	app_command_buf[6]  = 0x04;   // field 1 不是 4FSK，manchest, whiting, PN9,   
	app_command_buf[7]  = 0xaa;   // field 1 crc enble, check enbale, 发射也有CRC，cRC的Seed 和CRC_seed里面的一样  
	app_command_buf[8]  = 0x00;  //  field2 的长度（高4位）
	app_command_buf[9]  = 0x00;  	//  field 2 的长度， （低 8位)， 长度 = 0 表示这个field 没有使用
	app_command_buf[10]  = 0x00;   // field 2 不是 4FSK，manchest, whiting, PN9
	app_command_buf[11]  = 0x00;  //  field 2 的CRC设置
	app_command_buf[12]  = 0x00;  //  field 3 的长度， （低 8位)， 长度 = 0 表示这个field 没有使用
	app_command_buf[13]  = 0x00;  	//  field 3 的长度， （低 8位)， 长度 = 0 表示这个field 没有使用
	app_command_buf[14]  = 0x00;   //  field 3 不是 4FSK，manchest, whiting, PN9
	app_command_buf[15]  = 0x00;  //  field 3 的CRC设置
	spi_write(16, app_command_buf);  // 
  
    // spi_write(0x0C, RF_PKT_FIELD_4_LENGTH_12_8_8_data);  **************************************
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x12;    // 0x1219 
	app_command_buf[2]  = 0x08;    // 共 8个参数
	app_command_buf[3]  = 0x19;   // 0x1219
	app_command_buf[4]  = 0x00;   //  field4 的长度（高4位）
	app_command_buf[5]  = 0x00;   //  field 4 的长度， （低 8位)， 长度 = 0 表示这个field 没有使用
	app_command_buf[6]  = 0x00;   //   field 4 不是 4FSK，manchest, whiting, PN9
	app_command_buf[7]  = 0x00;   // field 4 的CRC设置
	app_command_buf[8]  = 0x00;  //  field5 的长度（高4位）
	app_command_buf[9]  = 0x00;  	//   field 5 的长度， （低 8位)， 长度 = 0 表示这个field 没有使用
	app_command_buf[10]  = 0x00;   //  field 5 不是 4FSK，manchest, whiting, PN9
	app_command_buf[11]  = 0x00;   // field 5 的CRC设置
    spi_write(12, app_command_buf);  //
    
    // ********************************************************************************
    spi_write(0x10, RF_MODEM_MOD_TYPE_12_data);   // //  2FSK ,  module source = PH fifo, disable manchest, tx, rx 不翻转， deviation 不翻转
        
	//spi_write(0x05, RF_MODEM_FREQ_DEV_0_1_data); **************************************
    app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x20;    // 0x200c                                                         
	app_command_buf[2]  = 0x01;    // 共 1个参数                                                     
	app_command_buf[3]  = 0x0c;   // 0x200c                                                          
	//app_command_buf[4]  = 0x4f; // frequency deviation 的Bit 0--7, 根据WDS产生
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
    spi_write(0x0C, RF_MODEM_TX_RAMP_DELAY_8_data);    	// 没有这个参数，不需要
    spi_write(0x0D, RF_MODEM_BCR_OSR_1_9_data);			// 没有这个参数，不需要
    spi_write(0x0B, RF_MODEM_AFC_GEAR_7_data);			// 没有这个参数，不需要
    spi_write(0x05, RF_MODEM_AGC_CONTROL_1_data);		// 没有这个参数，不需要
    spi_write(0x0D, RF_MODEM_AGC_WINDOW_SIZE_9_data);	// 没有这个参数，不需要
    spi_write(0x0F, RF_MODEM_OOK_CNT1_11_data);			// 没有这个参数，不需要   
    
	// spi_write(0x05, RF_MODEM_RSSI_COMP_1_data);	**************************************************
	app_command_buf[0] = 0x11;  
	app_command_buf[1] = 0x20;    // 0x204e                                                         
	app_command_buf[2] = 0x01;    // 共 1个参数                                                     
	app_command_buf[3] = 0x4e;   // 0x204e 
	if(rate==dr_500)
		app_command_buf[4]  = 0x3a; 
	else                                                         
		app_command_buf[4]  = 0x40;  //  rssi 读数偏差，与真实值的差值
    spi_write(5, app_command_buf);
            	     
    // ********************************************************************************    
    spi_write(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE13_7_0_12_data);  // 没有这个参数，不需要
    spi_write(0x10, RF_MODEM_CHFLT_RX1_CHFLT_COE1_7_0_12_data);   // 没有这个参数，不需要
    spi_write(0x10, RF_MODEM_CHFLT_RX2_CHFLT_COE7_7_0_12_data);   // 没有这个参数，不需要        
        
	// RF_PA **************************************************************************
	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x22;    // 0x2200                                                         
	app_command_buf[2]  = 0x04;    // 共 4个参数                                                     
	app_command_buf[3]  = 0x00;   // 0x2200                                                          
	app_command_buf[4]  = 0x08;  //0x10;   //   PA mode  = default , 使用Class E 模式，而不是 Switch Current 模式  ????????
	if(power==7)	
		app_command_buf[5]  = 127;   //  设置为最大功率
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
	app_command_buf[6]  =0x00; //???????? 0x00;   // CLK duty = 50%， 其他 = Default
	if((rate<=dr_115p2)||(rate==dr_500))
		app_command_buf[7]  = 0x3d;  // ???????? 0x5d;   // PA ramp time = default 
    else
    	app_command_buf[7]  = 0x5d;    
    spi_write(8, app_command_buf);        
    
    // ********************************************************************************     
	spi_write(0x0B, RF_SYNTH_PFDCP_CPFF_7_data);      // 没有这个参数，不需要
        
   	// header match ******************************************************************
   	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x30;    // 0x3000                                                         
	app_command_buf[2]  = 0x0c;    // 共 12个参数                                                     
	app_command_buf[3]  = 0x00;   // 0x3000                                                          
	app_command_buf[4]  = 's';    //0x00;   //  match 1 的值
	app_command_buf[5]  = 0xff;   //  match 1 mask
	app_command_buf[6]  = 0x40;   // eable match 1, match 1的值与同步字的相距的距离, 0 = match 
	app_command_buf[7]  = 'w';    //  match 2 的值                                        
	app_command_buf[8]  = 0xff;   //  match 2 mask                                        
	app_command_buf[9]  = 0x01;   // enable match 2, match 2的值与同步字的相距的距离  , and function   
	app_command_buf[10] = 'w';   //     match 3 的值                                        
	app_command_buf[11]  =0xff;;    //  match 3 mask                                        
	app_command_buf[12]  =0x02;;    // enable match 3, match 3的值与同步字的相距的距离    and function 
	app_command_buf[13]  = 'x';    //   match 4 的值                                        
	app_command_buf[14]  = 0xff;   //   match 4 mask                                        
	app_command_buf[15]  = 0x03;   //  enable match 4, match 4的值与同步字的相距的距离    and function 
        spi_write(16, app_command_buf); 
        
	rf_init_freq();
}        

void GPIO_SET(unsigned char data)
{
	unsigned char app_command_buf[7];
	
	app_command_buf[0] = 0x13;     // gpio 设置
	
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
	p[2] = 0x03;  // 3个参数
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
	p[2] = 0x02;   // 2个参数
	p[3] = 0x00;  // 0x0100
	p[4] = 0x01;   // Ph  int
	p[5] = 0x20; //  enable  packet sent interrupt	
	spi_write(0x06, p);  // enable  packet receive interrupt	
}	

void rf_standby(void)
{
	unsigned char p[2];
	
	p[0] = CHANGE_STATE ; 	// CHANGE_STATE 命令
	p[1] = 0x01 ; 			// sleep mode
	spi_write(2, p);  
} 

void tx_start(void)
{
	unsigned char p[5];
	
	p[0] = START_TX ; // start 命令
	p[1] = freq_channel ; // channel 0
	p[2] = 0x30; // 发射完后回到 Ready模式， 不重发，立即发射
	p[3] = 0;
	p[4] = 0; //payload_length; // 总共要发射10个字节	
	spi_write(5, p);  
} 

void rx_start(void)
{
	unsigned char p[8];
	
	p[0] = START_RX ; // start 命令
	p[1] = freq_channel ; // channel 0
	p[2] = 0x00; // 立即进入接收模式
	p[3] = 0;
	p[4] = 0;// payload_length; // 总共要接收10个字节
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
	
	p[0] = START_RX ; // start 命令
	p[1] = freq_channel ; // channel 0
	p[2] = 0x00; // 立即进入接收模式
	p[3] = 0;
	//p[4] = payload_length; // 总共要接收10个字节
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
	fifo_reset();  // 缓冲器清空			
	enable_rx_interrupt();	
	clr_interrupt();  // 清中断Factor	
	
//	if(mode == rx_test_mode)
//		rx_start_test();
//	else
		rx_start();    // 进入接收模式,接收到数据后，回到Ready模式
}	

void tx_data(void)
{	
	//unsigned char i;
	
	Flag.is_tx = 1;
	//LED_RED ^= 1;
	fifo_reset();  // 缓冲器清空	
	spi_write_fifo();  // 往缓冲区填数据			
	enable_tx_interrupt();	
	clr_interrupt();  // 清中断Factor	
	tx_start();    // 进入发射模式, 开始发射	
	rf_timeout = 0;
	Flag.rf_reach_timeout = 0;
	Flag.start_time  = HAL_GetTick();
	
	while(nIRQ)		// 等待中断
	{
		//wdtr = 0;
		//wdtr = 0xff;		
		if((HAL_GetTick()-Flag.start_time)  >=1000 )
		{
			sdn_reset();
 			SI4463_init();  // RF 模块初始化
			break;		// 则强制跳出
		}			
	}
	
	tx_cnt++;
  	rx_init();		//rf 发射完成，进入接收模式
}

//void tx_const_aa(void)
//{
//	//unsigned char i;
//	
//	//LED_RED = 1;
//	fifo_reset();  // 缓冲器清空	
//	spi_write_fifo();  // 往缓冲区填数据			
//	enable_tx_interrupt();	
//	clr_interrupt();  // 清中断Factor	
//	tx_start();    // 进入发射模式, 开始发射	
//	rf_timeout = 0;
//	Flag.rf_reach_timeout = 0;
//	
//	while(nIRQ)		// 等待中断
//	{
//	//	wdtr = 0;
//		//wdtr = 0xff;		
//	/*	if(Flag.rf_reach_timeout)
//		{
//			sdn_reset();
// 			SI4463_init();  // RF 模块初始化
//			break;		// 则强制跳出
//		}		
//	}	
//}*/	

unsigned char spi_byte(unsigned char data)
{
	unsigned char res;
	HAL_SPI_TransmitReceive(&hspi1, &data, &res, 1, 100);
	/*for (i = 0; i < 8; i++)		// 控制SCK 和 SDI，发射一个字节的命令，同时读取1个字节的数据
	{				// 没有包括nSEL的控制
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
unsigned char check_cts(void)			// 这个命令跟在命令后面，读取response
{
	unsigned char i;
   
	nSEL_H();   
	
	nSEL_L();
	spi_byte(0x44);
	i = spi_byte(0);
	nSEL_H();
	return (i);
}	

unsigned char check_id(unsigned char *p)			// 这个命令跟在命令后面，读取response
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
		i = check_cts();		// 检查是否可以发命令
		
	spi_write(1, p);    // 发命令
	
	i = 0;
	while(i!=0xff)
		i = check_cts();	//检查是否可以读数据了
		
	nSEL_H();   
	
	nSEL_L();
	spi_byte(0x44);
	for (i = 0; i< data_length; i++)	// 读数据
		spi_read_buf[i] = spi_byte(0xff);
	nSEL_H();
}	

void spi_fast_read( unsigned char api_command)
{
	unsigned char i,p[1];
	
	p[0] = api_command;
	i = 0;
	while(i!=0xff)
		i = check_cts();		// 检查是否可以发命令
		
	nSEL_H();   
	//
	nSEL_L();
	spi_byte(api_command);    // 发命令
	for (i = 0; i< 4; i++)
		spi_read_buf[i] = spi_byte(0xff);   // 将4个寄存器的值全部都读出来
	nSEL_H();
}		

void spi_write_fifo(void)
{
	unsigned char i;
	
	i = 0;
	while(i!=0xff)
		i = check_cts();		// 检查是否可以发命令
	nSEL_H();   
	//
	nSEL_L();
	spi_byte(WRITE_TX_FIFO);    
	
//	if(mode == tx_test_mode)
//	{
//		for (i = 0; i< payload_length; i++)
//		{
//			spi_byte(tx_test_aa_data[i]);   // 发送10个测试数据
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
		i = check_cts();		// 检查是否可以发命令
		
	nSEL_H();   
	//
	nSEL_L();
	spi_byte(READ_RX_FIFO);    
	for (i = 0; i< payload_length; i++)
		rx_buf[i] = spi_byte(0xff);   // 接收10个测试数据
	nSEL_H();
}	

void sdn_reset(void)
{	
	unsigned char i;
	
 	SDN_H();
 	delay_1ms(2);	// RF 模块复位 	
 	SDN_L();
 	delay_1ms(10);	// 延时10ms RF 模块进入工作状态
 
 	nSEL_H();   
	//
	nSEL_L();
	for (i = 0; i< 7; i++)
		spi_byte(RF_POWER_UP_data[i]);   // 发送power up 命令
	nSEL_H();
 
 	delay_1ms(20);	// 延时20ms RF 模块进入工作状态, 但是后面还是要判断 CTS, 此延时可以去掉 	
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
	app_command_buf[2]  = 0x01;    // 共 1个参数                                                     
	app_command_buf[3]  = 0x51;   // 0x2051                                                          
	app_command_buf[4]  = 0x08|BAND;  //  high performance mode , clk outpu = osc /4
    spi_write(5, app_command_buf); 	
	
	app_command_buf[0] = 0x11;  
	app_command_buf[1]  = 0x40;    // 0x4000                                                         
	app_command_buf[2]  = 0x08;    // 共 8个参数                                                     
	app_command_buf[3]  = 0x00;   // 0x4000                                                          
	app_command_buf[4]  = INTE;   //  default value 
	app_command_buf[5]  = FRAC_2;   //  defaul value 
	app_command_buf[6]  = FRAC_1;   //  default value 
	app_command_buf[7]  = FRAC_0;   // frac ,from WDS
	app_command_buf[8]  = step_500K_step1;   // channel step1  from wds 
	app_command_buf[9]  = step_500K_step0;   // channel step0  from wds 
	app_command_buf[10] = 0x20;  //  from wds 没有这个值   
	app_command_buf[11]  = VCO;    // from wds 没有这个值 
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
	SI4463_init();  // RF 模块初始化
 	tx_cnt=0;
 	rx_cnt=0;

}

void rf_proc()
{ 
	unsigned char  i,chksum;
	while(1) 	
	{ 	


	 	tx_data();		// 每间隔一段时间，发射一包数据，并接收 Acknowledge 信号
		if(!Flag.is_tx)
		{
			if(!nIRQ)
			{ 
				clr_interrupt();   // 清中断,并读出中断标志	

				if((spi_read_buf[4] &0x08) == 0)  // crc error check
				{
					spi_read_fifo();
					fifo_reset();
                    
					chksum = 0;
					for(i=4;i<payload_length-1;i++)		// 计算Checksum
					chksum += rx_buf[i];          	 		
                    
					if(( chksum == rx_buf[payload_length-1] )&&( rx_buf[4] == 0x41 ))  // 前面4个字节是Header
					{
						//LED_GREEN ^= 1;					// 数据收到了		     					
						rx_cnt++;
						

//						if((mode == slave_mode)&&(set==0))
//						{
//							delay_1ms(100);  
//							tx_data();
//						}	     						 
					} 
					else
						rx_init();     // 收到的数据不对，则要继续开始接收
				}
				else
				{
					sdn_reset();
					SI4463_init();  // RF 模块初始化
					rx_init();   // crc error
				}			
			}	
		}	
		
	}
}




