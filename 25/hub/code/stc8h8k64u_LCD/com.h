#ifndef COM_H_
#define COM_H_
#include "main.h" 


#define ON 0
#define OFF 1
#define TIME_OUT  950000
typedef struct
{
	 unsigned char master_recv;
	 unsigned char rec_num[8];

	 unsigned char lock_port;
	 unsigned long  int  timeout;
	 
}com_stru;
void com_recv_proc(void);
void com_tx_slaver(void);
void com_tx_to_master(void);


#endif
