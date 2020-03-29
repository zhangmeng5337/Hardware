#include "74hc595.h"
#include "display.h"
#include "settings.h"





unsigned char table[]= {0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09,0x11,0xc1,0x63,0x85,0x61,0x71};
//unsigned char table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71};
const unsigned char seg_table[16]= {0};
static unsigned char buf[4]= {MINUS,MINUS,MINUS,MINUS};
void display_dat_deal(float dat,unsigned char *header_code,unsigned char dattypes)
{
    uint32_t tmp;
        if(dattypes == 0)
        {
            if(dat<10)
            {
                tmp = dat*1000;
                buf[0] = table[tmp/1000]-DOT;
                buf[1] = table[tmp%1000/100];
                buf[2] = table[tmp%100/10];
                buf[3] = table[tmp%10];
            }
            else if(dat>=10&&dat<100)
            {
                tmp = dat*100;
                buf[0] =  table[tmp/1000];
                buf[1] =  table[tmp%1000/100]-DOT;
                buf[2] =  table[tmp%100/10];
                buf[3] =  table[tmp%10];
            }
            else if(dat>=100&&dat<1000)
            {
                tmp = dat*10;
                buf[0] =  table[tmp/1000];
                buf[1] =  table[tmp%1000/100];
                buf[2] =  table[tmp%100/10]-DOT;
                buf[3] =  table[tmp%10];
            }
            else
            {
                tmp = dat;
                buf[0] =  table[tmp/1000];
                buf[1] =  table[tmp%1000/100];
                buf[2] =  table[tmp%100/10];
                buf[3] =  table[tmp%10]-DOT;
            }

        }
        else if(dattypes == 1)
        {
            if(dat<10)
            {
                tmp = dat*1000;
                buf[0] = table[tmp/1000];
                buf[1] = table[tmp%1000/100];
                buf[2] = table[tmp%100/10];
                buf[3] = table[tmp%10];
            }
            else if(dat>=10&&dat<100)
            {
                tmp = dat*100;
                buf[0] =  table[tmp/1000];
                buf[1] =  table[tmp%1000/100];
                buf[2] =  table[tmp%100/10];
                buf[3] =  table[tmp%10];
            }
            else if(dat>=100&&dat<1000)
            {
                tmp = dat*10;
                buf[0] =  table[tmp/1000];
                buf[1] =  table[tmp%1000/100];
                buf[2] =  table[tmp%100/10];
                buf[3] =  table[tmp%10];
            }
            else
            {
                // tmp = dat*10;
                buf[0] =  table[tmp/1000];
                buf[1] =  table[tmp%1000/100];
                buf[2] =  table[tmp%100/10];
                buf[3] =  table[tmp%10];
            }
        }
        else if(dattypes == 2)//16进制显示
        {
            {
                tmp = dat;
                buf[0] = table[tmp>>16];
                buf[1] = table[(tmp>>8)&0x00f];
                buf[2] = table[(tmp>>4)&0x00f];
                buf[3] = table[tmp&0x0f];
            }

        }
        else if(dattypes == 3)//16进制显示
        {
            tmp = dat;
			buf[0] = header_code[0];
            if(dat<10)
            {
                tmp = dat*1000;
                buf[1] = table[tmp/1000]-DOT;
                buf[2] = table[tmp%1000/100];
                buf[3] = table[tmp%100/10];
               // buf[3] = table[tmp%10];
            }
            else if(dat>=10&&dat<100)
            {
                tmp = dat*100;
                buf[1] =  table[tmp/1000];
                buf[2] =  table[tmp%1000/100]-DOT;
                buf[3] =  table[tmp%100/10];
                //buf[3] =  table[tmp%10];
            }
            else if(dat>=100&&dat<1000)
            {
                tmp = dat*10;
                buf[1] =  table[tmp/1000];
                buf[2] =  table[tmp%1000/100];
                buf[3] =  table[tmp%100/10]-DOT;
                //buf[3] =  table[tmp%10];
            }
            else
            {
                tmp = dat;
                buf[1] =  table[tmp/1000];
                buf[2] =  table[tmp%1000/100];
                buf[3] =  table[tmp%100/10];
                //buf[3] =  table[tmp%10]-DOT;
            }

			if(dat<0)
			{
				// tmp = dat*10;
				buf[0] =  MINUS;
				buf[1] = buf[0];
				buf[2] =  buf[1];
				buf[3] =  buf[2];
			}

            
            //buf[1] = table[tmp/100];
            //buf[2] = table[tmp%100/10];
            //buf[3] = table[tmp%10];

        }
        else if(dattypes == 4)//16进制显示
        {
            {
                tmp = dat;
                buf[0] = header_code[0];;
                buf[1] = header_code[1];;
                buf[2] = header_code[2];;
                buf[3] = header_code[3];;
            }

        }

        if(dat<0)
        {
            // tmp = dat*10;
            buf[0] =  MINUS;
            buf[1] = buf[0];
            buf[2] =  buf[1];
            buf[3] =  buf[2];
        }
#if DEBUG_USER
        printf("display is:   %d\n",dat);
#endif

}

static unsigned char bitselect=0;

void display_off(void)
{
    seg_select(5);
}
void init_seg()
{
    bitselect = 0;

}
void diplay_blink_ctrl(unsigned char mode)
{
    static uint32_t gettime,gettime2;

    if(mode == 1)
    {
        if((HAL_GetTick()-gettime2)<DISPLAY_BLINK_ON)
        {
            if((HAL_GetTick()-gettime)>=DISPLAY_PERIOD)
            {
                gettime = HAL_GetTick();
                M74HC595_WriteData(buf[bitselect-1]);
                seg_select(bitselect);
                bitselect = bitselect +1;
                if(bitselect > 4)
                    bitselect = 1;
            }

        }
        else if((HAL_GetTick()-gettime2)>=DISPLAY_BLINK_ON&&(HAL_GetTick()-gettime2)<DISPLAY_BLINK_PERIOD)
        {
            display_off();
        }
        else if((HAL_GetTick()-gettime2)>=DISPLAY_BLINK_PERIOD)
            gettime2 = HAL_GetTick();

    }
    else
    {
        if((HAL_GetTick()-gettime)>=DISPLAY_PERIOD)
        {
            gettime = HAL_GetTick();
            M74HC595_WriteData(buf[bitselect-1]);
            seg_select(bitselect);
            bitselect = bitselect +1;
            if(bitselect > 4)
                bitselect = 1;
        }

    }


}
void display_proc(unsigned char flag)
{

    //if((get_params_mode()->status)>=WORK_OFF)
    {
        if(flag == 0)
        {
            if( get_params_mode()->mode ==NORMAL)
            {
                if(get_params_mode()->status !=WORK_ON&&get_params_mode()->modeNo >STAND_MODE)
                {

                    diplay_blink_ctrl(1);
                }
                else
                {
                    diplay_blink_ctrl(0);


                }



            }
            else if( get_params_mode()->mode == SETTING_MODE)
            {
                diplay_blink_ctrl(1);
            }
        }
        else  //test mode
        {
            if( get_params_mode()->mode==NORMAL)
            {
                diplay_blink_ctrl(1);


            }
        }

    }



}
