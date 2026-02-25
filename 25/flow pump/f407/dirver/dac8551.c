
#include "dac8551.h"
//#include "arm_math.h"

extern SPI_HandleTypeDef hspi2;
dac_stru dac_usr;

uint16_t Zero_Voltage_OutPut = No_Impedance;
//uint16_t LOAD_Channel = Load_AB;
uint16_t Channel_A_OutPut=32767, Channel_B_OutPut=16384;//max=65535


void dacInit(void)
{
 dac_usr.vref = 2.5;
 dac_usr.mode = DAC_4To20MA;
}



//------------------------------------------------------------------------------
//函数名称：写入DA输出寄存器的数据
//函数功能：D/A转换的子程序
//参数传递：unsigned int datasheet  输出的电压
//------------------------------------------------------------------------------
void DAC8552_DA_convert(uint16_t datasheet)
{
     uint16_t i;
     uint8_t buf[3];
	 
     buf[0]=No_Impedance; 
	 buf[1]=datasheet >> 8;  
	 buf[2]=datasheet;  
     
     SYNC_1;
     SYNC_0;
       ;
    HAL_SPI_Transmit(&hspi2, buf, 3, 1000);	
     SYNC_1;
}
//------------------------------------------------------------------------------
//函数名称：输出DAC
//函数功能：输出设定的电压
//参数传递：无
//------------------------------------------------------------------------------
void DAC8552_Out_Put_Voltage(float cur)
{
   float tmp;
   tmp = cur * 0.1;
	if(dac_usr.mode == DAC_4To20MA)
	{
		 dac_usr.dacVoltage = (tmp+0.4-dac_usr.zero)/dac_usr.vref*65535;
	}
	else
		 dac_usr.dacVoltage = (tmp-dac_usr.zero)/dac_usr.vref*65535;
   
	
    DAC8552_DA_convert(dac_usr.dacVoltage);

}

