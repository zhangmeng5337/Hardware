#include "app.h"
#include "modbus.h"
#include "eeprom.h"
#include "adc.h"
#include "FdrAlgorithm.h"
extern factor_stru factor_usr;
extern modbus_stru modbus_usr;
void params_init()
{   
	  uint32_t p[8];
	unsigned char addr;
	   addr = 0;
     flash_read(addr,p,8);
			 modbus_usr.RS485_Addr =0x01; 
			 modbus_usr.RS485_Baudrate = 9600; 
			 modbus_usr.RS485_Parity = 0;	
	   if(p[0]==0x0000005a)
		 {
      modbus_usr.RS485_Addr=p[1];
      modbus_usr.RS485_Baudrate=p[2];
      modbus_usr.RS485_Parity=p[3];	
			factor_usr.a0 = p[4];
			factor_usr.a1 = p[5];
			factor_usr.a2 = p[6];				 
		 }
		 else
		 {
			 flash_write(addr++,0x5a,1); 
			 flash_write(addr++,modbus_usr.RS485_Addr,1); 
			 flash_write(addr++,modbus_usr.RS485_Baudrate,1); 
			 flash_write(addr++,modbus_usr.RS485_Parity,1);

				factor_usr.a0 = 1;
				factor_usr.a1 = 1;
				factor_usr.a2 = 1;	
				flash_write(addr++,factor_usr.a0,1);	
				flash_write(addr++,factor_usr.a1,1);	
				flash_write(addr++,factor_usr.a2,1);	 		 
		 }


	}
	
void app_loop()
{
	
	RS485_Service();
}
