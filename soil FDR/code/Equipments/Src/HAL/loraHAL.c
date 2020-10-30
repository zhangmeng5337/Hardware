#include "loraHAL.h"
#include "loraHW.h"
#include "Protocol_C.h"

loraUart_stru loraUart;
LORAHW_stru lorahw;
LORA_Params_stru loraParams;

unsigned char Lora_Params_set()
{
	
}
void Lora_RxCpltCallback(unsigned char uartNo)
{
	if(uartNo == 2)
	{
		loraUart.receivedFlag1 = 1;
	}
	else if(uartNo == 6)
	{
		loraUart.receivedFlag2 = 1;
	}
}
void Lora_Process()
{  
	LORAHW_stru loraNo;

	if(loraUart.receivedFlag1 == 1)
	{
		if(protocolCAnaly(loraUart.lora1RxBuffer)  == 0)
		{   
		     loraNo.loraNo = 0;
			 loraNo.mode =  TransmitMode;
			 read_equipment();
			 sendProtocolCData(&loraNo);
		}
		  
	}
	if(loraUart.receivedFlag2 == 1)
	{
		
	}	
}
