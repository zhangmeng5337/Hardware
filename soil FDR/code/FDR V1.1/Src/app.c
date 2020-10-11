#include "app.h"
#include "modbus.h"
#include "eeprom.h"
#include "adc.h"
#include "FdrAlgorithm.h"
extern factor_stru factor_usr;
extern modbus_stru modbus_usr;
void params_init()
{   
	  uint32_t p[16];
	
	unsigned char addr;
	   addr = 0;
     flash_read(addr,p,16);
			 modbus_usr.RS485_Addr =0x01; 
			 modbus_usr.RS485_Baudrate = 9600; 
			 modbus_usr.RS485_Parity = 0;	
	   if(p[0]==0x0000005a)
		 {
      modbus_usr.RS485_Addr=p[1];
      modbus_usr.RS485_Baudrate=p[2];
      modbus_usr.RS485_Parity=p[3];	
//			factor_usr.a0 = DataMinusProc(&p[4],4);
//			factor_usr.a1 =  DataMinusProc(&p[8],4);
//			factor_usr.a2 =  DataMinusProc(&p[12],4);	
			 //初始参数
//			factor_usr.a0 = -2874.1;
//			factor_usr.a1 = 9204.2;
//			factor_usr.a2 = -7340.6;			 
		 }
		 else
		 {
			  //初始参数
				factor_usr.a0 = -2874.1;
				factor_usr.a1 = 9204.2;
				factor_usr.a2 = -7340.6;	 
			 flash_write(addr++,0x5a,1); 
			 flash_write(addr++,modbus_usr.RS485_Addr,1); 
			 flash_write(addr++,modbus_usr.RS485_Baudrate,1); 
			 flash_write(addr++,modbus_usr.RS485_Parity,1);

	      uint32_t tmp;
			  tmp = FloatToCharProc(factor_usr.a0);
				flash_write(addr++,tmp,1);	
			  tmp = FloatToCharProc(factor_usr.a1);
				flash_write(addr++,tmp,1);	
			  tmp = FloatToCharProc(factor_usr.a2);
				flash_write(addr++,tmp,1);		 		 
		 }


	}
	
void app_loop()
{
	
	RS485_Service();
}
