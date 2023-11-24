#include "modbus.h"
#include "uart.h"
#include "rs485.h"
#include "sys.h"
#include "config.h"

#include "crc.h"
modbus_stru modbus_recv;
modbus_stru modbus_tx;
void modbus_init()
{
    modbus_recv.address = 0;
    modbus_tx.address = 0;

}
modbus_stru *get_machine()
{
	return &modbus_recv;
}

/************************************************
func:解析rs485串口接收的数据
*************************************************/
void rs485_recv()
{

    rs485_stru *rs485_u;
    rs485_u =  get_uart_recv(RS485_No);

    if (rs485_u->recv_update == 1)
    {
        unsigned char i;
        for (i = 0; i < rs485_u->recv_len; i++)
        {
            if (rs485_u->buff[i] == DEV_ADDR)
                break;

        }
        unsigned int crc_tmp;
        unsigned int crc_cal;
        unsigned char index;
        unsigned char len;
        len = rs485_u->recv_len - i;
        index = rs485_u->recv_len - 2;
        //index = index-2;
        crc_tmp = rs485_u->buff[index];
        crc_tmp = crc_tmp << 8;
        crc_tmp = crc_tmp | rs485_u->buff[index - 1];
        crc_cal = CRC_Compute(&rs485_u->buff[i], len);
        memcpy(modbus_recv.payload, &rs485_u->buff[i], len);
        if (crc_cal == crc_tmp)
        {
            modbus_recv.address = modbus_recv.payload[0];
            modbus_recv.func =    modbus_recv.payload[1];
            modbus_recv.update = 1;
            /* switch (modbus_recv.func)
             {
                 case MODBUS_READ_CMD:
                     break;
                 case MODBUS_WRITE_ONE_CMD:
                     break;
                 case MODBUS_WRITE_MUL_CMD:
                     break;
            	default:break;
             }*/

        }
        rs485_u->recv_update = 0;
    }
}
void modbus_pack(unsigned char mode, unsigned char *pb)
{
    if (mode == MODBUS_READ_CMD)
    {

        //modbus_tx.func = pb;

    }
    else
    {
        //modbus_tx.func = get_congfig()->
    }
}
/****************************************************
func:modbus数据发送函数
*****************************************************/
void modbus_trans()
{
    unsigned char i;
    unsigned char pb[64];
    i = 0;
    pb[i++] = modbus_tx.address;
    pb[i++] = modbus_tx.func;

    memcpy(&pb[i], modbus_tx.reg, 2);
    i = i + 2;

    if (modbus_tx.func == MODBUS_WRITE_MUL_CMD || modbus_tx.func == MODBUS_READ_CMD)
    {
        memcpy(&pb[i], modbus_tx.regCount, 2);
        i = i + 2;

        memcpy(&pb[i], modbus_tx.regCount, 2);
        i = i + 2;

    }

    if (modbus_tx.func == MODBUS_WRITE_MUL_CMD || modbus_tx.func == MODBUS_WRITE_ONE_CMD)
    {
        memcpy(&pb[i], modbus_tx.payload, modbus_tx.regCount);
        i = i + modbus_tx.regCount;

    }

    pb[i++] = modbus_tx.crc;
    pb[i++] = modbus_tx.crc >> 8;
    i = i + 2;
    if (modbus_tx.retry_count > 0)
    {

        uart_transmit(RS485_No, pb, i);
        modbus_tx.retry_count--;
    }
    else
    {

        modbus_tx.update = 0;//发三次无响应，发送停止
        unsigned int tmp;
		tmp = 0;
		if(modbus_tx.address>0)
			tmp = 1<<(modbus_tx.address-1);
        modbus_recv.fault =modbus_recv.fault|tmp; 

    }

}

void analy_modbus_recv()
{
    if (modbus_recv.update == 1)
    {
        switch (modbus_recv.func)
        {
            case MODBUS_READ_CMD:
                break;
            case MODBUS_WRITE_ONE_CMD:
            {
                //modbus_tx.update = 0;
                modbus_tx.retry_count = 0;
            }
            break;
            case MODBUS_WRITE_MUL_CMD:
            {
                //modbus_tx.update = 0;
                modbus_tx.retry_count = 0;
            }
            break;

            break;
            default:
                break;
        }

    }

}
void modbus_proc()
{
    unsigned char pb[64];
    if (get_congfig()->update_setting == 1 || modbus_tx.update == 1) //pump set
    {
        //ctrl multi pump machine transmit data
        if (modbus_tx.update == 0)
        {
            modbus_tx.update = 1;
            modbus_tx.address = 0;
            modbus_tx.retry_count = 3;

        }

        if (modbus_tx.retry_count == 3 || modbus_tx.retry_count == 0)
        {
            modbus_tx.address ++;
            modbus_tx.retry_count = 3;
            if (modbus_tx.address > MODBUS_RTU_COUNT)
            {
                modbus_tx.address = 0;
                if (modbus_tx.retry_count == 0)
                    modbus_tx.update = 0;

            }
        }
        //*********************************************
        registerTick(MODBUS_TX_TICK_NO, 500);
        if (GetTickResult(MODBUS_TX_TICK_NO) == 1) //周期查询
        {
            modbus_pack(0, pb);
            modbus_trans();
            reset_registerTick(MODBUS_TX_TICK_NO);
        }


    }
    else //read pump every 3s
    {
        registerTick(MODBUS_TX_TICK_NO, 3000);
        if (GetTickResult(MODBUS_TX_TICK_NO) == 1) //周期查询
        {
            modbus_tx.address ++;
            if (modbus_tx.address > MODBUS_RTU_COUNT)
                modbus_tx.address = 0;
            modbus_pack(MODBUS_READ_CMD, pb);
            reset_registerTick(MODBUS_TX_TICK_NO);
            modbus_trans();

        }
        else
        {

            rs485_recv();

        }

    }
}
