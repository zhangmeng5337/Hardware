#include "74hc595.h"
void HC595_CS(void) 
{
	/**  步骤3：STCP产生一个上升沿，移位寄存器的数据移入存储寄存器  **/
	HC595_STCP_Low();   // 将STCP拉低
	delay1_us(1);           // 适当延时
	HC595_STCP_High();  // 再将STCP拉高，STCP即可产生一个上升沿
	delay1_us(1);
}
 
void HC595_SendByte(uint8_t byte)
{
	uint8_t i;
	for (i = 0; i < 8; i ++)  //一个字�?位，传输8次，一次一位，循环8次，刚好移完8�?
	{
	  /****  步骤1：将数据传到DS引脚    ****/
		if (byte & 0x80)        //先传输高位，通过与运算判断第八是否为1
			HC595_Data_High();    //如果第八位是1，则�?595 DS连接的引脚输出高电平
		else                    //否则输出低电�?
			HC595_Data_Low();
		
		/*** 步骤2：SHCP每产生一个上升沿，当前的bit就被送入移位寄存�?***/
		HC595_SHCP_Low();   // SHCP拉低
		delay1_us(1);           // 适当延时
		HC595_SHCP_High();  // SHCP拉高�?SHCP产生上升�?
		delay1_us(1);
		
		byte <<= 1;		// 左移一位，将低位往高位移，通过	if (byte & 0x80)判断低位是否�?
	}
}
 
void XL74HC595_MultiWrite(uint8_t *data, uint8_t Length)//混合写数�?
{
	uint8_t i;
	for (i = 0; i < Length; i ++ ) // len 个字�?
	{
		HC595_SendByte(data[i]);
	}
	
	HC595_CS(); //先把所有字节发送完，再使能输出
}


