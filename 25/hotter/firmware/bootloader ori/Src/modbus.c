#include "modbus.h"
#include "uart.h"
#include "rs485.h"

#include "crc.h"
modbus_stru modbus_recv;
modbus_tx_stru modbus_tx;
/************************************************
func:解析rs485串口接收的数据
*************************************************/
void rs485_recv()
{
	
   rs485_stru *rs485_u;
   rs485_u =  get_uart_recv(RS485_No);
   
	if(rs485_u->recv_update == 1)
	{
	    unsigned char i;
		for(i=0;i<rs485_u->recv_len;i++)
		{
			if(rs485_u->buff[i] == DEV_ADDR )
				break;

		}
		unsigned int crc_tmp;
		unsigned int crc_cal;
		unsigned char index;
		unsigned char len;
		len = rs485_u->recv_len-i;
		index = rs485_u->recv_len-2;
		//index = index-2;		
		crc_tmp = rs485_u->buff[index];
		crc_tmp = crc_tmp <<8;
		crc_tmp = crc_tmp|rs485_u->buff[index-1];
		crc_cal = CRC_Compute(&rs485_u->buff[i], len);
		memcpy(modbus_recv.payload,rs485_u->buff[i],len);
		if(crc_cal == crc_tmp)
		{
			modbus_recv.address = modbus_recv.payload[0];
			modbus_recv.func =    modbus_recv.payload[1];
			switch(modbus_recv.func)
			{
				case 0x01:break;
				case 0x02:break;
				case 0x03:break;				
			}
			
		}
     rs485_u->recv_update = 0;
	}
}
modbus_tx_stru*modbus_pack()
{
	return &modbus_tx;
}
/****************************************************
func:modbus数据发送函数
*****************************************************/
void modbus_trans()
{
  unsigned char len,i;
	unsigned char pb[64];
	i = 0;
	pb[i++] = modbus_tx.address;
	pb[i++] = modbus_tx.func;
	pb[i++] = modbus_tx.reg;
	pb[i++] = modbus_tx.regCount;
	memcpy(&pb[i],modbus_tx.payload,modbus_tx.regCount);
	pb[i++] = modbus_tx.crc>>8;
	pb[i++] = modbus_tx.crc;	
	i =i+2;
	i = modbus_tx.regCount + i;
    uart_transmit(RS485_No,pb ,i);
}
void modbus_proc()
{
	if(modbus_tx.update == 1 )
	{
		modbus_trans();
		modbus_tx.update = 0;
	}
	else
		rs485_recv();
}
