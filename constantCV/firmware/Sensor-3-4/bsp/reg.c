#include "reg.h"
#include "flash.h"

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
//  unsigned int reg;
//  unsigned int reg_remap;
//  unsigned char reg_count;
//  unsigned char val_c;
//  unsigned int val_u;
//  int val_i;
//  float val_f;
//  uint32_t val_u32ToFloat;
//  unsigned char dat_type;//0:unsigned char 1:unsigned int 2:int 3:float
//  unsigned char dat_pos;//0:float hight 16bit 1:float low 16bit
//  unsigned char record_flag;//0:not record 1:record


    {REG_X100,              0,  1,  0,  0,  1,  0,      0, 2,  0,  0}, //regx100,1 regcount,
    {REG_X10,               0,  1,  0,  0,  -1, 0,      0, 2,  0,  0},
    {REG_HF16,              0,  1,  0,  0,  0,  1.1,    0, 3,  0,  0},
    {REG_LF16,              0,  1,  0,  0,  0,  1.1,    0, 3,  1,  0},
    {REG_RANGZ_HF16,        0,  1,  0,  5,  0,  1.3,    0, 3,  0,  1},
    {REG_RANGZ_LF16,        0,  1,  0,  2,  0,  1.3,    0, 3,  1,  1},
    {REG_RANGF_HF16,        0,  1,  0,  5,  0,  -1.5,    0, 3,  0,  1},
    {REG_RANGF_LF16,        0,  1,  0,  3,  0,  -1.5,   0, 3,  1,  1},
    {REG_OFFSET_HF16,       0,  1,  0,  6,  0,  -1.7,   0, 3,  0,  1},
    {REG_OFFSET_LF16,       0,  1,  0,  2,  0,  -1.7,   0, 3,  1,  1},
    {REG_DEV_ADDR,          0,  1,  0,  1,  0,  0,      0, 1,  0,  1},
    {REG_RATE,              0,  1,  0,  3,  0,  0,      0, 1,  0,  1},
    {REG_CHECK,             0,  1,  0,  0,  0,  0,      0, 1,  0,  1},
    {REG_DECM_BIT,          0,  1,  0,  0,  0,  0,      0, 1,  0,  1},
    {REG_UNIT,              0,  1,  0,  0,  0,  0,      0, 1,  0,  1},
    {REG_ADC_RATE,          0,  1,  0,  0, 0,  0,      0, 1,  0,  1}

};






reg_stru reg_usr;
params_private_stru params_private;


reg_stru *GetReg(void)
{
    return &reg_usr;
}
params_private_stru *GetRegPrivate(void)
{
    return &params_private;
}

