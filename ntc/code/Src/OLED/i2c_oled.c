#include "i2c_oled.h"
#include "codetab.h"
#include "adc.h"
#include "app.h"
#include "iic.h"

extern I2C_HandleTypeDef hi2c1;
static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   
extern uint32_t sysTick_Count;

//OLED���ų�ʼ��
void OLED_GPIO_Config(void)
{
/*	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(I2C_OLED_SCL_GPIO_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_OD ;
	GPIO_InitStruct.GPIO_Pin = I2C_OLED_SCL_GPIO_PIN ;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(I2C_OLED_SCL_GPIO_PORT, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = I2C_OLED_SDA_GPIO_PIN ;
	GPIO_Init(I2C_OLED_SDA_GPIO_PORT, &GPIO_InitStruct);
	
	I2C_InitTypeDef I2C_InitStruct;
	
	RCC_APB1PeriphClockCmd(I2C_OLED_CLK,ENABLE);
	
	//Ӳ��IIC�ĳ�ʼ������
	I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStruct.I2C_AcknowledgedAddress = I2C_SLAVE_LEDGEADDR;
	I2C_InitStruct.I2C_ClockSpeed = I2C_Speed;
	I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_16_9;
	I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStruct.I2C_OwnAddress1 = I2C_OWN_ADDR;
	I2C_Init(I2C_x, &I2C_InitStruct);
	
	I2C_Cmd(I2C_x,ENABLE);*/
}

/*
*	��OLEDдһ���ֽ�
*	control=0x00 dataΪ���� 
*	control=0x40 dataΪ����
*	��������0 ���������ط���ֵ
*/
uint8_t I2C_OLED_WriteByte(uint8_t control,uint8_t data)
{
    uint8_t p[2];

	p[0] = control;
	p[1] = data;
	//I2C_WriteByte(OLED_ADDRESS,p[1],p[0]);
	HAL_I2C_Master_Transmit(&hi2c1,OLED_ADDRESS,p,2,200);
	/*//�ж������Ƿ�æ
	I2CTimeout = I2CT_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(I2C_x, I2C_FLAG_BUSY) == 1)
	{
		if(I2CTimeout-- == 0) return 1;
	}
	I2CTimeout = I2CT_LONG_TIMEOUT;
	
	//������ʼ�ź� ���EV5�¼�
	I2C_GenerateSTART(I2C_x, ENABLE);
	while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_MODE_SELECT) == ERROR)
	{
		if(I2CTimeout-- == 0)	return 2;
	}
	I2CTimeout = I2CT_LONG_TIMEOUT;
	
	//���ʹӻ���ַ ���EV6�¼�
	I2C_Send7bitAddress(I2C_x, OLED_ADDRESS, I2C_Direction_Transmitter);
	while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) == ERROR)
	{
		if(I2CTimeout-- == 0)	return 3;
	}
	I2CTimeout = I2CT_LONG_TIMEOUT;
	
	//���Ϳ����ֽڣ���һ���ֽ�����������ݣ� ���EV8�¼�
	I2C_SendData(I2C_x, control);
	while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_TRANSMITTING) == ERROR)
	{
		if(I2CTimeout-- == 0)	return 3;
	}
	I2CTimeout = I2CT_LONG_TIMEOUT;
	
	//�������ݻ����� ���EV8_2�¼�
	I2C_SendData(I2C_x, data);
	while(I2C_CheckEvent(I2C_x, I2C_EVENT_MASTER_BYTE_TRANSMITTED) == ERROR)
	{
		if(I2CTimeout-- == 0)	return 3;
	}
	I2CTimeout = I2CT_LONG_TIMEOUT;
	
	//����ֹͣ�ź�
	I2C_GenerateSTOP(I2C_x, ENABLE);*/
	return 0;
}


/*
*	��OLEDд����
*	I2C_Command: �������
*/
void WriteCmd(uint8_t I2C_Command)//д����
{
	I2C_OLED_WriteByte(0x00, I2C_Command);
}


/*
*	��OLEDд����
*	I2C_Data: ���ݴ���
*/
void WriteData(uint8_t I2C_Data)//д����
{
	I2C_OLED_WriteByte(0x40, I2C_Data);
}



 /**
  * @brief  OLED_Init����ʼ��OLED
  * @param  ��
  * @retval ��
  */
