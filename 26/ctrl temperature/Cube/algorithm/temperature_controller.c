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
 KalmanFilter_t kf;
KalmanConfig_t cfg;
 KalmanFilter_t slopekf;
 float slope[3],slopekft[3];
 float last[3];

TempControl_t g_tc[3];
void Hardware_SetHeaterOutput(unsigned char channel,uint32_t final_out)
 {
 if(channel == 0)
__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,final_out);
 else  if(channel == 1)
 __HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,final_out);	
 else  if(channel == 2)
__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,final_out);
 }

void TempControl_Step(TempControl_t *tc,unsigned char channel) {
    
    Hardware_SetHeaterOutput(channel,tc->output);
    // 5. 驱动执行器 
}


/* ========================= 示例主程序 ===================================== */

void kalmanProc(unsigned char channel)
{
	temperatureU[channel].temperatureFlt = Kalman_Update(&g_tc[channel].kalman, temperatureU[channel].temperatureOri, g_tc[0].pid_heat.periodMeter);
	slope[channel] = (temperatureU[channel].temperatureFlt - last[channel]) /g_tc[0].pid_heat.periodMeter;
	slopekft[channel] = Kalman_Update(&slopekf, slope[channel], g_tc[0].pid_heat.periodMeter);
	last[channel] = temperatureU[channel].temperatureFlt;
}
void controllerInit(void)
{
	temperatureU[0].periodMeter = 0.002;
	temperatureU[1].periodMeter = 0.1;	
	temperatureU[2].periodMeter = 0.1;
	Kalman_Init(kalman *kfp,KalmanFilter_t* cfg)
//	g_tc[0].pid_heat.periodMeter = temperatureU[0].periodMeter;
//	g_tc[1].pid_heat.periodMeter = temperatureU[1].periodMeter;
//	g_tc[2].pid_heat.periodMeter = temperatureU[2].periodMeter;
//	// 温控系统初始化：初始温度假设为25℃，初始目标温度25℃
//	TempControl_Init(&g_tc[0], 25.0f, -200.0f);
//	TempControl_Init(&g_tc[1], 25.0f, -200.0f);
//	TempControl_Init(&g_tc[2], 25.0f, -200.0f);
//	// 设置目标温度（例如：从室温加热到200℃）
//	TempControl_SetSetpoint(&g_tc[0], -200.0f);
//	TempControl_SetSetpoint(&g_tc[1], -200.0f);
//	TempControl_SetSetpoint(&g_tc[2], -200.0f);
	TemperatureControl_Init();
}
void controller(unsigned char channel) {


  ControlTask_Run(channel,temperatureU[channel].temperatureTarget);	
 //TempControl_Step(&g_tc[channel],channel);
    

}
