#ifndef GSM_H
#define GSM_H
#define Ser_Head 0xfdfd
#define Dev_Head 0xfefe

#define SIMCOM_TIME_OUT					600




//typedef struct
//{
//uint32_t 	rx_len;
//uint32_t 	rx_len_var;
//unsigned char 	receive_flag;
//unsigned int      real_index;
//unsigned int      loop_index;
//unsigned char	UsartReceiveData[BUFFERSIZE];
//
//}Uart_Types;




//网络注册状态


//网络注册状态

#define	SIMCOM_NET_INIT 				        12    	//开机未完成
#define	SIMCOM_NET_NOT 						0    	//开机未完成
#define	SIMCOM_READY_YES					1     //开机完成
#define	SIMCOM_ATE0   						2    	//关闭回显
//#define	SIMCOM_SIM_OK  						3    	//SIM卡ok
#define	SIMCOM_Network_Intensity_READY		                3  	//信号满足要求
#define	SIMCOM_GPRS_READY  					4    	//gprs ok
#define	SIMCOM_NET_PORT_CLOSE				        5    	//关闭tcp连接
#define	SIMCOM_NET_TRANSPARENT				        6    	//透传模式
#define	SIMCOM_Network_Normal_ModeT                             7
#define	SIMCOM_Connect_Platform                                 8
#define	SIMCOM_NET_OK						9	//链接成功
#define	SIMCOM_NET_IDLE						10
#define	SIMCOM_NetOpen_READY				        11     //net open

#define	SIMCOM_NET_ERROR					0XFF	//错误
unsigned char Get_Network_status(void);
void SIMCOM_Register_Network(void);
void test(void);
void Set_Network_status(unsigned char status);
void gps_test(void);
#endif
