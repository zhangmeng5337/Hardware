#include "reg.h"
#include "flash.h"
#include "key.h"
#include "cw32l010_uart.h"
#include "uart_hal.h"



//typedef struct
//{
// unsigned int reg;
// unsigned int reg_remap;
// unsigned char reg_count;
// unsigned char val_c;
// unsigned int val_u;
// int val_i;
// float val_f;
// unsigned char dat_type;//0:unsigned char 1:unsigned int 2:int 3:float
// unsigned char dat_pos;//0:float hight 16bit 1:float low 16bit
// unsigned char record_flag;//0:not record 1:record
//
//}reg_dat_pack;

reg_dat_pack reg_dat_usr[REG_SIZE] =
{
//typedef struct
//{
// unsigned int reg;
// unsigned int reg_remap;
// unsigned char reg_count;
//// unsigned char val_c;
//// unsigned int val_u;
//// int val_i;
//// float val_f;
// uint32_t val_u32ToFloat;
// uint32_t val_u32ToFloat_defaut;
// unsigned char dat_type;//0:unsigned char 1:unsigned int 2:int 3:float
// unsigned char dat_pos;//0:float hight 16bit 1:float low 16bit
// unsigned char record_flag;//0:not record 1:record
// unsigned char enable;
// 
//}reg_dat_pack;

    {REG_X100,        0,  1, 0,          0,          2,  0,      0, 1}, //regx100,1 regcount,
    {REG_X10,         0,  1, 0,          0,          2,  0,      0, 1},
    {REG_HF16,        0,  1, 0x3fcccccd, 0x40266666,  3,  0,     0, 1},
    {REG_LF16,        0,  1, 0x3fcccccd, 0x40266666,  3,  1,     0, 1},
    {REG_RANGZ_HF16,  0,  1, 0,          0,          3,  0,      1, 1},
    {REG_RANGZ_LF16,  0,  1, 0,          0,          3,  1,      1, 1},
    {REG_RANGF_HF16,  0,  1, 0x3f800000, 0x3f800000,  3,  0,     1, 1},
    {REG_RANGF_LF16,  0,  1, 0x3f800000, 0x3f800000,  3,  1,     1, 1},
    {REG_OFFSET_HF16, 0,  1, 0x3f800000, 0x40000000,  3,  0,    1, 1},
    {REG_OFFSET_LF16, 0,  1, 0x3f800000, 0x40000000,  3,  1,    1, 1},
    {REG_DEV_ADDR,    0,  1, 1,          2,          1,  0,      1, 1},
    {REG_RATE,        0,  1, 3,          4,          1,  0,      1, 1},
    {REG_CHECK,       0,  1, 0,          1,          1,  0,      1, 1},
    {REG_DECM_BIT,    0,  1, 0,          2,          1,  0,      1, 1},
    {REG_UNIT,        0,  1, 2,          3,          1,  0,      1, 1},
    {REG_ADC_RATE,    0,  1, 0,          1,          1,  0,      1, 1},
    {REG_CLR_ZEROE,   0,  1,0x01,    0x01,          1,  0,      1, 1},
    {REG_HF16_4,        0,  1, 0x3fcccccd, 0x40266666,  3,  0,     0, 1},
    {REG_LF16_4,        0,  1, 0x3fcccccd, 0x40266666,  3,  1,     0, 1}




};






static reg_stru reg_usr;
static params_private_stru params_private;


reg_stru *GetReg(void)
{
    return &reg_usr;
}
params_private_stru *GetRegPrivate(void)
{
    return &params_private;
}

//4ac00000---->4a c0 00 00
void FlshDatToRegDat(reg_dat_pack *pb, uint8_t *dat, unsigned char type)
{
	uint16_t convUint16;

    pb->val_u32ToFloat = dat[0];
    pb->val_u32ToFloat = pb->val_u32ToFloat << 8 | dat[1];
	convUint16 = dat[2];
	convUint16 = convUint16 << 8 | dat[3];
	pb->val_u32ToFloat_defaut =  convUint16;


}
void FlshDatToRegDatNoDefault(reg_dat_pack *pb, uint8_t *dat, unsigned char type)
{
	uint16_t convUint16;

    pb->val_u32ToFloat = dat[0];
    pb->val_u32ToFloat = pb->val_u32ToFloat << 8 | dat[1];
	//convUint16 = dat[2];
	//convUint16 = convUint16 << 8 | dat[3];
	//pb->val_u32ToFloat_defaut =  convUint16;


}

