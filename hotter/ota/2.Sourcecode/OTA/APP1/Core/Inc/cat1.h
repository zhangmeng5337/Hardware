#ifndef __CAT1_H__
#define __CAT1_H__

#include "stm32L4xx.h"
#include "time.h"
#include "usart.h"
#include <string.h>
#include <stdio.h>
#include "time.h"
#include "gpio.h"
#include "my_string.h"
#include "stmflash.h"

extern int reboot_flag;

/*设置CAT1状态的枚举变量*/
typedef enum
{
    CAT1_IDIE = 0,
    CAT1_SEND,
    CAT1_WAIT,
    CAT1_ACCESS
} teCAT1_TaskStatus;


/*设置AT指令集的枚举变量*/
typedef enum
{
    //AT_RST,
    AT,
    ATE0,
    AT_CGSN,
    AT_CCID,
    AT_CEREG,
    AT_CGATT,
    AT_CSQ_1,
    AT_CMQTTSTART,
    AT_CMQTTACCQ,
    AT_CMQTTCONNECT,
    AT_CSQ_2,
    AT_CMQTTTOPIC,
    AT_TOPIC,
    AT_CMQTTPAYLOAD,
    AT_MESSAGE,
    AT_CMQTTPUB,
		/*HTTP相关指令*/
		AT_HTTPINIT_1,
		AT_HTTPPARA_VERSION,
		AT_HTTPACTION_1,
		AT_HTTPREAD_1,
		AT_HTTPTERM_1,	
		AT_HTTPINIT_2,
		AT_HTTPPARA_DATA,
		AT_HTTPACTION_2,
		AT_HTTPREAD_2,
		AT_HTTPTERM_2,
} teATCmdNum;

/*设置模组状态的枚举变量*/
typedef enum
{
    SUCCESS_REC = 0,
    TIME_OUT,
    NO_REC
} teATStatus;

/*设置AT指令集的结构体*/
typedef struct
{
    char *ATSendStr;
    char *ATRecStr;
    uint16_t TimeOut;
    teATStatus ATStatus;
    uint8_t RtyNum;
} tsATCmds;

void CAT1_Init(void);
void CAT1_Task(void);
void Msg_Handler(void);
void MQTTSendData(void);
void Get_Version(void);
void Update_Task(void);
void Download_BIN(void);
#endif















