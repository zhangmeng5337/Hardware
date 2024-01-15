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
            crc_cal = CRC_Compute(&rs485_u->recv_buf[i], len);
        }
        else
        {
            len = rs485_u->recv_buf[modbus_recv.dev_addr_index + 4]; //read data len
            len = len << 8; //read data len
            len = len | rs485_u->recv_buf[modbus_recv.dev_addr_index + 5]; //read data len

            crc_tmp = rs485_u->recv_buf[len + 4];
            crc_tmp = crc_tmp << 8;
            crc_tmp = crc_tmp | rs485_u->recv_buf[len + 3];
            crc_cal = CRC_Compute(&rs485_u->recv_buf[i], len);

        }



        if (crc_cal == crc_tmp)
        {
            memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
                   len);
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
*****************************************************/
unsigned char modbus_trans(unsigned char addr, unsigned char func, unsigned int reg,
                  unsigned char *payload, unsigned int reg_count, unsigned char len)
{
    unsigned char i, result = 0;
    unsigned char pb[64];
    i = 0;
    result = 0;


    if (func == MODBUS_READ_CMD)
    {

        modbus_tx.func = MODBUS_READ_CMD;
        modbus_tx.regCount = len;
        modbus_tx.reg = reg;
        modbus_tx.retry_count = reg_count;
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
    if (modbus_tx.retry_count > 0)
    {

        uart_transmit(RS485_No, pb, i);
        if (modbus_recv.address == modbus_tx.address
                && modbus_recv.func == modbus_tx.func)
        {
            modbus_tx.retry_count = 0;
            result = 1;

        }

        else
            modbus_tx.retry_count--;
    }
    else
    {
        unsigned int i;
        i = 1;
        i = i << modbus_recv.address;
        modbus_recv.fault = modbus_recv.fault | i;
        result = 1;

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
                    memcpy(get_machine()->reg_data[modbus_recv.address - 1],
                           &modbus_recv.payload[4], modbus_recv.regCount * 2);
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

    }

}
/*************************************************
func：控制modbus的收发
***************************************************/
unsigned char  modbus_ctrl()
{
    if (get_config()->update_setting == 1
            || (GetTickResult(MODBUS_TEMP_TX_TICK_NO) == 1))
    {
#if CTRL_EN
        if (GetTickResult(MODBUS_TEMP_TX_TICK_NO) == 1)//every 180s，cal pid out
        {
            float u;
            u = get_pid_output();
			//if(get_config()->set_tout)
            if (u == 0)
            {
                get_config()->machine = 0;//power off
                get_config()->set_tout_tmp = 20; //set out temp
            }
            else
                get_config()->set_tout_tmp = u;
        }
#endif
        if (get_config()->update_setting == 1)
        {
            modbus_tx.update = 2;
			config_save();//位置不能变
            get_config()->update_setting = 0;

        }

        else
            modbus_tx.update = 1;//tx flag
        // modbus_tx.address = 0;
        //modbus_tx.retry_count = 3;//retry count
    }

//    if (modbus_tx.update == 1) //pump set
//    {
//        if (modbus_tx.retry_count == 0)
//        {
//            if (modbus_tx.address < MODBUS_RTU_COUNT)
//            {
//                modbus_tx.retry_count = 3;
//                modbus_tx.address = modbus_tx.address + 1;
//            }
//            else
//            {
//                modbus_tx.retry_count = 3;
//                modbus_tx.address = 1;
//                modbus_tx.update = 0;
//            }
//        }
//    }
    return modbus_tx.update;
}

void modbus_proc()
{
    unsigned char pb[512];
    unsigned char result, first_flag;
    result = modbus_ctrl();
#if CTRL_EN

    if (result)//pid cal and every 180s
    {
        if (result == 1) //pid update
        {
            if (GetTickResult(MODBUS_TEMP_TX_TICK_NO) == 1) //180s,ctrl pump every 180s
            {

                if(get_config()->set_tindoor<15)//below 15 no pid ctrl
                {
                memcpy(pb, get_config()->set_tout, 2);

				}
				else 
					 memcpy(pb, get_config()->set_tout_tmp, 2);
                registerTick(MODBUS_TX_TICK_NO, 500);


                if (GetTickResult(MODBUS_TX_TICK_NO) == 1) //every 500ms tx
                {

                    if (first_flag == 0)
                    {
                        first_flag = 1;
                        modbus_tx.retry_count = 3;
                        modbus_tx.address = 1;
                    }
                    if (modbus_trans(0, MODBUS_WRITE_ONE_CMD, TEMPERATURE_REG, pb, 1, 1) == 1)
                    {
                        if (modbus_tx.address < MODBUS_RTU_COUNT)
                        {
                            modbus_tx.retry_count = 3;
                            modbus_tx.address = modbus_tx.address + 1;

                        }
                        else
                        {
                            modbus_tx.retry_count = 3;
                            modbus_tx.address = 1;
                            modbus_tx.update = 0;
							first_flag = 0;
                            reset_registerTick(MODBUS_TEMP_TX_TICK_NO);
                        }

                    }
                    reset_registerTick(MODBUS_TX_TICK_NO);

                }

            } //180s,ctrl pump every 180s


        }
        else if (result == 2)//mqtt command
        {
            registerTick(MODBUS_TX_TICK_NO, 500);
            memcpy(pb, get_config()->machine, 1);

            if (GetTickResult(MODBUS_TX_TICK_NO) == 1) //every 500ms tx
            {

                if (first_flag == 0)
                {
                    first_flag = 1;
                    modbus_tx.retry_count = 3;
                    modbus_tx.address = 1;
                }
                modbus_trans(modbus_tx.address, MODBUS_WRITE_ONE_CMD, CONTROLLER_REG, pb, 1, 1);
                    if (modbus_tx.address < MODBUS_RTU_COUNT)
                    {
                        modbus_tx.retry_count = 3;
                        modbus_tx.address = modbus_tx.address + 1;

                    }
                    else
                    {
                        modbus_tx.retry_count = 3;
                        modbus_tx.address = 1;
                        modbus_tx.update = 0;
						first_flag = 0;
                        reset_registerTick(MODBUS_TEMP_TX_TICK_NO);
                    }
                reset_registerTick(MODBUS_TX_TICK_NO);

            }


        }
  
//        if (GetTickResult(MODBUS_TEMP_TX_TICK_NO) == 1) //180s,ctrl pump every 180s
//        {
//
//            reset_registerTick(MODBUS_TEMP_TX_TICK_NO);
//            memcpy(pb, get_config()->set_tout_tmp, 2);
//            memcpy(pb, get_config()->machine, 1);
//
//            registerTick(MODBUS_TX_TICK_NO, 500);
//            if (GetTickResult(MODBUS_TX_TICK_NO) == 1)
//            {
//                reset_registerTick(MODBUS_TX_TICK_NO);
//                while (modbus_trans(0, MODBUS_WRITE_ONE_CMD, TEMPERATURE_REG, pb, 1, 1) == 1)
//                    ;
//
//            }
//
//
//        }

    }

    else //read pump every 3s
#endif

    {
        registerTick(MODBUS_TX_TICK_NO, 3000);
        if (GetTickResult(MODBUS_TX_TICK_NO) == 1) //周期查询
        {
			if (first_flag == 0)
			{
				first_flag = 1;
				modbus_tx.retry_count = 3;
				modbus_tx.address = 1;
			}
			if (modbus_trans(0, MODBUS_READ_CMD, CONTROLLER_REG, pb, 125, 125) == 1)
			{
				if (modbus_tx.address < MODBUS_RTU_COUNT)
				{
					modbus_tx.retry_count = 3;
					modbus_tx.address = modbus_tx.address + 1;
			
				}
				else
				{
					modbus_tx.retry_count = 3;
					modbus_tx.address = 1;
					modbus_tx.update = 0;
					first_flag = 0;
					reset_registerTick(MODBUS_TEMP_TX_TICK_NO);
				}
			
			}
			reset_registerTick(MODBUS_TX_TICK_NO);
        }
        else
        {
            rs485_recv();
        }

    }
}
