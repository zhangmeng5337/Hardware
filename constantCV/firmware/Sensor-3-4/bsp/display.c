#include "display.h"
#include "74hc595.h"
#include "reg.h"
#include "main.h"
#include "sensor.h"
#include "stdlib.h"
#include "math.h"

static dis_stru dis_usr;
//共阳极数码管的编码：

//unsigned char ledcode[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e,0xff,0x8c}; //0-F,空白，P

//共阴极数码管的编码：
unsigned char ledcode[] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x00, 0x39, 0x5e, 0x79, 0x71, 0x00, 0x73, 0x00}; //0-F,空白，P
unsigned char dot_dat = 0x80;//.
unsigned char neg_dat = 0x40;//- 1011111011

void display_sel(unsigned char led_num, unsigned char status)
{
    switch (led_num)
    {
        case 1:           

			GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
            if (status == 0)
                PA07_SETLOW();
            else
                PA07_SETHIGH();

            // GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_RESET);

            break;
        case 2:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_SET); 
			GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
            if (status == 0)
                GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_RESET);
            else
                GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            break;
        case 3:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
            if (status == 0)
                GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_RESET);
            else
                GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);

            break;
        case 4:
            GPIO_WritePin(DIAG1_GPIO_PORT, DIAG1_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG2_GPIO_PORT, DIAG2_GPIO_PIN, GPIO_Pin_SET);
            GPIO_WritePin(DIAG3_GPIO_PORT, DIAG3_GPIO_PIN, GPIO_Pin_SET);
            if (status == 0)
                GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_RESET);
            else
                GPIO_WritePin(DIAG4_GPIO_PORT, DIAG4_GPIO_PIN, GPIO_Pin_SET);
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
    dis_usr.update_cycle =5;
    dis_usr.dot_pos = 4;
    dis_usr.mode = NUM_MODE;

}
dis_stru *GetDisp(void)
{
    return &dis_usr;
}
unsigned char GetSeg(unsigned char dat)
{
    return ledcode[dat];
}

