#include "power_energy.h"
#include "config.h"
#include "stmflash.h"

energy_stru energy_usr[ENERGY_COUNT] =
{
    {0, DELI, {0}},
    {0, DELI, {0}},
    {0, DELI, {0}},
    {0, DELI, {0}},
    {0, DELI, {0}}


};
energylist_stru energylist;
energylist_stru *get_energy_data(void)
{
    return &energylist;
}

void energy_init()
{
    unsigned char i, k, l;
    k = 17;
    for (i = 0; i < ENERGY_COUNT; i++)
    {
        energy_usr[i].addr = k++;
        energy_usr[i].energy_typ = 0;
        for (l = 0; l < ENERGY_BUF_SIZE; l++)
            energy_usr[i].payload[l] = 88888888.333;

    }
    energylist.pb = energy_usr;
}

void deli_proc(unsigned char index, unsigned char *p)
{
		float uin32_to_float;
		unsigned char i, j;
		for (j = 0; j < ENERGY_COUNT; j++)
		{
			if (energy_usr[j].addr > 0 && energy_usr[j].addr == p[0])
				break;
	
		}
		if (j != ENERGY_COUNT)
		{
	//		  for (i = 0; i < ENERGY_COUNT; i++)
	//		  {
	//			  if (energy_usr[i].addr == 0 && energy_usr[i].addr == p[0])
	//			  {
			energy_usr[j].addr = p[0];
	//				  j = i;
	//				  break;
	//			  }
	//		  }
	
		}
		else
			return ;

    if (index == INSTR_DELI_SINDEX)
    {
        if (p[2] == 52)
        {
            uint32_t tmp;
            unsigned char k;
            i = 4;
            k = 3;
            //0x700-0x705
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;

            //0x706-0x70b
            i = 1;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            //0x70c-0x719
            i = 7;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;
            energy_usr[j].payload[i] = *(float *)tmp;
            i++;

        }
    }
    else if (index == INSTR_DELI_EINDEX)
    {
        if (p[2] == 4)
        {
            uint32_t tmp;
            unsigned char k;
            i = 0;
            k = 3;
            //0x73c-0x73d
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            energy_usr[j].payload[0] = *(float *)tmp;
            energy_usr[j].payload[0] = energy_usr[j].payload[0] / 1000.0;
        }
    }
}
void zhengtai_proc(unsigned char index, unsigned char *p)
{     
    float uin32_to_float;
    unsigned char i, j;
    for (j = 0; j < ENERGY_COUNT; j++)
    {
        if (energy_usr[j].addr > 0 && energy_usr[j].addr == p[0])
            break;

    }
    if (j != ENERGY_COUNT)
    {
//        for (i = 0; i < ENERGY_COUNT; i++)
//        {
//            if (energy_usr[i].addr == 0 && energy_usr[i].addr == p[0])
//            {
        energy_usr[j].addr = p[0];
//                j = i;
//                break;
//            }
//        }

    }
    else
        return ;

    if (index == INSTR_ZT_SINDEX)//0x0006 0x0007
    {
        if (p[2] == 4)
        {
            uint16_t tmp;
            unsigned char k;
            i = 4;
            k = 3;
            //0x700-0x705
            tmp = uint8Touint16(&p[k]);
            k = k + 2;
            if (tmp & 0x8000)
            {
                tmp = ~tmp + 1;
                energy_usr[j].payload[ENERGY_BUF_SIZE - 2] =    tmp;
                energy_usr[j].payload[ENERGY_BUF_SIZE - 2] =
                    - energy_usr[j].payload[ENERGY_BUF_SIZE - 2];

            }
            else
                energy_usr[j].payload[ENERGY_BUF_SIZE - 2] =    tmp;

            tmp = uint8Touint16(&p[k]);
            k = k + 2;
            if (tmp & 0x8000)
            {
                tmp = ~tmp + 1;
                energy_usr[j].payload[ENERGY_BUF_SIZE - 1] =    tmp;
                energy_usr[j].payload[ENERGY_BUF_SIZE - 1] =
                    - energy_usr[j].payload[ENERGY_BUF_SIZE - 1];

            }
            else
                energy_usr[j].payload[ENERGY_BUF_SIZE - 1] =    tmp;


        }

    }
    else if (index == INSTR_ZT_EINDEX) //0x401e 0x401f
    {
        uint32_t tmp;
        unsigned char k;
        k = 3;
        if (p[2] == 4)
        {
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
			uin32_to_float =  *(float *)(&tmp);
            energy_usr[j].payload[0] = uin32_to_float;
            energy_usr[j].payload[0] = energy_usr[j].payload[0] *
                                       energy_usr[j].payload[ENERGY_BUF_SIZE - 1];
            energy_usr[j].payload[0] = energy_usr[j].payload[0] *
                                       energy_usr[j].payload[ENERGY_BUF_SIZE - 2];
            energy_usr[j].payload[0] = energy_usr[j].payload[0] * 0.1;

        }


    }
    else
    {

        if (p[2] == 52)
        {
            uint32_t tmp;
            float uin32_to_float;
            unsigned char k;
            k = 3;
            for (i = 1; i < 10; i++)
            {
                tmp = uint8Touint322(&p[k]);
                k = k + 4;
                uin32_to_float =  *(float *)(&tmp);
                energy_usr[j].payload[i] = uin32_to_float;
            }
            i = 13;
            tmp = uint8Touint322(&p[k]);
            k = k + 4;
            uin32_to_float =  *(float *)(&tmp);
            energy_usr[j].payload[i] = uin32_to_float;
            for (i = 10; i < 13; i++)
            {
                tmp = uint8Touint322(&p[k]);
                k = k + 4;
                uin32_to_float =  *(float *)(&tmp);
                energy_usr[j].payload[i] = uin32_to_float;
            }


        }

    }

}

void power_cal(unsigned char index, unsigned char *p)
{
    if (index >= INSTR_DELI_SINDEX && index <= INSTR_DELI_EINDEX)
    {
        deli_proc(index, p);
    }
    else if (index >= INSTR_ZT_SINDEX && index <= INSTR_ZT_EINDEX)
    {
        zhengtai_proc(index, p);

    }
}
