#include "modbus.h"
#include "uart.h"
#include "rs485.h"
#include "sys.h"
#include "config.h"
#include "equipment_ctrl.h"
#include "crc.h"


modbus_stru modbus_recv;
modbus_stru modbus_tx;

void analy_modbus_recv(void);

void modbus_init()
{
    modbus_recv.address = 0;
    modbus_tx.address = 0;

}
modbus_stru *get_recv_machine()
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
            if (rs485_u->recv_buf[i] > 0 && rs485_u->recv_buf[i] <= DEV_SIZE) //addr ok
            {
                modbus_recv.dev_addr_index = i;
                break;

            }

        }
        unsigned int crc_tmp;
        unsigned int crc_cal;
        unsigned char index;
        unsigned char len;
        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == MODBUS_READ_CMD)
        {
            len = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2]; //read data len
            crc_tmp = rs485_u->recv_buf[len + 4];
            crc_tmp = crc_tmp << 8;
            crc_tmp = crc_tmp | rs485_u->recv_buf[len + 3];
            crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 3);
        }
        else
        {
            if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == MODBUS_WRITE_ONE_CMD)
            {
                len = 2; //read data len
                crc_tmp = rs485_u->recv_buf[len + 5];
                crc_tmp = crc_tmp << 8;
                crc_tmp = crc_tmp | rs485_u->recv_buf[len + 4];
                crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 4);
            }
            else if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] ==
                     MODBUS_WRITE_MUL_CMD)
            {
                len = rs485_u->recv_buf[modbus_recv.dev_addr_index + 4]; //read data len
                len = len << 8;
                len = len | rs485_u->recv_buf[modbus_recv.dev_addr_index + 5]; //read data len

                crc_tmp = rs485_u->recv_buf[len + 7];
                crc_tmp = crc_tmp << 8;
                crc_tmp = crc_tmp | rs485_u->recv_buf[len + 6];
                crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], 6);
            }

        }



        if (crc_cal == crc_tmp)
        {
            memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
                   len + 3);
            modbus_recv.address = modbus_recv.payload[0];
            modbus_recv.func =    modbus_recv.payload[1];
            modbus_recv.update = 1;
            analy_modbus_recv();


        }
        memset(rs485_u->recv_buf, 0, BUFFER_SIZE);
        rs485_u->recv_update = 0;
    }
}

/****************************************************
func:modbus数据发送函数
return： 1 data tx;2:data tx completed and time out
*****************************************************/
unsigned char modbus_trans(unsigned char addr, unsigned char func,
                           unsigned int reg,
                           unsigned char *payload, unsigned int reg_count, unsigned char len,
                           unsigned char retry_times)
{
    unsigned char i, result = 0;
    unsigned char pb[64];
    i = 0;
    result = 0;

    if (modbus_tx.retry_count > 0)
    {
        if (func == MODBUS_READ_CMD)
        {

            modbus_tx.func = MODBUS_READ_CMD;
            modbus_tx.regCount = reg_count;
            modbus_tx.reg = reg;
            // modbus_tx.retry_count = reg_count;
            modbus_recv.reg = modbus_tx.reg;
        }
        else
        {
            modbus_tx.func = func;
            modbus_tx.reg = reg;
            modbus_tx.regCount = reg_count;
            memcpy(modbus_tx.payload, payload, len);
        }


        pb[i++] = modbus_tx.address;
        pb[i++] = modbus_tx.func;

        memcpy(&pb[i], modbus_tx.reg, 2);
        i = i + 2;

        if (modbus_tx.func == MODBUS_WRITE_MUL_CMD || modbus_tx.func == MODBUS_READ_CMD)
        {
            memcpy(&pb[i], modbus_tx.regCount, 2);
            i = i + 2;

        }

        if (modbus_tx.func == MODBUS_WRITE_MUL_CMD
                || modbus_tx.func == MODBUS_WRITE_ONE_CMD)
        {
            memcpy(&pb[i], modbus_tx.payload, modbus_tx.regCount * 2);
            i = i + modbus_tx.regCount * 2;

        }

        modbus_tx.crc = CRC_Compute(pb, i);
        pb[i++] = modbus_tx.crc;
        pb[i++] = modbus_tx.crc >> 8;
        i = i + 2;
        modbus_tx.retry_count--;
        uart_transmit(RS485_No, pb, i);
        result = 1;

    }
    else
    {
        result = 2;
    }
    return result;
}

