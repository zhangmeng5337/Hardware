#include "modbus.h"
#include "crc.h"
#include "uart.h"


// 鍋囪浠庣珯瀵勫瓨鍣ㄦ暟�?
unsigned char partNo[] = {"CNG-35"};
modbus_stru modbus_usr;
modbus_reg_cmd_stru modbus_cmd_list;
//{0x0000,   {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},    2,          0,      0,     0,           0},
//   reg      support cmd  cmd count ,uintval int val float val data type uint
modbus_reg_cmdCheck_stru  modbus_reg_list[REG_COUNT] =
{

    {0x0001, {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD}, 2, 1,0,{0,1},1}
};


void modbus_init()
{
    //modbus_cmd_list.pb = modbus_reg_list.RegCmd;
    modbus_usr.DevAddr;
    //    if (modbus_usr.DevAddr > DEV_ADDR_MAX)
    //        modbus_usr.DevAddr = 1;
    //    modbus_cmd_list.RegNum = GetReg()->pb;
    modbus_cmd_list.RegCmdDat = modbus_reg_list;

    //getuart()->recv_update = 0;
    //memset(get_uart_recv(RS485_No)->recv_buf, 0, RX_BUF_SIZE);
    //getuart()->index = 0;

}

unsigned char cmd_support_check(void)
{
    unsigned int i, j;
    unsigned char result;
    unsigned char cmdsize;
    //unsigned int reg_remap;
    result = 0;
    for (i = 0; i < REG_COUNT; i++)
    {
        //    modbus_cmd_list.RegNum = &(GetReg()->pb[i]);
        //              reg_remap = modbus_cmd_list.RegNum[i].reg_remap;
        if (modbus_usr.RegStart == modbus_reg_list[i].Reg
                && modbus_reg_list[i].enable == 1)
        {
            //            modbus_cmd_list.RegNum = &modbus_reg_list[i].Reg;
            modbus_cmd_list.RegCmdDat = &modbus_reg_list[i];
            cmdsize  = modbus_cmd_list.RegCmdDat->CmdSize;
            for (j = 0; j < cmdsize; j++)
            {
                if (modbus_cmd_list.RegCmdDat->SupportCmd[j] == modbus_usr.Func)
                {
                    result = 1;
                    break;
                }
            }
            if (j == SUPPORT_CMD_SIZE)
                break;
            break;
        }

    }
    return result;
}
unsigned char ModbusResponseAnalyMaster(unsigned char *pb)
{
    //    unsigned char func;
    unsigned char i;
    unsigned char len;
    unsigned int crcRecv, crcCal;

    i = 0;
    modbus_usr.Addr = pb[i++];
    modbus_usr.Func = pb[i++];

    if (modbus_usr.Addr == modbus_usr.DevAddr)
    {
        if (modbus_usr.Func == MODBUS_FUNC_READ ||
                modbus_usr.Func == MODBUS_FUNC_ONLYREAD)
        {
            modbus_usr.RegStart = pb[2];
            len =  pb[2];
            crcRecv = pb[len + 4];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[len + 3];
            crcCal = CRC_Compute(pb, len + 3);
            if (crcCal != crcRecv)
            {
                return 0;
            }
            else
            {
                memset(modbus_usr.payload, 0, MODBUS_RECV_SIZE);
                memcpy(modbus_usr.payload, pb + 3, len);
            }
        }
        else if (modbus_usr.Func == MODBUS_FUNC_ONE_WRITE ||
                 modbus_usr.Func == MODBUS_FUNC_MUL_WRITE)
        {
            len =  8;
            crcRecv = pb[7];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[6];
            crcCal = CRC_Compute(pb, len - 2);
            if (crcCal != crcRecv)
            {
                return 0;
            }
            else
            {
                memset(modbus_usr.payload, 0, MODBUS_RECV_SIZE);
                memcpy(modbus_usr.payload, pb + 2, len - 2);
            }
        }
    }
    else
    {
        return 0;

    }
    return modbus_usr.Func;
}
uint32_t real_rx_f = 0;

