void delay_us(unsigned char)
{

}
void IIC_Init()
{
	iic_sck(set);
	iic_sda(set);
}
void IIC_Ack()
{
	iic_sda(reset);	
	iic_sck(set);
	delay_us(1);
	iic_sck(reset);
	iic_sda(set);
}
void IIC_nAck()
{
	iic_sda(set);	
	iic_sck(set);
	delay_us(1);
	iic_sck(reset);
	iic_sda(set);
}

IIC_Start()
{
	iic_sck(set);
	iic_sda(set);
	delay_us(1);
	iic_sda(reset);	
	delay_us(1);
	iic_sck(reset);
	delay_us(3);

}
IIC_Stop()
{
	iic_sda(reset);
	delay_us(2);
	iic_sck(set);
	delay_us(2);	
	iic_sda(set);	
}
unsigned char IIC_Write(unsigned char data)
{
	unsigned char i,ack_bit;
	for(i=0;i<8;i++)
	{
		if(data&0x80)
		{
			iic_sda(set);
		}
		else
		{
			iic_sda(reset);
		}
		iic_sck(set);
		delay_us(1);
		iic_sck(reset);
		data = data <<1;
	}
	iic_sda(set);		
	iic_sck(set);
	ack_bit = iic_sda_read();
	iic_sck(reset);
	return ack_bit;
}
unsigned char IIC_Read()
{
	unsigned char i,data=0;
	for(i=0;i<8;i++)
	{
		iic_sck(set);
		if(iic_sda_read())
			data = data |0x01;
		if(i<7)
			data = data <<1;
		iic_sck(reset);
	}
	return data;
} 
unsigned char ack;
unsigned char iic_data;

void Write_Byte(unsigned char addr,unsigned char data)
{
	 IIC_Init();
	 IIC_Start();
	 ack=IIC_Write(SLAVE_ADDRESS|0x00);
	 ack=IIC_Write(addr);
	 ack=IIC_Write(data);
	 IIC_Stop();
}
void Fast_Write_Byte(unsigned char addr,unsigned char data)
{
	unsigned char i;
	 IIC_Init();
	 IIC_Start();
	 ack=IIC_Write(SLAVE_ADDRESS|0x00);
	 ack=IIC_Write(addr);
	 for(i = 0;i<len;i++)
	 {
		 ack=IIC_Write(data);
		 IIC_Ack();
	 }
	 IIC_Stop();
}

void Read_Byte(unsigned char addr)
{
	 IIC_Init();
	 IIC_Start();
	 ack=IIC_Write(SLAVE_ADDRESS|0x01);
	 ack=IIC_Write(addr);
	 *iic_data=IIC_Read();
	 IIC_nAck();
	 IIC_Stop();
}
void Fast_Read_Byte(unsigned char addr,unsigned char len)
{
	unsigned char i;
	 IIC_Init();
	 IIC_Start();
	 ack=IIC_Write(SLAVE_ADDRESS|0x01);
	 ack=IIC_Write(addr);
	 for(i = 0;i<len;i++)
	 {
		 iic_data=IIC_Read();
		 IIC_Ack();
	 }
	 IIC_Stop();
}