void FlashDatToRegFloatDat(reg_dat_pack *pb, uint8_t *dat,
                           unsigned char dat_pos)
{
    uint32_t convUint32;
    if (dat_pos == 0)
    {
        convUint32 = dat[0];
        convUint32 = convUint32 << 8 | dat[1];
        convUint32 = convUint32 << 16;
        pb->val_u32ToFloat =  pb->val_u32ToFloat &
                              0x0000ffff;
        pb->val_u32ToFloat =  pb->val_u32ToFloat |
                              convUint32;

        convUint32 = (pb + 1)->val_u32ToFloat &
                     0x0000ffff;
        (pb + 1)->val_u32ToFloat =  pb->val_u32ToFloat |
                                    convUint32;

        convUint32 = dat[2];
        convUint32 = convUint32 << 8 | dat[3];
        convUint32 = convUint32 << 16;
        pb->val_u32ToFloat_defaut =  pb->val_u32ToFloat_defaut &
                                     0x0000ffff;
        pb->val_u32ToFloat_defaut =  pb->val_u32ToFloat_defaut |
                                     convUint32;

        convUint32 = (pb + 1)->val_u32ToFloat_defaut &
                     0x0000ffff;
        (pb + 1)->val_u32ToFloat_defaut =  pb->val_u32ToFloat_defaut |
                                           convUint32;


    }
    else
    {
        convUint32 = dat[0];
        convUint32 = convUint32 << 8 | dat[1];
        // convUint32 = convUint32 << 16;
        (pb + 1)->val_u32ToFloat = (pb + 1)->val_u32ToFloat & //l16
                                   0xffff0000;
        (pb + 1)->val_u32ToFloat = (pb + 1)->val_u32ToFloat |
                                   convUint32;
        convUint32 =  pb->val_u32ToFloat &     //h16
                      0xffff0000;
        (pb)->val_u32ToFloat = (pb + 1)->val_u32ToFloat |
                               convUint32;

        convUint32 = dat[2];
        convUint32 = convUint32 << 8 | dat[3];
        // convUint32 = convUint32 << 16;
        (pb + 1)->val_u32ToFloat_defaut = (pb + 1)->val_u32ToFloat_defaut & //l16
                                          0xffff0000;
        (pb + 1)->val_u32ToFloat_defaut = (pb + 1)->val_u32ToFloat_defaut |
                                          convUint32;
        convUint32 =  pb->val_u32ToFloat_defaut &     //h16
                      0xffff0000;
        (pb)->val_u32ToFloat_defaut = (pb + 1)->val_u32ToFloat_defaut |
                                      convUint32;


    }
}
void ModbusDatToRegFloatDat(reg_dat_pack *pb, uint8_t *dat,
                            unsigned char dat_pos)
{
    uint32_t convUint32;
    if (dat_pos == 0)
    {
        convUint32 = dat[0];
        convUint32 = convUint32 << 8 | dat[1];
        convUint32 = convUint32 << 16;
        pb->val_u32ToFloat =  pb->val_u32ToFloat &
                              0x0000ffff;
        pb->val_u32ToFloat =  pb->val_u32ToFloat |
                              convUint32;

        convUint32 = (pb)->val_u32ToFloat &
                     0xffff0000;
        (pb + 1)->val_u32ToFloat = (pb + 1)->val_u32ToFloat &
                                   0x0000ffff;

        (pb + 1)->val_u32ToFloat = (pb + 1)->val_u32ToFloat |
                                   convUint32;



    }
    else
    {
        convUint32 = dat[0];
        convUint32 = convUint32 << 8 | dat[1];
        // convUint32 = convUint32 << 16;
        (pb)->val_u32ToFloat = (pb)->val_u32ToFloat &   //l16
                               0xffff0000;
        (pb)->val_u32ToFloat = (pb)->val_u32ToFloat |
                               convUint32;

        convUint32 = (pb)->val_u32ToFloat & 0x0000ffff;
        (pb - 1)->val_u32ToFloat = (pb - 1)->val_u32ToFloat &
                                   0xffff0000;
        (pb - 1)->val_u32ToFloat = (pb - 1)->val_u32ToFloat |
                                   convUint32;



//        convUint32 = dat[2];
//        convUint32 = convUint32 << 8 | dat[3];
//        // convUint32 = convUint32 << 16;
//        (pb + 1)->val_u32ToFloat_defaut = (pb + 1)->val_u32ToFloat_defaut & //l16
//                                          0xffff0000;
//        (pb + 1)->val_u32ToFloat_defaut = (pb + 1)->val_u32ToFloat_defaut |
//                                          convUint32;
//        convUint32 = (pb)->val_u32ToFloat_defaut &    //h16
//                     0xffff0000;
//        (pb + 1)->val_u32ToFloat_defaut = (pb + 1)->val_u32ToFloat_defaut |
//                                          convUint32;

    }
}

