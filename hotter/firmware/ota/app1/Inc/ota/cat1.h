#ifndef __CAT1_H__
#define __CAT1_H__

#include "main.h"
#include "time.h"
#include "uart.h"
#include <string.h>
#include <stdio.h>
#include "time.h"
//#include "gpio.h"
#include "my_string.h"
#include "stmflash.h"
#include "config.h"
extern int reboot_flag;

/*设置CAT1状态的枚举变量*/
typedef enum
{
    CAT1_IDIE = 0,
    CAT1_SEND,
    CAT1_WAIT,
    CAT1_MQTT_REC,
    CAT1_ACCESS
} teCAT1_TaskStatus;


/*设置AT指令集的枚举变量*/
typedef enum
{
    //AT_RST,
    AT,
    ATE0,
    AT_CPIN,
    AT_CGSN,
    AT_CCID,
    AT_CGATT,
    
    //mqtt
    AT_MCONFIG,
    AT_MIPSTART,
    AT_MCONNECT,
    AT_MSUB,

    /*HTTP版本号*/
    AT_SAPBR_1,
    AT_SAPBR_2,
    AT_SAPBR_3,
    AT_HTTPINIT_1,
    AT_HTTPPARA_1,
    AT_HTTPPARA_2,
    AT_HTTPACTION_1,
    AT_HTTPREAD_1,
    
    /*HTTP get相关指令*/
    AT_HTTPINIT_2,
    AT_HTTPPARA_3,
    AT_HTTPPARA_4,
    AT_HTTPACTION_2,
    AT_HTTPREAD_2,
    AT_HTTPTERM_1,
    
    /*HTTP post相关指令*/
    AT_HTTPINIT_3,
    AT_HTTPPARA_5,
    AT_HTTPPARA_6,
    AT_HTTPDATA_1,
    AT_HTTPDATA_2,	//发数据
    AT_HTTPACTION_6,
    AT_HTTPREAD_POST,

    //mqtt订阅消息接收
    AT_MPUB_RECV,
    AT_MPUB
} teATCmdNum;

/*设置模组状态的枚举变量*/
typedef enum
{
    SUCCESS_REC = 0,
    TIME_OUT,
    NO_REC,
    ERROR_STATUS
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
void Get_Version(void);
void Update_Task(void);
void Download_BIN(void);
void MQTTSendData(unsigned char *s);


#endif















