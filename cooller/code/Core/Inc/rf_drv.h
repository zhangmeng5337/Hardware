/**--------------File Info------------------------------------------------------------------------------
File name:			  rf200_drv.h
** Last modified Date:  2018-05-30
** Last Version:		V1.02
** Descriptions:		RF200 FRAME HANDLE
**------------------------------------------------------------------------------------------------------
** Created date:		2018-05-30
** Version:			  	V1.02
** Descriptions:		RF200 FRAME HANDLE
**
********************************************************************************************************/
#ifndef __RF200_DRV_H
#define __RF200_DRV_H
#include "main.h"
#define RF_HEADER 	0xbb
#define RF_TYPE 	0x01
#define RF_COMMAND 	0xc0
#define RF_RECV_FLAG 	100
#define RF_RETRY_TIMES 	5

typedef struct
{
    unsigned char  wait_card_id;
	unsigned char tx_card_id;
	unsigned char retry_times;
	unsigned char id_status[18];
	float average_T;
    float card_T[18];
} rf_recv_stru;
void rf_ctrl_proc(void);
rf_recv_stru*get_rf_status(void);

#endif

