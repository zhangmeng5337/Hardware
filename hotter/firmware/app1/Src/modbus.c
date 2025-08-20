#include "modbus.h"
#include "uart.h"
#include "rs485.h"
#include "sys.h"
#include "config.h"
#include "equipment_ctrl.h"
#include "crc.h"
#include "hotter.h"
#include "pid.h"
#include "schedule.h"
#include "lte_hal.h"
#include "stmflash.h"
#include "power_energy.h"
modbus_pump_cmd_stru  modbus_cmd_list[] =
{
    //
    //    {0x00, MODBUS_WRITE_ONE_CMD, CONTROLLER_REG, 2, {0, 1, 2}, TRUE, FALSE, 0, 0},
    //    {0x00, MODBUS_WRITE_ONE_CMD, TEMPERATURE_REG, 2, {0, 1, 2}, TRUE, FALSE, 0, 0},
    //    {0x00, MODBUS_READ_CMD, 0x0000, 0x0008, {0, 1, 2}, TRUE, TRUE, 0, 0},
    //    {0x00, MODBUS_READ_CMD, 0x0040, 72, {0, 1, 2}, TRUE, FALSE, 0, 0},
    //    {0x00, MODBUS_READ_CMD, 0x73c, 2, {0, 1, 2}, TRUE, FALSE, 0, 0},//delixi
    //    {0x00, MODBUS_READ_CMD, 0x0006, 1, {0, 1, 2}, TRUE, FALSE, 0, 0},//
    //    {0x00, MODBUS_READ_CMD, 0x401e, 2, {0, 1, 2}, TRUE, FALSE, 0, 0}//zhengtai

    {0x00, MODBUS_WRITE_ONE_CMD, CONTROLLER_REG, 2, {0, 1, 2}, 50, 0, 0},//0
    {0x00, MODBUS_WRITE_ONE_CMD, TEMPERATURE_REG, 2, {0, 1, 2}, 50, 0, 0},//1
    //******************air pump*********************************
    {0x00, MODBUS_READ_CMD, 0x0000, 0x000b, {0, 1, 2}, 50, 0, 0},//2
    {0x00, MODBUS_READ_CMD, 0x0028, 9, {0, 1, 2}, 50, 0, 0},//3
    {0x00, MODBUS_READ_CMD, 0x0040, 72, {0, 1, 2}, 200, 0, 0},//4
    //*******************power energy************************************
    {0x00, MODBUS_READ_CMD, 0x700, 26, {0, 1, 2}, 100, 0, 0},//delixi 5
    {0x00, MODBUS_READ_CMD, 0x73c, 2, {0, 1, 2}, 50, 0, 0},//delixi 6

    {0x00, MODBUS_READ_CMD, 0x0006, 2, {0, 1, 2}, 50, 0, 0},//zhengtai 7
    {0x00, MODBUS_READ_CMD, 0x2000, 26, {0, 1, 2}, 100, 0, 0},//zhengtai 8
    {0x00, MODBUS_READ_CMD, 0x401e, 2, {0, 1, 2}, 50, 0, 0},//zhengtai 9

    {0x00, MODBUS_WRITE_ONE_CMD, 0x401e, 2, {0, 1, 2}, 50, 0, 0}//zhengtai 10


};

modbus_cmd_pack_stru cmd_list;

static unsigned char flag = 0;



//设备地址从1开始
//mqtt每次只发送需要操作的命令，不要什么都发
modbus_stru modbus_recv;
modbus_stru modbus_tx;
unsigned int modbus_read_reg_list[2] = {0x0000, 0x0040};//64-135
unsigned int modbus_read_reg_len[2] = {8, 72};
//modbus_pump_pack_tx modbus_pump_pack;

void analy_modbus_recv(void);
//void cmd_enable(unsigned char index, unsigned char status)
//{
//    unsigned char i;
//    for (i = 0; i < CMD_SIZE; i++)
//    {
//        if (i != index)
//        {
//            cmd_list.pb[i].enable = 0;
//        }

//        else
//        {

//            cmd_list.pb[i].enable = 1;

//        }
//        if (status == 0)
//            cmd_list.pb[i].status = 0;

//    }
//}
//void cmd_disable(unsigned char index)
//{
//    unsigned char i;
//    for (i = 0; i < CMD_SIZE; i++)
//    {
//        if (i == index)
//        {

//            cmd_list.pb[i].enable = 0;

//        }
//    }
//}

