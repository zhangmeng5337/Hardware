#ifndef __MODBUS_RTU_H__
#define __MODBUS_RTU_H__
#include "main.h"

#define RS485_ADDR   0x01  //板子所属地址
#define ADDR_MAX     4096  //地址加寄存器最大值
#define INFOSIZE 123  //返回数组长度 6*n+9 n为单个数据帧个数   117

//extern uint8_t  rs485_end_flag ; //1#485完成标志
// extern uint8_t  rs485_end_flag_2 = 0; //2#485完成标志
// extern uint8_t  rs485_end_flag_3 = 0; //3#485完成标志

//extern char RS485_RX_BUFF[512];//接收缓冲区2048字节
extern uint16_t RS485_RX_CNT;//接收计数器
extern uint8_t RS485_FrameFlag;//帧结束标记
extern uint8_t RS485_TX_BUFF[512];//发送缓冲区
extern uint16_t RS485_TX_CNT;//发送计数器





//extern uint32_t FID_V;                 //FID 采样体积

/* 单个数据帧结构 */
typedef struct TEM_SET{
	int16_t FID_T;
	int16_t MS_T;
	int16_t TRAP_T;
	int16_t BY_T;	
}T_SET;


//数据联合体 进行byte 拆分 add by qwn
union bit32_data
{
    uint32_t data;
    struct 
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    }byte;
};

union int_bit32_data
{
    int32_t data;
    struct 
    {
        uint8_t byte0;
        uint8_t byte1;
        uint8_t byte2;
        uint8_t byte3;
    }byte;
};

//数据联合体 进行byte 拆分 add by qwn
union bit16_data
{
    uint16_t data;
    struct 
    {
        uint8_t byte0;
        uint8_t byte1;
    }byte;
};


//结构体嵌套 组成发送数组
/* 单个数据帧结构 */
typedef struct Databack{
	uint8_t id;
	uint8_t len;
	uint8_t byte3;
	uint8_t byte2;
	uint8_t byte1;
	uint8_t byte0;
}DATA;

/* 信息结构体 */
typedef struct CSInfoStrcutre sendback;
typedef struct CSInfoStrcutre{
    uint8_t HEAD;
    uint8_t VER;
		uint8_t LEN;
		uint8_t ID1;
		uint8_t ID2;
		uint8_t TYPE;
		uint8_t STATE;
		DATA FIDV_1;
		DATA FIDF_2;
		DATA MSV_3;
		DATA MSF_4;
		DATA HEATV_5;
		DATA HEATF_6;
		DATA V1_9;
		DATA V2_A;
		DATA V3_B;
		DATA V4_C;
    DATA Pump_F;
    DATA Va6_10;
    DATA Va12_11;
    DATA FIDT_12;
    DATA MST_13;
    DATA CAPT_14;
    DATA DILT_16;
    DATA PRESURE_17;
		DATA WARINING_18;
    uint8_t CRC_SUM;
    uint8_t END;
}* ptrCSInfo;

extern sendback infoS;								//回复数据结构体
extern sendback infoS_test;								//回复数据结构体
extern sendback infoS_test1;								//回复数据结构体
extern T_SET TEM_SET;										//温度设置管理结构体
extern uint32_t FID_V;                 //FID 采样体积
extern uint32_t FID_S;                 //FID 采样流速
extern uint8_t FID_F[2];              //FID 采样流量
extern uint32_t MS_V;                  //MS 采样体积
extern uint32_t MS_S;  							  //MS 采样流速
extern uint8_t MS_F[2]; 
extern uint32_t HEAT_BACK_V; 					//加热反吹体积
extern uint32_t HEAT_BACK_S; 					//加热反吹流速
extern uint8_t HEAT_BACK_F[2]; 					//加热反吹流速
extern uint32_t MFC_V; 								//备用 MFC 体积
extern uint8_t MFC_F[2];                 //备用 MFC 流量
//extern uint32_t VALVE1_STATE;          //阀 1 状态
//extern uint32_t VALVE2_STATE;          //阀 2 状态
//extern uint32_t VALVE3_STATE;          //阀 3 状态
//extern uint32_t VALVE4_STATE; 					//阀 4 状态
//extern uint32_t VALVE5_STATE;					//阀 5 状态
//extern uint32_t VALVE6_STATE;					//阀 6 状态
//extern uint32_t PUMP_STATE;						//泵状态
//extern uint32_t SIX_VALVE_STATE;				//六位阀状态
//extern uint32_t TWELVE_VALVE_STATE;		//十二通阀状态
extern uint8_t FID_T[2];									//FID 除水温度
extern uint8_t MS_T[2];									//MS 除水温度
extern uint8_t TRAP_T[2];								//捕集管温度
extern uint8_t BY_T[2];									//备用温度
extern uint8_t DILUTER_T[2];							//稀释仪温度
extern uint32_t SAMP_GAS_T;						//样气压力
/********************************
 * 
 * 外设设置变量
 * 
 * ******************************/
//extern uint32_t VALVE1_SET;						//阀 1
//extern uint32_t VALVE2_SET;						//阀 2
//extern uint32_t VALVE3_SET;						//阀 3
//extern uint32_t VALVE4_SET;						//阀 4
//extern uint32_t VALVE5_SET;						//阀 5
//extern uint32_t VALVE6_SET;						//阀 6
//extern uint32_t PUMP_SET;							//泵
//extern uint32_t GMS_SET;								//GSMS 触发
//extern uint32_t SIX_VALVE_SET;					//六位阀
//extern uint32_t TWELVE_VALVE_SET;			//十二通阀
extern uint8_t FID_T_SET[2];							//FID 除水温度
extern uint8_t MS_T_SET[2];							//MS 除水温度
extern uint8_t TRAP_T_SET[2];						//捕集管温度
extern uint8_t BY_T_SET[2];							//备用温度
//extern uint8_t FID_F_SET[2];							//FID 采样流量
//extern uint8_t MS_F_SET[2];							//MS 采样流量
//extern uint8_t HEAT_BACK_F_SET[2];				//加热反吹流量
//extern uint8_t BY_MFC_F_SET[2];					//备用 MFC 流量
extern uint8_t hty[2];
extern uint8_t fid[2];
extern uint8_t ms[2];
extern uint8_t heat[2];
extern uint8_t command[19][11];
extern uint8_t command2[2][10] ;
extern uint8_t warning[8][11];
extern int16_t tF,tC,tM ;
extern float tF_printf ;
//数据打包函数
void CSInfo_2Array_uint8(ptrCSInfo infoSeg,uint8_t* infoArray);
//CRC校验函数
uint16_t CRC16 (uint8_t arr_buff[], uint8_t len);
//上位机通信校验函数
char calsum(const char *data, int nLen);
#endif /*__ MODBUS_RTU_H__ */