//void ModbusToDatConv(uint16_t RegNum, uint8_t *dat)
//{
//    unsigned int tmp;
//    unsigned int i;
//    uint32_t tmp_f, tmp_f2;
//    reg_usr.pb = reg_dat_usr;
//    for (i = 0; i < REG_SIZE; i++)
//    {
//        if (reg_usr.pb[i].reg_remap == RegNum)
//        {
//            switch (reg_usr.pb[i].dat_type)
//            {
//                case 0://unsigned char
//                    reg_usr.pb[i].val_c = dat[0];
//                    reg_usr.pb[i].val_c = reg_usr.pb[i].val_c << 8 | dat[1];
//                    break;
//                case 1://unsigned int
//
//                    reg_usr.pb[i].val_u = dat[0];
//                    reg_usr.pb[i].val_u = reg_usr.pb[i].val_u << 8 | dat[1];
//                    break;
//                case 2:// int
//                    tmp = dat[0];
//                    tmp = tmp << 8 | dat[1];
//                    if (tmp & 0x8000)
//                    {
//                        tmp = ~tmp + 1;
//                        reg_usr.pb[i].val_i = tmp;
//                        reg_usr.pb[i].val_i = -reg_usr.pb[i].val_i;
//                    }
//                    else
//                        reg_usr.pb[i].val_i = tmp;
//
//                    break;
//                case 3:// float
//
//                    tmp_f = uint32Tofloat_pos(dat, reg_usr.pb[i].dat_pos);
//                    tmp_f2 = *(uint32_t *) & (reg_usr.pb[i].val_f);
//                    if (reg_usr.pb[i].dat_pos == 0)
//                    {
//                        tmp_f2 = tmp_f2 & 0x0000ffff;
//                        tmp_f2 = tmp_f2 | tmp_f;
//
//                        reg_usr.pb[i].val_f = *(float *)&tmp_f2;
//                        reg_usr.pb[i + 1].val_f = reg_usr.pb[i].val_f;
//                    }
//                    else
//                    {
//                        tmp_f2 = tmp_f2 & 0xffff0000;
//                        tmp_f2 = tmp_f2 | tmp_f;
//                        reg_usr.pb[i].val_f = *(float *)&tmp_f2;
//                        reg_usr.pb[i - 1].val_f = reg_usr.pb[i].val_f;
//                    }
//
//
//                    break;
//            }
//        }
//    }
//}
void ModbusDatToRegDat(uint16_t RegNum, uint8_t *dat, unsigned char dir)
{

    unsigned int i;

    reg_usr.pb = reg_dat_usr;
    for (i = 0; i < REG_SIZE; i++)
    {
        if (((reg_usr.pb[i].reg == RegNum) || (reg_usr.pb[i].reg_remap == RegNum))
                && reg_usr.pb[i].enable == 1)
        {
            if (dir == CHANGE_REG)
            {
                if (reg_usr.pb[i].reg == RegNum)
                {
                    reg_usr.pb[i].reg_remap = dat[0];
                    reg_usr.pb[i].reg_remap = reg_usr.pb[i].reg_remap << 8;
                    reg_usr.pb[i].reg_remap =  reg_usr.pb[i].reg_remap | dat[1];

                }
            }

            if (reg_usr.pb[i].dat_type == 3)
                ModbusDatToRegFloatDat(&reg_usr.pb[i], &dat[2], reg_usr.pb[i].dat_pos);
            else
            {
                FlshDatToRegDatNoDefault(&reg_usr.pb[i], &dat[2], reg_usr.pb[i].dat_type);
            }


        }
    }
}
void ModbusDatToMulRegDat(uint16_t RegNum, uint8_t *dat, unsigned char dir)
{

    unsigned int i;

    reg_usr.pb = reg_dat_usr;
    for (i = 0; i < REG_SIZE; i++)
    {
        if (((reg_usr.pb[i].reg == RegNum) || (reg_usr.pb[i].reg_remap == RegNum))
                && reg_usr.pb[i].enable == 1)
        {
            if (dir == CHANGE_REG)
            {
                if (reg_usr.pb[i].reg == RegNum)
                {
                    reg_usr.pb[i].reg_remap = dat[0];
                    reg_usr.pb[i].reg_remap = reg_usr.pb[i].reg_remap << 8;
                    reg_usr.pb[i].reg_remap =  reg_usr.pb[i].reg_remap | dat[1];

                }
            }

            if (reg_usr.pb[i].dat_type == 3)
                ModbusDatToRegFloatDat(&reg_usr.pb[i], &dat[0], reg_usr.pb[i].dat_pos);
            else
            {
                FlshDatToRegDatNoDefault(&reg_usr.pb[i], &dat[0], reg_usr.pb[i].dat_type);
            }


        }
    }
}