uint32_t datx_proc(float dat)
{
    uint32_t result;
    unsigned int decm_bit ;
	float val;
    decm_bit = GetReg()->pb[eREG_DECM_BIT].val_u32ToFloat;
	  val = *(float *)(&(GetReg()->pb[eREG_RANGZ_HF16].val_u32ToFloat));
	  if(val<=-100)
	  	{
			if(decm_bit>=1)
				decm_bit = 1;
	  }
    if (decm_bit > 3)
        decm_bit = 1;
    if (dis_usr.signedFlag == 0)
    {
        if (dat <= 1) //1.002
        {

            if (decm_bit == 0)//no dot
            {
                result = dat;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 1;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                dis_usr.dat_bits = 2;
                result = dat * 10;
            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 2;
                result = dat * 100;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 1;
                result = dat * 1000;
                dis_usr.dat_bits = 4;
            }

        }
        else  if (dat < 10) //9.123
        {

            if (decm_bit == 0)//no dot
            {
                result = dat;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 1;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dat * 10;
                dis_usr.dat_bits = 2;

            }

            else if (decm_bit == 2)//99.22
            {
                dis_usr.dot_pos = 2;
                result = dat * 100;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 1;
                result = dat * 1000;
                dis_usr.dat_bits = 4;
            }

        }
        else  if (dat < 100) //99.12 10.12
        {

            if (decm_bit == 0)//no dot
            {
                result = dat;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 2;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dat * 10;
                dis_usr.dat_bits = 3;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 2;
                result = dat * 100;
                dis_usr.dat_bits = 4;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 2;
                dis_usr.dat_bits = 4;
                result = dat * 100;
            }

        }
        else  if (dat < 1000) //999.1
        {

            if (decm_bit == 0)//no dot
            {
                result = dat;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dat * 10;
                dis_usr.dat_bits = 4;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 3;
                result = dat * 10;
                dis_usr.dat_bits = 4;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 3;
                dis_usr.dat_bits = 4;
                result = dat * 10;
            }

        }
        else  //1000.11
        {

            result = dat;
            dis_usr.dot_pos = 4;
            dis_usr.dat_bits = 4;
        }

    }
    else //neg data 3bit data
    {
        if (dat <= 1) //1
        {

            if (decm_bit == 0)//no dot -1.002
            {
                result = dat;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 1;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dat * 10;
                dis_usr.dat_bits = 2;
            }

            else if (decm_bit == 2)//1.23
            {
                dis_usr.dot_pos = 2;
                result = dat * 100;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 2;
                result = dat * 100;
                dis_usr.dat_bits = 3;
            }

        }
        else  if (dat < 10) //-9.123
        {

            if (decm_bit == 0)//no dot
            {
                result = dat;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 1;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dat * 10;
                dis_usr.dat_bits = 2;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 2;
                result = dat * 100;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)//9.123
            {
                dis_usr.dot_pos = 2;
                result = dat * 100;
                dis_usr.dat_bits = 3;
            }

        }
        else  if (dat <= 100) //99.12 10.12
        {

            if (decm_bit == 0)//no dot
            {
                result = dat;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 3;
                result = dat * 10;
                dis_usr.dat_bits = 4;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 3;
                result = dat * 10;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 3;
                dis_usr.dat_bits = 3;
                result = dat * 10;
            }

        }
        else  if (dat < 1000) //999.1
        {

            if (decm_bit == 0)//no dot
            {
                result = dat;
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 1)
            {
                dis_usr.dot_pos = 4;
                result = dat ;
                dis_usr.dat_bits = 3;

            }

            else if (decm_bit == 2)
            {
                dis_usr.dot_pos = 4;
                result = dat ;
                dis_usr.dat_bits = 3;
            }
            else if (decm_bit == 3)
            {
                dis_usr.dot_pos = 4;
                dis_usr.dat_bits = 3;
                result = dat ;
            }

        }
        else  //1000.11
        {

            result = dat;
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
            dis_usr.dis_buf[0] = 0x0b; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = dis_usr.dis_val_u  / 100;
            dis_usr.dis_buf[2] = dis_usr.dis_val_u % 100 / 10;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;


        }
        else if (dis_usr.dat_bits == 2)
        {
            dis_usr.dis_buf[0] = 0x0b; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = 0x0b;
            dis_usr.dis_buf[2] = dis_usr.dis_val_u / 10;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;


        }

        else if (dis_usr.dat_bits == 1)
        {
            dis_usr.dis_buf[0] = 0x0b; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = 0x0b;
            dis_usr.dis_buf[2] = 0x0b;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u / 10;

        }
        else
            ;
    }
    else
    {

        if (dis_usr.dat_bits == 4)
        {
//            dis_usr.dis_buf[0] = 0x0b; //0.1*1000 = 100 0.01
//            dis_usr.dis_buf[1] = 0x0b;
//            dis_usr.dis_buf[2] = 0x0b;
//            dis_usr.dis_buf[3] = 0x0b;
		dis_usr.dis_buf[0] = dis_usr.dis_val_u / 1000; //0.1*1000 = 100 0.01
		dis_usr.dis_buf[1] = dis_usr.dis_val_u % 1000 / 100;
		dis_usr.dis_buf[2] = dis_usr.dis_val_u % 100 / 10;
		dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;

        }
        else if (dis_usr.dat_bits == 3)
        {
            dis_usr.dis_buf[0] = 0x0b; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = dis_usr.dis_val_u  / 100;
            dis_usr.dis_buf[2] = dis_usr.dis_val_u % 100 / 10;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;


        }
        else if (dis_usr.dat_bits == 2)
        {
            dis_usr.dis_buf[0] = 0x0b; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = 0xfb;
            dis_usr.dis_buf[2] = dis_usr.dis_val_u / 10;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u % 10;


        }

        else if (dis_usr.dat_bits == 1)
        {
            dis_usr.dis_buf[0] = 0x0b; //0.1*1000 = 100 0.01
            dis_usr.dis_buf[1] = 0x0b;
            dis_usr.dis_buf[2] = 0x0b;
            dis_usr.dis_buf[3] = dis_usr.dis_val_u / 10;

        }
        else
            ;

    }

}

static unsigned char bit_count = 1;
static	  unsigned char tmp_dat = 0;