void modbus_init()
{
    modbus_recv.address = 1;
    modbus_tx.address = 1;
    modbus_tx.retry_count = RETRY_COUNT;
    modbus_tx.update = 1;
    memset(modbus_tx.error_count, 0,  get_config()->dev_size);
    cmd_list.pb = modbus_cmd_list;
    // cmd_enable(PWR_INDEX, 0);
    *get_power() = 10;
    cmd_list.dev_max_addr =   get_config()->dev_size;

}
modbus_stru *get_recv_machine()
{
    return &modbus_recv;
}
modbus_stru *get_tx_machine()
{
    return &modbus_tx;
}
void get_cmd_list(unsigned char *buf)
{
    modbus_cmd_list[PUMP_WCMD_ANYSTART].addr = buf[0];
    modbus_cmd_list[PUMP_WCMD_ANYSTART].func = buf[1];
    modbus_cmd_list[PUMP_WCMD_ANYSTART].reg = buf[2];
    modbus_cmd_list[PUMP_WCMD_ANYSTART].reg = modbus_cmd_list[PUMP_WCMD_ANYSTART].reg << 8;
    modbus_cmd_list[PUMP_WCMD_ANYSTART].reg = modbus_cmd_list[PUMP_WCMD_ANYSTART].reg | buf[3];
    modbus_cmd_list[PUMP_WCMD_ANYSTART].regCount = 2;
    modbus_cmd_list[PUMP_WCMD_ANYSTART].payload[0] = buf[4];
    modbus_cmd_list[PUMP_WCMD_ANYSTART].payload[1] = buf[5];
    cmd_list.devtype = PUMP;
    cmd_list.wr = 2;
    cmd_list.addr = 1;


}

/************************************************
func:解析rs485串口接收的数据
*************************************************/
void rs485_recv()
{

    rs485_stru *rs485_u;
    rs485_u =  get_uart_recv(RS485_No);
    unsigned char i;
    unsigned int crc_tmp;
    unsigned int crc_cal;
    unsigned char index;
    unsigned char len;
    unsigned int buf[32];

    if (rs485_u->recv_update != 0)
    {

        for (i = 0; i < rs485_u->recv_len; i++)
        {
            if (cmd_list.cmd_seq == INSTR_DELI_INDEX ||
                    cmd_list.cmd_seq == INSTR_ZT_INDEX)
            {
                if (rs485_u->recv_buf[i] == DELI_ADDR ||
                        rs485_u->recv_buf[i] == ZT_ADDR) //addr ok
                {
                    //  *get_power() = 1.3;
                    modbus_recv.dev_addr_index = i;
                    break;

                }

            }
            else
            {
                if (rs485_u->recv_buf[i] > 0
                        && rs485_u->recv_buf[i] <=  get_config()->dev_size_tmp) //addr ok
                {
                    modbus_recv.dev_addr_index = i;
                    break;

                }
            }



        }
        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == MODBUS_READ_CMD)//read
        {
            len = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2]; //read data len
            crc_tmp = rs485_u->recv_buf[ len + 3];
            crc_tmp = crc_tmp << 8;//0,0,26,96,96,26
            crc_tmp = crc_tmp | rs485_u->recv_buf[4 + len];
            crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 3);
            if (rs485_u->recv_buf[i] == DELI_ADDR ||
                    rs485_u->recv_buf[i] == ZT_ADDR) //addr ok
            {
                ;//    *get_power() = 1.7;

            }

        }
        else
        {

        }



        if (crc_cal == crc_tmp)
        {
            if (rs485_u->recv_buf[i] == DELI_ADDR ||
                    rs485_u->recv_buf[i] == ZT_ADDR) //addr ok
            {
                ;//*get_power() = 1.8;

            }

            modbus_recv.address = rs485_u->recv_buf[modbus_recv.dev_addr_index];
            if (modbus_recv.address >= 1)
            {
                modbus_recv.func =    rs485_u->recv_buf[modbus_recv.dev_addr_index + 1];
                //modbus_recv.reg =  rs485_u->recv_buf[3];


                if (modbus_recv.func == MODBUS_READ_CMD)
                {
                    memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
                           len + 3);
                    modbus_recv.recv_len = len + 3;
                }

                else
                {


                }

                modbus_recv.update = 1;
                analy_modbus_recv();

            }

        }
        else
        {
            if (rs485_u->recv_buf[i] == DELI_ADDR ||
                    rs485_u->recv_buf[i] == ZT_ADDR) //addr ok
            {
                ;//*get_power() = 1.9;

            }

        }
        memset(rs485_u->recv_buf, 0, RSBUFFER_SIZE);
        rs485_u->recv_update = 0;
        rs485_u->recv_len = 0;
    }
}
/************************************************
func:解析rs485串口接收的数据
*************************************************/
void json_rs485_recv()
{

    rs485_stru *rs485_u;
    rs485_u =  get_uart_recv(RS485_No);
    unsigned char i;
    unsigned int crc_tmp;
    unsigned int crc_cal;
    unsigned char index;
    unsigned char len;
    unsigned int buf[32];

    if (rs485_u->recv_update != 0)
    {

        for (i = 0; i < rs485_u->recv_len; i++)
        {
            {
                if (rs485_u->recv_buf[i] > 0
                        && rs485_u->recv_buf[i] <=  get_config()->dev_size_tmp) //addr ok
                {
                    modbus_recv.dev_addr_index = i;
                    break;

                }
            }



        }
        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == MODBUS_READ_CMD)//read
        {
            len = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2]; //read data len
            crc_tmp = rs485_u->recv_buf[ len + 3];
            crc_tmp = crc_tmp << 8;//0,0,26,96,96,26
            crc_tmp = crc_tmp | rs485_u->recv_buf[4 + len];
            crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 3);
        }
        else
        {

        }



        if (crc_cal == crc_tmp)
        {

            modbus_recv.address = rs485_u->recv_buf[modbus_recv.dev_addr_index];
            if (modbus_recv.address >= 1)
            {
                modbus_recv.func =    rs485_u->recv_buf[modbus_recv.dev_addr_index + 1];
                //modbus_recv.reg =  rs485_u->recv_buf[3];


                if (modbus_recv.func == MODBUS_READ_CMD)
                {
                    memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
                           len + 3);
                    modbus_recv.recv_len = len + 3;
                }

                modbus_recv.update = 1;
                analy_modbus_recv();

            }

        }
        memset(rs485_u->recv_buf, 0, RSBUFFER_SIZE);
        rs485_u->recv_update = 0;
        rs485_u->recv_len = 0;
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
void modbus_trans2(modbus_pump_cmd_stru pbDat)
{
    unsigned char i;

    i = 0;


    pb2[i++] = pbDat.addr;
    pb2[i++] = pbDat.func;
    if (pbDat.func == MODBUS_READ_CMD)
    {

        modbus_tx.func = MODBUS_READ_CMD;
        modbus_tx.regCount = pbDat.regCount;
        modbus_tx.reg = pbDat.reg;
        pb2[i++] = pbDat.reg >> 8;
        pb2[i++] = pbDat.reg;


    }
    else
    {
        modbus_tx.func = pbDat.func;
        modbus_tx.reg = pbDat.reg;
        modbus_tx.regCount = pbDat.regCount;
        pb2[i++] = pbDat.reg >> 8;
        pb2[i++] = pbDat.reg;


    }



    if (pbDat.func == MODBUS_READ_CMD)
    {

        pb2[i++] = pbDat.regCount >> 8;
        pb2[i++] = pbDat.regCount;

    }
    if (modbus_tx.func == MODBUS_WRITE_ONE_CMD)
    {
        memcpy(&pb2[i], pbDat.payload, pbDat.regCount);
        i = i + pbDat.regCount;

    }
    else if (pbDat.func == MODBUS_WRITE_MUL_CMD)
    {
        //            pb2[i++] = 0;
        //            //pb2[i] = 1;
        //            memcpy(&pb2[i], &modbus_tx.regCount, 1);
        //            i = i + 1;

    }


    pbDat.crc = CRC_Compute(pb2, i);
    pb2[i++] = pbDat.crc >> 8;
    pb2[i++] = pbDat.crc;

    uart_transmit(RS485_No, pb2, i);
    //    result = 1;
}

