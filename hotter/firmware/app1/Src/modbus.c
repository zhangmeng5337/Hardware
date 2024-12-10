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
modbus_pump_cmd_stru  modbus_cmd_list[] =
{

    {0x00, MODBUS_WRITE_ONE_CMD, CONTROLLER_REG, 2, {0, 1, 2}, TRUE, FALSE, 0, 0},
    {0x00, MODBUS_WRITE_ONE_CMD, TEMPERATURE_REG, 2, {0, 1, 2}, TRUE, FALSE, 0, 0},
    {0x00, MODBUS_READ_CMD, 0x0000, 0x0008, {0, 1, 2}, TRUE, TRUE, 0, 0},
    {0x00, MODBUS_READ_CMD, 0x0040, 72, {0, 1, 2}, TRUE, FALSE, 0, 0}
};

modbus_cmd_pack_stru cmd_list;




//设备地址从1开始
//mqtt每次只发送需要操作的命令，不要什么都发
modbus_stru modbus_recv;
modbus_stru modbus_tx;
unsigned int modbus_read_reg_list[2] = {0x0000, 0x0040};//64-135
unsigned int modbus_read_reg_len[2] = {8, 72};
//modbus_pump_pack_tx modbus_pump_pack;

void analy_modbus_recv(void);
void cmd_enable(unsigned char index, unsigned char status)
{
    unsigned char i;
    for (i = 0; i < CMD_SIZE; i++)
    {
        if (i != index)
        {
            cmd_list.pb[i].enable = 0;
        }

        else
        {

            cmd_list.pb[i].enable = 1;

        }
        if (status == 0)
            cmd_list.pb[i].status = 0;

    }
}

void modbus_init()
{
    modbus_recv.address = 1;
    modbus_tx.address = 1;
    modbus_tx.retry_count = RETRY_COUNT;
    modbus_tx.update = 1;
    memset(modbus_tx.error_count, 0,  get_config()->dev_size);
    cmd_list.pb = modbus_cmd_list;
    cmd_enable(PWR_INDEX, 0);

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
    unsigned char i;
    unsigned int crc_tmp;
    unsigned int crc_cal;
    unsigned char index;
    unsigned char len;
    unsigned int buf[32];

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

        for (i = 0; i < rs485_u->recv_len; i++)
        {
            if (rs485_u->recv_buf[i] > 0
                    && rs485_u->recv_buf[i] <=  get_config()->dev_size) //addr ok
            {
                modbus_recv.dev_addr_index = i;
                break;

            }

        }
//        unsigned int pb3, pb4;
//        if (rs485_u->recv_buf[modbus_recv.dev_addr_index] == 1 &&
//                rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
//                rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] == 0x90)
//        {
//            get_hotter(1)->status2[0] = modbus_recv.address;
//            get_hotter(1)->status2[1] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 1];
//            get_hotter(1)->status2[2] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2];
//            for (i = 0; i < 72; i++)
//            {
//                pb4 = 2 * i;
//                pb4 = pb4 + 3;
//                pb3 = rs485_u->recv_buf[pb4];
//                pb3 = buf[i] << 8;
//                pb4 = 2 * i;
//                pb4 = pb4 + 4;
//                pb3 =  buf[i] | rs485_u->recv_buf[pb4];
//                //get_hotter(1)->status2[3+i] = buf[i];
//                get_hotter(1)->status2[3 + i] = pb3;
//            }

        //memcpy(get_hotter(1)->status,&rs485_u->recv_buf[modbus_recv.dev_addr_index],21);
        //crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index],  rs485_u->recv_buf[modbus_recv.dev_addr_index+2] + 3);
        //  get_hotter(1)->status2[3+i] = crc_cal; //crc_cal 1010
        //  get_hotter(1)->status2[4+i] = crc_cal>>8;

        //}




        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == MODBUS_READ_CMD)//read
        {
            len = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2]; //read data len
            crc_tmp = rs485_u->recv_buf[ len + 3];
            crc_tmp = crc_tmp << 8;//0,0,26,96,96,26
            crc_tmp = crc_tmp | rs485_u->recv_buf[4 + len];
            crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 3);
