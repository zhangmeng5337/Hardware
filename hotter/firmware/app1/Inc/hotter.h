#ifndef HOTTER_H_
#define HOTTER_H_
#include "modbus.h"
#define STATUS1_SIZE 23
#define STATUS2_SIZE  17
#define STATUS3_SIZE  143
#define AIR_PUMP_SIZE  3
//控制板--->服务端(周期性上传)
//{
//    "Dev ID": "866289037465624",
//    "Status Data": {
//        "Out Tem": 45,
//        "In Tem": 26,
//        "Front Pressure": 0.5,
//        "After Pressure": 0.5,
//        "Status": 4,
//        "air_pump 1":[1,25,36,14,55,96],//设备地址;控制标志;模式选择;L4采暖回差;L5采暖设定温度;故障代码
//        "air_pump 2":[2,25,36,14,55,96],
//        "air_pump 3":[3,25,36,14,55,96],
//	    "air_pump 4":[],
//
//    },
//    "Dev Params": {
//        "Version": "V3.0.1",
//        "Set Out Tem": 45,
//        "Set In Tem": 23,
//        "Upload Period(second)": 10
//    }
//}

typedef struct 
{
	unsigned int status[STATUS1_SIZE];//设备地址;故障标志位，staus1控制标志;模式选择;L4采暖回差;L5采暖设定温度;故障代码:;modbus失联
	unsigned char control_reg[10];
	unsigned char update[DEV_SIZE];
	unsigned int status2[STATUS2_SIZE];//设备地址;故障标志位，staus2
	unsigned int status3[STATUS3_SIZE];//设备地址;故障标志位，staus2
}hotter_stru;
float *get_power(void);
hotter_stru *get_hotter(unsigned dev_addr);
uint32_t *get_power2(void);


#endif

