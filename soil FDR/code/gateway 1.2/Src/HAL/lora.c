#include "lora.h"
#include "bsp.h"
LORA_Params_stru LORA_Master;
LORA_Params_stru LORA_Slave;
uart_set_stru uart_set;
lora_usr_stru lora_usr_status;
extern uart_data_stru uart_data;
void LoraUartProcess()
{

	if(uart_data.receivedFlag&0x01)//nbiot
	{
	
	}
	else if(uart_data.receivedFlag&0x02)//lora master
	{
	
	}	
	else if(uart_data.receivedFlag&0x10)//lora slave
	{
	
	}	
}
void LoraParamsAnaly(LORA_Params_stru p)
{
	lora_usr_status.uart_rate = uart_set.uart_rate;
		switch(LORA_Master.reg0&0x07)//lora空中速度波特率
		{
			case 0x00:lora_usr_status.air_rate = 300;break;
			case 0x01:lora_usr_status.air_rate = 1200;break;
			case 0x02:lora_usr_status.air_rate = 2400;break;
			case 0x03:lora_usr_status.air_rate = 4800;break;
			case 0x04:lora_usr_status.air_rate = 9600;break;
			case 0x05:lora_usr_status.air_rate = 19200;break;
			case 0x06:lora_usr_status.air_rate = 38400;break;
			case 0x07:lora_usr_status.air_rate = 62500;break;	
      default: 	lora_usr_status.air_rate = 2400;break;		
		}
		switch(LORA_Master.reg1&0xc0)//lora分包设定
		{
			case 0x00:lora_usr_status.payloadSize = 240;break;
			case 0x40:lora_usr_status.payloadSize = 128;break;
			case 0x80:lora_usr_status.payloadSize = 64;break;
			case 0xc0:lora_usr_status.payloadSize = 32;break;
			default:lora_usr_status.payloadSize = 240;
		}	
		
		switch(LORA_Master.reg1&0x20)//loraRSSI 环境噪声使能
		{
			case 0x00:lora_usr_status.rssiNoise = 0;break;
			case 0x20:lora_usr_status.rssiNoise = 1;break;
			default: lora_usr_status.rssiNoise = 0;break;
		}	
		switch(LORA_Master.reg1&0x03)//lora发射功率
		{
			case 0x00:lora_usr_status.power = 30;break;
			case 0x01:lora_usr_status.power = 27;break;
			case 0x02:lora_usr_status.power = 24;break;
			case 0x03:lora_usr_status.power = 21;break;		
			default: lora_usr_status.power = 30;break;
		}
		if(LORA_Master.reg2<84)//lora信道控制(CH)
		{
			lora_usr_status.channel = LORA_Master.reg2;
		}	
		
		if(LORA_Master.reg3&0x80)//lora启用rssi
		{
			lora_usr_status.rssi = 1;
		}		
		else
			lora_usr_status.rssi = 0;
		
		if(LORA_Master.reg3&0x40)//lora传输方式
		{
			lora_usr_status.txMode = 1;
		}		
		else
			lora_usr_status.txMode = 0;
		
		if(LORA_Master.reg3&0x20)//lora中继功能
		{
			lora_usr_status.relay = 1;
		}		
		else
			lora_usr_status.relay = 0;

		if(LORA_Master.reg3&0x10)//loraLBT
		{
			lora_usr_status.LBT = 1;
		}		
		else
			lora_usr_status.LBT = 0;	

		if(LORA_Master.reg3&0x08)//lora word
		{
			lora_usr_status.wor = 1;
		}		
		else
			lora_usr_status.wor = 0;	

		switch(LORA_Master.reg3&0x07)//lora period
		{
			case 0x00:lora_usr_status.worPeriod = 500;break;
			case 0x01:lora_usr_status.worPeriod = 1000;break;
			case 0x02:lora_usr_status.worPeriod = 1500;break;
			case 0x03:lora_usr_status.worPeriod = 2000;break;
			case 0x04:lora_usr_status.worPeriod = 2500;break;	
			case 0x05:lora_usr_status.worPeriod = 3000;break;	
			case 0x06:lora_usr_status.worPeriod = 3500;break;
			case 0x07:lora_usr_status.worPeriod = 4000;break;			
		}		
     lora_usr_status.crptH = 	LORA_Master.crptH;	
     lora_usr_status.crptL = 	LORA_Master.crptL;
		
}

void loraTransmit(unsigned char lora_num,unsigned char mode,unsigned char *q,unsigned char len,unsigned char ack)
{
	if(lora_num == LORA_MASTER)
	switch(mode)
	{
		case TX_MODE: loraGpioSet(LORA_MASTER,mode);
		              lora_uart_tx(LORA_MASTER,q,len); break;
		case WOR_MODE: break;		
		case CONFIG_MODE: break;
		case SLEEP_MODE: break;			
	} 
	if(ack == 1)
	{
		
	}
}
void LoraModeConfig(unsigned char lora_num,unsigned char mode,unsigned char startaddr,unsigned char len,unsigned char wr)
{
	unsigned char *p;
	unsigned char *tmp;	
	unsigned char len_sum;
	unsigned char ack;
	ack = 0;
	len = 0;
	if(wr == 0)
		p[len_sum++] = 0xc0;
	else
		p[len_sum++] = 0xc1;	
	if(lora_num == LORA_MASTER)
  memcpy(tmp,&LORA_Master,9);	
	else
		memcpy(tmp,&LORA_Slave,9);		
  memcpy(p+1,tmp+startaddr,len);
  len_sum = len_sum + len; 	
	ack = 1;
	loraTransmit(lora_num,mode,p,len_sum,ack);
}

void LORA_Params_Init()
{

}

void LORA_Params_Set(unsigned char p)
{
	if(p == LORA_MASTER)
	{
		switch(LORA_Master.reg0&0xe0)//lora串口波特率
		{
			case 0x00:uart_set.uart_rate = 1200;break;
			case 0x20:uart_set.uart_rate = 2400;break;
			case 0x40:uart_set.uart_rate = 4800;break;
			case 0x60:uart_set.uart_rate = 9600;break;
			case 0x80:uart_set.uart_rate = 19200;break;
			case 0xa0:uart_set.uart_rate = 38400;break;
			case 0xc0:uart_set.uart_rate = 57600;break;
			case 0xe0:uart_set.uart_rate = 115200;break;			
		}
		
		
		uart_set.uart_Num = 2;
		uart_init(uart_set);
		LoraParamsAnaly(LORA_Master);
	}
	else
	{
	
	}
}