//         get_hotter(1)->status[3+i] = crc_tmp; //crc_cal 1010
//         get_hotter(1)->status[4+i] = crc_cal; //crc_cal 1010



        }
        else
        {
//            if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == MODBUS_WRITE_ONE_CMD)
//            {
//                len = 4; //read data len
//                crc_tmp = rs485_u->recv_buf[len + 2];
//                crc_tmp = crc_tmp << 8;
//                crc_tmp = crc_tmp | rs485_u->recv_buf[len + 3];
//                crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 2);
//            }
//            else if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] ==
//                     MODBUS_WRITE_MUL_CMD)
//            {
//                len = 4; //read data len
//                crc_tmp = rs485_u->recv_buf[len + 2];
//                crc_tmp = crc_tmp << 8;
//                crc_tmp = crc_tmp | rs485_u->recv_buf[len + 3];
//                crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index], len + 2);
//
//            }

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

//unsigned int pb3,pb4;
//if(rs485_u->recv_buf[modbus_recv.dev_addr_index] ==1&&
//  rs485_u->recv_buf[modbus_recv.dev_addr_index+1] == 0x03&&
//  rs485_u->recv_buf[modbus_recv.dev_addr_index+2] == 0x90)
//  {
//  get_hotter(1)->status2[0] = modbus_recv.address;
//  get_hotter(1)->status2[1] = rs485_u->recv_buf[modbus_recv.dev_addr_index+1];
//  get_hotter(1)->status2[2] = rs485_u->recv_buf[modbus_recv.dev_addr_index+2];
//      for(i=0; i<72;i++)
//      {
//         pb4 = 2*i;
//         pb4 = pb4 + 3;
//          pb3 = rs485_u->recv_buf[pb4];
//          pb3 = buf[i]<<8;
//         pb4 = 2*i;
//         pb4 = pb4 + 4;
//          pb3 =  buf[i]|rs485_u->recv_buf[pb4];
//          //get_hotter(1)->status2[3+i] = buf[i];
//        get_hotter(1)->status2[3+i] = pb3;
//      }

                    //memcpy(get_hotter(1)->status,&rs485_u->recv_buf[modbus_recv.dev_addr_index],21);
                    //crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index],  rs485_u->recv_buf[modbus_recv.dev_addr_index+2] + 3);
                    //  get_hotter(1)->status2[3+i] = crc_cal; //crc_cal 1010
                    //  get_hotter(1)->status2[4+i] = crc_cal>>8;

//}

//              if(rs485_u->recv_buf[modbus_recv.dev_addr_index] ==1&&
//                  rs485_u->recv_buf[modbus_recv.dev_addr_index+1] == 0x03&&
//                  rs485_u->recv_buf[modbus_recv.dev_addr_index+2] == 0x10)
//                  {
//                  get_hotter(1)->status[0] = 4;
//                  get_hotter(1)->status[1] = rs485_u->recv_buf[modbus_recv.dev_addr_index+1];
//                  get_hotter(1)->status[2] = rs485_u->recv_buf[modbus_recv.dev_addr_index+2];
//                      for(i=0; i<=cmd_list.pb[STATUS1_INDEX].regCount;i++)
//                      {
//                          buf[i] = rs485_u->recv_buf[2*i+3];
//                          buf[i] = buf[i]<<8;
//                          buf[i] =  buf[i]+rs485_u->recv_buf[2*i+4];
//                          get_hotter(1)->status[3+i] = buf[i];
//
//                      }
//
//                      //memcpy(get_hotter(1)->status,&rs485_u->recv_buf[modbus_recv.dev_addr_index],21);
//                          crc_cal = CRC_Compute(&rs485_u->recv_buf[modbus_recv.dev_addr_index],  rs485_u->recv_buf[modbus_recv.dev_addr_index+2] + 3);
//                      get_hotter(1)->status[3+i] = crc_cal; //crc_cal 1010
//                      //get_hotter(1)->status[4+i] = crc_cal>>8;
//                      }
                }

                else
                {
//                    if (modbus_recv.func == MODBUS_WRITE_ONE_CMD)
//                    {
//                        memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
//                               6);
//                        modbus_recv.recv_len = 6;
//
//                    }
//
//                    else
//                    {
//                        memcpy(modbus_recv.payload, &rs485_u->recv_buf[modbus_recv.dev_addr_index],
//                               len + 2);
//                        modbus_recv.recv_len = len + 2;
//
//                    }


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
    unsigned char i, result = 0;

    i = 0;
    result = 0;

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
    result = 1;
}


