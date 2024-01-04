#include "modbus.h"
#include "crc.h"
#include "uart.h"
#include "string.h"
#include "stdlib.h"
#include "tm7780.h"


modbus_stru modbus_recv;
//modbus_stru modbus_tx;
unsigned  char analy_modbus_recv(void);


//void modbus_init()
//{
//    modbus_recv.address = 0;
//    modbus_tx.address = 0;

//}
//modbus_stru *get_recv_machine()
//{
//    return &modbus_recv;
//}

/************************************************
func:解析rs485串口接收的数据
*************************************************/
unsigned char rs485_recv()
{
    unsigned int crc_tmp;
    unsigned int crc_cal;
    unsigned char index;
    unsigned char len;
    unsigned char status ;

    uart_stru *uart_usr_modbus;
    status = 0;
    uart_usr_modbus =  get_uart();

    if (uart_usr_modbus->recv_flag == 1)
    {
        unsigned char i;
        for (i = 0; i < uart_usr_modbus->wptr; i++)
        {
            if (uart_usr_modbus->recv_buf[i] == DEV_ADDR) //addr ok
                break;

        }

        len = uart_usr_modbus->wptr - i;
        index = uart_usr_modbus->wptr - 2;
        //index = index-2;
        crc_tmp = uart_usr_modbus->recv_buf[index];
        crc_tmp = crc_tmp << 8;
        crc_tmp = crc_tmp | uart_usr_modbus->recv_buf[index - 1];
        crc_cal = CRC_Compute(&uart_usr_modbus->recv_buf[i], len);
        memcpy(modbus_recv.payload, &uart_usr_modbus->recv_buf[i], len);
       // if (crc_cal == crc_tmp)
        {
            modbus_recv.address = modbus_recv.payload[0];
            modbus_recv.func =    modbus_recv.payload[1];
            status = analy_modbus_recv();
			memset(uart_usr_modbus->recv_buf, 0, BUFFER_SIZE);
			uart_usr_modbus->recv_flag = 0;
			uart_usr_modbus->wptr =0;

        }
	   if(uart_usr_modbus->wptr >=14)
	   	{
		   memset(uart_usr_modbus->recv_buf, 0, BUFFER_SIZE);
		   uart_usr_modbus->recv_flag = 0;

	   }

    }
    return status ;
}

/****************************************************
func:modbus数据发送函数
*****************************************************/
void modbus_proc(unsigned char mode)
{
    unsigned char i;
    unsigned int crc;
    unsigned char pb[64];
    i = 0;


    if (mode == POWER_REQUEST)
    {
        pb[i++] = DEV_ADDR;
        pb[i++] = MODBUS_READ_CMD;
        pb[i++] = 0;
        pb[i++] = 6;
        //pb[i++] = (unsigned char)get_power()->U16_AC_V;

        memcpy(&pb[i], &get_power()->U16_AC_V, 2);
        i = i + 2;
        memcpy(&pb[i], &get_power()->U16_AC_I, 2);
        i = i + 2;
        memcpy(&pb[i], &get_power()->U16_AC_P, 2);
        i = i + 2;

        crc = CRC_Compute(pb, i);
        pb[i++] = crc;
        pb[i++] = crc >> 8;
        //i = i + 2;
        UartSendbyte(pb, i);
    }
    else if (mode == POWER_CALI)
    {
	pb[i++] = DEV_ADDR;
	pb[i++] = POWER_CALI;
	pb[i++] = 0;
	pb[i++] = 0;
	//pb[i++] = (unsigned char)get_power()->U16_AC_V;
	
//	memcpy(&pb[i], &get_power()->U16_AC_V, 2);
//	i = i + 2;
//	memcpy(&pb[i], &get_power()->U16_AC_I, 2);
//	i = i + 2;
//	memcpy(&pb[i], &get_power()->U16_AC_P, 2);
//	i = i + 2;
	
	crc = CRC_Compute(pb, i);
	pb[i++] = crc;
	pb[i++] = crc >> 8;
	//i = i + 2;
	UartSendbyte(pb, i);

    }


}

unsigned  char analy_modbus_recv()
{
    unsigned char result ;
    result = 0;
    switch (modbus_recv.func)
    {
        case MODBUS_READ_CMD:
        {
            result = 1;
        };
        break;
        case POWER_CALI:
        {
            result = 2;
        }
        break;
        default:
            break;
    }
    return result;

}

