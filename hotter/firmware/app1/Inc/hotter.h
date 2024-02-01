#ifndef HOTTER_H_
#define HOTTER_H_

//���ư�--->�����(�������ϴ�)
//{
//    "Dev ID": "866289037465624",
//    "Status Data": {
//        "Out Tem": 45,
//        "In Tem": 26,
//        "Front Pressure": 0.5,
//        "After Pressure": 0.5,
//        "Status": 4,
//        "air_pump 1":[1,25,36,14,55,96],//�豸��ַ;���Ʊ�־;ģʽѡ��;L4��ů�ز�;L5��ů�趨�¶�;���ϴ���
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
	unsigned char status[32];//�豸��ַ;���Ʊ�־;ģʽѡ��;L4��ů�ز�;L5��ů�趨�¶�;���ϴ���
	unsigned char control_reg[32];
}hotter_stru;

hotter_stru *get_hotter(unsigned dev_addr);


#endif