unsigned char ModbusResponseAnalySlave()
{
    //    unsigned char func;
    // unsigned char i;
    unsigned char j;
    unsigned int crcRecv, crcCal;
    unsigned char *pb;
    // i = 0;
    for (j = 0; j <= get_uart_recv(RS485_No)->recv_len; j++)

    {
        if (get_uart_recv(RS485_No)->recv_buf[j] == modbus_usr.DevAddr || 
			get_uart_recv(RS485_No)->recv_buf[j] == 0xff)
        {

            if (get_uart_recv(RS485_No)->recv_buf[j + 1] == MODBUS_FUNC_READ ||
                    get_uart_recv(RS485_No)->recv_buf[j + 1] == MODBUS_FUNC_ONLYREAD ||
                    get_uart_recv(RS485_No)->recv_buf[j + 1] == MODBUS_FUNC_ONE_WRITE ||
                    get_uart_recv(RS485_No)->recv_buf[j + 1] == MODBUS_FUNC_MUL_WRITE)
            {
                modbus_usr.Addr = get_uart_recv(RS485_No)->recv_buf[j];
                modbus_usr.Func = get_uart_recv(RS485_No)->recv_buf[j + 1];
                pb = &get_uart_recv(RS485_No)->recv_buf[j];
                break;
            }
        }
    }


    if (modbus_usr.Addr == modbus_usr.DevAddr)
    {
        if (modbus_usr.Func == MODBUS_FUNC_READ ||
                modbus_usr.Func == MODBUS_FUNC_ONLYREAD ||
                modbus_usr.Func == MODBUS_FUNC_MUL_WRITE)
        {

            modbus_usr.RegStart = pb[2];
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = pb[4];
            modbus_usr.RegCount = modbus_usr.RegCount << 8;
            modbus_usr.RegCount = modbus_usr.RegCount | pb[5];

            if (modbus_usr.Func == MODBUS_FUNC_READ ||
                    modbus_usr.Func == MODBUS_FUNC_ONLYREAD)
            {
                modbus_usr.len =  modbus_usr.RegCount * 2;
                crcRecv = pb[6];
                crcRecv = crcRecv << 8;
                crcRecv = crcRecv | pb[7];
                //crcCal = CRC_Compute(pb, modbus_usr.len+4);
            }
            if (modbus_usr.Func == MODBUS_FUNC_MUL_WRITE)
            {
                modbus_usr.len =  pb[6];
                //  modbus_usr.len =  modbus_usr.len << 8 + pb[7];
                crcRecv = pb[modbus_usr.len + 7];
                crcRecv = crcRecv << 8;
                crcRecv = crcRecv | pb[modbus_usr.len + 8];
                //crcCal = CRC_Compute(pb, modbus_usr.len+4);
            }
        }
        else if (modbus_usr.Func == MODBUS_FUNC_ONE_WRITE)
        {
            modbus_usr.RegStart = pb[2];
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = 1;
            modbus_usr.len =  modbus_usr.RegCount * 2;
            crcRecv = pb[ modbus_usr.len + 4];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[ modbus_usr.len + 5];
            //crcCal = CRC_Compute(pb,  modbus_usr.len + 4);
        }
        crcCal = CRC_Compute(pb,  modbus_usr.len + 4);
        if (crcCal != crcRecv)
        {
            return 0;
        }
        else
        {

            if (modbus_usr.Func == MODBUS_FUNC_ONE_WRITE)
            {
                memset(modbus_usr.payload, 0, MODBUS_RECV_SIZE);
                memcpy(modbus_usr.payload, pb + 2, modbus_usr.len + 2);
            }

            else if (modbus_usr.Func == MODBUS_FUNC_MUL_WRITE)
            {
                memset(modbus_usr.payload, 0, MODBUS_RECV_SIZE);
                memcpy(modbus_usr.payload, pb + 2,  modbus_usr.len + 6);
            }
        }

    }
    else
    {
        return 0;

    }
    return modbus_usr.Func;
}
void modbus_tx(unsigned char *buf, unsigned int len)
{
    unsigned char *pb;
    pb = buf;
    modbus_usr.crc =  CRC_Compute(pb, len);
    modbus_usr.tx_buf[len + 1] = modbus_usr.crc;
    modbus_usr.tx_buf[len] = modbus_usr.crc >> 8;
    uart_transmit(RS485_No,modbus_usr.tx_buf,len + 2);
}
uint32_t real_rx = 0;
unsigned int findRegIndex(unsigned int reg)
{
   unsigned int k;
   unsigned int index;
   index = REG_COUNT;
        for (k = 0; k < REG_COUNT; k++)
        {
            if(reg == modbus_reg_list[k].Reg)
            {
				index = k;
				break;
			}
        }
	return index;
}
void modbus_read_pack(void)
{
    //unsigned char tx_buf[128];
    unsigned char i, index,l;
    //  unsigned int reg_tmp;
    i = 0;
    modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
    modbus_usr.tx_buf[i++] = modbus_usr.Func;

    if (modbus_usr.Func == MODBUS_FUNC_ONLYREAD ||
            modbus_usr.Func == MODBUS_FUNC_READ)
    {
        real_rx = real_rx + 8;

        modbus_usr.tx_buf[i++] = modbus_usr.len ;

		index = findRegIndex(modbus_usr.RegStart);
        for (l = index; l < (index + modbus_usr.RegCount); l++)
        {
            if(index < REG_COUNT)
            {
			  	modbus_usr.tx_buf[i++] = modbus_reg_list[l].payload[0];
			  	modbus_usr.tx_buf[i++] = modbus_reg_list[l].payload[1];			  
			}

        }		
        modbus_tx(modbus_usr.tx_buf, modbus_usr.len + 3);
    }

}



