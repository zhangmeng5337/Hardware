#ifndef _LTE_HAL_H
#define _LTE_HAL_H
#include "main.h"
#define LTE_SHORT_DELAY_MQTT  300
#define LTE_SHORT_DELAY  50
#define LTE_LONG_DELAY  1000
#define LTE_LONG_DELAY2  1000

typedef enum
{
    //AT_RST,
    AT,
    ATE0,
    AT_CPIN,
    AT_CGSN,
    AT_CCID,
    AT_CGATT,
    AT_IDLE,
    AT_REBOOT,
    //mqtt
    AT_MCONFIG,
    AT_MIPSTART,
    AT_MCONNECT,
    AT_MSUB,
     AT_MSUB_1,   
    AT_MIPCLOSE,

    /*HTTP�汾��*/
    AT_SAPBR_1,
    AT_SAPBR_2,
    AT_SAPBR_3,
    AT_HTTPINIT_1,
    AT_HTTPPARA_1,
    AT_HTTPPARA_2,
    AT_HTTPACTION_1,
    AT_HTTPACTION_2,
    AT_HTTPACTION_3,

    AT_HTTPREAD_1,
    
    /*HTTP get���ָ��*/
    AT_HTTPINIT_2,
    AT_HTTPPARA_3,
    AT_HTTPPARA_4,
    AT_HTTPREAD_2,
    AT_HTTPTERM_1,
    
    /*HTTP post���ָ��*/
    AT_HTTPINIT_3,
    AT_HTTPPARA_5,
    AT_HTTPPARA_6,
    AT_HTTPDATA_1,
    AT_HTTPDATA_2,	//������
    AT_HTTPACTION_6,
    AT_HTTPREAD_POST,
	AT_HTTPPARA_7,
	AT_HTTPACTION_21,
	AT_HTTPTERM_21,
	AT_HTTPTERM_22,

    //mqtt������Ϣ����
    AT_MPUB_RECV,
    AT_MPUB
} teATCmdNum;



/*����ģ��״̬��ö�ٱ���*/
typedef enum
{
    SUCCESS_REC = 0,
    TIME_OUT,
    NO_REC,
    NOT_CONNECT,
    NET_CONNECT,
    ERROR_STATUS
} teATStatus;
typedef struct
{
    char *ATSendStr;
    char *ATRecStr;
	unsigned char net_step;
	unsigned char net_status;
    teATStatus ATStatus;
    uint8_t RtyNum;
} tsATCmds;

uint8_t *lte_Check_Cmd(uint8_t *str);
uint8_t lte_Send_Cmd(uint8_t *cmd, uint8_t *ack, unsigned int WaitTime);

uint8_t lte_Info_Show(void);
void lte_CallNum(uint8_t *Num);
void lte_CmdShowOff(void);
void lte_SendEN_SMS(uint8_t *phone,uint8_t *text);
void lte_proc(void);
void lte_init(void);
uint8_t lte_Send_Cmd_mqtt(uint8_t mode,uint8_t *cmd, uint8_t *ack, unsigned int WaitTime);


unsigned char  ATRec(char *s);
void CAT1_Init(void);
void clear_uart_buf(unsigned char flag);

#endif


