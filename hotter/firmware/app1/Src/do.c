#include "do.h"
stru_do_stru do_usr;


void do_init()
{
    unsigned int i;
	i=0;	
	do_usr.do_No_buf[i++] =  DO_ctrl_7;
	do_usr.do_No_buf[i++] =  DO_ctrl_6;
	do_usr.do_No_buf[i++] =  DO_ctrl_5;
	do_usr.do_No_buf[i++] =  DO_ctrl_4;
	do_usr.do_No_buf[i++] =  DO_ctrl_3;
	do_usr.do_No_buf[i++] =  DO_ctrl_2;
	do_usr.do_No_buf[i++] =  DO_ctrl_1;
	do_usr.do_No_buf[i++] =  DO_ctrl_0 ;
	do_usr.do_No_buf[i++] =  DO_ctrl_15 ;
	do_usr.do_No_buf[i++] =  DO_ctrl_14;
	do_usr.do_No_buf[i++] =  DO_ctrl_13;
	do_usr.do_No_buf[i++] =  DO_ctrl_12;
	do_usr.do_No_buf[i++] =  DO_ctrl_11;
	do_usr.do_No_buf[i++] = DO_ctrl_10;
	do_usr.do_No_buf[i++] =  DO_ctrl_9;
	do_usr.do_No_buf[i++] =  DO_ctrl_8;
		
	do_usr.do_No_buf[i++] =  DO_ctrl_19;
	do_usr.do_No_buf[i++] =  DO_ctrl_18;
	do_usr.do_No_buf[i++] =  DO_ctrl_17;
	do_usr.do_No_buf[i++] =  DO_ctrl_16;
	do_usr.do_No_buf[i++] =  DO_ctrl_20;
	do_usr.do_No_buf[i++] =  DO_ctrl_21;
	do_usr.do_No_buf[i++] =  DO_ctrl_22;
	do_usr.do_No_buf[i++] =  DO_ctrl_23;

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
		
		byte <<= 1;		// 左移一位，将低位往高位移，通过	if (byte & 0x80)判断低位是否为1
	}
}
 
void XL74HC595_MultiWrite(uint8_t *data, uint8_t Length)//混合写数据
{
	uint8_t i;
	for (i = 0; i < Length; i ++ ) // len 个字节
	{
		HC595_SendByte(data[i]);
	}
	
	HC595_CS(); //先把所有字节发送完，再使能输出
}

/*****************************************************
do_NO_sel：do端口号，对应寄存器
bit_set：0/1, 0开，1关
******************************************************/
void do_NO_set(unsigned int do_NO_sel,unsigned char bit_set)
{
    unsigned int no_tmp;
	unsigned char shift_bit;
	if(do_NO_sel<(DO_SIZE-1))//3个级联，最大24bit
	{		
		if(do_NO_sel>=8 && do_NO_sel <16)
		{
			no_tmp = do_NO_sel - 8;
		}
		else if(do_NO_sel>=16)
		{
			no_tmp = do_NO_sel - 16;
		}
		else 
			no_tmp = do_NO_sel;
		switch(no_tmp)
		{
			case 7:shift_bit = 0x01;break;
			case 6:shift_bit = 0x02;break;
			case 5:shift_bit = 0x04;break;
			case 4:shift_bit = 0x08;break;
			case 3:shift_bit = 0x10;break;
			case 2:shift_bit = 0x20;break;
			case 1:shift_bit = 0x40;break;
			case 0:shift_bit = 0x80;break;	
		}
		if(bit_set == OFF)
		{
			//shift_bit = ~shift_bit;
			if(do_NO_sel<8)
			{			
				do_usr.do_No_out[0] = do_usr.do_No_out[0]|shift_bit;
			}
			else if(do_NO_sel>=8&&do_NO_sel<16)
				do_usr.do_No_out[1] = do_usr.do_No_out[1]|shift_bit;
			else if(do_NO_sel>16)
				do_usr.do_No_out[2] = do_usr.do_No_out[2]|shift_bit;
			else 
				 ;
	
		}
		else if(bit_set == ON)
		{shift_bit = ~shift_bit;
			if(do_NO_sel<8)
			{
				
				do_usr.do_No_out[0] = do_usr.do_No_out[0]&shift_bit;
			}
			else if(do_NO_sel>=8&&do_NO_sel<16)
				do_usr.do_No_out[1] = do_usr.do_No_out[1]&shift_bit;
			else if(do_NO_sel>=16)
				do_usr.do_No_out[2] = do_usr.do_No_out[2]&shift_bit;
	
		}

	}


	
	


}
void do_ctrl_proc(unsigned int do_NO_sel,unsigned char bit_set)
{
    HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port,PWR_CTRL_Pin,GPIO_PIN_SET);
	do_NO_set(do_NO_sel,bit_set);
	XL74HC595_MultiWrite(do_usr.do_No_out, 2);
}

void do_off()
{
	do_usr.do_No_out[0]=0xff;
	do_usr.do_No_out[1]=0xff;	
	XL74HC595_MultiWrite(do_usr.do_No_out, 2);	
	HAL_GPIO_WritePin(PWR_CTRL_GPIO_Port,PWR_CTRL_Pin,GPIO_PIN_SET);
}




