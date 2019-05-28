#ifndef UARTPARSE_H
#define UARTPARSE_H
#define PAYLOAD_HEAD			0xFF
#define COMMANDINFO_SIZE		sizeof(CommandInfo_typedef)

#define PAYLOAD_LEN         0X07
#define SN_LEN     			0x04 

#define KEY_UD_EW_SN_RS_INDEX 		0X00+PAYLOAD_LEN
#define KEY_O1_O8_INDEX 			0X01+PAYLOAD_LEN
#define KEY_INCH_INDEX  			0X02 +PAYLOAD_LEN
#define SN_INDEX     			    0X03 +PAYLOAD_LEN 
#define FREQ_INDEX     			    0X04 +PAYLOAD_LEN+SN_LEN 


#define KEYFUNC_DEFAULT		0x01
#define SN_DEFAULT                  0x5a
#define CHANNEL_DEFAULT          0x01

/*#define KEY_E_W_INDEX 		0X01+PAYLOAD_LEN	
#define KEY_S_N_INDEX 		0X01+PAYLOAD_LEN*/	
	
/*#define KEY_O3_O4_INDEX 	0X02+PAYLOAD_LEN
#define KEY_O5_O6_INDEX 	0X02+PAYLOAD_LEN
#define KEY_START_INDEX 	0X01+PAYLOAD_LEN
#define KEY_STOP_INDEX 	    0X01+PAYLOAD_LEN*/


/*#define KEY_E_W_INCH_INDEX
#define KEY_S_N_INCH_INDEX
#define KEY_O1_O2_INCH_INDEX
#define KEY_O3_O4_INCH_INDEX
#define KEY_O5_O6_INCH_INDEX*/

typedef struct {
unsigned int  freq;
unsigned char KEY_H8;
unsigned char KEY_L8;
unsigned char inhibition; 
unsigned char sn[SN_LEN];
unsigned char len;
}params_typedef;


typedef struct{
unsigned char head;
unsigned char funcode[14];
unsigned char crc;
unsigned char tag;


}CommandInfo_typedef;
params_typedef system_params_get(void);
#endif

