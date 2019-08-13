#ifndef PROTOCOL_H
#define PROTOCOL_H
#define STATIC_MODE			        0X81
#define DYNAMIC_MODE			0x91
#define NODE_TO_SERVERH		0xAB
#define NODE_TO_SERVERL			0xAB
#define SERVER_TO_NODEH		0xAA
#define SERVER_TO_NODEL		       0xAA
#define LORA_ACK    		              0x24

typedef struct{
	unsigned int header;
	unsigned char id[3];	
	unsigned char func;
	unsigned char len;
	unsigned char checksum;
	unsigned char seq_num;
	unsigned char *p;
}DataPack_stru;
#endif