void modbus_oneReg_write_pack(void)
{

    //unsigned char tx_buf[256];
    unsigned int i, j,index,l,m;
    i = 0;
    if (modbus_usr.Func == MODBUS_FUNC_ONE_WRITE)
    {



        modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
        modbus_usr.tx_buf[i++] = modbus_usr.Func;
		index = findRegIndex(modbus_usr.RegStart);
		j = 0;
		m = 2;
		//for (l = index; l < (index + modbus_usr.RegCount); l++)
        //{
            if(index < REG_COUNT)
            {

               	modbus_reg_list[l].payload[0] = modbus_usr.payload[m++];
               	modbus_reg_list[l].payload[1] = modbus_usr.payload[m++];			  
			}

       // }
        //tx_buf[i++] = modbus_usr.len;

        for (j = 0; j < (modbus_usr.len + 2); j++)
            modbus_usr.tx_buf[i++] = modbus_usr.payload[j];
        modbus_tx(modbus_usr.tx_buf, modbus_usr.len + 4);

    }

}
void modbus_mulReg_write_pack(void)
{
    //  unsigned char tx_buf[128];
    unsigned char i, k;
	unsigned int index,j,l,m;
    //  unsigned int reg_tmp;

    //  tx_buf[i++] = modbus_usr.DevAddr;
    //  tx_buf[i++] = modbus_usr.Func;
		index = findRegIndex(modbus_usr.RegStart);
		j = 0;
		m = 5;
		for (l = index; l < (index + modbus_usr.RegCount); l++)
        {
            if(index < REG_COUNT)
            {

               	modbus_reg_list[l].payload[0] = modbus_usr.payload[m++];
               	modbus_reg_list[l].payload[1] = modbus_usr.payload[m++];			  
			}

        }

    i = 0;
    modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
    modbus_usr.tx_buf[i++] = modbus_usr.Func;
    //tx_buf[i++] = modbus_usr.len;
    for (k = 0; k < 4; k++)
        modbus_usr.tx_buf[i++] = modbus_usr.payload[k];
    modbus_tx(modbus_usr.tx_buf, 6);


}

void modbus_analy_func(unsigned char func)
{

    switch (func)
    {


        case MODBUS_FUNC_READ:
            if (cmd_support_check())
                modbus_read_pack();
            break;
        case MODBUS_FUNC_ONLYREAD:
            if (cmd_support_check())
                modbus_read_pack();
            break;
        case MODBUS_FUNC_ONE_WRITE:
            if (cmd_support_check())
                modbus_oneReg_write_pack();
            break;
        case MODBUS_FUNC_MUL_WRITE:
            if (cmd_support_check())
                modbus_mulReg_write_pack();
            break;

        default:
            break;
    }
}

void modbus_recv_proc()
{
    rs485_stru  *p;
	p = get_uart_recv(RS485_No);
        if (p->recv_update == 1)
        {
            //cs1237_pwr_pd();
            modbus_usr.Func = ModbusResponseAnalySlave();//rs485 data analy
            p->recv_update = 0;
            memset(p->recv_buf, 0, RSBUFFER_SIZE);
           
            modbus_analy_func(modbus_usr.Func);
            p->recv_len = 0;

        }


}
