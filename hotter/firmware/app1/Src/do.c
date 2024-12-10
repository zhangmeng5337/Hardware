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
	/**  ����3��STCP����һ�������أ���λ�Ĵ�������������洢�Ĵ���  **/
	HC595_STCP_Low();   // ��STCP����
	HAL_Delay(1);           // �ʵ���ʱ
	HC595_STCP_High();  // �ٽ�STCP���ߣ�STCP���ɲ���һ��������
	HAL_Delay(1);
}
 
void HC595_SendByte(uint8_t byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)  //һ���ֽ�8λ������8�Σ�һ��һλ��ѭ��8�Σ��պ�����8λ
	{
	  /****  ����1�������ݴ���DS����    ****/
		if (byte & 0x80)        //�ȴ����λ��ͨ���������жϵڰ��Ƿ�Ϊ1
			HC595_Data_High();    //����ڰ�λ��1������ 595 DS���ӵ���������ߵ�ƽ
		else                    //��������͵�ƽ
			HC595_Data_Low();
		
		/*** ����2��SHCPÿ����һ�������أ���ǰ��bit�ͱ�������λ�Ĵ��� ***/
		HC595_SHCP_Low();   // SHCP����
		HAL_Delay(1);           // �ʵ���ʱ
		HC595_SHCP_High();  // SHCP���ߣ� SHCP����������
		HAL_Delay(1);
		
		byte <<= 1;		// ����һλ������λ����λ�ƣ�ͨ��	if (byte & 0x80)�жϵ�λ�Ƿ�Ϊ1
	}
}
 
void XL74HC595_MultiWrite(uint8_t *data, uint8_t Length)//���д����
{
	uint8_t i;
	for (i = 0; i < Length; i ++ ) // len ���ֽ�
	{
		HC595_SendByte(data[i]);
	}
	
	HC595_CS(); //�Ȱ������ֽڷ����꣬��ʹ�����
}

/*****************************************************
do_NO_sel��do�˿ںţ���Ӧ�Ĵ���
bit_set��0/1, 0����1��
******************************************************/
void do_NO_set(unsigned int do_NO_sel,unsigned char bit_set)
{
    unsigned int no_tmp;
	unsigned char shift_bit;
	if(do_NO_sel<(DO_SIZE-1))//3�����������24bit
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