void display_val_proc(unsigned char dis_mode)
{
    float tmp;
    static uint32_t tick_tmp;
		  

    if (bit_count > 4)
        bit_count = 1;


    if (dis_mode == NUM_MODE)
    {
        dis_usr.dot_pos = 4;
        dis_usr.signedFlag = 0;
        // dis_usr.dis_val = 9998;
	//	if ((GetTick() - tick_tmp) >= 10)
		  {
			  dis_usr.dis_val =	getadc()->data_unit_app;
			  //dis_usr.dis_val = -100;
			  tick_tmp = GetTick();
		
		  }
//			else
//				 dis_usr.dis_val =	dis_usr.dis_val;

		 
        tmp = dis_usr.dis_val;
		
        if (tmp < 0)
        {
            dis_usr.signedFlag = 1;
			//if(dis_usr.dis_val<-9999)
		  // 	dis_usr.dis_val = -9999;
            dis_usr.dis_val = fabs(dis_usr.dis_val);
        }
        else
        {
           //if(dis_usr.dis_val<0.001)
		   //	dis_usr.dis_val = 0;
			if(dis_usr.dis_val>9999)
			 dis_usr.dis_val = 9998;

            dis_usr.signedFlag = 0;
        }

            dis_usr.dis_val_u = datx_proc(dis_usr.dis_val);

        disp_dat_proc();


    }

    //XL74HC595_MultiWrite(&tmp_dat, 1); //

    if (dis_usr.signedFlag == 0)
    {
        if ((dis_usr.dot_pos) == bit_count)
        {
            tmp_dat = ledcode[dis_usr.dis_buf[bit_count - 1]] | dot_dat;

        }
        else
        {
            tmp_dat = ledcode[dis_usr.dis_buf[bit_count - 1]];

        }
        // XL74HC595_MultiWrite(&tmp_dat, 1); //
    }
    if (dis_usr.signedFlag == 1)
    {

        if ((dis_usr.dat_bits + bit_count) >= 4)
        { 
          
            if(dis_usr.dis_buf[bit_count - 1]<=0x0f&&bit_count==1)
             tmp_dat = ledcode[dis_usr.dis_buf[bit_count - 1]] | neg_dat;
			else
			tmp_dat = ledcode[dis_usr.dis_buf[bit_count - 1]];	
        }
        else
        {	if(dis_usr.dis_buf[bit_count - 1]<=0x0f)
            	tmp_dat = ledcode[dis_usr.dis_buf[bit_count - 1]] ;
			else
				tmp_dat = 0;	
        }

	if ((dis_usr.dot_pos) == bit_count)
	{
		tmp_dat = tmp_dat| dot_dat;
	
	}
	else
	{
		tmp_dat =tmp_dat;
	
	}

    }
    display_sel(bit_count++, 0);//bit_count++

  XL74HC595_MultiWrite(&tmp_dat, 1); //

}
void dis_test()
{static uint32_t delay_tick = 0, tick_tmp = 0;
if ((GetTick() - tick_tmp) >= dis_usr.update_cycle)
{ 
	tick_tmp = GetTick();

	  display_sel(bit_count++, 0);//bit_count++
	  XL74HC595_MultiWrite(&tmp_dat, 1); //
	  if (bit_count > 4)
		  bit_count = 1;
}

}
void display_led(unsigned char Cusor, unsigned BitSel)
{
    unsigned char tmp_dat;
    static unsigned char i = 1;
    static uint32_t delay_tick = 0, tick_tmp = 0;
    if ((GetTick() - tick_tmp) >= dis_usr.update_cycle)
    {
        tick_tmp = GetTick();

        tmp_dat = 0;
        XL74HC595_MultiWrite(&tmp_dat, 1); //
        if (Cusor == 1)
        {
            if (i == BitSel)
            {
                if ((GetTick() - delay_tick) <= 500)
                {
                    tmp_dat = 0;
                }
                else if ((GetTick() - delay_tick) <= 1000)
                {
                    //delay_tick = GetTick();
                    tmp_dat = dis_usr.dis_buf[i - 1];
                }
                else
                {
                    delay_tick = GetTick();
                    tmp_dat = tmp_dat;

                }

            }
            else
            {
                //delay_tick = GetTick();
                tmp_dat = dis_usr.dis_buf[i - 1];
            }

        }
        else
        {
            delay_tick = GetTick();
            tmp_dat = dis_usr.dis_buf[i - 1];
        }

        display_sel(i, 0);
        XL74HC595_MultiWrite(&tmp_dat, 1); //
        i++;
        if (i > 4)
            i = 1;
    }
}
void display()
{
    static uint32_t tick_tmp;



    dis_usr.update_cycle = 5;
    if ((GetTick() - tick_tmp) >= dis_usr.update_cycle)
    {
        tick_tmp = GetTick();

		display_val_proc(dis_usr.mode);
    }

}