void RegDatToModbus(uint16_t RegNum, uint8_t *dat)
{
    unsigned int  i;
    reg_usr.pb = reg_dat_usr;
    for (i = 0; i < REG_SIZE; i++)
    {

        if (reg_usr.pb[i].reg_remap == RegNum && reg_usr.pb[i].enable == 1)
        {
            if (reg_usr.pb[i].dat_type == 3)
            {
                if (reg_usr.pb[i].dat_pos == 1) //01020304
                {

                    dat[0] = reg_usr.pb[i].val_u32ToFloat >> 8;
                    dat[1] = reg_usr.pb[i].val_u32ToFloat;
                }
                else
                {
                    dat[0] = reg_usr.pb[i].val_u32ToFloat >> 24;
                    dat[1] = reg_usr.pb[i].val_u32ToFloat >> 16;

                }

            }
            else
            {
                dat[0] = reg_usr.pb[i].val_u32ToFloat >> 8;
                dat[1] = reg_usr.pb[i].val_u32ToFloat;

            }


        }
    }
}
void REGDatConvToPC(uint16_t RegNum, uint8_t *dat)
{
    unsigned int  i, j;
    reg_usr.pb = reg_dat_usr;
    for (i = 0; i < REG_SIZE; i++)
    {

        if (reg_usr.pb[i].reg == RegNum && reg_usr.pb[i].enable == 1)
        {
            j = 0;
            dat[j++] = reg_usr.pb[i].reg_remap >> 8;
            dat[j++] = reg_usr.pb[i].reg_remap ;
            if (reg_usr.pb[i].dat_type == 3)
            {
                floatTouint32_pos(reg_usr.pb[i].val_u32ToFloat, dat + j, reg_usr.pb[i].dat_pos);

            }
            else
            {
                dat[j++] = reg_usr.pb[i].val_u32ToFloat >> 8;
                dat[j++] = reg_usr.pb[i].val_u32ToFloat;

            }

        }
    }

}