static unsigned int zt_tmp;

void machine_status_anly(unsigned int status_num)
{
    unsigned int i;
    //  unsigned int byte_uint, index_cal;
    unsigned int pb3, pb4;


    rs485_stru *rs485_u;
    rs485_u =  get_uart_recv(RS485_No);
    // j = 1;
    if (status_num == cmd_list.pb[STATUS1_INDEX].reg)
    {

        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
                rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] ==
                cmd_list.pb[STATUS1_INDEX].regCount * 2)
        {
            get_hotter(modbus_recv.address)->status[0] = modbus_recv.address;
            //get_hotter(modbus_recv.address)->status[1] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 1];
            // get_hotter(modbus_recv.address)->status[2] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2];
            for (i = 0; i < cmd_list.pb[STATUS1_INDEX].regCount; i++)
            {
                pb4 = 2 * i;
                pb4 = pb4 + 3;
                pb3 = rs485_u->recv_buf[pb4];
                pb3 = pb3 << 8;

                pb4 = 2 * i;
                pb4 = pb4 + 4;
                pb3 =  pb3 | rs485_u->recv_buf[pb4];
                //get_hotter(1)->status2[3+i] = buf[i];
                get_hotter(modbus_recv.address)->status[1 + i] = pb3;
            }
        }

    }
    else if (status_num == cmd_list.pb[STATUS2_INDEX].reg)
    {

        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
                rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] ==
                cmd_list.pb[STATUS2_INDEX].regCount * 2)
        {
            get_hotter(modbus_recv.address)->status2[0] = modbus_recv.address;
            //get_hotter(modbus_recv.address)->status2[1] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 1];
            // get_hotter(modbus_recv.address)->status2[2] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2];
            for (i = 0; i < cmd_list.pb[STATUS2_INDEX].regCount; i++)
            {
                pb4 = 2 * i;
                pb4 = pb4 + 3;
                pb3 = rs485_u->recv_buf[pb4];
                pb3 = pb3 << 8;

                pb4 = 2 * i;
                pb4 = pb4 + 4;
                pb3 =  pb3 | rs485_u->recv_buf[pb4];
                //get_hotter(1)->status2[3+i] = buf[i];
                get_hotter(modbus_recv.address)->status2[1 + i] = pb3;
            }
        }

    }
    else if (status_num == cmd_list.pb[STATUS3_INDEX].reg)
    {

        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
                rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] ==
                cmd_list.pb[STATUS3_INDEX].regCount * 2)
        {
            get_hotter(modbus_recv.address)->status2[0] = modbus_recv.address;
            //get_hotter(modbus_recv.address)->status2[1] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 1];
            // get_hotter(modbus_recv.address)->status2[2] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2];
            for (i = 0; i < cmd_list.pb[STATUS3_INDEX].regCount; i++)
            {
                pb4 = 2 * i;
                pb4 = pb4 + 3;
                pb3 = rs485_u->recv_buf[pb4];
                pb3 = pb3 << 8;

                pb4 = 2 * i;
                pb4 = pb4 + 4;
                pb3 =  pb3 | rs485_u->recv_buf[pb4];
                //get_hotter(1)->status2[3+i] = buf[i];
                get_hotter(modbus_recv.address)->status3[1 + i] = pb3;
            }
        }

    }

    else //if (status_num == cmd_list.pb[INSTR_DELI_SINDEX].reg)
    {
        power_cal(cmd_list.cmd_seq, &rs485_u->recv_buf[modbus_recv.dev_addr_index]);
        //        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
        //                rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] == 0x04)
        //        {
        //            //*get_power() = 1.5;
        //
        //            pb4 = modbus_recv.dev_addr_index + 3;
        //            pb3 = rs485_u->recv_buf[pb4];
        //            uint32_t tmp;
        //            float res;
        //            tmp = uint8Touint322(&rs485_u->recv_buf[pb4]);
        //
        //            res = *(float *)&tmp;
        //            if (rs485_u->recv_buf[modbus_recv.dev_addr_index] == DELI_ADDR)
        //                res = res / 1000.0;
        //            //*get_power2() = 5;
        //
        //            //res = (float)tmp ;
        //            *get_power() = res;//uint32Tofloat(&rs485_u->recv_buf[3]);
        //
        //        }
    }
    //    else if (status_num == cmd_list.pb[INSTR_ZT_SINDEX].reg)
    //    {
    //
    //        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
    //                rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] == 0x04)
    //        {
    //            //*get_power() = 1.5;
    //
    //            pb4 = modbus_recv.dev_addr_index + 3;
    //            pb3 = rs485_u->recv_buf[pb4];
    //            uint32_t tmp;
    //            float res;
    //            tmp = uint8Touint322(&rs485_u->recv_buf[pb4]);
    //
    //            res = *(float *)&tmp;
    //            if (rs485_u->recv_buf[modbus_recv.dev_addr_index] == DELI_ADDR)
    //                res = res / 1000.0;
    //            else
    //            {
    //                // res = zt_tmp;
    //                res = res * zt_tmp;
    //            }
    //
    //            *get_power() = res;//uint32Tofloat(&rs485_u->recv_buf[3]);
    //
    //        }
    //        else if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
    //                 rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] == 0x02)
    //        {
    //            pb4 = modbus_recv.dev_addr_index +  3;
    //            pb3 = rs485_u->recv_buf[pb4];
    //            zt_tmp = rs485_u->recv_buf[pb4];
    //            zt_tmp = zt_tmp << 8;
    //            zt_tmp =  zt_tmp | rs485_u->recv_buf[pb4 + 1];
    //
    //
    //        }
    //    }

}


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
    rs485_stru *rs485_u;
    rs485_u =  get_uart_recv(RS485_No);

    // if (modbus_recv.update == 1)
    {
        switch (modbus_recv.func)
        {
            case MODBUS_READ_CMD:
            {
                if (cmd_list.cmd_seq >= INSTR_DELI_SINDEX && cmd_list.cmd_seq <= INSTR_ZT_EINDEX)

                {
                    machine_status_anly(cmd_list.pb[cmd_list.cmd_seq].reg);
                }
                else
                {
                    if (modbus_recv.address > 0 && modbus_recv.address <=  get_config()->dev_size)
                    {

                        modbus_recv.error_count[modbus_recv.address] = 0;
                        unsigned int i;
                        i = 1;
                        i = i << (modbus_recv.address - 1);
                        i = ~i;
                        modbus_recv.fault = modbus_recv.fault & i;
                        if (cmd_list.pb[cmd_list.cmd_seq].last_reg ==
                                cmd_list.pb[STATUS1_INDEX].reg)
                        {
                            get_hotter(modbus_recv.address)->status[0] = 1;//设备地址
                            //  if(modbus_recv.recv_len <= (cmd_list.pb[STATUS1_INDEX].regCount*2+5))//status2
                            {
                                get_hotter(modbus_recv.address)->status[0] = 2;//设备地址
                                machine_status_anly(cmd_list.pb[STATUS1_INDEX].reg);
                            }


                        }
                        else  if (cmd_list.pb[cmd_list.cmd_seq].last_reg ==
                                  cmd_list.pb[STATUS2_INDEX].reg)
                        {
                            //   if(modbus_recv.recv_len <= (cmd_list.pb[STATUS2_INDEX].regCount*2+5))//status2
                            {
                                machine_status_anly(cmd_list.pb[STATUS2_INDEX].reg);


                            }
                        }
                    }
                    else
                        ;

                }



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
        modbus_recv.update = 0;
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
            || get_schedule()->current_plan_pwr_update == 1)
    {

        if (get_config()->update_setting == 1 ||
                get_schedule()->current_plan_pwr_update == 1)
        {
            modbus_tx.update = 2;  //server setting
            config_save();//位置不能变
            get_config()->update_setting = 0;
            get_schedule()->current_plan_pwr_update = 2;
            modbus_tx.retry_count = RETRY_COUNT;
            modbus_tx.address = 0;

        }
    }

    return modbus_tx.update;
}
void reg_payload_val()
{

    if (get_schedule()->current_plan_pwr_update == 2)
    {

        if (get_schedule()->current_plan < (SCHEDULE_SIZE))
        {
            cmd_list.pb[PWR_INDEX].payload[0] = 0x00;
            cmd_list.pb[PWR_INDEX].payload[1] =
                get_schedule()->buf[get_schedule()->current_plan].pwr_state | 0x72;

        }
    }
    else
    {

        cmd_list.pb[PWR_INDEX].payload[0] = get_config()->machine >> 8;
        cmd_list.pb[PWR_INDEX].payload[1] = get_config()->machine | 0x40;

    }
    cmd_list.pb[PWR_INDEX].regCount = 2;

}
void modbus_proc_poll()
{
    if (get_config()->mode == OFF_MODE)
        registerTick(MODBUS_TX_TICK_NO, MODBUS_TX_TIME_LONG);
    else
        registerTick(MODBUS_TX_TICK_NO,  modbus_cmd_list[cmd_list.cmd_seq].timout);

    if (GetTickResult(MODBUS_TX_TICK_NO) == 1)
    {
        if (modbus_tx.ctrl_mode == 0) //global ctrl
        {
            if (cmd_list.cmd_seq <= CMD_SIZE)
            {
                if (cmd_list.retry_count > 0)
                {
                    modbus_cmd_list[cmd_list.cmd_seq].addr = cmd_list.addr;

                    modbus_trans2(modbus_cmd_list[cmd_list.cmd_seq]);
                    cmd_list.retry_count--;
                    cmd_list.pb[cmd_list.cmd_seq].last_reg =
                        cmd_list.pb[cmd_list.cmd_seq].reg;//
                    modbus_recv.error_count[cmd_list.pb[cmd_list.cmd_seq].addr] =
                        modbus_recv.error_count[cmd_list.pb[cmd_list.cmd_seq].addr] + 1;
                    if (modbus_recv.error_count[cmd_list.pb[cmd_list.cmd_seq].addr] >= FAULT_COUNT)
                    {
                        unsigned int i;
                        i = 1;
                        i = i << (cmd_list.pb[cmd_list.cmd_seq].addr);
                        modbus_recv.fault = modbus_recv.fault | i;
                        modbus_recv.error_count[cmd_list.pb[cmd_list.cmd_seq].addr] =
                            FAULT_COUNT;
                        //                      cmd_list.retry_count = RETRY_COUNT;
                        //
                        //                      if(cmd_list.addr<DEV_SIZE)
                        //                      {
                        //                          cmd_list.addr++;
                        //                      }
                        //                      else
                        //                      {
                        //                          cmd_list.addr = 0;
                        //                          cmd_list.retry_count = RETRY_COUNT;
                        //                          cmd_list.pb[cmd_list.cmd_seq].status = 2;
                        //                          cmd_list.cmd_seq ++ ;
                        //                          if(cmd_list.cmd_seq<CMD_SIZE)
                        //                              cmd_list.pb[cmd_list.cmd_seq].status = 1;
                        //
                        //                      }


                    }

                }
                //                else
                //                {
                //                    cmd_list.retry_count = RETRY_COUNT;
                //                    if (cmd_list.addr <  get_config()->dev_size_tmp)
                //                    {
                //                        cmd_list.addr++;
                //                    }
                //                    else
                //                    {
                //                        cmd_list.addr = 0;
                //
                //                        cmd_list.pb[cmd_list.cmd_seq].status = 2;
                //                        cmd_list.cmd_seq ++ ;
                //                        //if(cmd_list.cmd_seq>=CMD_SIZE)
                //                        //  cmd_list.pb[cmd_list.cmd_seq].status = 1;
                //
                //                    }
                //
                //
                //                }


            }
            else
            {
                //                if (cmd_list.cmd_seq >= CMD_SIZE)
                //                {
                //                    cmd_list.retry_count = RETRY_COUNT;
                //                    cmd_list.cmd_seq = 0;
                //                    cmd_list.pb[cmd_list.cmd_seq].status = 1;
                //
                //
                //                }
                //                if (cmd_list.pb[cmd_list.cmd_seq].enable == 0 && cmd_list.cmd_seq < CMD_SIZE)
                //                {
                //                    cmd_list.retry_count = RETRY_COUNT;
                //                    cmd_list.pb[cmd_list.cmd_seq].status = 2;
                //                    cmd_list.cmd_seq ++;
                //                    //cmd_list.pb[cmd_list.cmd_seq].status = 1;
                //
                //                }


            }
        }
        else  //node ctrl
        {
            if (cmd_list.retry_count > 0)
            {
                cmd_list.addr = modbus_node_addr();
                modbus_cmd_list[cmd_list.cmd_seq].addr = cmd_list.addr;
                modbus_trans2(modbus_cmd_list[cmd_list.cmd_seq]);
                cmd_list.retry_count--;
                cmd_list.pb[cmd_list.cmd_seq].last_reg =
                    cmd_list.pb[cmd_list.cmd_seq].reg;//
                modbus_recv.error_count[cmd_list.pb[cmd_list.cmd_seq].addr] =
                    modbus_recv.error_count[cmd_list.pb[cmd_list.cmd_seq].addr] + 1;
                if (modbus_recv.error_count[cmd_list.pb[cmd_list.cmd_seq].addr] >= FAULT_COUNT)
                {
                    unsigned int i;
                    i = 1;
                    i = i << (cmd_list.pb[cmd_list.cmd_seq].addr);
                    modbus_recv.fault = modbus_recv.fault | i;
                    cmd_list.retry_count = RETRY_COUNT;

                }

            }
            else
            {
                cmd_list.retry_count = RETRY_COUNT;
                // cmd_list.pb[cmd_list.cmd_seq].status = 2;
            }

        }
        reset_registerTick(MODBUS_TX_TICK_NO);
        if (get_config()->mode == OFF_MODE)
            registerTick(MODBUS_TX_TICK_NO, MODBUS_TX_TIME_LONG);
        else
            registerTick(MODBUS_TX_TICK_NO,   modbus_cmd_list[cmd_list.cmd_seq].timout);


    }


}
void modbus_data_pack(unsigned char pack_num)
{
    if (pack_num == PWR_INDEX)
    {
        if (get_schedule()->current_plan_pwr_update == 2)
        {

            if (get_schedule()->current_plan < (SCHEDULE_SIZE))
            {
                cmd_list.pb[PWR_INDEX].payload[0] = 0x00;
                cmd_list.pb[PWR_INDEX].payload[1] =
                    get_schedule()->buf[get_schedule()->current_plan].pwr_state | 0x72;

            }
        }
        else
        {

            cmd_list.pb[PWR_INDEX].payload[0] = get_config()->machine >> 8;
            cmd_list.pb[PWR_INDEX].payload[1] = get_config()->machine | 0x40;

        }
        cmd_list.pb[PWR_INDEX].regCount = 2;

    }
    else if (pack_num == PID_INDEX)
    {
        unsigned int u;
        float u1;

        if (flag == 0)
        {
            fuzzy_proc(get_config()->mode);  //smart ctrl
            flag = 1;

        }

        u1 = get_pid_output();
        u = (unsigned int)u1;
        if (get_config()->set_tindoor < 15) //below 15 no pid ctrl
        {

            memcpy(cmd_list.pb[PID_INDEX].payload, get_config()->set_tout, 2);
        }
        else
        {
            cmd_list.pb[PID_INDEX].payload[0] = u >> 8;
            cmd_list.pb[PID_INDEX].payload[1] = u;

        }
        cmd_list.pb[PWR_INDEX].regCount = 2;

    }
    else if (pack_num == STATUS1_INDEX)
    {

        ;//cmd_list.pb[STATUS1_INDEX].regCount = 2;

    }
    else if (pack_num == STATUS2_INDEX)
    {
        ;//cmd_list.pb[STATUS2_INDEX].regCount = 2;

    }


}
//unsigned char cmd_proc(unsigned char num, unsigned char addr)
//{
//    unsigned char i;
//    unsigned char result;
//    result = 0;
//    for (i = 0; i < CMD_SIZE; i++)
//    {
//        if (num == i)
//            break;
//    }
//    if (i == CMD_SIZE)
//        result = 0;
//    else
//    {
//        modbus_data_pack(num);
//        cmd_enable(num, 1);
//        if (cmd_list.pb[num].status == 0)
//        {
//            cmd_list.pb[num].status = 1;
//            cmd_list.retry_count = RETRY_COUNT;
//            cmd_list.cmd_seq = num;
//            cmd_list.addr = addr;
//
//        }
//
//        else  if (cmd_list.pb[num].status == 2)
//        {
//            cmd_disable(num);
//
//            for (i = num; i < CMD_SIZE; i++)
//                cmd_list.pb[num].status = 0;
//            // cmd_list.cmd_seq = num;
//            result = 1;
//        }
//
//        modbus_proc_poll();//
//
//    }
//
//    return result;
//}
//void dev_tick_task()
//{
//    switch (cmd_list.devState)
//    {
//        case STATUS1_INDEX:
//            if (cmd_proc(STATUS1_INDEX, 0) == 1)
//            {
//                cmd_list.devState = STATUS2_INDEX;

//            }
//            break;
//        case STATUS2_INDEX:
//            if (cmd_proc(STATUS2_INDEX, 0) == 1)
//            {
//                cmd_list.devState = STATUS1_INDEX;

//            }
//            break;
//        default:
//            cmd_list.devState = STATUS1_INDEX;
//            break;

//    }

//}
//void pid_tick_task()
//{
//    switch (cmd_list.state)
//    {
//        case PID_INDEX:
//            if (cmd_proc(PID_INDEX, 0) == 1)
//            {
//                if (get_config()->instru_num == DELI)//delixi
//                {
//                    cmd_list.state = INSTR_DELI_INDEX;
//                    get_config()->dev_size_tmp = DELI_ADDR;
//                }
//                else
//                {
//                    cmd_list.state = INSTR_ZT_CURR_INDEX;
//                    get_config()->dev_size_tmp = ZT_ADDR;

//                }
//            }
//            break;
//        case INSTR_DELI_INDEX:
//            if (cmd_proc(INSTR_DELI_INDEX, DELI_ADDR) == 1)
//            {
//                cmd_list.state = CMD_SIZE;
//            }
//            break;

//        case INSTR_ZT_CURR_INDEX:
//            if (cmd_proc(INSTR_ZT_CURR_INDEX, ZT_ADDR) == 1)
//            {
//                get_config()->dev_size_tmp = ZT_ADDR;
//                cmd_list.state = INSTR_ZT_INDEX;
//            }
//            break;
//        case INSTR_ZT_INDEX:
//            if (cmd_proc(INSTR_ZT_INDEX, ZT_ADDR) == 1)
//            {
//                get_config()->dev_size_tmp = ZT_ADDR;
//                cmd_list.state = CMD_SIZE;
//            }

//            break;
//        case CMD_SIZE:
//            cmd_disable(PID_INDEX);
//            cmd_disable(INSTR_DELI_INDEX);
//            cmd_disable(INSTR_ZT_INDEX);
//            cmd_disable(INSTR_ZT_CURR_INDEX);

//            cmd_list.pb[PID_INDEX].status = 0;
//            cmd_list.pb[INSTR_ZT_INDEX].status = 0;
//            cmd_list.pb[INSTR_DELI_INDEX].status = 0;
//            cmd_list.pb[INSTR_ZT_CURR_INDEX].status = 0;
//            get_config()->dev_size_tmp = get_config()->dev_size;
//            cmd_list.pb[STATUS1_INDEX].status = 0;
//            reset_registerTick(MODBUS_MQTT_PID_TICK_NO);
//            registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);


//            if (get_config()->count >= 3)
//            {
//                set_indoor_temp()->temp_average = 1;
//            }
//            else
//                get_config()->count++;
//            break;
//        default:
//            cmd_list.state = PID_INDEX;
//            break;
//    }

//}
/*******************************
specify cmd_list.devtype,cmd_list.addr,cmd_list.dev_max_addr
********************************/
void dev_poll_proc(void)
{
    if (cmd_list.wr == 0)
    {
        if (cmd_list.devtype == PUMP)
        {
            reg_payload_val();
            cmd_list.start_cmd = PUMP_CMD_START;
            if (cmd_list.retry_count == 0)
            {
                cmd_list.retry_count = RETRY_COUNT;
                if ((cmd_list.cmd_seq - cmd_list.start_cmd + 1) < PUMP_RCMD_SIZE)
                {
                    cmd_list.cmd_seq++;
                }
                else
                {

                    //cmd_list.addr ++;
                    if (cmd_list.addr < cmd_list.dev_max_addr)
                    {
                        cmd_list.addr++;
                        cmd_list.cmd_seq = cmd_list.start_cmd;
                    }
                    else
                    {
                        cmd_list.devtype = POWER_ENERGY;
                        cmd_list.start_cmd = ENERGY_CMD_START;
                        cmd_list.addr = ENERGY_SADDR;
                        cmd_list.cmd_seq = cmd_list.start_cmd;
                    }
                }

            }

        }
        else
        {
            cmd_list.start_cmd = ENERGY_CMD_START;
            if (cmd_list.retry_count == 0)
            {
                cmd_list.retry_count = RETRY_COUNT;
                if ((cmd_list.cmd_seq - cmd_list.start_cmd + 1) < ENERGY_CMD_SIZE)
                {
                    cmd_list.cmd_seq++;
                }
                else
                {

                    //cmd_list.addr ++;
                    if (cmd_list.addr < (get_config()->energy_size + ENERGY_SADDR))
                    {
                        cmd_list.cmd_seq = cmd_list.start_cmd;
                        cmd_list.addr++;
                    }
                    else
                    {
                        cmd_list.devtype = PUMP;
                        cmd_list.start_cmd = PUMP_CMD_START;
                        cmd_list.addr = PUMP_SADDR;
                        cmd_list.cmd_seq = cmd_list.start_cmd;
                    }
                }

            }

        }

    }
    else if (cmd_list.wr == 1)
    {
        if (cmd_list.devtype == PUMP)
        {
            reg_payload_val();
            cmd_list.start_cmd = PUMP_WCMD_START;
            if (cmd_list.retry_count == 0)
            {
                cmd_list.retry_count = RETRY_COUNT;
                if ((cmd_list.cmd_seq - cmd_list.start_cmd + 1) < PUMP_WCMD_SIZE)
                {
                    cmd_list.cmd_seq++;
                }
                else
                {

                    //cmd_list.addr ++;
                    if (cmd_list.addr < cmd_list.dev_max_addr)
                    {
                        cmd_list.cmd_seq = cmd_list.start_cmd;
                        cmd_list.addr++;
                    }
                    else
                    {
                        cmd_list.devtype = PUMP;
                        cmd_list.start_cmd = PUMP_CMD_START;
                        cmd_list.addr = PUMP_SADDR;
                        cmd_list.wr = 0;
                        cmd_list.cmd_seq = cmd_list.start_cmd;
                    }
                }

            }

        }

    }
    else  //pump reg write
    {
        cmd_list.start_cmd = PUMP_WCMD_ANYSTART;
        if (cmd_list.retry_count == 0)
        {
            cmd_list.retry_count = RETRY_COUNT;
            {
                cmd_list.cmd_seq = cmd_list.start_cmd;
                //cmd_list.addr ++;
                if (cmd_list.addr < cmd_list.dev_max_addr)
                {
                    cmd_list.addr++;
                }
                else
                {
                    cmd_list.devtype = PUMP;
                    cmd_list.start_cmd = PUMP_CMD_START;
                    cmd_list.addr = PUMP_SADDR;
                    cmd_list.wr = 0;
                }
            }

        }

    }
    modbus_proc_poll();

}

void modbus_proc_sec(void)
{

   // get_config()->connectTimeOut = 10;

    if ((get_uart_recv(RS485_No)->recv_update == 0
            &&  get_mqtt_status() >= AT_MPUB_RECV) || get_config()->connectTimeOut >= 10)
    {
        if (get_config()->update_setting == 1 ||
                get_schedule()->current_plan_pwr_update == 1)//server command
        {
            get_config()->update_setting = 0;
            if (modbus_tx.update == 0)
            {
                config_save();//位置不能变
                modbus_tx.update = 1;  //server setting
            }
            if (get_schedule()->current_plan_pwr_update == 1)
                get_schedule()->current_plan_pwr_update = 2;

            get_config()->dev_size_tmp = get_config()->dev_size;//air pum size
            reset_registerTick(MODBUS_MQTT_PID_TICK_NO);
            registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);


        }
        else
        {
            modbus_tx.update = 0;  //server setting
            if (cmd_list.state >= CMD_SIZE)
                cmd_list.state = PWR_INDEX;

            if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1
                    && get_config()->mode <= OFF_MODE) //pid poll
            {
                unsigned int u;
                float u1;

                fuzzy_proc(get_config()->mode);  //smart ctrl
                u1 = get_pid_output();
                u = (unsigned int)u1;
                if (get_config()->set_tindoor < 15) //below 15 no pid ctrl
                {

                    memcpy(cmd_list.pb[PID_INDEX].payload, get_config()->set_tout, 2);
                }
                else
                {
                    cmd_list.pb[PID_INDEX].payload[0] = u >> 8;
                    cmd_list.pb[PID_INDEX].payload[1] = u;

                }
                cmd_list.state = PID_INDEX;
                cmd_list.wr = 1;
                cmd_list.devtype = PUMP;
                cmd_list.cmd_seq = PUMP_WCMD_START;
                cmd_list.addr = PUMP_SADDR;

                reset_registerTick(MODBUS_MQTT_PID_TICK_NO);
                reset_registerTick(MODBUS_POLL_TICK_NO);
                registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);
                // pid_tick_task();

            }
            else  //read dev
            {

                dev_poll_proc();
            }


        }

    }
    else
    {
        rs485_recv();

    }

}
//void modbus_proc()
//{


