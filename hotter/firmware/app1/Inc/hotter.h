#ifndef HOTTER_H_
#define HOTTER_H_

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
	unsigned char status[32];//设备地址;控制标志;模式选择;L4采暖回差;L5采暖设定温度;故障代码
	unsigned char control_reg[32];
}hotter_stru;

hotter_stru *get_hotter(unsigned dev_addr);


#endif