void analy_modbus_recv()
{
    if (modbus_recv.update == 1)
    {
        switch (modbus_recv.func)
        {
            case MODBUS_READ_CMD:
            {
                if (modbus_recv.address > 0 && modbus_recv.address <= 3)
                {
                    modbus_recv.regCount = modbus_recv.payload[2];
                    memcpy(get_machine()->reg_data[modbus_recv.address - 1],
                           &modbus_recv.payload[3], modbus_recv.regCount);

                    //modbus_recv.reg = modbus_recv.reg<<8;
                    //modbus_recv.reg = modbus_recv.reg|modbus_recv.payload[3];

                    if (modbus_recv.reg == FAULT_REG)
                    {
                        unsigned int tmp;
                        tmp = modbus_recv.payload[3];
                        // tmp = tmp << 8;
                        //modbus_recv.reg = 0;

                        modbus_recv.fault = modbus_recv.fault | modbus_recv.payload[3];
                    }
                }


            };
            break;
            case MODBUS_WRITE_ONE_CMD:
            {
                modbus_tx.retry_count = 0;
            }
            break;
            case MODBUS_WRITE_MUL_CMD:
            {
                modbus_tx.retry_count = 0;
            }
            break;

            default:
                break;
        }
        modbus_recv.func = 0;
    }

}
/*************************************************
func：控制modbus的收发
return:
    0:no data tx;
    1:normal tx;
    2:server tx;
    3:pid
***************************************************/
unsigned char  modbus_ctrl()
{
    if (get_config()->update_setting == 1
            || (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1))
    {
		#if CTRL_EN
		    if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1)//every 180s，cal pid out
		    {
		        float u;
		        u = get_pid_output();
		        if (u == 0)
		        {
		            get_config()->machine = 0;//power off
		            get_config()->set_tout_tmp = 20; //set out temp
		        }
		        else
		            get_config()->set_tout_tmp = u;

		        modbus_tx.update = 3;//pid

		    }
		#endif
        if (get_config()->update_setting == 1)
        {
            modbus_tx.update = 2;  //server setting
            config_save();//位置不能变
            get_config()->update_setting = 0;

        }

        else
            modbus_tx.update = 1;//normal poll
        // modbus_tx.address = 0;
        //modbus_tx.retry_count = 3;//retry count
    }

    return modbus_tx.update;
}
void modbus_tx_proc(unsigned char mode)
{

    unsigned char pb[512];
	unsigned int len;
	len = 0;
    registerTick(MODBUS_TX_TICK_NO, 500);
    if (GetTickResult(MODBUS_TX_TICK_NO) == 1) //every 500ms tx
    {
        switch (mode)
        {
            case 1:
                memcpy(pb, get_config()->machine, 1);
                modbus_tx.func = MODBUS_WRITE_ONE_CMD;
                modbus_tx.reg = CONTROLLER_REG;
				len++;
                break;
            case 2:
                if (get_config()->set_tindoor < 15) //below 15 no pid ctrl
                {
                    memcpy(pb, get_config()->set_tout, 2);
                }
                else
                    memcpy(pb, get_config()->set_tout_tmp, 2);
                modbus_tx.func = MODBUS_WRITE_ONE_CMD;
                modbus_tx.reg = TEMPERATURE_REG;
				len++;
                break;
            case 3:
                modbus_tx.func = MODBUS_READ_CMD;
                modbus_tx.reg = FAULT_REG;
                len = 1;
                break;
        }


//tx proc ctrl
        if (modbus_tx.address > 0 && (modbus_recv.address != modbus_tx.address
                                      || modbus_recv.func != modbus_tx.func))//返回数据不对
        {


            if (modbus_tx.address <= MODBUS_RTU_COUNT)
            {
                if (modbus_tx.address < MODBUS_RTU_COUNT)
                {
                    modbus_trans(modbus_tx.address, modbus_tx.func, modbus_tx.reg, pb, len, 1, 3);
                    if (modbus_tx.retry_count == 0)
                    {
                        modbus_tx.retry_count = MODBUS_RETRY_COUNT;
                        unsigned int i;
                        i = (modbus_tx.address);
                        i = i << 8;
                        modbus_recv.fault = modbus_recv.fault | i;
                        modbus_tx.address = modbus_tx.address + 1;
                    }

                }


            }
        }
        else //data ack is ok tx next addr
        {
            if (modbus_tx.address <= MODBUS_RTU_COUNT)
            {
                modbus_tx.retry_count = MODBUS_RETRY_COUNT;
                modbus_tx.address = modbus_tx.address + 1;
                modbus_tx.retry_count = MODBUS_RETRY_COUNT;
                if (modbus_tx.address < MODBUS_RTU_COUNT)
                    modbus_trans(modbus_tx.address, modbus_tx.func, modbus_tx.reg, pb, len, 1, 3);

            }

        }
        if (modbus_tx.address > MODBUS_RTU_COUNT)
        {
            modbus_tx.retry_count = MODBUS_RETRY_COUNT;
            modbus_tx.address = 1;
            modbus_tx.update = 0;
            reset_registerTick(MODBUS_POLL_TICK_NO);

        }


        if (mode == 2)
            reset_registerTick(MODBUS_MQTT_PID_TICK_NO);
        reset_registerTick(MODBUS_TX_TICK_NO);

    }



}

void modbus_proc()
{
    unsigned char result, first_flag;
    result = modbus_ctrl();

    if (result == 2) //mqtt command
    {
        modbus_tx_proc(1);//tx
    }
    else
    {

        if (result == 3) //
        {
            if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1)
            {
                modbus_tx_proc(2);//tx//cal pid
            }

        }
        else
        {
            registerTick(MODBUS_POLL_TICK_NO, 3000);
            if (GetTickResult(MODBUS_POLL_TICK_NO) == 1)
            {
                modbus_tx_proc(3);///read dev
            }
        }
    }
    rs485_recv();
}
