#include "modbus.h"
#include "crc.h"
#include "cmd_queue.h"
#include "reg.h"
#include "uart_hal.h"
#include "sensor.h"
#include "key.h"

#include "flash.h"

// 鍋囪浠庣珯瀵勫瓨鍣ㄦ暟�?
unsigned char partNo[] = {"BS-6AS"};
modbus_stru modbus_usr;
modbus_reg_cmd_stru modbus_cmd_list;
//{0x0000,   {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},    2,          0,      0,     0,           0},
//   reg      support cmd  cmd count ,uintval int val float val data type uint
modbus_reg_cmdCheck_stru  modbus_reg_list[REG_COUNT] =
{

    {REG_X100,          {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},                                               2},
    {REG_X10,           {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},                                               2},
    {REG_HF16,          {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},                                               2},
    {REG_LF16,          {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},                                               2},
    {REG_RANGZ_HF16,    {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},                                               2},
    {REG_RANGZ_LF16,    {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},                                               2},
    {REG_RANGF_HF16,    {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},                                               2},
    {REG_RANGF_LF16,    {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD},                                               2},
    {REG_OFFSET_HF16,   {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD, MODBUS_FUNC_ONE_WRITE, MODBUS_FUNC_MUL_WRITE}, 4},
    {REG_OFFSET_LF16,   {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD, MODBUS_FUNC_ONE_WRITE, MODBUS_FUNC_MUL_WRITE}, 4},
    {REG_RATE,          {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD, MODBUS_FUNC_ONE_WRITE, MODBUS_FUNC_MUL_WRITE}, 4},
    {REG_DEV_ADDR,      {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD, MODBUS_FUNC_ONE_WRITE, MODBUS_FUNC_MUL_WRITE}, 4},
    {REG_CHECK,         {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD, MODBUS_FUNC_ONE_WRITE, MODBUS_FUNC_MUL_WRITE}, 4},
    {REG_DECM_BIT,      {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD, MODBUS_FUNC_ONE_WRITE, MODBUS_FUNC_MUL_WRITE}, 4},
    {REG_UNIT,          {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD, MODBUS_FUNC_ONE_WRITE, MODBUS_FUNC_MUL_WRITE}, 4},
    {REG_ADC_RATE,      {MODBUS_FUNC_READ, MODBUS_FUNC_ONLYREAD, MODBUS_FUNC_ONE_WRITE, MODBUS_FUNC_MUL_WRITE}, 4}

};


void modbus_init()
{
    //modbus_cmd_list.pb = modbus_reg_list.RegCmd;
    modbus_usr.DevAddr = GetReg()->pb[eREG_DEV_ADDR].val_u;
    modbus_cmd_list.RegNum = GetReg()->pb;
    modbus_cmd_list.RegCmdDat = modbus_reg_list;

}

