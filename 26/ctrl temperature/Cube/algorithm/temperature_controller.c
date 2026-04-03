/******************************************************************************
 * 文件: temp_control_heater_only.c
 * 描述: 单加热器温控系统（无主动制冷），适应 -150℃~200℃ 温区
 *       实现大温差全速加热 + 分段自适应PID + 卡尔曼滤波 + 前馈
 * 注意: 降温只能依靠被动散热，稳定时间取决于散热条件，可能不满足30s要求
 * 版本: 3.0 (单加热器版)
 ******************************************************************************/

#include <stdint.h>
#include <math.h>
#include "temperature_controller.h"
#include "main.h"
#include "2dpid.h"

/* ======================= 硬件抽象层（需用户实现） ========================= */
extern TIM_HandleTypeDef htim4;
 temperatureStru temperatureU[3];
 KalmanFilter_t kf[3];
 KalmanFilter_t cfg;
 KalmanFilter_t slopekf;
 float slope[3],slopekft[3];
 float last[3];
 float pwmMax = 40000.0;
extern unsigned char divT;
TempControl_t g_tc[3];uint32_t pwm;
void Hardware_SetHeaterOutput(unsigned char channel,float final_out)
 {
	 
	 pwm = final_out*pwmMax;
 if(channel == 0)
__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,pwm);
 else  if(channel == 1)
 __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,pwm);	
 else  if(channel == 2)
__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,pwm);
 }

void TempControl_Step(TempControl_t *tc,unsigned char channel) {
    
    Hardware_SetHeaterOutput(channel,tc->output);
    // 5. 驱动执行器 
}


/* ========================= 示例主程序 ===================================== */

void kalmanProc(unsigned char channel)
{
	temperatureU[channel].temperatureFlt = Kalman_Update(&kf[channel], temperatureU[channel].temperatureOri, g_tc[channel].pid_heat.periodMeter);
	slope[channel] = (temperatureU[channel].temperatureFlt - last[channel]) /g_tc[0].pid_heat.periodMeter;
	slopekft[channel] = Kalman_Update(&slopekf, temperatureU[channel].temperatureFlt, g_tc[0].pid_heat.periodMeter);
	last[channel] = temperatureU[channel].temperatureFlt;
}
void controllerInit(void)
{
	temperatureU[0].periodMeter = 0.002;
	temperatureU[1].periodMeter = 0.002;	
	temperatureU[2].periodMeter = 0.002;
	temperatureU[0].periodTask = 15;
	temperatureU[1].periodTask = 15;
	temperatureU[2].periodTask = 15;
	temperatureU[0].div = 15;
	temperatureU[1].div = 15;
	temperatureU[2].div = 15;	
	
	cfg.Q = 0.0001;
	cfg.R = 10;
	Kalman_Init(&kf[0],&cfg);
	Kalman_Init(&kf[1],&cfg);
	Kalman_Init(&kf[2],&cfg);	
		cfg.Q = 2.5;
	cfg.R = 0.01;
	Kalman_Init(&slopekf,&cfg);
	TemperatureControl_Init();
}
void controller(unsigned char channel) {

  sysTickGet(channel);
  ControlTask_Run(channel,temperatureU[channel].temperatureTarget);	
	updatePIDperiod();
 //TempControl_Step(&g_tc[channel],channel);
    

}