void RegDatToFlash(uint32_t pb, uint8_t *dat)
{
    dat[0] = pb >> 8;
    dat[1] = (pb) & 0x00ff;
}
void RegWrite(void)
{
    uint8_t buf[READ_SIZE];
    uint16_t i, j;
    {
        buf[0] = 0;
        buf[1] = DAT_HEAD;
        i = 0;
        j = 2;
        buf[j++] = 0;
        buf[j++] = params_private.zero_cmd;

        buf[j++] = 0;
        buf[j++] = params_private.typ;

        buf[j++] = 0;
        buf[j++] = params_private.pga;

//        buf[j++] = 0;
//        buf[j++] = params_private.sei;
        buf[j++] = 0;
        buf[j++] = params_private.unit;

//        buf[j++] = params_private.offset>>8;
//        buf[j++] = params_private.offset;
        floatTouint32_m(params_private.zero_value, buf + j);
        j = j + 4;

        floatTouint32_m(params_private.offset, buf + j);
        j = j + 4;

        floatTouint32_m(params_private.coe, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cur_set, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.vol_set, buf + j);
        j = j + 4;

        floatTouint32_m(params_private.coe1, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.coe2, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.coe3, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.coe4, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.coe5, buf + j);
        j = j + 4;

        floatTouint32_m(params_private.cal1ADC, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal2ADC, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal3ADC, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal4ADC, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal5ADC, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal1val, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal2val, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal3val, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal4val, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.cal5val, buf + j);
        j = j + 4;
        floatTouint32_m(params_private.maskzero, buf + j);
        j = j + 4;
		buf[j++] = 0;
        buf[j++] = params_private.filter_level;


        reg_usr.pb = reg_dat_usr;
        for (i = 0; i < REG_SIZE; i++)
        {
            //DatConvToFlash(&reg_usr.pb[i], &buf[j], reg_usr.pb[i].dat_type);

            buf[j] = reg_usr.pb[i].reg_remap >> 8;
            buf[j + 1] = reg_usr.pb[i].reg_remap;

            if (reg_usr.pb[i].record_flag == 1)
            {
                if (reg_usr.pb[i].dat_type != 3)
                {
                    if (GetRegPrivate()->mode == 2)
                    {
                        reg_usr.pb[i].val_u32ToFloat = reg_usr.pb[i].val_u32ToFloat_defaut;
                    }

                    RegDatToFlash(reg_usr.pb[i].val_u32ToFloat, &buf[j + 2]);
                    if (GetRegPrivate()->mode == 1)
                    {
                        reg_usr.pb[i].val_u32ToFloat_defaut = reg_usr.pb[i].val_u32ToFloat;
                    }
					RegDatToFlash(reg_usr.pb[i].val_u32ToFloat_defaut, &buf[j + 4]);
                    j = j + 6;

                }
                else
                {
                    if (GetRegPrivate()->mode == 2)
                    {
                        reg_usr.pb[i].val_u32ToFloat = reg_usr.pb[i].val_u32ToFloat_defaut;
                    }
                    if (GetRegPrivate()->mode == 1)
                    {
                        reg_usr.pb[i].val_u32ToFloat_defaut = reg_usr.pb[i].val_u32ToFloat;
                    }

                    if (reg_usr.pb[i].dat_pos == 0)
                    {
                        RegDatToFlash(reg_usr.pb[i].val_u32ToFloat >> 16, &buf[j + 2]);
                        RegDatToFlash(reg_usr.pb[i].val_u32ToFloat_defaut >> 16, &buf[j + 4]);
                        j = j + 6;

                    }
                    else
                    {
                        RegDatToFlash(reg_usr.pb[i].val_u32ToFloat, &buf[j + 2]);
                        RegDatToFlash(reg_usr.pb[i].val_u32ToFloat_defaut, &buf[j + 4]);
                        j = j + 6;
                    }

                }

            }
			else
			{
			 buf[j + 2] = 0;
			 buf[j + 3] = 0;
			buf[j + 4] = 0;
			buf[j + 5] = 0;

            j = j + 6;

			}

        }
		//__disable_irq();
		getuart()->recv_update = 1;//stop adc
        flash_write_byte(0, buf, READ_SIZE);
		//__enable_irq();
		uart_init();	
    }

}


