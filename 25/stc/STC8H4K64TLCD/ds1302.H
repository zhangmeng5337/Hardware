//DS3078接口
sbit SSCL=P4^3;
sbit SSDA=P4^4;

//开启SD30XX的I2C总线
bit I2CStart(void)
{
	SSDA=1;
	SSCL=1;
	delay(5);
	if(!SSDA)return 0;	//SDA线为低电平则总线忙,退出
	SSDA=0;
	delay(5);
	while(SSDA)return 0;//SDA线为高电平则总线出错,退出
	SSCL=0;
	delay(5);
	return 1;
}
//关闭SD30XX的I2C总线
void I2CStop(void)
{
	SSDA=0;
	SSCL=0;
	delay(5);
	SSCL=1;
	delay(5);
	SSDA=1;
}
//发送 ACK
void I2CAck(void)
{	
	SSDA=0;
	SSCL=0;
	delay(5);
	SSCL=1;
	delay(5);
	SSCL=0;
}
//发送NO ACK
void I2CNoAck(void)
{	
	SSDA=1;
	SSCL=0;
	delay(5);
	SSCL=1;
	delay(5);
	SSCL=0;
}
//读取ACK信号
bit I2CWaitAck(void) 	 //返回为:1=有ACK,0=无ACK
{
	SSCL=0;
	SSDA=1;		//设置SDA为输入
	delay(5);
	SSCL=1;
	delay(5);
	while(SSDA)
	{
		SSCL=0;
		return 0;
	}
	SSCL=0;
	return 1;
}
//MCU向SD30XX发送一个字节
void I2CSendByte(uchar demand) //数据从高位到低位//
{
	uchar i=8;                       
	while(i--)
	{
		SSCL=0;
		delay(2);
		SSDA=(bit)(demand&0x80);
		demand<<=1;
		delay(5);
		SSCL=1;
		delay(5);
	}
	SSCL=0;
}
//MCU从SD30XX读入一字节
uchar I2CReceiveByte(void)      //数据从高位到低位//
{
	uchar i=8;
	uchar ddata=0;
	SSDA=1;			//设置SDA为输入
	while(i--)
	{
		ddata<<=1;      //数据从高位开始读取
		SSCL=0;
		delay(5);
		SSCL=1;
		delay(5);	//从高位开始 ddata|=SDA;ddata<<=1
		if(SSDA)
		{
			ddata|=0x01;
		}
	}
	SSCL=0;
	return ddata;
}
//I2C写一个字节
bit I2CWriteOneByte(uchar add,date)
{		
	if(!I2CStart())return 0;
	I2CSendByte(0x64);      
	I2CWaitAck();   
	I2CSendByte(add);		//设置写地址      
	I2CWaitAck();	
	I2CSendByte(date);		//写数据
	I2CWaitAck();	
	I2CStop(); 
	return 1;
}

//I2C读一个字节程序
uchar I2CReadOneByte(uchar add)
{		
	uchar dat;
	if(!I2CStart())return 0;
	I2CSendByte(0x64);      
	if(!I2CWaitAck()){I2CStop(); return 0;}
  	I2CSendByte(add);		//设置要读的地址
	I2CWaitAck();
	I2CStart();	
 	I2CSendByte(0x65);	
	I2CWaitAck();	
	dat=I2CReceiveByte();		//读数据
	I2CNoAck();
	I2CStop(); 
	return  dat;
}
//写SD30XX允许程序
bit WriteTimeOn(void)
{		
	if(!I2CWriteOneByte(0x10,0x80))return 0;
	I2CWriteOneByte(0x0f,0xff);
	return	1;
}
//写SD30XX禁止程序
bit WriteTimeOff(void)
{		
	if(!I2CWriteOneByte(0x0f,0x7b))return 0;
	I2CWriteOneByte(0x10,0);
	return	1;
}
//读SD30XX实时数据寄存器
bit I2CReadDate(void)
{
	if(!I2CStart())
	{
		return 0;
	}
	I2CSendByte(0x65); 
    if(!I2CWaitAck())
	{
		I2CStop(); 
		return 0;
	}
	miao=I2CReceiveByte();
	I2CAck();
	fen=I2CReceiveByte();
	I2CAck();
	shi=I2CReceiveByte();
	I2CAck();
	zhou=I2CReceiveByte();
	I2CAck();
	ri=I2CReceiveByte();
	I2CAck();
	yue=I2CReceiveByte();
	I2CAck();
	nian=I2CReceiveByte();
	I2CNoAck();		//读时间完成，发送NoAck
	I2CStop();
	shi=shi&0x3f;
	return	1;
}
//写SD30XX实时数据寄存器(秒,分,时,周,,日,月,年)
//写时间操作要求一次对实时时间寄存器(00H~06H)依次写入
//不可以单独对7个时间数据中的某一位进行写操作,否则可能会引起时间数据的错误进位
//要修改其中某一个数据,应一次性写入全部7个实时时钟数据
bit I2CWriteDate(uchar daa,dab,dac,dad,dae,daf,dag)	
{
	WriteTimeOn();				//使能，开锁
	if(!I2CStart())return 0;
	I2CSendByte(0x64); 
	if(!I2CWaitAck()){I2CStop(); return 0;}
	I2CSendByte(0x00);			//设置写起始地址      
	I2CWaitAck();	
	I2CSendByte(daa);		//second     
	I2CWaitAck();	
	I2CSendByte(dab);		//minute      
	I2CWaitAck();	
	I2CSendByte(dac|0x80);	//hour,同时设置小时寄存器最高位（0:为12小时制,1:为24小时制）
	I2CWaitAck();	
	I2CSendByte(dad);		//week      
	I2CWaitAck();	
	I2CSendByte(dae);		//day      
	I2CWaitAck();	
	I2CSendByte(daf);		//month      
	I2CWaitAck();	
	I2CSendByte(dag);		//year      
	I2CWaitAck();	
	I2CStop();

	WriteTimeOff();				//使能,关锁
	return	1;
}
