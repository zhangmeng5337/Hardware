#include "display.h"
#include "74hc595.h"
#include "reg.h"

dis_stru dis_usr;
//共阳极数码管的编码：

//unsigned char ledcode[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff,0x8c}; //0-F,空白，P

//共阴极数码管的编码：
unsigned char ledcode[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71, 0x00, 0x73}; //0-F,空白，P
unsigned char dot_dat = 0x80;//.
unsigned char neg_dat = 0xfb;//- 11111011

void display_sel(unsigned char led_num)
{
    switch (led_num)
    {
        case 0:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_RESET);
            GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
            break;
        case 1:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_RESET);
            GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
            break;
        case 2:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_RESET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
            break;
        case 3:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_RESET);
            break;
        case 5:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
            break;
        default:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
            break;


    }
}

void display_init()
{
    dis_usr.update_cycle = 40;
    dis_usr.dot_pos = 4;
    dis_usr.mode = NUM_MODE;

}
uint32_t datx_proc(unsigned int range, float dat)
{
    uint32_t result;
    unsigned int decm_bit ;
    decm_bit = GetReg()->pb[REG_DECM_BIT_INDEX].val_u;
    if (dis_usr.signedFlag == 0)
    {
        if (dis_usr.dis_val <= 1) //1.002
        {

            if (decm_bit == 0)//no dot
            {
                result = dis_usr.dis_val;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 1;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                dis_usr.dat_bits = 2;
				result = dis_usr.dis_val * 10;
            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 2;
                result = dis_usr.dis_val * 100;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 1;
                result = dis_usr.dis_val * 1000;
                dis_usr.dat_bits = 4;
            }

        }
        else  if (dis_usr.dis_val < 10) //9.123
        {

            if (decm_bit == 0)//no dot
            {
                result = dis_usr.dis_val;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 1;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dis_usr.dis_val * 10;
                dis_usr.dat_bits = 2;

            }

            else if (decm_bit == 2)//99.22
            {
                dis_usr.dot_pos = 2;
                result = dis_usr.dis_val * 100;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 1;
                result = dis_usr.dis_val * 1000;
                dis_usr.dat_bits = 4;
            }

        }
        else  if (dis_usr.dis_val < 100) //99.12 10.12
        {

            if (decm_bit == 0)//no dot
            {
                result = dis_usr.dis_val;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 2;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dis_usr.dis_val * 10;
                dis_usr.dat_bits = 3;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 2;
                result = dis_usr.dis_val * 100;
                dis_usr.dat_bits = 4;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 2;
                dis_usr.dat_bits = 4;
                result = dis_usr.dis_val * 100;
            }

        }
        else  if (dis_usr.dis_val < 1000) //999.1
        {

            if (decm_bit == 0)//no dot
            {
                result = dis_usr.dis_val;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dis_usr.dis_val * 10;
                dis_usr.dat_bits = 4;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 3;
                result = dis_usr.dis_val * 10;
                dis_usr.dat_bits = 4;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 3;
                dis_usr.dat_bits = 4;
                result = dis_usr.dis_val * 10;
            }

        }
        else  //1000.11
        {

            result = dis_usr.dis_val;
            dis_usr.dot_pos = 4;
            dis_usr.dat_bits = 4;
        }

    }
    else //neg data 3bit data
    {
        if (dis_usr.dis_val <= 1) //1
        {

            if (decm_bit == 0)//no dot -1.002
            {
                result = dis_usr.dis_val;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 1;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dis_usr.dis_val * 10;
                dis_usr.dat_bits = 2;
            }

            else if (decm_bit == 2)//1.23
            {
                dis_usr.dot_pos = 2;
                result = dis_usr.dis_val * 100;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 2;
                result = dis_usr.dis_val * 100;
                dis_usr.dat_bits = 3;
            }

        }
        else  if (dis_usr.dis_val < 10) //-9.123
        {

            if (decm_bit == 0)//no dot
            {
                result = dis_usr.dis_val;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 1;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dis_usr.dis_val * 10;
                dis_usr.dat_bits = 2;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 2;
                result = dis_usr.dis_val * 100;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)//9.123
            {
                dis_usr.dot_pos = 2;
                result = dis_usr.dis_val * 100;
                dis_usr.dat_bits = 3;
            }

        }
        else  if (dis_usr.dis_val < 100) //99.12 10.12
        {

            if (decm_bit == 0)//no dot
            {
                result = dis_usr.dis_val;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 2;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dis_usr.dis_val * 10;
                dis_usr.dat_bits = 3;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 3;
                result = dis_usr.dis_val * 10;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 3;
                dis_usr.dat_bits = 3;
                result = dis_usr.dis_val * 10;
            }

        }
        else  if (dis_usr.dis_val < 1000) //999.1
        {

            if (decm_bit == 0)//no dot
            {
                result = dis_usr.dis_val;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 4;
                result = dis_usr.dis_val ;
                dis_usr.dat_bits = 3;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 4;
                result = dis_usr.dis_val ;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 3;
                result = dis_usr.dis_val ;
            }

        }
        else  //1000.11
        {

            result = dis_usr.dis_val;
            dis_usr.dot_pos = 4;
            dis_usr.dat_bits = 4;
        }

    }



    return result;

}
void disp_dat_proc()
{

    if (dis_usr.signedFlag == 0)
    {

        if (dis_usr.dat_bits == 4)
        {
            dis_usr.dis_buf[0] = dis_usr.dis_val_u / 1000; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = dis_usr.dis_val_u % 1000 / 100;
            dis_usr.dis_buf[2] = dis_usr.dis_val_u % 100 / 10;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;

        }
        else if (dis_usr.dat_bits == 3)
        {
            dis_usr.dis_buf[0] = 11; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = dis_usr.dis_val_u  / 100;
            dis_usr.dis_buf[2] = dis_usr.dis_val_u % 100 / 10;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;


        }
        else if (dis_usr.dat_bits == 2)
        {
            dis_usr.dis_buf[0] = 11; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = 11;
            dis_usr.dis_buf[2] = dis_usr.dis_val_u / 10;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;


        }

        else if (dis_usr.dat_bits == 1)
        {
            dis_usr.dis_buf[0] = 11; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = 11;
            dis_usr.dis_buf[2] = 11;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u / 10;

        }
        else
					;
        }
	else
	{
	
	if (dis_usr.dat_bits == 4)
	{
		dis_usr.dis_buf[0] = 11; //0.1*1000 = 100 0.01
		dis_usr.dis_buf[1] = 11;
		dis_usr.dis_buf[2] = 11;
		dis_usr.dis_buf[3] = 11;
	
	}
	else if (dis_usr.dat_bits == 3)
	{
		dis_usr.dis_buf[0] = 11; //0.1*1000 = 100 0.01
		dis_usr.dis_buf[1] = dis_usr.dis_val_u	/ 100;
		dis_usr.dis_buf[2] = dis_usr.dis_val_u % 100 / 10;
		dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;
	
	
	}
	else if (dis_usr.dat_bits == 2)
	{
		dis_usr.dis_buf[0] = 11; //0.1*1000 = 100 0.01
		dis_usr.dis_buf[1] = 11;
		dis_usr.dis_buf[2] = dis_usr.dis_val_u / 10;
		dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;
	
	
	}
	
	else if (dis_usr.dat_bits == 1)
	{
		dis_usr.dis_buf[0] = 11; //0.1*1000 = 100 0.01
		dis_usr.dis_buf[1] = 11;
		dis_usr.dis_buf[2] = 11;
		dis_usr.dis_buf[3] = dis_usr.dis_val_u / 10;
	
	}
	else
				;

	}

}
void display_val_proc(unsigned char dis_mode)
{
    float tmp;
    unsigned char i;


    if (dis_mode == NUM_MODE)
    {
        dis_usr.dot_pos = 4;
        dis_usr.signedFlag = 0;
        tmp = dis_usr.dis_val;
        if (tmp < 0)
        {
            dis_usr.signedFlag = 1;
            dis_usr.dis_val = fabs(dis_usr.dis_val);
        }
        else
        {
            dis_usr.signedFlag = 0;
        }
        if (dis_usr.dis_val <= 1) //1
        {


            dis_usr.dis_val_u = datx_proc(1, dis_usr.dis_val);


        }
        else  if (dis_usr.dis_val < 10) //10.001
        {
            dis_usr.dis_val_u = datx_proc(10, dis_usr.dis_val);

        }
        else  if (dis_usr.dis_val < 100) //100.1
        {
            dis_usr.dis_val_u = datx_proc(100, dis_usr.dis_val);

        }
        else
        {
            dis_usr.dis_val_u = datx_proc(1000, dis_usr.dis_val);

        }
		disp_dat_proc();


    }
		unsigned char tmp_dat;
    if ((dis_usr.dot_pos - 1) == i) if (dis_usr.signedFlag == 0)
    {
			  
			tmp_dat = ledcode[dis_usr.dis_buf[i]] | dot_dat;
        XL74HC595_MultiWrite(&tmp_dat, 1); //
    }
    if (dis_usr.signedFlag == 1)
    {
       if((dis_usr.dat_bits+i)==4)
			 {
			 tmp_dat = ledcode[dis_usr.dis_buf[i]] | neg_dat;
			 }
        XL74HC595_MultiWrite(&tmp_dat, 1); //
    }	
    display_sel(i++);
    if (i >= 4)
        i = 1;
}
void display(void)
{
    static uint32_t tick_tmp;
    static unsigned char i = 0;
    if ((GetTick() - tick_tmp) >= dis_usr.update_cycle)
    {
        tick_tmp = GetTick();
        display_val_proc(dis_usr.mode);

    }
}
