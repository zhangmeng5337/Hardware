#ifndef _PID_h_
#define _PID_h_

#include "main.h"
#define MAX_EK 20		//最大误差值
#define MAX_TIME_CNT 50	//最大误差时间计数 300 *50 = 15000ms = 15s
#define CAL_TIME 300 //计算周期300ms 和freertos.c文件中定时计算时间有关 约为300ms
typedef struct
{
	float ref;  //??
	float fd;  //??
	float kp;
	float ki;
	float kd;
	float Integ;
	float err;
	float err_pre;
	float err_pre_pre;
	float max;
	float min;
	float out;
	float Sv;
//	float Sv;//用户设定值
//	float Pv;
//	float Pv1;//上次温度
//	float Kp;
//	float Ki;
//	float Kd;
//	float T;  //PID计算周期--采样周期
//	float Ti;
//	float Td; 
//	float Ek;  //本次偏差
//	float Ek_1;//上次偏差
//	float SEk; //历史偏差之和
//	float Iout;
//	float Pout;
//	float Dout;	
//	float OUT0;
//	float OUT;
//	float speed;
//	int low_cnt;
//	uint16_t C10ms;
//	uint16_t pwmcycle;//pwm周期
//	uint32_t times;

}PID;





typedef struct
{
	float Sv;//用户设定值
	float Kp_SET;
	float Ki_SET;
	float Kd_SET;
	int change_flag;
}PID_UPDATE;

extern PID pid1; //存放PID算法所需要的数据
extern PID pid2; //存放PID算法所需要的数据
extern PID pid3; //存放PID算法所需要的数据
extern PID_UPDATE pid_update1;	//更新PID参数
extern PID_UPDATE pid_update2;	//更新PID参数
extern PID_UPDATE pid_update3;	//更新PID参数
//#define pwmout_1    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET)    //停止加热
//#define pwmout_0    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET)  //加热

//#define pwmout2_1    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET)    //停止加热
//#define pwmout2_0    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET)  //加热

//#define pwmout3_1    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET)    //停止加热
//#define pwmout3_0    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_SET)  //加热

void PIDOUT_init(void);//pid输出脚初始化
void PID_out(void); //输出PID运算结果到负载
void PID1_Calc(void);
void PID2_Calc(void);
void PID3_Calc(void);
void PID_Calc_overshoot(void);
void PID1_Init(void);
void PID2_Init(void);
void PID3_Init(void);
void PID1_SET();
void PID2_SET();
void PID3_SET();
int cmd_rx(uint8_t *rxbuf,uint8_t rx_len);
#endif

