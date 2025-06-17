#include "modbus.h"
#include "crc.h"
#include "cmd_queue.h"

// 假设从站寄存器数组


modbus_stru modbus_usr;
modbus_reg_cmd_stru modbus_cmd_list;
//{0x0000,   {0x03, 0X04},    2,          0,      0,     0,           0},
//   reg      support cmd  cmd count ,uintval int val float val data type uint
modbus_reg_stru  modbus_reg_list[REG_COUNT] =
{

    {0x0000, {0x03, 0X04}, 2,0,0,0,0},
    {0x0001, {0x03, 0X04}, 2,0,0,0,0},
    {0x0002, {0x03, 0X04}, 2,0,0,0,2},
    {0x0003, {0x03, 0X04}, 2,0,0,0,2},
    {0x0104, {0x03, 0X04}, 2,0,0,0,2},
    {0x0105, {0x03, 0X04}, 2,0,0,0,2},
    {0x0106, {0x03, 0X04}, 2,0,0,0,2},
    {0x0107, {0x03, 0X04}, 2,0,0,0,2},
    {0x0108, {0x03, 0X04, 0x06, 0X10}, 4,0,0,0,2},
    {0x0109, {0x03, 0X04, 0x06, 0X10}, 4,0,0,0,2},
    {0x012c, {0x03, 0X04, 0x06, 0X10}, 4,0,0,0,0},
    {0x012d, {0x03, 0X04, 0x06, 0X10}, 4,0,0,0,0},
    {0x012e, {0x03, 0X04, 0x06, 0X10}, 4,0,0,0,0},
    {0x012f, {0x03, 0X04, 0x06, 0X10}, 4,0,0,0,0},
    {0x0130, {0x03, 0X04, 0x06, 0X10}, 4,0,0,0,0},
    {0x0131, {0x03, 0X04, 0x06, 0X10}, 4,0,0,0,0}

};


void modbus_init()
{
    modbus_cmd_list.pb = modbus_reg_list;

}

unsigned char cmd_support_check(void)
{
	unsigned int i,j;
	unsigned char result;
	result = 0;
	for(i=0;i<REG_COUNT;i++)
	{
		if(modbus_usr.RegStart == modbus_cmd_list.pb[i].Reg)
		{
		    for(j=0;j<modbus_cmd_list.pb[j].CmdSize;j++)
		    {
			if(modbus_cmd_list.pb[i].SupportCmd[j] == modbus_usr.Func)
				result = 1;
			    break;
			}
			if(j == SUPPORT_CMD_SIZE)
				break;
			
		}
	}
	return result;
}
unsigned char ModbusResponseAnalyMaster(unsigned char *pb)
{
    unsigned char func;
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
unsigned char ModbusResponseAnalySlave(unsigned char *pb)
{
    unsigned char func;
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
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = pb[4];
            modbus_usr.RegCount = modbus_usr.RegCount << 8;
            modbus_usr.RegCount = modbus_usr.RegCount | pb[5];
            modbus_usr.len =  modbus_usr.RegCount*2;
            crcRecv = pb[7];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[6];
            crcCal = CRC_Compute(pb, 6);
            if (crcCal != crcRecv)
            {
                return 0;
            }
            else  //data to be response
            {
                ;
            }
        }
        else if (modbus_usr.Func == MODBUS_FUNC_ONE_WRITE )
        {
            modbus_usr.RegStart = pb[2];
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = 1;
            modbus_usr.len =  modbus_usr.RegCount*2;
            crcRecv = pb[ modbus_usr.len+5];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[ modbus_usr.len+4];
            crcCal = CRC_Compute(pb,  modbus_usr.len +4);
            if (crcCal != crcRecv)
            {
                return 0;
            }
            else  //data to be write
            {
                memset(modbus_usr.payload, 0, MODBUS_RECV_SIZE);
                memcpy(modbus_usr.payload, pb + 2, modbus_usr.len +2);
            }
        }
        else if ( modbus_usr.Func == MODBUS_FUNC_MUL_WRITE)
        {
            modbus_usr.RegStart = pb[2];
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = pb[4];
            modbus_usr.RegCount = modbus_usr.RegCount << 8;
            modbus_usr.RegCount = modbus_usr.RegCount | pb[5];

            modbus_usr.len =  pb[6];
            crcRecv = pb[modbus_usr.len+8];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[modbus_usr.len+7];
            crcCal = CRC_Compute(pb, modbus_usr.len +7);
            if (crcCal != crcRecv)
            {
                return 0;
            }
            else  //data to be writeN BV
            {
                memset(modbus_usr.payload, 0, MODBUS_RECV_SIZE);
                memcpy(modbus_usr.payload, pb + 2,  modbus_usr.len +5);
            }
        }	 
    }
    else
    {
        return 0;

    }
    return modbus_usr.Func;
}
void modbus_data_pack()
{
	unsigned char tx_buf[128];
	unsigned char i,j,k;
	unsigned int reg_tmp;
	i = 0;
	tx_buf[i++] = modbus_usr.DevAddr;
	tx_buf[i++] = modbus_usr.Func;

	if(modbus_usr.Func == MODBUS_FUNC_ONLYREAD ||
		modbus_usr.Func == MODBUS_FUNC_READ)
	{
		reg_tmp = modbus_usr.RegStart;
		tx_buf[i++] = modbus_usr.RegStart>>8;//dat
		tx_buf[i++] = modbus_usr.RegStart>>8 ;

		for(k=0;k<modbus_usr.RegCount;k++)
		{
		for(j = 0;j<REG_COUNT;j++)
		{
			if(modbus_cmd_list.pb[j].Reg == reg_tmp)
			{
			tx_buf[i++] = reg_tmp >> 8;//reg
			tx_buf[i++] = reg_tmp ;
      
			}
				break;
		}
		if(j == REG_COUNT)
		{
		tx_buf[i++] = 0xff;//dat
		tx_buf[i++] = 0xff ;

		}
		reg_tmp = reg_tmp + 1;

		}



		}
}
void modbus_analy_func(unsigned char func)
{
   
    switch (func)
    {
        case MODBUS_FUNC_READ:
			if(cmd_support_check())
				;
            break;
        case MODBUS_FUNC_ONLYREAD:
			if(cmd_support_check())
				;
            break;
        case MODBUS_FUNC_ONE_WRITE:
			if(cmd_support_check())
				;
            break;
        case MODBUS_FUNC_MUL_WRITE:
			if(cmd_support_check())
				;
            break;
        default:
            break;
    }
}

void modbus_recv_proc()
{
    qsize size = 0;
    unsigned char res;
    res = 1;
    size = queue_find_cmd(modbus_usr.request, modbus_usr.DevAddr,
                          QUEUE_MAX_SIZE - 3);
    if (size > 3)
    {
        modbus_usr.Func = ModbusResponseAnalySlave(modbus_usr.request);
        modbus_analy_func(modbus_usr.Func);
		
    }
}