unsigned char RegRead(void)
{
    uint8_t buf[READ_SIZE];
    uint16_t i, j,k;
    uint16_t tmp;

    unsigned char result;
    result = 0;
    reg_usr.pb = reg_dat_usr;
    flash_read_byte(0, buf, READ_SIZE);
    i = 0;
    if (buf[1] == DAT_HEAD)
    {

        j = 2;
        params_private.zero_cmd =  buf[j+1];
        j = j + 2;
        params_private.typ =  buf[j+1];
        j = j + 2;
        params_private.pga =  buf[j+1];
        j = j + 2;
//        params_private.sei =  buf[j];
//        j = j + 2;
        params_private.unit =  buf[j+1];
        j = j + 2;
        params_private.zero_value = uint32TofloatR(buf + j);
        j = j + 4;

        params_private.offset = uint32TofloatR(buf + j);
        j = j + 4;

//        params_private.offset = buf[j];
//        params_private.offset = params_private.offset <<8 + buf[j+1];
//        j = j + 2;
        params_private.coe = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cur_set = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.vol_set = uint32TofloatR(buf + j);
        j = j + 4;

        params_private.coe1 = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.coe2 = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.coe3 = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.coe4 = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.coe5 = uint32TofloatR(buf + j);
        j = j + 4;

        params_private.cal1ADC = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal2ADC = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal3ADC = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal4ADC = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal5ADC = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal1val = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal2val = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal3val = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal4val = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.cal5val = uint32TofloatR(buf + j);
        j = j + 4;
        params_private.maskzero = uint32TofloatR(buf + j);
        j = j + 4;
		params_private.filter_level =  buf[j+1];
		j = j + 2;

        i = 0;
        for (; j < REG_SIZE * 6; j = j + 6)
        {
           // while (reg_usr.pb[i].record_flag != 1 && i < eREG_ADC_RATE)

            tmp = buf[j];
            tmp = tmp << 8;
            tmp = tmp + buf[j + 1];
            reg_usr.pb[i].reg_remap = tmp;

            if (reg_usr.pb[i].record_flag == 1)
            {
                if (reg_usr.pb[i].dat_type == 3)
                {
                    if (reg_usr.pb[i].dat_pos == 0)
                    {
                        FlashDatToRegFloatDat(&reg_usr.pb[i], &buf[j + 2], reg_usr.pb[i].dat_pos);

                    }

                    else  if (reg_usr.pb[i].dat_pos == 1)
                    {
                        FlashDatToRegFloatDat(&reg_usr.pb[i - 1], &buf[j + 2], reg_usr.pb[i].dat_pos);
                    }

                }
                else
                {
                    FlshDatToRegDat(&reg_usr.pb[i], &buf[j + 2], reg_usr.pb[i].dat_type);

                }
            }
			else
			{

			}
                   if (i < eREG_CLR_ZEROE)
                    i++;
				   else 
				   	break;
        }
      
    }
	else
		  result = 1;
    return result;
}

void reg_init(void)
{
    unsigned int i;
    reg_usr.update = 0;
    params_private.coe = 1;
    params_private.cur_set = 0.8;
    params_private.vol_set = 1;
    params_private.offset = 0;
    params_private.pga = 0;
    //params_private.sei = 0;
    params_private.typ = 1;
    params_private.unit = 2;//MPa
    params_private.coe1 = 0;
    params_private.coe2 = 0;
    params_private.coe3 = 1;
    params_private.coe4 = 0;
    params_private.zero_cmd = 0;
	params_private.maskzero = 0;

    if (RegRead() == 0)
    {

    }
	else
	{
	for (i = 0; i < REG_SIZE; i++)
	{
		reg_usr.pb[i].reg_remap = reg_usr.pb[i].reg;
	
	}
	
	RegWrite();

	}
		


}


void reg_proc(void)
{
    if (reg_usr.update == 1||getKey()->update == 1)
    {
        RegWrite();
        reg_usr.update = 0;
		getKey()->update  = 0;
        GetRegPrivate()->mode = 0;
    }


}