void machine_status_anly(unsigned char status_num)
{
    unsigned int i, index, j, buf;
    unsigned int byte_uint, index_cal;
    unsigned int pb3, pb4;
    rs485_stru *rs485_u;
    rs485_u =  get_uart_recv(RS485_No);
    j = 1;
    if (status_num == cmd_list.pb[STATUS1_INDEX].reg)
    {

        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
                rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] == 0x10)
        {
            get_hotter(modbus_recv.address)->status[0] = modbus_recv.address;
            //get_hotter(modbus_recv.address)->status[1] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 1];
            // get_hotter(modbus_recv.address)->status[2] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2];
            for (i = 0; i < 8; i++)
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



//        get_hotter(modbus_recv.address)->status[0] = modbus_recv.address;//设备地址
//
//        for (i = 0; i < 8; i++)
//        {
//            index_cal = 2 * i;
//            index_cal = index_cal + 3;
//            byte_uint = rs485_u->recv_buf[index_cal];
//          byte_uint = byte_uint << 8;
//
//            index_cal = 2 * i;
//            index_cal = index_cal + 4;
//            byte_uint =  byte_uint | rs485_u->recv_buf[index_cal];
//            get_hotter(modbus_recv.address)->status[2 + i] = byte_uint;
//
//        }

//        j = j << (modbus_recv.address - 1);
//        if (modbus_recv.fault & j)
//            get_hotter(modbus_recv.address)->status[1] = 1;


    }
    else if (status_num == cmd_list.pb[STATUS2_INDEX].reg)
    {

        if (rs485_u->recv_buf[modbus_recv.dev_addr_index + 1] == 0x03 &&
                rs485_u->recv_buf[modbus_recv.dev_addr_index + 2] == 0x90)
        {
            get_hotter(modbus_recv.address)->status2[0] = modbus_recv.address;
            //get_hotter(modbus_recv.address)->status2[1] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 1];
            // get_hotter(modbus_recv.address)->status2[2] = rs485_u->recv_buf[modbus_recv.dev_addr_index + 2];
            for (i = 0; i < 72; i++)
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




//        index = 3;
//        j = 1;
//        get_hotter(modbus_recv.address)->status2[0] = modbus_recv.address;//设备地址
//
//        byte_uint = rs485_u->recv_buf[index];
//        byte_uint = byte_uint << 8;
//        byte_uint =  byte_uint + rs485_u->recv_buf[index + 1];
//        get_hotter(modbus_recv.address)->status2[1] = byte_uint;//ctrl flag 2
//
//        byte_uint = rs485_u->recv_buf[index + 20];
//        byte_uint = byte_uint << 8;
//        byte_uint =  byte_uint + rs485_u->recv_buf[index + 21];
//        get_hotter(modbus_recv.address)->status2[3] = byte_uint;//ctrl flag 3
//        for (i = 0; i < 55; i++)
//        {
//            index_cal = 2 * i;
//            index_cal = index_cal + 34;
//            index_cal = index_cal + index;
//            byte_uint = rs485_u->recv_buf[index_cal];
//            byte_uint = byte_uint << 8;
//
//            index_cal = 2 * i;
//            index_cal = index_cal + 35;
//            index_cal = index_cal + index;
//            byte_uint =  byte_uint | rs485_u->recv_buf[index_cal];
//            get_hotter(modbus_recv.address)->status[4 + i] = byte_uint;
//
//        }
        //  memcpy(&get_hotter(modbus_recv.address)->status2[4],buf,55);
    }
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
    // if (modbus_recv.update == 1)
    {
        switch (modbus_recv.func)
        {
            case MODBUS_READ_CMD:
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

void modbus_proc_poll()
{
    registerTick(MODBUS_TX_TICK_NO, MODBUS_TX_TIME);

    if (GetTickResult(MODBUS_TX_TICK_NO) == 1)
    {
        if (modbus_tx.ctrl_mode == 0) //global ctrl
        {
            if (cmd_list.cmd_seq < CMD_SIZE && cmd_list.pb[cmd_list.cmd_seq].enable == 1)
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
                else
                {
                    cmd_list.retry_count = RETRY_COUNT;
                    if (cmd_list.addr <  get_config()->dev_size)
                    {
                        cmd_list.addr++;
                    }
                    else
                    {
                        cmd_list.addr = 0;

                        cmd_list.pb[cmd_list.cmd_seq].status = 2;
                        cmd_list.cmd_seq ++ ;
                        //if(cmd_list.cmd_seq>=CMD_SIZE)
                        //  cmd_list.pb[cmd_list.cmd_seq].status = 1;

                    }


                }


            }
            else
            {
                if (cmd_list.cmd_seq >= CMD_SIZE)
                {
                    cmd_list.retry_count = RETRY_COUNT;
                    cmd_list.cmd_seq = 0;
                    cmd_list.pb[cmd_list.cmd_seq].status = 1;


                }
                if (cmd_list.pb[cmd_list.cmd_seq].enable == 0 && cmd_list.cmd_seq < CMD_SIZE)
                {
                    cmd_list.retry_count = RETRY_COUNT;
                    cmd_list.pb[cmd_list.cmd_seq].status = 2;
                    cmd_list.cmd_seq ++;
                    //cmd_list.pb[cmd_list.cmd_seq].status = 1;

                }


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
                cmd_list.pb[cmd_list.cmd_seq].status = 2;
            }

        }
        reset_registerTick(MODBUS_TX_TICK_NO);
        registerTick(MODBUS_TX_TICK_NO, MODBUS_TX_TIME);

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
                    get_schedule()->buf[get_schedule()->current_plan].pwr_state | 0x40;

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
void modbus_proc()
{
    unsigned char result, first_flag;
    if (get_uart_recv(RS485_No)->recv_update == 0)
    {
        if (get_config()->update_setting == 1 ||
                get_schedule()->current_plan_pwr_update == 1)
        {
            if (modbus_tx.update == 0)
            {
                config_save();//位置不能变
                modbus_tx.update = 1;  //server setting

            }

            if (get_config()->mode != 3)
            {
                get_config()->update_setting = 1;
                if (get_schedule()->current_plan_pwr_update == 1)
                    get_schedule()->current_plan_pwr_update = 2;

                modbus_data_pack(PWR_INDEX);

                cmd_enable(PWR_INDEX, 1);
                if (cmd_list.pb[PWR_INDEX].status == 0)
                {
                    cmd_list.pb[PWR_INDEX].status = 1;
                    cmd_list.retry_count = RETRY_COUNT;
                    cmd_list.cmd_seq = PWR_INDEX;
                    cmd_list.addr = 0;

                }

                else  if (cmd_list.pb[PWR_INDEX].status == 2)
                {
                    cmd_list.pb[PWR_INDEX].status = 0;
                    modbus_tx.update = 0;
                    cmd_list.cmd_seq = PID_INDEX;
                    cmd_list.pb[PID_INDEX].status = 0;
                    get_config()->update_setting = 0;
                    registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);
                }

                modbus_proc_poll();//

            }
            else
            {
                cmd_list.pb[PWR_INDEX].status = 0;
                modbus_tx.update = 0;
                get_config()->update_setting = 0;

            }

        }
        else
        {

            modbus_tx.update = 0;

            if (GetTickResult(MODBUS_MQTT_PID_TICK_NO) == 1
                    && get_config()->mode <= 2) //pid poll
            {
                if (get_config()->mode != 3)
                {
                    modbus_data_pack(PID_INDEX);
                    cmd_enable(PID_INDEX, 1);
                    if (cmd_list.pb[PID_INDEX].status == 0)
                    {
                        cmd_list.pb[PID_INDEX].status = 1;
                        cmd_list.retry_count = RETRY_COUNT;
                        cmd_list.cmd_seq = PID_INDEX;
                        cmd_list.addr = 0;

                    }
                    else  if (cmd_list.pb[PID_INDEX].status == 2)
                    {
                        reset_registerTick(MODBUS_MQTT_PID_TICK_NO);
                        registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);
                        cmd_list.pb[PID_INDEX].status = 0;

                    }
                    modbus_proc_poll();//

                }

            }
            else  //read dev
            {
                registerTick(MODBUS_MQTT_PID_TICK_NO, PID_TICK_TIME);
                registerTick(MODBUS_POLL_TICK_NO, MODBUS_POLL_TIME);
                if (GetTickResult(MODBUS_POLL_TICK_NO) == 1)
                {



                    if (cmd_list.pb[STATUS1_INDEX].status == 0)
                    {
                        modbus_data_pack(STATUS1_INDEX);
                        cmd_enable(STATUS1_INDEX, 1);
                        cmd_list.pb[STATUS1_INDEX].status = 1;
                        cmd_list.retry_count = RETRY_COUNT;
                        cmd_list.cmd_seq = STATUS1_INDEX;
                        cmd_list.addr = 0;

                    }
                    else  if (cmd_list.pb[STATUS1_INDEX].status == 2)
                    {
                        modbus_data_pack(STATUS2_INDEX);
                        cmd_enable(STATUS2_INDEX, 1);
                        //reset_registerTick(MODBUS_POLL_TICK_NO);
                        //registerTick(MODBUS_POLL_TICK_NO, MODBUS_POLL_TIME);
                        cmd_list.pb[STATUS1_INDEX].status = 1;
                        cmd_list.pb[STATUS2_INDEX].status = 1;
                        cmd_list.retry_count = RETRY_COUNT;
                        cmd_list.cmd_seq = STATUS2_INDEX;
                        cmd_list.addr = 0;

                    }

                    else  if (cmd_list.pb[STATUS2_INDEX].status == 2)
                    {
                        // reset_registerTick(MODBUS_POLL_TICK_NO);
                        // registerTick(MODBUS_POLL_TICK_NO, MODBUS_POLL_TIME);
                        cmd_list.pb[STATUS1_INDEX].status = 0;
                        cmd_list.pb[STATUS2_INDEX].status = 0;

                    }
                    modbus_proc_poll();//

                }
            }
        }

    }
    else
        rs485_recv();
}


