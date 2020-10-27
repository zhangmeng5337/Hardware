#include "app.h"
#include "modbus.h"
#include "eeprom.h"
#include "adc.h"
#include "FdrAlgorithm.h"
extern factor_stru factor_usr;//湿度算法系数
extern modbus_stru modbus_usr;//modbus数据
extern sensor_stru sensor_usr;//adc采集转换数据
/************************************************
功能：存储关键参数
*************************************************/
void fator_save_proc(unsigned char addr)
{
				uint32_t tmp;
			  tmp = FloatToCharProc(factor_usr.a0,10);//湿度系数1
				flash_write(addr++,tmp>>24,1);
				flash_write(addr++,(tmp>>16&0x000000ff),1);
				flash_write(addr++,(tmp>>8&0x000000ff),1);
				flash_write(addr++,(tmp&0x000000ff),1);	
			 
			  tmp = FloatToCharProc(factor_usr.a1,10);//湿度系数2
				flash_write(addr++,tmp>>24,1);
				flash_write(addr++,(tmp>>16&0x000000ff),1);
				flash_write(addr++,(tmp>>8&0x000000ff),1);
				flash_write(addr++,(tmp&0x000000ff),1);	
				
			  tmp = FloatToCharProc(factor_usr.a2,10);//湿度系数3
				flash_write(addr++,tmp>>24,1);
				flash_write(addr++,(tmp>>16&0x000000ff),1);
				flash_write(addr++,(tmp>>8&0x000000ff),1);
				flash_write(addr++,(tmp&0x000000ff),1);
	      
			  tmp = FloatToCharProc(sensor_usr.ADC_REF,1000);//参考电压
				flash_write(addr++,tmp>>24,1);
				flash_write(addr++,(tmp>>16&0x000000ff),1);
				flash_write(addr++,(tmp>>8&0x000000ff),1);
				flash_write(addr++,(tmp&0x000000ff),1);
				
			  tmp = FloatToCharProc(sensor_usr.CalibrationProbeVref,1000);//探头空测电压
				flash_write(addr++,tmp>>24,1);
				flash_write(addr++,(tmp>>16&0x000000ff),1);
				flash_write(addr++,(tmp>>8&0x000000ff),1);
				flash_write(addr++,(tmp&0x000000ff),1);

}
/**************************************************************
传感器参数初始化
***************************************************************/
void params_init()
{   
	  uint32_t p[25];
	
	unsigned char addr;
	   addr = 0;
     flash_read(addr,p,25);
			 modbus_usr.RS485_Addr =0x01; 
			 modbus_usr.RS485_Baudrate = 9600; 
			 modbus_usr.RS485_Parity = 0;
	     sensor_usr.CalibrationT = 0;
	     sensor_usr.CalibrationR = 0;	
	   if(p[0]==0x0000005a)//参数存储过，直接读取参数
		 {
      modbus_usr.RS485_Addr=p[1];
      modbus_usr.RS485_Baudrate=p[2];
      modbus_usr.RS485_Parity=p[3];	
			 
			factor_usr.a0 = DataMinusProc(&p[4],1,10);
			factor_usr.a1 =  DataMinusProc(&p[8],1,10);
			factor_usr.a2 =  DataMinusProc(&p[12],1,10);
			 if(p[16]!=0xffffffff)
				sensor_usr.ADC_REF = DataMinusProc(&p[16],1,1000);	
			if(p[20]!=0xffffffff)
				sensor_usr.CalibrationProbeVref = DataMinusProc(&p[20],1,1000);		
			if(p[24]!=0xffffffff)
				sensor_usr.CalibrationT = p[24];

						 
		 }
		 else
		 {
			  //初始参数
			 flash_init();
				factor_usr.a0 = -2874.1;
				factor_usr.a1 = 9204.2;
				factor_usr.a2 = -7340.6;	
			 
			 flash_write(addr++,0x5a,1); 
			 flash_write(addr++,modbus_usr.RS485_Addr,1); 
			 flash_write(addr++,modbus_usr.RS485_Baudrate,1); 
			 flash_write(addr++,modbus_usr.RS485_Parity,1);
       fator_save_proc(addr);
					
			flash_write(addr+12+4+4,sensor_usr.CalibrationT,1);			 		 
		 }


	}
void params_save()
{
			unsigned char addr;
			addr = 0;	
			flash_init();
			flash_write(addr++,0x5a,1);
			flash_write(addr++,modbus_usr.RS485_Addr,1);
			flash_write(addr++,modbus_usr.RS485_Baudrate,1);
			flash_write(addr++,modbus_usr.RS485_Parity,1);
			fator_save_proc(addr);
			flash_write(addr+12+4+4,sensor_usr.CalibrationT,1);		//温度校准
}
void app_loop()
{
	
	RS485_Service();//主程序
}