//4ac00000---->4a c0 00 00
void DatConvToReg(reg_dat_pack *pb, uint8_t *dat, unsigned char type)
{
    unsigned int tmp;
    switch (type)
    {
        case 0://unsigned char
            pb->val_c = dat[0];
            pb->val_c = pb->val_c << 8 | dat[1];
            break;
        case 1://unsigned int

            pb->val_u = dat[0];
            pb->val_u = pb->val_u << 8 | dat[1];
            break;
        case 2:// int
            tmp = dat[0];
            tmp = tmp << 8 | dat[1];
            if (tmp & 0x8000)
            {
                tmp = ~tmp + 1;
                pb->val_i = tmp;
                pb->val_i = -pb->val_i;
            }
            else
                pb->val_i = tmp;

            break;
        case 3:// float
            pb->val_u32ToFloat =  uint32Tofloat(dat);

            break;

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
void RegDatConv(uint16_t RegNum, uint8_t *dat, unsigned char dir)
{
    unsigned int tmp;
    unsigned int i;
    uint32_t tmp_f, tmp_f2;
    reg_usr.pb = reg_dat_usr;
    for (i = 0; i < REG_SIZE; i++)
    {
        if ((reg_usr.pb[i].reg == RegNum) || (reg_usr.pb[i].reg_remap == RegNum))
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
            switch (reg_usr.pb[i].dat_type)
            {
                case 0://unsigned char
                    reg_usr.pb[i].val_c = dat[2];
                    reg_usr.pb[i].val_c = reg_usr.pb[i].val_c << 8 | dat[3];
                    break;
                case 1://unsigned int

                    reg_usr.pb[i].val_u = dat[2];
                    reg_usr.pb[i].val_u = reg_usr.pb[i].val_u << 8 | dat[3];
                    break;
                case 2:// int
                    tmp = dat[2];
                    tmp = tmp << 8 | dat[3];
                    if (tmp & 0x8000)
                    {
                        tmp = ~tmp + 1;
                        reg_usr.pb[i].val_i = tmp;
                        reg_usr.pb[i].val_i = -reg_usr.pb[i].val_i;
                    }
                    else
                        reg_usr.pb[i].val_i = tmp;

                    break;
                case 3:// float

                    tmp_f = uint32Tofloat_pos(&dat[2], reg_usr.pb[i].dat_pos);
                    tmp_f2 = *(uint32_t *) & (reg_usr.pb[i].val_f);
                    if (reg_usr.pb[i].dat_pos == 0)
                    {
                        tmp_f2 = tmp_f2 & 0x0000ffff;
                        tmp_f2 = tmp_f2 | tmp_f;

                        reg_usr.pb[i].val_f = *(float *)&tmp_f2;
                        reg_usr.pb[i + 1].val_f = reg_usr.pb[i].val_f;
                    }
                    else
                    {
                        tmp_f2 = tmp_f2 & 0xffff0000;
                        tmp_f2 = tmp_f2 | tmp_f;
                        reg_usr.pb[i].val_f = *(float *)&tmp_f2;
                        reg_usr.pb[i - 1].val_f = reg_usr.pb[i].val_f;
                    }


                    break;
            }

        }
    }
}

void DatConvToModbus(uint16_t RegNum, uint8_t *dat)
{
    unsigned int tmp, i;
    reg_usr.pb = reg_dat_usr;
    for (i = 0; i < REG_SIZE; i++)
    {

        if (reg_usr.pb[i].reg_remap == RegNum)
        {
            switch (reg_usr.pb[i].dat_type)
            {
                case 0://unsigned char
                    dat[0] = 0;
                    dat[1] = reg_usr.pb[i].val_c;
                    break;
                case 1://unsigned int

                    dat[0] = reg_usr.pb[i].val_u >> 8;
                    dat[1] = reg_usr.pb[i].val_u;

                    break;
                case 2:// int
                    tmp = reg_usr.pb[i].val_i;
                    // tmp = tmp << 8 | reg_usr.pb[i].val_i;
//                    if (reg_usr.pb[i].val_i < 0)
//                    {
//                        tmp = ~reg_usr.pb[i].val_i + 1;
                    dat[0] = tmp >> 8;
                    dat[1] = tmp;

//                    }
//                    else
//                    {
//                        dat[0] = tmp >> 8;
//                        dat[1] = tmp;
//                    }
                    break;
                case 3:// float
                    floatTouint32_pos(reg_usr.pb[i].val_f, dat, reg_usr.pb[i].dat_pos);
                    break;

            }
            break;
        }
    }



}
void REGDatConvToPC(uint16_t RegNum, uint8_t *dat)
{
    unsigned int tmp, i, j;
    reg_usr.pb = reg_dat_usr;
    for (i = 0; i < REG_SIZE; i++)
    {

        if (reg_usr.pb[i].reg == RegNum)
        {
            j = 0;
            dat[j++] = reg_usr.pb[i].reg_remap >> 8;
            dat[j++] = reg_usr.pb[i].reg_remap ;
            switch (reg_usr.pb[i].dat_type)
            {
                case 0://unsigned char
                    dat[j++] = 0;
                    dat[j++] = reg_usr.pb[i].val_c;
                    break;
                case 1://unsigned int

                    dat[j++] = reg_usr.pb[i].val_u >> 8;
                    dat[j++] = reg_usr.pb[i].val_u;

                    break;
                case 2:// int
                    tmp = reg_usr.pb[i].val_i;
                    // tmp = tmp << 8 | reg_usr.pb[i].val_i;
//                    if (reg_usr.pb[i].val_i < 0)
//                    {
//                        tmp = ~reg_usr.pb[i].val_i + 1;
                    dat[j++] = tmp >> 8;
                    dat[j++] = tmp;

//                    }
//                    else
//                    {
//                        dat[0] = tmp >> 8;
//                        dat[1] = tmp;
//                    }
                    break;
                case 3:// float
                    floatTouint32_pos(reg_usr.pb[i].val_f, dat + j, reg_usr.pb[i].dat_pos);
                    break;

            }
            break;
        }
    }



}

void DatConvToFlash(reg_dat_pack *pb, uint8_t *dat, unsigned char type)
{
    unsigned int tmp;
    switch (type)
    {
        case 0://unsigned char
            dat[0] = pb->val_c >> 8;
            dat[1] = pb->val_c & 0x00ff;

            break;
        case 1://unsigned int
            dat[0] = pb->val_u >> 8;
            dat[1] = pb->val_u & 0x00ff;

            break;
        case 2:// int

//            if (pb->val_i < 0)
//            {
//                tmp = pb->val_i;
//                tmp = ~tmp + 1;
//            }
//            else
            tmp = pb->val_i;


            dat[0] = tmp >> 8;
            dat[1] = tmp & 0x00ff;


            break;
        case 3:// float
            //if(pb->dat_pos == 0)
            floatTouint32_pos(pb->val_f, dat, pb->dat_pos);
            break;

    }
}
void RegWrite(void)
{
    uint8_t buf[512];
    uint16_t i, j;
    {
        buf[0] = 0;
        buf[1] = DAT_HEAD;
        i = 0;
        j = 2;
        buf[j++] = 0;
        buf[j++] = params_private.typ;

        buf[j++] = 0;
        buf[j++] = params_private.pga;

        buf[j++] = 0;
        buf[j++] = params_private.sei;
        buf[j++] = 0;
        buf[j++] = params_private.unit;

//        buf[j++] = params_private.offset>>8;
//        buf[j++] = params_private.offset;

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

        reg_usr.pb = reg_dat_usr;
        for (i = 0; i < REG_SIZE; i++)
        {
            //DatConvToFlash(&reg_usr.pb[i], &buf[j], reg_usr.pb[i].dat_type);

            buf[j] = reg_usr.pb[i].reg_remap >> 8;
            buf[j + 1] = reg_usr.pb[i].reg_remap;

            // if (reg_usr.pb[i].record_flag == 1)
            {
                DatConvToFlash(&reg_usr.pb[i], &buf[j + 2], reg_usr.pb[i].dat_type);
                j = j + 4;
            }

        }
        flash_write_byte(0, buf, READ_SIZE);
    }

}
void ModbusSetReg(uint16_t RegNum, uint8_t *dat)
{
    unsigned int tmp;
    unsigned int i;
    uint32_t tmp_f, tmp_f2;
    reg_usr.pb = reg_dat_usr;
    for (i = 0; i < REG_SIZE; i++)
    {
        if (reg_usr.pb[i].reg == RegNum)
        {
            reg_usr.pb[i].reg_remap = dat[0];
            reg_usr.pb[i].reg_remap =  reg_usr.pb[i].reg_remap << 8 +
                                       dat[1];
            switch (reg_usr.pb[i].dat_type)
            {
                case 0://unsigned char
                    reg_usr.pb[i].val_c = dat[2];
                    reg_usr.pb[i].val_c = reg_usr.pb[i].val_c << 8 | dat[3];
                    break;
                case 1://unsigned int

                    reg_usr.pb[i].val_u = dat[2];
                    reg_usr.pb[i].val_u = reg_usr.pb[i].val_u << 8 | dat[3];
                    break;
                case 2:// int
                    tmp = dat[2];
                    tmp = tmp << 8 | dat[3];
                    if (tmp & 0x8000)
                    {
                        tmp = ~tmp + 1;
                        reg_usr.pb[i].val_i = tmp;
                        reg_usr.pb[i].val_i = -reg_usr.pb[i].val_i;
                    }
                    else
                        reg_usr.pb[i].val_i = tmp;

                    break;
                case 3:// float

                    tmp_f = uint32Tofloat_pos(dat, reg_usr.pb[i].dat_pos);
                    tmp_f2 = *(uint32_t *) & (reg_usr.pb[i].val_f);
                    if (reg_usr.pb[i].dat_pos == 0)
                    {
                        tmp_f2 = tmp_f2 & 0x0000ffff;
                        tmp_f2 = tmp_f2 | tmp_f;
                    }
                    else
                    {
                        tmp_f2 = tmp_f2 & 0xffff0000;
                        tmp_f2 = tmp_f2 | tmp_f;
                    }
                    reg_usr.pb[i].val_f = *(float *)&tmp_f2;
                    break;
            }
        }
    }
}

unsigned char RegRead(void)
{
    uint8_t buf[512];
    uint16_t i, j;
    uint16_t tmp;
    uint32_t convUint32;
    unsigned char result;
    result = 0;
    reg_usr.pb = reg_dat_usr;
    flash_read_byte(0, buf, READ_SIZE);
    i = 0;
    if (buf[1] == DAT_HEAD)
    {

//float offset;//before zero value
//float coe;
//float cur_set;
//float vol_set;
//unsigned char typ;
//unsigned char pga;
//unsigned char sei;
//float coe1;
//float coe2;
//float coe3;
//float coe4;


        j = 3;

        params_private.typ =  buf[j];
        j = j + 2;
        params_private.pga =  buf[j];
        j = j + 2;
        params_private.sei =  buf[j];
        j = j + 2;
        params_private.unit =  buf[j];
        j = j + 2;

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


        for (; j < REG_SIZE * 4; j = j + 4)
        {
//            while (reg_usr.pb[i].record_flag != 1 && i < REG_SIZE)
//                if (i < REG_SIZE)
//                    i++;
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
                        reg_usr.pb[i].val_u32ToFloat = *(uint32_t *) & (reg_usr.pb[i].val_f);//f to u
                        reg_usr.pb[i].val_u32ToFloat = reg_usr.pb[i].val_u32ToFloat & 0x0000ffff; //h16
                        DatConvToReg(&reg_usr.pb[i], &buf[j + 2], reg_usr.pb[i].dat_type);
                        reg_usr.pb[i].val_u32ToFloat = reg_usr.pb[i].val_u32ToFloat & 0xffff0000; //h16
                        convUint32 = reg_usr.pb[i + 1].val_u32ToFloat & 0x0000ffff;

                        reg_usr.pb[i].val_u32ToFloat = reg_usr.pb[i].val_u32ToFloat | convUint32;
                        reg_usr.pb[i].val_f = *(float *) & (reg_usr.pb[i].val_u32ToFloat);
                        reg_usr.pb[i + 1].val_f =  reg_usr.pb[i].val_f;
                    }

                    else  if (reg_usr.pb[i].dat_pos == 1)
                    {
                        reg_usr.pb[i].val_u32ToFloat = *(uint32_t *) & (reg_usr.pb[i].val_f);
                        reg_usr.pb[i].val_u32ToFloat = reg_usr.pb[i].val_u32ToFloat & 0xffff0000;
                        DatConvToReg(&reg_usr.pb[i], &buf[j + 3], reg_usr.pb[i].dat_type);
                        reg_usr.pb[i].val_u32ToFloat = reg_usr.pb[i].val_u32ToFloat & 0x0000ffff;
                        convUint32 = reg_usr.pb[i - 1].val_u32ToFloat & 0xffff0000;

                        reg_usr.pb[i].val_u32ToFloat = reg_usr.pb[i].val_u32ToFloat | convUint32;
                        reg_usr.pb[i].val_f = *(float *) & (reg_usr.pb[i].val_u32ToFloat);
                        reg_usr.pb[i - 1].val_f =  reg_usr.pb[i].val_f;

                    }

                }
                else
                {
                    DatConvToReg(&reg_usr.pb[i], &buf[j + 2], reg_usr.pb[i].dat_type);

                }
            }
            i++;
        }
        result = 1;
    }
    return result;
}

void reg_init(void)
{
    unsigned int i;
    reg_usr.update = 0;
    params_private.coe = 1;
    params_private.cur_set = 1;
    params_private.vol_set = 1;
    params_private.offset = 0;
	params_private.pga = 0;
	params_private.sei = 0;
	params_private.typ = 0;
	params_private.unit = 0;
    if (RegRead() == 0)
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
    if (reg_usr.update == 1)
    {
        RegWrite();
        reg_usr.update = 0;

    }
}


