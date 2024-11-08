#include "modbus.h"
#include "uart.h"
#include "rs485.h"
#include "sys.h"
#include "config.h"
#include "equipment_ctrl.h"
#include "crc.h"
#include "hotter.h"
#include "pid.h"

//设备地址从1开始
//mqtt每次只发送需要操作的命令，不要什么都发
modbus_stru modbus_recv;
modbus_stru modbus_tx;
unsigned int modbus_read_reg_list[2] = {0x0000, 0x0040};//64-135
unsigned int modbus_read_reg_len[2] = {8, 72};

void analy_modbus_recv(void);

void modbus_init()
{
    modbus_recv.address = 1;
    modbus_tx.address = 1;
    modbus_tx.retry_count = RETRY_COUNT;
    modbus_tx.update = 1;
    memset(modbus_tx.error_count, 0, DEV_SIZE);

}
modbus_stru *get_recv_machine()
{
    return &modbus_recv;
}
modbus_stru *get_tx_machine()
{
    return &modbus_tx;
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
//          static unsigned char addr_tmp=0;
//          if(modbus_recv.payload[0]!=rs485_u->recv_buf[0]&&rs485_u->recv_buf[0]>0)
//          {
//          memcpy(modbus_recv.payload+addr_tmp,rs485_u->recv_buf,7);
//          addr_tmp = addr_tmp + 7;
//          modbus_recv.update = 1;

//        }
//        if(addr_tmp>=14)
//          addr_tmp = 0;
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
        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == MODBUS_READ_CMD)//read
        {
            len = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2]; //read data len
            crc_tmp = rs485_u->recv_buf[modbus_recv.dev_addr_index + 3 + len];
            crc_tmp = crc_tmp << 8;
            crc_tmp = crc_tmp | rs485_u->recv_buf[modbus_recv.dev_addr_index + 4 + len];
            crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 3);
        }
        else
        {
            if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == MODBUS_WRITE_ONE_CMD)
            {
                len = 4; //read data len
                crc_tmp = rs485_u->recv_buf[len + 2];
                crc_tmp = crc_tmp << 8;
                crc_tmp = crc_tmp | rs485_u->recv_buf[len + 3];
                crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 2);
            }
            else if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] ==
                     MODBUS_WRITE_MUL_CMD)
            {
                len = 4; //read data len
                crc_tmp = rs485_u->recv_buf[len + 2];
                crc_tmp = crc_tmp << 8;
                crc_tmp = crc_tmp | rs485_u->recv_buf[len + 3];
                crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 2);

            }

        }



        if (crc_cal == crc_tmp)
        {

            modbus_recv.address = rs485_u->recv_buf[0];
            if (modbus_recv.address > 1)
            {
                modbus_recv.func =    rs485_u->recv_buf[1];
                modbus_recv.reg =  rs485_u->recv_buf[3];
                if (modbus_recv.func == MODBUS_READ_CMD)
                {
                    memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
                           len + 3);
                    modbus_recv.recv_len = len + 3;

                }

                else
                {
                    if (modbus_recv.func == MODBUS_WRITE_ONE_CMD)
                    {
                        memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
                               len + 2);
                        modbus_recv.recv_len = len + 2;

                    }

                    else
                    {
                        memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
                               len + 2);
                        modbus_recv.recv_len = len + 2;

                    }


                }

                modbus_recv.update = 1;
                analy_modbus_recv();

            }





        }
        memset(rs485_u->recv_buf, 0, BUFFER_SIZE);
        rs485_u->recv_update = 0;
    }
}
unsigned int modbus_node_addr()
{
    unsigned char addr;
    addr = 0;
    if (modbus_tx.ctrl_mode == 1)
    {
        if (get_config()->machine & 0x0001)
        {
            addr = 1;
        }
        else if (get_config()->machine & 0x0002)
        {
            addr = 2;
        }
        else if (get_config()->machine & 0x0004)
        {
            addr = 3;
        }
        else if (get_config()->machine & 0x0008)
        {
            addr = 4;
        }
        else if (get_config()->machine & 0x0010)
        {
            addr = 5;
        }
        else if (get_config()->machine & 0x0020)
        {
            addr = 6;
        }
        else if (get_config()->machine & 0x0040)
        {
            addr = 7;
        }
        else if (get_config()->machine & 0x0080)
        {
            addr = 8;
        }
        else if (get_config()->machine & 0x0100)
        {
            addr = 9;
        }
        else if (get_config()->machine & 0x0200)
        {
            addr = 10;
        }
        else
            addr = 0xff;

    }
    return addr;
}
/****************************************************
func:modbus数据发送函数
return： 1 data tx;2:data tx completed and time out
*****************************************************/
unsigned char pb2[64];
unsigned char modbus_trans(unsigned char addr, unsigned char func,
                           unsigned int reg,
                           unsigned char *payload, unsigned int reg_count, unsigned char len,
                           unsigned char retry_times)
{
    unsigned char i, result = 0;

    i = 0;
    result = 0;

    if (modbus_tx.retry_count > 0)
    {
        pb2[i++] = modbus_tx.address;
        pb2[i++] = modbus_tx.func;
        if (func == MODBUS_READ_CMD)
        {

            modbus_tx.func = MODBUS_READ_CMD;
            modbus_tx.regCount = reg_count;
            modbus_tx.reg = reg;
            // modbus_tx.retry_count = reg_count;
            modbus_recv.reg = modbus_tx.reg;
            pb2[i++] = modbus_tx.reg >> 8;
            pb2[i++] = modbus_tx.reg;


        }
        else
        {
            modbus_tx.func = func;
            modbus_tx.reg = reg;
            modbus_tx.regCount = reg_count;
            pb2[i++] = modbus_tx.reg >> 8;
            pb2[i++] = modbus_tx.reg;

            memcpy(modbus_tx.payload, payload, reg_count);
        }


//
//        //  pb2[i++] = 0;
//        // memcpy(&pb2[i], &modbus_tx.reg, 2);
//
//        pb2[i++] = modbus_tx.reg >> 8;
//        pb2[i++] = modbus_tx.reg;
        // i = i + 2;

        if (modbus_tx.func == MODBUS_READ_CMD)
        {
//            unsigned char tmp;
//            tmp =  modbus_tx.regCount;
//            pb2[i++] = 0;
//            memcpy(&pb2[i], &tmp, 1);
//            i = i + 1;
            pb2[i++] = reg_count >> 8;
            pb2[i++] = reg_count;

        }
        if (modbus_tx.func == MODBUS_WRITE_ONE_CMD)
        {
            memcpy(&pb2[i], modbus_tx.payload, modbus_tx.regCount);
            i = i + modbus_tx.regCount;

        }
        else if (modbus_tx.func == MODBUS_WRITE_MUL_CMD)
        {
//            pb2[i++] = 0;
//            //pb2[i] = 1;
//            memcpy(&pb2[i], &modbus_tx.regCount, 1);
//            i = i + 1;

        }

        // pb2[i++] = modbus_tx.address;
//        pb2[i++] = modbus_tx.func;
//        pb2[i++] = 0;
//              pb2[i++] = 0x51;
//              pb2[i++] = 0;
//              pb2[i++] = 1;
//        pb2[i++] = 1;
//        pb2[i++] = 3;
//        pb2[i++] = 0;
//              pb2[i++] = 0x01;
//              pb2[i++] = 0;
//              pb2[i++] = 1;
        modbus_tx.crc = CRC_Compute(pb2, i);
        pb2[i++] = modbus_tx.crc >> 8;
        pb2[i++] = modbus_tx.crc;



        //i = i + 2;
        // modbus_tx.retry_count--;
        uart_transmit(RS485_No, pb2, i);
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
            if (modbus_recv.address > 0 && modbus_recv.address <= DEV_SIZE)
            {
                modbus_recv.error_count[modbus_recv.address] = 0;
                unsigned int i;
                i = 1;
                i = i << modbus_recv.address;
                i = ~i;
                modbus_recv.fault = modbus_recv.fault & i;
//                    if (modbus_recv.reg == FAULT_REG)
//                    {
//                        unsigned int tmp;
//                        tmp = modbus_recv.payload[4];
//                        get_hotter(modbus_recv.address)->status[5], modbus_recv.payload[4];
//
//
//                        modbus_recv.fault = modbus_recv.fault | modbus_recv.payload[4];
//                    }
//                    else
                get_hotter(modbus_recv.address)->status[0] = modbus_recv.address;//设备地址
                if (modbus_tx.last_reg == modbus_read_reg_list[0])
                {

                    get_hotter(modbus_recv.address)->status[2] =
                        modbus_recv.payload[4]; //;控制标志;
                    get_hotter(modbus_recv.address)->status[3] =
                        modbus_recv.payload[6]; //;;模式选择
                    get_hotter(modbus_recv.address)->status[4] =
                        modbus_recv.payload[10]; //L4采暖回差,L5采暖设定温度;
                    get_hotter(modbus_recv.address)->status[5] =
                        modbus_recv.payload[12]; //L4采暖回差,L5采暖设定温度;

//                        memcpy(get_hotter(modbus_recv.address)->status[5], modbus_recv.payload[84],
//                               1); //L5采暖设定温度;故障代码
                    unsigned int i;
                    i = 1;
                    i = i << (modbus_recv.address - 1);

                    if (modbus_recv.fault & i)
                        get_hotter(modbus_recv.address)->status[1] = 1;

                }
                else  if (modbus_tx.last_reg == modbus_read_reg_list[1])
                {
                    unsigned char index, sindex;
                    unsigned int i;
                    unsigned int buf[256];
                    unsigned char buf2[256];
                    index = 4;
                    sindex = 5;
                    memcpy(buf, &modbus_recv.payload[4], modbus_recv.recv_len);

                    for (i = 0; i < modbus_recv.recv_len / 2; i++)
                    {
                        buf2[i] = buf[i];
                    }
                    memcpy(&get_hotter(modbus_recv.address)->status[6], buf2,
                           modbus_recv.recv_len / 2);
//                        get_hotter(modbus_recv.address)->status[j++] =
//                            modbus_recv.payload[4]; //;控制标志;
//                        get_hotter(modbus_recv.address)->status[j++] =
//                            modbus_recv.payload[6]; //;;模式选择
//                        get_hotter(modbus_recv.address)->status[j++] =
//                            modbus_recv.payload[10]; //L4采暖回差,L5采暖设定温度;
//                        get_hotter(modbus_recv.address)->status[j++] =
//                            modbus_recv.payload[12]; //L4采暖回差,L5采暖设定温度;

//                        memcpy(get_hotter(modbus_recv.address)->status[5], modbus_recv.payload[84],
//                               1); //L5采暖设定温度;故障代码

                    i = 1;
                    i = i << (modbus_recv.address - 1);

                    if (modbus_recv.fault & i)
                        get_hotter(modbus_recv.address)->status[1] = 1;

                }
            }
            else
                ;


        };
        break;
        case MODBUS_WRITE_ONE_CMD:
        {
            modbus_tx.retry_count = 0;
            modbus_recv.error_count[modbus_recv.address] = 0;
            unsigned int i;
            i = 1;
            i = i << modbus_recv.address;
            i = ~i;
            modbus_recv.fault = modbus_recv.fault & i;
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
        if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1&&
                modbus_tx.update!=3)//every 180s，cal pid out
        {



            modbus_tx.update = 3;//pid
            modbus_tx.retry_count = RETRY_COUNT;
            modbus_tx.address = 0;

        }
#endif
        if (get_config()->update_setting == 1)
        {
            modbus_tx.update = 2;  //server setting
            config_save();//位置不能变
            get_config()->update_setting = 0;
            modbus_tx.retry_count = RETRY_COUNT;
            modbus_tx.address = 0;

        }

        else
        {
            if (modbus_tx.update == 1)
            {
                modbus_tx.update = 1;//normal poll
                modbus_tx.retry_count = RETRY_COUNT;

            }
            //modbus_tx.retry_count = 1;
            // modbus_tx.address = 0;

        }

        // modbus_tx.address = 0;
        //modbus_tx.retry_count = 3;//retry count
    }

    return modbus_tx.update;
}
void modbus_tx_proc(unsigned char mode)
{

    unsigned char pb[64];
    unsigned int len;
    len = 0;
    registerTick(MODBUS_TX_TICK_NO, 300);
    if (GetTickResult(MODBUS_TX_TICK_NO) == 1) //every 500ms tx
    {
        switch (mode)
        {
        case 1:
            modbus_tx.func = MODBUS_READ_CMD;
            modbus_tx.reg  = modbus_read_reg_list[0];
            len = modbus_read_reg_len[0];
            break;
        case 2:
            // memcpy(pb, &get_config()->machine, 2);//pwr on or off machine
            // get_config()->machine = 0;
            pb[0] = get_config()->machine >> 8;
            pb[1] = get_config()->machine ;
            modbus_tx.func = MODBUS_WRITE_ONE_CMD;
            modbus_tx.reg = CONTROLLER_REG;
            len = len + 2;
            break;
        case 3:
            unsigned int u;
            float u1;
            u1 = get_pid_output();
            u = (unsigned int)u1;
            if (get_config()->set_tindoor < 15) //below 15 no pid ctrl
            {
                memcpy(pb, get_config()->set_tout, 2);
            }
            else
                //memcpy(pb, &u, 2);
                pb[0] = u;
            modbus_tx.func = MODBUS_WRITE_ONE_CMD;
            modbus_tx.reg = TEMPERATURE_REG;
            len++;
            break;

        case 4:
            modbus_tx.func = MODBUS_READ_CMD;
            modbus_tx.reg  = modbus_read_reg_list[1];
            len = modbus_read_reg_len[1];

            break;


        }


//tx proc ctrl

        if (modbus_tx.ctrl_mode == 0) //global ctrl
        {
            if (modbus_tx.address <= DEV_SIZE)
            {
                if (modbus_tx.retry_count > 0)
                {
                    //if (modbus_tx.reg == modbus_read_reg_list[1])
                    modbus_tx.retry_count--;
                    modbus_trans(modbus_tx.address, modbus_tx.func, modbus_tx.reg, pb, len, 1, 3);
                    modbus_tx.last_reg = modbus_tx.reg;
                    modbus_recv.error_count[modbus_tx.address] =
                        modbus_recv.error_count[modbus_tx.address] + 1;
                    if (modbus_recv.error_count[modbus_tx.address] >= FAULT_COUNT)
                    {
                        unsigned int i;
                        i = 1;
                        i = i << (modbus_tx.address - 1);
                        modbus_recv.fault = modbus_recv.fault | i;
                    }

                }
                else
                {
                    //modbus_tx.update = 0;
                    modbus_tx.retry_count = RETRY_COUNT;
                    modbus_tx.address = modbus_tx.address + 1;

                }

            }
            else
            {
                modbus_tx.address = 1;
                modbus_tx.retry_count = RETRY_COUNT;
                if (modbus_tx.update != 1)
                {
                    if(modbus_tx.update == 3)
                        reset_registerTick(MODBUS_MQTT_PID_TICK_NO);
                    else
                    {

                    }
                    modbus_tx.update = 1;

                }
                else
                {
                    modbus_tx.update = 4;
                }

                if (modbus_tx.update == 4)
                {
                    reset_registerTick(MODBUS_POLL_TICK_NO);
                    //  reset_registerTick(MODBUS_MQTT_PID_TICK_NO);
                }

            }

        }
        else //node ctrl
        {
            if (modbus_tx.retry_count > 0)
            {
                modbus_tx.address = modbus_node_addr();
                modbus_trans(modbus_tx.address, modbus_tx.func, modbus_tx.reg, pb, len, 1, 3);
                modbus_tx.last_reg = modbus_tx.reg;
                modbus_recv.error_count[modbus_tx.address] =
                    modbus_recv.error_count[modbus_tx.address] + 1;
                modbus_tx.retry_count--;


            }
            else
            {
                if (modbus_recv.error_count[modbus_tx.address] >= FAULT_COUNT)
                {
                    unsigned int i;
                    i = 1;
                    i = i << (modbus_tx.address - 1);

                    modbus_recv.fault = modbus_recv.fault | i;
                }
                else
                {
                    unsigned int i;
                    i = 1;
                    i = i << modbus_tx.address;

                    modbus_recv.fault = modbus_recv.fault | i;

                }
                if (modbus_tx.update != 1)
                {
                    modbus_tx.update = 1;

                }
                modbus_tx.retry_count = RETRY_COUNT;
                {
                    unsigned int i;
                    i = 1;
                    i = i << (modbus_tx.address - 1);
                    get_config()->machine = get_config()->machine & (~i);

                    if (modbus_node_addr() == 0xff || modbus_node_addr() == 0x00)
                    {
                        modbus_tx.ctrl_mode = 0;
                        modbus_tx.address = 1;
                        get_config()->machine = 0;
                        reset_registerTick(MODBUS_POLL_TICK_NO);
                        reset_registerTick(MODBUS_MQTT_PID_TICK_NO);

                    }

                }

            }

        }
        // if (mode == 2)

        reset_registerTick(MODBUS_TX_TICK_NO);

    }



}

void modbus_proc()
{
    unsigned char result, first_flag;
    result = modbus_ctrl();

    if (result == 2) //mqtt command
    {
        modbus_tx_proc(2);//tx
    }
    else
    {

        if (result == 3) //
        {
            if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1)
            {
                modbus_tx_proc(3);//tx//cal pid
            }

        }
        else if (result == 1) //
        {
            registerTick(MODBUS_POLL_TICK_NO, 3000);
            if (GetTickResult(MODBUS_POLL_TICK_NO) == 1)
            {
                modbus_tx_proc(1);///read dev
            }
        }
        else if (result == 4) //
        {
            registerTick(MODBUS_POLL_TICK_NO, 3000);
            if (GetTickResult(MODBUS_POLL_TICK_NO) == 1)
            {
                modbus_tx_proc(4);///read dev
            }
        }

    }


    rs485_recv();
}
void modbus_test()
{
    modbus_tx_proc(1);
    rs485_recv();


}