unsigned char cmd_support_check(void)
{
    unsigned int i, j;
    unsigned char result;
    //unsigned int reg_remap;
    result = 0;
    for (i = 0; i < REG_SIZE; i++)
    {
//    modbus_cmd_list.RegNum = &(GetReg()->pb[i]);
//              reg_remap = modbus_cmd_list.RegNum[i].reg_remap;
        if (modbus_usr.RegStart == GetReg()->pb[i].reg_remap)
        {
            modbus_cmd_list.RegNum = &GetReg()->pb[i];
            modbus_cmd_list.RegCmdDat = &modbus_reg_list[i];
            for (j = 0; j < modbus_cmd_list.RegCmdDat[j].CmdSize; j++)
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
unsigned char ModbusResponseAnalySlave()
{
//    unsigned char func;
   // unsigned char i;
    unsigned char j;
    unsigned int crcRecv, crcCal;
    unsigned char *pb;
   // i = 0;
    for (j = 0; j <= getuart()->index; j++)
    {
        if (getuart()->rx_buf[j] == modbus_usr.DevAddr || getuart()->rx_buf[j] == 0xff)
        {

            if (getuart()->rx_buf[j + 1] == MODBUS_FUNC_READ ||
                    getuart()->rx_buf[j + 1] == MODBUS_FUNC_ONLYREAD ||
                    getuart()->rx_buf[j + 1] == MODBUS_FUNC_ONE_WRITE ||
                    getuart()->rx_buf[j + 1] == MODBUS_FUNC_MUL_WRITE ||
                    getuart()->rx_buf[j + 1] == PC_FUNC_REG_READ ||
                    getuart()->rx_buf[j + 1] == PC_FUNC_REG_WRITE ||
                    getuart()->rx_buf[j + 1] == PC_FUNC_SET_REGS_WRITE ||
                    getuart()->rx_buf[j + 1] == PC_FUNC_READ_REGS)
            {
                modbus_usr.Addr = getuart()->rx_buf[j];
                modbus_usr.Func = getuart()->rx_buf[j + 1];
                pb = &getuart()->rx_buf[j];
                break;
            }
        }
    }


    if (modbus_usr.Addr == modbus_usr.DevAddr)
    {
        if (modbus_usr.Func == PC_FUNC_REG_READ ||
                modbus_usr.Func == PC_FUNC_READ_REGS ||
                modbus_usr.Func == MODBUS_FUNC_READ ||
                modbus_usr.Func == MODBUS_FUNC_ONLYREAD)
        {
            modbus_usr.RegStart = pb[2];
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = pb[4];
            modbus_usr.RegCount = modbus_usr.RegCount << 8;
            modbus_usr.RegCount = modbus_usr.RegCount | pb[5];
            modbus_usr.len =  modbus_usr.RegCount * 2;
            crcRecv = pb[6];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[7];
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
        else if (modbus_usr.Func == PC_FUNC_REG_WRITE ||
                 modbus_usr.Func == MODBUS_FUNC_ONE_WRITE)
        {
            modbus_usr.RegStart = pb[2];
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = 1;
            modbus_usr.len =  modbus_usr.RegCount * 2;
            crcRecv = pb[ modbus_usr.len + 4];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[ modbus_usr.len + 5];
            crcCal = CRC_Compute(pb,  modbus_usr.len + 4);
            if (crcCal != crcRecv)
            {
                return 0;
            }
            else  //data to be write
            {
                memset(modbus_usr.payload, 0, MODBUS_RECV_SIZE);
                memcpy(modbus_usr.payload, pb + 2, modbus_usr.len + 2);
            }
        }
        else if (modbus_usr.Func == PC_FUNC_SET_REGS_WRITE)
        {
            modbus_usr.RegStart = pb[2];
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = 1;
            modbus_usr.len =  modbus_usr.RegCount * 2;
            crcRecv = pb[ modbus_usr.len + 6];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[ modbus_usr.len + 7];
            crcCal = CRC_Compute(pb,  modbus_usr.len + 6);
            if (crcCal != crcRecv)
            {
                return 0;
            }
            else  //data to be write
            {
                memset(modbus_usr.payload, 0, MODBUS_RECV_SIZE);
                memcpy(modbus_usr.payload, pb + 2, modbus_usr.len + 4);
            }
        }
        else if (modbus_usr.Func == MODBUS_FUNC_MUL_WRITE)
        {
            modbus_usr.RegStart = pb[2];
            modbus_usr.RegStart = modbus_usr.RegStart << 8;
            modbus_usr.RegStart = modbus_usr.RegStart | pb[3];
            modbus_usr.RegCount = pb[4];
            modbus_usr.RegCount = modbus_usr.RegCount << 8;
            modbus_usr.RegCount = modbus_usr.RegCount | pb[5];

            modbus_usr.len =  pb[6];
            //  modbus_usr.len =  modbus_usr.len << 8 + pb[7];
            crcRecv = pb[modbus_usr.len + 7];
            crcRecv = crcRecv << 8;
            crcRecv = crcRecv | pb[modbus_usr.len + 8];
            crcCal = CRC_Compute(pb, modbus_usr.len + 7);
            if (crcCal != crcRecv)
            {
                return 0;
            }
            else  //data to be writeN BV
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
    uart_tx(modbus_usr.tx_buf, len + 2);

}
void modbus_read_pack(void)
{
    //unsigned char tx_buf[128];
    unsigned char i, k;
//  unsigned int reg_tmp;
    i = 0;
    modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
    modbus_usr.tx_buf[i++] = modbus_usr.Func;

    if (modbus_usr.Func == MODBUS_FUNC_ONLYREAD ||
            modbus_usr.Func == MODBUS_FUNC_READ)
    {
        //reg_tmp = modbus_usr.RegStart;
        //tx_buf[i++] = modbus_usr.len>>8;//dat
        modbus_usr.tx_buf[i++] = modbus_usr.len ;

        for (k = 0; k < modbus_usr.RegCount; k++)
        {
            DatConvToModbus(modbus_usr.RegStart + k, &modbus_usr.tx_buf[i]);
            i = i + 2;
        }
//        modbus_usr.crc =  CRC_Compute(tx_buf, modbus_usr.len + 3);
//
//        tx_buf[i++] = modbus_usr.crc;
//      tx_buf[i++] = modbus_usr.crc >> 8;
        modbus_tx(modbus_usr.tx_buf, modbus_usr.len + 3);
    }
//    uart_tx(tx_buf, i);
//    SysTickDelay(5);
}

void modbus_Usr_read_pack(void)
{
    // unsigned char tx_buf[128];
    unsigned char i, k;
    i = 0;
    if (modbus_usr.Func == PC_FUNC_REG_READ ||
            modbus_usr.Func == PC_FUNC_READ_REGS)
    {
        if (modbus_usr.Func == PC_FUNC_REG_READ)
        {
            if (modbus_usr.RegStart == 0x0000) //read ver range PartNo
            {

                modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
                modbus_usr.tx_buf[i++] = modbus_usr.Func;
                modbus_usr.tx_buf[i++] = 2;
                modbus_usr.tx_buf[i++] = 0;
                modbus_usr.tx_buf[i++] = VERSION;
                for (k = 0; k < strlen(partNo); k++)
                    modbus_usr.tx_buf[i++] = partNo[k];
                modbus_usr.tx_buf[2] = i - 3;
                modbus_tx(modbus_usr.tx_buf, modbus_usr.tx_buf[2] + 3);
            }
            else if (modbus_usr.RegStart == 0x0001) //read ver range PartNo
            {

                modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
                modbus_usr.tx_buf[i++] = modbus_usr.Func;
                modbus_usr.tx_buf[i++] = 2;
                modbus_usr.tx_buf[i++] = 0;
                modbus_usr.tx_buf[i++] = VERSION;
                for (k = 0; k < strlen(partNo); k++)
                    modbus_usr.tx_buf[i++] = partNo[k];
                modbus_usr.tx_buf[2] = i - 3;
                modbus_tx(modbus_usr.tx_buf, modbus_usr.tx_buf[2] + 3);
            }
        }
        else if (modbus_usr.Func == PC_FUNC_READ_REGS)
        {

            i = 0;
            modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
            modbus_usr.tx_buf[i++] = modbus_usr.Func;
            modbus_usr.tx_buf[i++] = modbus_usr.len ;
            for (k = 0; k < modbus_usr.RegCount; k++)
            {
                REGDatConvToPC(modbus_usr.RegStart + k, &modbus_usr.tx_buf[i]);
                i = i + 2;
            }
            modbus_tx(modbus_usr.tx_buf, modbus_usr.len + 3);
        }
    }
}

void modbus_oneReg_write_pack(void)
{

    //unsigned char tx_buf[256];
    unsigned int i, j;
    i = 0;
    if (modbus_usr.Func == MODBUS_FUNC_ONE_WRITE)
    {
        RegDatConv(modbus_usr.RegStart, &modbus_usr.payload[0], CHANGE_DAT);
        if (modbus_usr.RegStart == GetReg()->pb[eREG_RATE].reg_remap ||
                modbus_usr.RegStart == GetReg()->pb[eREG_CHECK].reg_remap)
        {
            getuart()->reconfig = 1;
        }
        if (modbus_usr.RegStart == GetReg()->pb[eREG_ADC_RATE].reg_remap)
        {
            *getAdcReconfig() = 1;
        }
        modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
        modbus_usr.tx_buf[i++] = modbus_usr.Func;
        //tx_buf[i++] = modbus_usr.len;
        for (j = 0; j < (modbus_usr.len + 2); j++)
            modbus_usr.tx_buf[i++] = modbus_usr.payload[j];
//      crc_cal =  CRC_Compute(tx_buf, modbus_usr.len + 4);
//
//      tx_buf[i++] = crc_cal;
//      tx_buf[i++] = crc_cal >> 8;
        modbus_tx(modbus_usr.tx_buf, modbus_usr.len + 4);

    }
    if (modbus_usr.Func == PC_FUNC_READ_REGS)
    {
        ModbusSetReg(modbus_usr.RegStart, &modbus_usr.payload[2]);
    }

    modbus_usr.tick = GetTick();
}
void modbus_mulReg_write_pack(void)
{
//  unsigned char tx_buf[128];
    unsigned char i, k;
//  unsigned int reg_tmp;

//  tx_buf[i++] = modbus_usr.DevAddr;
//  tx_buf[i++] = modbus_usr.Func;

    if (modbus_usr.Func == MODBUS_FUNC_MUL_WRITE)
    {
        k = 3;
        for (i = 0; i < modbus_usr.RegCount; i++)
        {
            if ((modbus_usr.RegStart + i) == GetReg()->pb[eREG_RATE].reg_remap ||
                (modbus_usr.RegStart + i) == GetReg()->pb[eREG_CHECK].reg_remap)
            {
                getuart()->reconfig = 1;
            }
            if ((modbus_usr.RegStart + i) == GetReg()->pb[eREG_ADC_RATE].reg_remap)
            {
                *getAdcReconfig() = 1;
            }
            RegDatConv(modbus_usr.RegStart + i, &modbus_usr.payload[k + 2], CHANGE_DAT);
            k = k + 2;
        }
    }
    i = 0;
    modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
    modbus_usr.tx_buf[i++] = modbus_usr.Func;
    //tx_buf[i++] = modbus_usr.len;
    for (k = 0; k < 4; k++)
        modbus_usr.tx_buf[i++] = modbus_usr.payload[k];
    //      crc_cal =  CRC_Compute(tx_buf, modbus_usr.len + 4);
    //
    //      tx_buf[i++] = crc_cal;
    //      tx_buf[i++] = crc_cal >> 8;
    modbus_tx(modbus_usr.tx_buf, 6);

    modbus_usr.tick = GetTick();
	
}
void modbus_PrivaReg_write_pack(void)
{
//  unsigned char tx_buf[128];
    unsigned char i, k;
    uint32_t tmp;
//  unsigned int reg_tmp;

//  tx_buf[i++] = modbus_usr.DevAddr;
//  tx_buf[i++] = modbus_usr.Func;

    if (modbus_usr.Func == MODBUS_FUNC_MUL_WRITE)
    {
        k = 3;
        for (i = 0; i < modbus_usr.RegCount; i++)
        {
		
			if ((modbus_usr.RegStart + i) == 0x0006)
			{
				GetRegPrivate()->typ = modbus_usr.payload[k];
				GetRegPrivate()->typ = modbus_usr.payload[k + 1];				
			}
			else if ((modbus_usr.RegStart + i) == 0x0007)
			{
				GetRegPrivate()->pga = modbus_usr.payload[k];
				GetRegPrivate()->pga = modbus_usr.payload[k + 1];					
			}
			else if ((modbus_usr.RegStart + i) == 0x0008)
			{
				GetRegPrivate()->sei = modbus_usr.payload[k];
				GetRegPrivate()->sei = modbus_usr.payload[k + 1];					
			}
			else if ((modbus_usr.RegStart + i) == 0x0009)
			{
				GetRegPrivate()->unit = modbus_usr.payload[k];
				GetRegPrivate()->unit = modbus_usr.payload[k + 1];				
			}
			else if ((modbus_usr.RegStart + i) == 0x000a)
			{
			   GetRegPrivate()->coe = uint32TofloatR(&modbus_usr.payload[k]);
				
			}
			else if ((modbus_usr.RegStart + i) == 0x000c)
			{
				GetRegPrivate()->offset = modbus_usr.payload[k];
//				GetRegPrivate()->offset = GetRegPrivate()->offset << 8 +
					               //       modbus_usr.payload[k + 1];				
			}
			else if ((modbus_usr.RegStart + i) == 0x000d)
			{
				GetRegPrivate()->coe1 = uint32TofloatR(&modbus_usr.payload[k]);				
			}
			else if ((modbus_usr.RegStart + i) == 0x000f)
			{
				GetRegPrivate()->coe2 = uint32TofloatR(&modbus_usr.payload[k]);				
			}
			 else if ((modbus_usr.RegStart + i) == 0x0011)
			 {
				 GetRegPrivate()->coe3 = uint32TofloatR(&modbus_usr.payload[k]); 			 
			 }
			 else if ((modbus_usr.RegStart + i) == 0x0013)
			 {
				 GetRegPrivate()->coe4 = uint32TofloatR(&modbus_usr.payload[k]); 			 
			 }
		     k = k + 2;
        }
    }
    i = 0;
    modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
    modbus_usr.tx_buf[i++] = modbus_usr.Func;
    //tx_buf[i++] = modbus_usr.len;
    for (k = 0; k < 4; k++)
        modbus_usr.tx_buf[i++] = modbus_usr.payload[k];
    //      crc_cal =  CRC_Compute(tx_buf, modbus_usr.len + 4);
    //
    //      tx_buf[i++] = crc_cal;
    //      tx_buf[i++] = crc_cal >> 8;
    modbus_tx(modbus_usr.tx_buf, 6);

    modbus_usr.tick = GetTick();
}

void modbus_mulRegChange_write_pack(void)
{
//  unsigned char tx_buf[128];
    unsigned char i, k;
//  unsigned int reg_tmp;

//  tx_buf[i++] = modbus_usr.DevAddr;
//  tx_buf[i++] = modbus_usr.Func;

    if (modbus_usr.Func == PC_FUNC_SET_REGS_WRITE)
    {
        k = 0;
        for (i = 0; i < modbus_usr.RegCount; i++)
        {
            RegDatConv(modbus_usr.RegStart + i, &modbus_usr.payload[k + 2], CHANGE_REG);
            k = k + 2;
        }
    }
    i = 0;
    modbus_usr.tx_buf[i++] = modbus_usr.DevAddr;
    modbus_usr.tx_buf[i++] = modbus_usr.Func;
    //tx_buf[i++] = modbus_usr.len;
    for (k = 0; k < 6; k++)
        modbus_usr.tx_buf[i++] = modbus_usr.payload[k];
    //      crc_cal =  CRC_Compute(tx_buf, modbus_usr.len + 4);
    //
    //      tx_buf[i++] = crc_cal;
    //      tx_buf[i++] = crc_cal >> 8;
    modbus_tx(modbus_usr.tx_buf, 8);

    modbus_usr.tick = GetTick();
}

void modbus_analy_func(unsigned char func)
{

    switch (func)
    {
        case PC_FUNC_REG_READ:
            modbus_Usr_read_pack();
            break;

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
        case PC_FUNC_SET_REGS_WRITE:
            modbus_mulRegChange_write_pack();
            break;
		case PC_FUNC_REG_WRITE:
				modbus_PrivaReg_write_pack();
			break;

//        case USR_FUNC_ONE_WRITE:
//            if (cmd_support_check())
//                modbus_oneReg_write_pack();
//            break;
//        case USR_FUNC_MUL_WRITE:
//            if (cmd_support_check())
//                modbus_mulReg_write_pack();
//            break;

        default:
            break;
    }
}

void modbus_recv_proc()
{
    uart_stru  *p;
//    unsigned char res;
    // res = 1;

   if(getKey()->update == 0)
   	{
   p = getuart();
   
   if (p->recv_update == 1)
   {
	   modbus_usr.Func = ModbusResponseAnalySlave();
	   modbus_analy_func(modbus_usr.Func);
	   p->recv_update = 0;
	   memset(p->rx_buf, 0, RX_BUF_SIZE);
	   p->index = 0;
   }
   if ((GetTick() - modbus_usr.tick) >= 5000
		   && modbus_usr.tick != 0) //delay 5s write to flash
   {
	   modbus_usr.tick = 0;
	   GetReg()->update = 1;
   }

   }
   else
   	{
   	    
		 modbus_usr.DevAddr =  getKey()->indat[0].addr;
   }


}
