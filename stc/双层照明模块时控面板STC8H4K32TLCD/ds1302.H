//DS3078接口



//读SD30XX实时数据寄存器
/*bit I2CReadDate(void)
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
}*/
//写SD30XX实时数据寄存器(秒,分,时,周,,日,月,年)
//写时间操作要求一次对实时时间寄存器(00H~06H)依次写入
//不可以单独对7个时间数据中的某一位进行写操作,否则可能会引起时间数据的错误进位
//要修改其中某一个数据,应一次性写入全部7个实时时钟数据
/*bit I2CWriteDate(uchar daa,dab,dac,dad,dae,daf,dag)	
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
}*/