void OLED_Init(void)
{
	HAL_Delay(1000);	// 1s,�������ʱ����Ҫ,�ϵ����ʱ��û�д�����������
	
	WriteCmd(0x40);//---set low column address
	WriteCmd(0xB0);//---set high column address

	WriteCmd(0xC8);//-not offset

	WriteCmd(0x81);//���öԱȶ�
	WriteCmd(0xff);

	WriteCmd(0xa1);//���ض�������

	WriteCmd(0xa6);//
	
	WriteCmd(0xa8);//��������·��
	WriteCmd(0x1f);//duty = 32
	
	WriteCmd(0xd3);
	WriteCmd(0x00);
	
	WriteCmd(0xd5);
	WriteCmd(0xf0);
	
	WriteCmd(0xd9);
	WriteCmd(0x22);
	
	WriteCmd(0xda);
	WriteCmd(0x02);
	
	WriteCmd(0xdb);
	WriteCmd(0x49);
	
	WriteCmd(0x8d);
	WriteCmd(0x14);
	
	WriteCmd(0xaf);
}


 /**
  * @brief  OLED_Fill�����������Ļ
  * @param  fill_Data:Ҫ��������
	* @retval ��
  */
void OLED_Fill(unsigned char fill_Data)//ȫ�����
{
	unsigned char m,n;
	for(m=0;m<4;m++)
	{
		WriteCmd(0xb0+m);		//page1-page7
		WriteCmd(0x00);			//low column start address
		WriteCmd(0x10);			//high column start address
		for(n=0;n<128;n++)
		{
			WriteData(fill_Data);
		}
	}
}

/*
*	������ʼ������
*	x���������λ��0-255	0x00-0xff
*	y: �������λ��0-3		0x00-0x03
*/
void OLED_SetStartPoint(unsigned char x,unsigned char y)
{
	WriteCmd(0xb0+y);
	WriteCmd(0x0f&x);									//�е�ַ�߰�λ
	WriteCmd(((0xf0&x)>>4)|0x10);			//�е�ַ�Ͱ�λ
}

/*
*	OLED����
*	ȫ��
*/
void OLED_Clear(void)
{
	OLED_Fill(0x00);		//ȫ��
}


 /**
  * @brief  OLED_ON����OLED�������л���
  * @param  ��
	* @retval ��
  */
void OLED_ON(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X14);  //������ɱ�
	WriteCmd(0XAF);  //OLED����
}


 /**
  * @brief  OLED_OFF����OLED���� -- ����ģʽ��,OLED���Ĳ���10uA
  * @param  ��
	* @retval ��
  */
void OLED_OFF(void)
{
	WriteCmd(0X8D);  //���õ�ɱ�
	WriteCmd(0X10);  //�رյ�ɱ�
	WriteCmd(0XAE);  //OLED����
}


 /**
  * @brief  OLED_ShowStr����ʾcodetab.h�е�ASCII�ַ�,��6*8��8*16��ѡ��
  * @param  x,y : ��ʼ������(x:0~127, y:0~3);
	*					ch[] :- Ҫ��ʾ���ַ���; 
	*					TextSize : �ַ���С(1:6*8 ; 2:8*16)
	* @retval ��
  */
void OLED_ShowStr(unsigned char x, unsigned char y, char ch[], unsigned char TextSize)
{
	unsigned char c = 0,i = 0,j = 0;
	switch(TextSize)
	{
		case 1:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 126)
				{
					x = 0;
					y++;
				}
				OLED_SetStartPoint(x,y);
				for(i=0;i<6;i++)
					WriteData(F6x8[c][i]);
				x += 6;
				j++;
			}
		}break;
		case 2:
		{
			while(ch[j] != '\0')
			{
				c = ch[j] - 32;
				if(x > 120)
				{
					x = 0;
					y++;
				}
				OLED_SetStartPoint(x,y);
				for(i=0;i<8;i++)
					WriteData(F8X16[c*16+i]);
				OLED_SetStartPoint(x,y+1);
				for(i=0;i<8;i++)
					WriteData(F8X16[c*16+i+8]);
				x += 8;
				j++;
			}
		}break;
	}
}


 /**
  * @brief  OLED_ShowCN����ʾcodetab.h�еĺ���,16*16����
  * @param  x,y: ��ʼ������(x:0~127, y:0~3); 
	*					N:������codetab.h�е�����
	* @retval ��
  */
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char N)
{
	unsigned char wm=0;
	unsigned int  adder=32*N;
	OLED_SetStartPoint(x , y);
	for(wm = 0;wm < 16;wm++)
	{
		WriteData(F16x16[adder]);
		adder += 1;
	}
	OLED_SetStartPoint(x,y + 1);
	for(wm = 0;wm < 16;wm++)
	{
		WriteData(F16x16[adder]);
		adder += 1;
	}
}

