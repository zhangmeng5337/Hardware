#include "do.h"
stru_do_stru do_usr;
stru_do_stru *do_p;
unsigned char do_index[16];

void do_init()
{
    unsigned int i,k;
    i = 1;
	 k = 15;
    do_usr.do_status = 0;
	for(i = 0;i<16;i++)
	{
    do_index[i] = k;
	k--;
	}


    do_off();

}



void HC595_CS(void)
{
    /**  步骤3：STCP产生一个上升沿，移位寄存器的数据移入存储寄存器  **/
    HC595_STCP_Low();   // 将STCP拉低
    HAL_Delay(1);           // 适当延时
    HC595_STCP_High();  // 再将STCP拉高，STCP即可产生一个上升沿
    HAL_Delay(1);
}

void HC595_SendByte(uint8_t byte)
{
    uint8_t i;
    for (i = 0; i < 8; i ++)  //一个字节8位，传输8次，一次一位，循环8次，刚好移完8位
    {
        /****  步骤1：将数据传到DS引脚    ****/
        if (byte & 0x80)        //先传输高位，通过与运算判断第八是否为1
            HC595_Data_High();    //如果第八位是1，则与 595 DS连接的引脚输出高电平
        else                    //否则输出低电平
            HC595_Data_Low();

        /*** 步骤2：SHCP每产生一个上升沿，当前的bit就被送入移位寄存器 ***/
        HC595_SHCP_Low();   // SHCP拉低
        HAL_Delay(1);           // 适当延时
        HC595_SHCP_High();  // SHCP拉高， SHCP产生上升沿
        HAL_Delay(1);

        byte <<= 1;     // 左移一位，将低位往高位移，通过   if (byte & 0x80)判断低位是否为1
    }
}

void XL74HC595_MultiWrite(uint8_t *data, uint8_t Length)//混合写数据
{
    uint8_t i;
    for (i = 0; i < Length; i ++)  // len 个字节
    {
        HC595_SendByte(data[i]);
    }

    HC595_CS(); //先把所有字节发送完，再使能输出
}
void do_status_cal(unsigned int do_NO_sel, unsigned char bit_set)
{
    unsigned int val;
    switch (do_NO_sel)
    {
        case dq1:
            val = 0x0001;
            break;
        case dq2:
            val = 0x0002;
            break;
        case dq3:
            val = 0x0004;
            break;
        case dq4:
            val = 0x0008;
            break;
        case dq5:
            val = 0x0010;
            break;
        case dq6:
            val = 0x0020;
            break;
        case dq7:
            val = 0x0040;
            break;
        case dq8:
            val = 0x0080;
            break;
        case dq9:
            val = 0x100;
            break;
        case dq10:
            val = 0x200;
            break;
        case dq11:
            val = 0x400;
            break;
        case dq12:
            val = 0x800;
            break;
        case dq13:
            val = 0x1000;
            break;
        case dq14:
            val = 0x2000;
            break;
        case dq15:
            val = 0x4000;
            break;
        default:
            val = 0x00;
            break;

    }
    if (bit_set == ON)
    {
        do_usr.do_status = do_usr.do_status | val;
    }
    else
    {
        do_usr.do_status = do_usr.do_status & (~val);

    }
}
/*****************************************************
do_NO_sel：do端口号，对应寄存器
bit_set：0/1, 0开，1关
******************************************************/
void do_NO_set(unsigned int do_NO_sel, unsigned char bit_set)
{
    unsigned int no_tmp;
    unsigned char shift_bit;
	
    if (do_NO_sel < (DO_SIZE - 1)) //3个级联，最大24bit
    {
        if (do_NO_sel >= 8 && do_NO_sel < 16)
        {
            no_tmp = do_NO_sel - 8;
        }
        else if (do_NO_sel >= 16)
        {
            no_tmp = do_NO_sel - 16;
        }
        else
            no_tmp = do_NO_sel;
        switch (no_tmp)
        {
            case 0:
                shift_bit = 0x01;
                break;
            case 1:
                shift_bit = 0x02;
                break;
            case 2:
                shift_bit = 0x04;
                break;
            case 3:
                shift_bit = 0x08;
                break;
            case 4:
                shift_bit = 0x10;
                break;
            case 5:
                shift_bit = 0x20;
                break;
            case 6:
                shift_bit = 0x40;
                break;
            case 7:
                shift_bit = 0x80;
                break;
        }

        do_status_cal(do_NO_sel, bit_set);

        if (bit_set == OFF)
        {
            //shift_bit = ~shift_bit;
            if (do_NO_sel < 8)
            {
                do_usr.do_No_out[1] = do_usr.do_No_out[1] | shift_bit;

            }
            else if (do_NO_sel >= 8 && do_NO_sel < 16)
            {
                do_usr.do_No_out[0] = do_usr.do_No_out[0] | shift_bit;

            }

            else if (do_NO_sel > 16)
                do_usr.do_No_out[0] = do_usr.do_No_out[0] | shift_bit;
            else
                ;

        }
        else if (bit_set == ON)
        {
            shift_bit = ~shift_bit;
            if (do_NO_sel < 8)
            {

                do_usr.do_No_out[1] = do_usr.do_No_out[1] & shift_bit;
            }
            else if (do_NO_sel >= 8 && do_NO_sel < 16)
                do_usr.do_No_out[0] = do_usr.do_No_out[0] & shift_bit;
            else if (do_NO_sel >= 16)
                do_usr.do_No_out[0] = do_usr.do_No_out[0] & shift_bit;

        }

    }






}
void do_ctrl_proc(unsigned int do_NO_sel, unsigned char bit_set)
{
    HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_SET);
    do_NO_set(do_NO_sel, bit_set);
    XL74HC595_MultiWrite(do_usr.do_No_out, 2);
}

void do_off()
{
    do_usr.do_No_out[0] = 0xff;
    do_usr.do_No_out[1] = 0xff;
    XL74HC595_MultiWrite(do_usr.do_No_out, 2);
    HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port, PWR_CTRL_Pin, GPIO_PIN_SET);
}
long unsigned int get_do_status(void)
{
    return do_usr.do_status;
}
stru_do_stru *set_do_out(void)
{
    do_p = &do_usr;

    return do_p;
}
void do_num_sel(int num)
{
    unsigned int tmp, nor_num;
    unsigned char i, k;
    nor_num = 1;
    k = 0;
    tmp = (unsigned int)num;
    for (i = 0; i < DO_SIZE; i++)
    {
        if (tmp & nor_num)
        {
            if (do_usr.do_out & nor_num)
                do_ctrl_proc(do_index[k], OFF);
            else
                do_ctrl_proc(do_index[k], ON);
        }
        k++;
        nor_num = nor_num << 1;
    }
}
void do_proc()
{
    if (do_usr.do_update == 1)
    {
        do_usr.do_update = 0;
        do_num_sel(do_usr.do_num);

    }
}



