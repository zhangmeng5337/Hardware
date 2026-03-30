#ifndef _modbusmfc_h_
#define _modbusmfc_h_
//#include "stm32f10x.h"
#include "modbus_crc.h"
#include "main.h"

//?豸MFC????
#define MFC_FID 0x00    //FID设备id
#define MFC_MS 0x02    //MS设备fid
#define MFC_HEAT 0x04    //HEAT设备id
#define MFC_READ 0x03 //功能码 读
#define MFC_WRITE 0x06 //功能码 写
#define MFC_SET 0x10 //功能码 设置
//#define MFC_FLOW_ADD 0x0010 //???????

#define MFC_FLOW_16_HIGH_ADD 0x0016 //流量高16位地址
#define MFC_FLOW_16_LOW_ADD 0x0017 //流量低16位地址

#define MFC_FLOWCUM_32H_HIGH_ADD 0x0018 //累计流量高32位高位
#define MFC_FLOWCUM_32H_LOW_ADD 0x0019 //累计流量高32位低位
#define MFC_FLOWCUM_32L_HIGH_ADD 0x001A //累计流量低32位高位
#define MFC_FLOWCUM_32L_LOW_ADD 0x001B //累计流量低32位低位

#define MFC_COTR_ADD 0x0021 //阀门控制方式

#define MFC_SETFLOW_16_HIGH_ADD 0x0022 //设定流量高16位地址
#define MFC_SETFLOW_16_LOW_ADD 0x0023 //设定流量低16位地址
#define MFC_SETZERO_ADD 0x0025 //设定流量高16位地址   
//#define MFC_SETZERO_ADD 0x0025 //设定流量高16位地址


//#define CSMS483_READ_SV_ADD 0x0000 //????????????
//#define CSMS483_WRITE_SV_ADD 0x0000 //д??????????

typedef struct 
{
	//uint8_t slave_add;
	uint32_t cum_flow_data[2];	//流量数据	
	//uint8_t data_type;		//数据类型
	uint16_t cum_data_uint;		//数据单位
	uint16_t data_time_day;		//天数
	uint16_t data_time_hour;		//时
	uint16_t data_time_min;		//分
	uint16_t data_time_sec;		//秒
}MFC_FLOWCUM;

typedef struct 
{
	uint8_t slave_add;		//设备地址
	uint32_t flow_data;	//流量数据	
	uint8_t data_type;		//数据类型
	MFC_FLOWCUM cum;			//累计流量结构体
}MFC_FLOW;




extern uint16_t Reg[];





//void Host_send03(void);
void MFC_Read03_slave(uint8_t slave,uint16_t StartAddr,uint16_t num);
void MFC_write06_slave(uint8_t slave,uint16_t StartAddr,uint16_t *num);
void MFC_write10_slave(uint8_t slave,uint16_t StartAddr,uint16_t reg_len,uint8_t len,uint8_t *num);

void MFC_Read_flow(uint8_t slave);
//void MFC_Read_setflow(uint8_t slave);
void MFC_SET_flow(uint8_t slave,uint8_t Flow[],int buf_size);
void MFC_Read_cum_flow(uint8_t slave);
void MFC_RESET_cum_flow(uint8_t slave);
void MFC_SET_zeropoint(uint8_t slave);
void MFC_deal_rxdata(uint8_t *rxbuf,uint8_t rx_len,MFC_FLOW *mfc);
void MFC_Read_flowandcumflow(uint8_t slave);
#endif