//    if ((get_uart_recv(RS485_No)->recv_update == 0
//            &&  get_mqtt_status() == AT_MPUB_RECV) || get_config()->connectTimeOut >= 10) //????)
//    {
//        if (get_config()->update_setting == 1 ||
//                get_schedule()->current_plan_pwr_update == 1)//server command
//        {
//            if (modbus_tx.update == 0)
//            {
//                config_save();//位置不能变
//                modbus_tx.update = 1;  //server setting

//            }

//            if (get_schedule()->current_plan_pwr_update == 1)
//                get_schedule()->current_plan_pwr_update = 2;
//            get_config()->update_setting = 1;
//            get_config()->dev_size_tmp = get_config()->dev_size;
//            if (get_config()->mode != OFF_MODE)
//            {

//                if (cmd_proc(PWR_INDEX, 0) == 1)
//                {
//                    modbus_tx.update = 0;
//                    get_config()->update_setting = 0;
//                    //registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);

//                }

//            }
//            else
//            {
//                //cmd_list.pb[PWR_INDEX].status = 0;
//                modbus_tx.update = 0;
//                get_config()->update_setting = 0;

//            }
//            registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);
//            cmd_list.state = PID_INDEX;

//        }
//        else
//        {
//            modbus_tx.update = 0;
//            if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1
//                    && get_config()->mode <= OFF_MODE) //pid poll
//            {
//                pid_tick_task();

//            }
//            else  //read dev
//            {
//                flag = 0;
//                //cmd_list.pb[PID_INDEX].status = 0;
//                if (get_config()->mode != OFF_MODE)
//                {
//                    cmd_list.state = PID_INDEX;

//                }
//                else
//                {
//                    if (get_config()->instru_num == DELI)//delixi
//                    {
//                        cmd_list.state = INSTR_DELI_INDEX;
//                        get_config()->dev_size_tmp = DELI_ADDR;
//                    }
//                    else
//                    {
//                        cmd_list.state = INSTR_ZT_CURR_INDEX;
//                        get_config()->dev_size_tmp = ZT_ADDR;

//                    }
//                }
//                get_config()->dev_size_tmp = get_config()->dev_size;
//                registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);
//                registerTick(MODBUS_POLL_TICK_NO, MODBUS_POLL_TIME);
//                if (GetTickResult(MODBUS_POLL_TICK_NO) == 1)
//                {
//                    dev_tick_task();
//                }
//            }
//        }

//    }
//    else
//        rs485_recv();
//}


