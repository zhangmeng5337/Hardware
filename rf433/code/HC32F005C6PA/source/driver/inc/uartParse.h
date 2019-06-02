#ifndef UARTPARSE_H
#define UARTPARSE_H
#define COMMANDINFO_SIZE			sizeof(CommandInfo_typedef)

#define PAYLOAD_LEN         			0X02
#define SN_LEN     					0x04 

#define FUNC_INDEX     			   	 0x00

#define KEY_H8_INDEX 				0X00+PAYLOAD_LEN
#define KEY_L8_INDEX 				0X01+KEY_H8_INDEX
#define KEY_INCH_INDEX  				0X01 +KEY_L8_INDEX
#define SN_INDEX     			   	       0X01 +KEY_INCH_INDEX 
#define FREQ_INDEX     			    	SN_INDEX+1 


#define KEYFUNC_DEFAULT			0x01
#define SN_DEFAULT                  		0x5a
#define CHANNEL_DEFAULT          	0x01

#define WAKEUP_CMD				0x57
#define FORMAT_CMD				0x56
#define SETTING_CMD				0x12
#define SNACK_CMD				0x58

typedef struct {

unsigned char KEY_H8;
unsigned char KEY_L8;
unsigned char inhibition; 
unsigned char sn[SN_LEN];
unsigned int  freq;
unsigned int  CommandStatusToggleFlag;
}params_typedef;
typedef struct {

unsigned char head;
unsigned char func;
unsigned char sn[SN_LEN];
unsigned int  freq;
unsigned int  crc;
}rfparams_typedef;

typedef struct{
unsigned char head;
unsigned char funcode[11];//功能2byte，sn码4byte,频点2byte，按键2byte,拟制关系1byte，预留1byte
unsigned int crc;
unsigned char tag;
}CommandInfo_typedef;
params_typedef *system_params_get(void);
void uartPrase(void);
#endif

