#ifndef GSM_H
#define GSM_H
#define Ser_Head 0xfdfd
#define Dev_Head 0xfefe
#include "stm8l15x.h"
#define SIMCOM_TIME_OUT					60

#define buffer_size  128

typedef struct
{
uint32_t 	rx_len;
uint32_t 	rx_len_var;
unsigned char 	receive_flag;
unsigned int      real_index;
unsigned int      loop_index;
unsigned char	UsartReceiveData[buffer_size];

}Uart_Types;


//网络注册状态


//网络注册状态


#define	SIMCOM_NET_NOT 						0    	//开机未完成
#define	SIMCOM_POWER_ON                                         1
#define	SIMCOM_READY_YES                                        2
#define	SIMCOM_CLOSE_ECHO                                       3
#define	SIMCOM_CARD_DET                                         4
#define	SIMCOM_NET_CLOSE                                        5
#define	SIMCOM_NET_PASS_THROUGH                                 6
#define	SIMCOM_Connect_Platform                                 7
#define	SIMCOM_NET_ERROR					0XFF	//错误
#define	SIMCOM_NET_OK						8	//链接成功
//#define	SIMCOM_READY_YES					1     //开机完成
//#define	SIMCOM_ATE0   						2    	//关闭回显






//#define	SIMCOM_SIM_OK  						3    	//SIM卡ok
//#define	SIMCOM_Network_Intensity_READY		                4  	//信号满足要求
//#define	SIMCOM_GPRS_READY  					5    	//gprs ok
//#define	SIMCOM_NET_PORT_CLOSE				        6    	//关闭tcp连接
//#define	SIMCOM_NET_TRANSPARENT				        7    	//透传模式
//
//#define	SIMCOM_Network_Normal_ModeT         8
//

//#define	SIMCOM_NET_IDLE						14
//
//
//#define	SIMCOM_NetOpen_READY				11     //net open
//
//#define	SIMCOM_NET_OK2     13

unsigned char Get_Network_status(void);
void SIMCOM_Register_Network(void);
void test(void);
void Set_Network_status(void);
#endif
