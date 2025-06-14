#include "reg.h"
#include "flash.h"
reg_dat_pack reg_dat_usr[REG_SIZE] =
{
    {REG_X100,          1,  2,  0,  0,      0,      0,  0,  0 }, //regx100,1 regcount,
    {REG_X10,           1,  2,  0,  0,      0,      0,  0,  0  },
    {REG_HF16,          1,  3,  0,  0,      0,      0,  0,  0  },
    {REG_LF16,          1,  3,  0,  0,      0,      0,  0,  0  },
    {REG_RANGZ_HF16,    1,  3,  0,  5,      0,      5,  1,  0  },
    {REG_RANGZ_LF16,    1,  3,  0,  2,      0,      5,  1,  1  },
    {REG_RANGF_HF16,    1,  3,  0,  5,      0,      6,  1,  0  },
    {REG_RANGF_LF16,    1,  3,  0,  3,      0,      6,  1,  1  },
    {REG_OFFSET_HF16,   1,  3,  0,  6,      0,      7,  1,  0  },
    {REG_OFFSET_LF16,   1,  3,  0,  2,      0,      7,  1,  1  },
    {REG_RATE,          1,  1,  0,  1,      0,      1,  1,  0  },
    {REG_DEV_ADDR,      1,  1,  0,  1,      0,      1,  1,  0  },
    {REG_CHECK,         1,  1,  0,  2,      0,      2,  1,  0  },
    {REG_DECM_BIT,      1,  1,  0,  3,      0,      3,  1,  0  },
    {REG_UNIT,          1,  1,  0,  1,      0,      1,  1,  0  },
    {REG_ADC_RATE,      1,  1,  0,  40,     0,      0,  1,  0  }

};



reg_stru reg_usr;
reg_stru*GetReg(void)
{
  return &reg_usr;
}

//4ac00000---->4a c0 00 00
void DatConvToReg(reg_dat_pack *pb, uint8_t *dat, unsigned char type)
{
    unsigned int tmp;
    uint32_t tmp_f;
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
            pb->val_f = uint32Tofloat(dat);
            break;

    }
}
void DatConvToFlash(reg_dat_pack *pb, uint8_t *dat, unsigned char type)
{
    unsigned int tmp;
    uint32_t tmp_f;
    switch (type)
    {
        case 0://unsigned char
            dat[0] = pb->val_c & 0x00ff;
            dat[1] = pb->val_c >> 8;
            break;
        case 1://unsigned int
            dat[0] = pb->val_u >> 8;
            dat[1] = pb->val_u & 0x00ff;

            break;
        case 2:// int

            if (pb->val_i < 0)
            {
                tmp = pb->val_i;
                tmp = ~tmp + 1;
            }
            else
                tmp = pb->val_i;

            dat[0] = tmp & 0x00ff;
            dat[1] = tmp >> 8;


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
        reg_usr.pb = reg_dat_usr;
        for (i = 0; i < REG_SIZE; i++)
        {

            if (reg_usr.pb[i].record_flag == 1)
            {
                DatConvToFlash(&reg_usr.pb[i], &buf[j], reg_usr.pb[i].dat_type);
                j = j + 2;
            }
        }


        flash_write_byte(0, buf, READ_SIZE);
    }

}
unsigned char RegRead(void)
{
    uint8_t buf[512];
    uint16_t i, j;
    unsigned char result;
    result = 0;
    reg_usr.pb = reg_dat_usr;
    flash_read_byte(0, buf, READ_SIZE);
    i = 0;
    if (buf[1] == DAT_HEAD)
    {
        for (j = 2; j < REG_SIZE * 2; j = j + 2)
        {
            while (reg_usr.pb[i].record_flag != 1&&i < REG_SIZE)
                if (i < REG_SIZE)
                    i++;
            if (reg_usr.pb[i].record_flag == 1)
            {
                if (reg_usr.pb[i].dat_pos == 0)
                    DatConvToReg(&reg_usr.pb[i], &buf[j], reg_usr.pb[i].dat_type);
                else  if (reg_usr.pb[i].dat_pos == 1)
                    DatConvToReg(&reg_usr.pb[i], &buf[j - 2], reg_usr.pb[i].dat_type);
                i++;
            }




        }
        result = 1;
    }
    return result;
}

void reg_init(void)
{
    if (RegRead() == 0)
    {

        RegWrite();
    }

}


