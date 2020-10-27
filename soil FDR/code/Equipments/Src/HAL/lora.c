#include "lora.h"
#include "ParamsSet.h"
#include "uart.h"
extern uart_usr_stru lora_master_uart;
extern uart_usr_stru lora_slave_uart;

extern LORA_Params_stru LORA_Master;
extern LORA_Params_stru LORA_Slave;
void lora_process()
{
	if(lora_master_uart.receivedFlag== 1)
	{
		memcpy(LORA_Master.buffer+lora_master_uart.buffer[1],&lora_master_uart.buffer[3],lora_master_uart.len);
		unsigned char i;
		i=0;
		LORA_Master.addH = LORA_Master.buffer[i++];
		LORA_Master.addL =  LORA_Master.buffer[i++];
		LORA_Master.netid = LORA_Master.buffer[i++];
		LORA_Master.reg0 =  LORA_Master.buffer[i++];
		LORA_Master.reg1 = LORA_Master.buffer[i++];
		LORA_Master.reg2 =  LORA_Master.buffer[i++];
		LORA_Master.reg3 = LORA_Master.buffer[i++];
		LORA_Master.crptH =  LORA_Master.buffer[i++];		
		LORA_Master.crptL =  LORA_Master.buffer[i++];		
		LoraParamsAnaly(LORA_Master);
		unsigned char tmp;
		tmp = 0;
		if(lora_master_uart.buffer[0] == 0xc0||lora_master_uart.buffer[0] == 0xc1)
			tmp = 2;
		if(lora_master_uart.buffer[0] == 0xc1)
			tmp = 1;	

		loraModeConfig(LORA_MASTER,tmp,lora_master_uart.buffer[1],lora_master_uart.len,0);
	}
}