//��ʾ���� 00-99
void OLED_ShowData(uint8_t x,uint8_t y,uint16_t data)
{
	uint8_t shi,ge;
	uint8_t i=0;
	
	if(data>99)		//��ֹ���ݹ���
		data=99;
	
	shi=data/10;
	ge=data%10;
	
	OLED_SetStartPoint(x,y);		//��ʾʮλ �ϰ벿��
	for(i=0;i<32;i++)
	{
		WriteData(NUM[shi*64+i]);
	}
	OLED_SetStartPoint(x,y+1);	//��ʾʮλ �°벿��
	for(i=0;i<32;i++)
	{
		WriteData(NUM[shi*64+i+32]);
	}
	
	OLED_SetStartPoint(x,y+2);	//��ʾ��λ �ϰ벿��
	for(i=0;i<32;i++)
	{
		WriteData(NUM[ge*64+i]);
	}
	OLED_SetStartPoint(x,y+3);	//��ʾ��λ �°벿��
	for(i=0;i<32;i++)
	{
		WriteData(NUM[ge*64+i+32]);
	}
}


 /**
  * @brief  OLED_DrawBMP����ʾBMPλͼ
  * @param  x0,y0 :��ʼ������(x0:0~127, y0:0~3);
	*					x1,y1 : ���Խ���(������)������(x1:0~127,y1:0~3)
	* @retval ��
  */
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char *BMP)
{
	unsigned int j=0;
	unsigned char x,y;

	for(y=y0;y<=y1;y++)
	{
		OLED_SetStartPoint(x0,y);
    for(x=x0;x<=x1;x++)
		{
			WriteData(BMP[j++]);
		}
	}
}
void OLED_Show_Unit(void)
{
	  OLED_DrawBMP(30,0,46,3,BMP_UNIT);
}
void OLED_Show_STOP(void)
{
	OLED_DrawBMP(100,0,127,3,(unsigned  char *)BMP_STOP);
}
void OLED_Show_Alarm(void)
{
	OLED_DrawBMP(100,0,127,3,(unsigned  char *)BMP_ALARM);
}
void OLED_Show_Hot(void)						
{
	OLED_DrawBMP(79,0,99,3,(unsigned  char *)BMP_HOT);
}
void OLED_Show_Cold(void)					
{
	OLED_DrawBMP(79,0,99,3,(unsigned  char *)BMP_COLD);
}
void OLED_Show_NO(void)						
{
	OLED_DrawBMP(79,0,99,3,(unsigned  char *)BMP_MODE_NO);
}
void OLED_Show_Value(uint16_t x)				    
{
	OLED_ShowData(47,0,x);
}
void OLED_Show_Gear(uint16_t x)					
{
	OLED_DrawBMP(0,0,29,3,(unsigned  char *)BMP_Gear[x]);
}
void OLED_Show_Run(void)
{
	OLED_DrawBMP(100,0,127,3,(unsigned  char *)BMP_RUN);
}

void oled_test(void)
{
	 OLED_Show_Unit();
	 OLED_Show_STOP();
	 OLED_Show_Alarm();
	 OLED_Show_Hot();					
	 OLED_Show_Cold();					
	 OLED_Show_NO() ;					
	 OLED_Show_Value(3);					
	 OLED_Show_Gear(2);

}
void oled_process(unsigned char num)
{
   if(sysTick_Count>=50)
   	{
		sysTick_Count = 0;
				 OLED_Show_Unit();		 //��ʾ�¶�
		
		}

		if(num == 0x01)
			{
				OLED_Show_Value(getParams()->temperature);//�¶���ʾ
				
		
			}
			else if(num == 0x02)
			{
				OLED_Show_Gear(getParams()->gear);	  //��λ��ʾ
		
			}
			else if(num == 0x03)
			{
				OLED_Show_NO();   //��λ��ʾ
		
			}
			else if(num == 0x04)
			{
				OLED_Show_Cold();	  //������ʾ
		
			}
			else if(num == 0x05)
			{
				OLED_Show_Hot();	  //������ʾ
		
			}
			else if(num == 0x06)
			{
				OLED_Show_Alarm();	//��ʾ����ͼ��
			}
			else if(num == 0x07)
			{
				OLED_Show_Run();		//��ˮ��ģʽ��ʾ����ͼ��
		
			}
			else if(num == 0x08)
			{
				OLED_Show_STOP();		//�ر�ģʽ��ʾֹͣͼ��
		
			}
			else if(num == 0x09)
			{
				OLED_Show_Unit();		//��ʾ�¶�
		
			}

    

}

