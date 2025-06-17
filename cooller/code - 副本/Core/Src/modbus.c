#include "modbus.h"
#include "crc.h"
#include "uart.h"
//modbus_stru modbus_usr;
modbus_pack_stru modbus_recv;
modbus_pack_stru modbus_tx;

uart_recv_stru *uart_usr;
unsigned char  Modbus_03_Solve(void)
{
   modbus_recv.RS485_Addr = uart_usr->uartRecBuff[0];
	modbus_recv.func = uart_usr->uartRecBuff[1];
	modbus_recv.len =  uart_usr->uartRecBuff[2];
	memset(modbus_recv.modbus_data,0,32);
	memcpy(modbus_recv.modbus_data,&uart_usr->uartRecBuff[3],modbus_recv.len);
	return 3;
}
unsigned char Modbus_06_Solve(void)
{	
    modbus_recv.RS485_Addr = uart_usr->uartRecBuff[0];
	modbus_recv.func = uart_usr->uartRecBuff[1];
//	modbus_recv.len =  uart_usr.uartRecBuff[2];
	memset(modbus_recv.modbus_data,0,32);
	memcpy(modbus_recv.modbus_data,&uart_usr->uartRecBuff[2],4);
	return 6;
}
unsigned char Modbus_16_Solve(void)
{	
    modbus_recv.RS485_Addr = uart_usr->uartRecBuff[0];
	modbus_recv.func = uart_usr->uartRecBuff[1];
//	modbus_recv.len =  uart_usr.uartRecBuff[2];
	memset(modbus_recv.modbus_data,0,32);
	memcpy(modbus_recv.modbus_data,&uart_usr->uartRecBuff[2],4);
	return 16;
}
modbus_pack_stru *get_modbus_recv_dat()
{
	return &modbus_recv;
}
unsigned char RS485_Service(void)
{
    unsigned int recCRC,calCRC,rx_cnt;
    unsigned char rs485_result;
	
    rs485_result = 1;
	uart_usr = get_uart_recv_stru(UART_AIR);
    if(uart_usr->uartRecFlag==1)
    {
        if(uart_usr->uartRecBuff[0]>= MIN_ADDR &&
                uart_usr->uartRecBuff[0]<= MAX_ADDR)//地址正确
        {
            if((uart_usr->uartRecBuff[1]==03)||(uart_usr->uartRecBuff[1]==06)||(uart_usr->uartRecBuff[1]==16))//功能码正确
            {
                rx_cnt = uart_usr->uartRecBuff[2]+5;
                calCRC=CRC_Compute(uart_usr->uartRecBuff,rx_cnt-2);//计算所接收数据的CRC
                recCRC=uart_usr->uartRecBuff[rx_cnt-1]|(((unsigned int)uart_usr->uartRecBuff[rx_cnt-2])<<8);//接收到的CRC(低字节在前，高字节在后)
                if(calCRC==recCRC)//CRC校验正确
                {
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                    switch(uart_usr->uartRecBuff[1])//根据不同的功能码进行处理
                    {
                    case 3: //读寄存器
                    {
                        rs485_result =Modbus_03_Solve();
                        break;
                    }
                    case 6: //读寄存器
                    {
                        rs485_result = Modbus_06_Solve();
                        break;
                    }

                    case 16: //读寄存器
                    {
                        rs485_result = Modbus_16_Solve();
                        break;
                    }



                    }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                }

            }
        }
        uart_usr->uartRecFlag=0;
    }
    return rs485_result;

}
void modbus_pack(unsigned char addr,unsigned char*p,unsigned char len)
{
   unsigned int calCRC;
   unsigned char i;
	modbus_tx.RS485_Addr = addr;
	modbus_tx.func = p[0];
	memcpy(modbus_tx.modbus_txdata,&p[1],len-1);
   
    i = 0;


    modbus_tx.modbus_txdata[i++]= addr;
	memcpy(modbus_tx.modbus_txdata,&p[1],len-1);
	i=len;
    calCRC=CRC_Compute(modbus_tx.modbus_txdata,len);

    modbus_tx.modbus_txdata[i++]=(calCRC>>8)&0xFF;
    modbus_tx.modbus_txdata[i++]=(calCRC)&0xFF;
    uart_transmit(UART_AIR, modbus_tx.modbus_txdata, i);
}
