#include "app.h"
#include "modbus.h"
#include "eeprom.h"
#include "adc.h"
extern modbus_stru modbus_usr;
void params_init()
{   
	  uint32_t p[4];
     flash_read(0,p,4);
			 modbus_usr.RS485_Addr =0x01; 
			 modbus_usr.RS485_Baudrate = 9600; 
			 modbus_usr.RS485_Parity = 0;	
	   if(p[0]==0x0000005a)
		 {
      modbus_usr.RS485_Addr=p[1];
      modbus_usr.RS485_Baudrate=p[2];
      modbus_usr.RS485_Parity=p[3];			 
		 }
		 else
		 {
			 flash_write(0,0x5a,1); 
			 flash_write(1,modbus_usr.RS485_Addr,1); 
			 flash_write(2,modbus_usr.RS485_Baudrate,1); 
			 flash_write(3,modbus_usr.RS485_Parity,1); 		 
		 }


	}
	
void app_loop()
{
	Get_Adc_Average(50);
	//RS485_Service();
}
