#ifndef __BSP_STEP_MOTOR_INIT_H
#define	__BSP_STEP_MOTOR_INIT_H

#include "main.h"

/*宏定义*/
/*******************************************************/
//宏定义对应开发板的接口 1 、2 、3 、4
#define CHANNEL_SW 1


//Motor 方向 
//#define MOTOR_Dir_Pin                  	GPIO_PIN_1   
//#define MOTOR_Dir_GPIO_Port            	GPIOE                    
//#define MOTOR_DIR_GPIO_CLK_ENABLE()   	__HAL_RCC_GPIOE_CLK_ENABLE()

//Motor 使能 
//#define MOTOR_Rst_Pin                  	GPIO_PIN_0
//#define MOTOR_Rst_GPIO_Port            	GPIOE                       
//#define MOTOR_EN_GPIO_CLK_ENABLE()    	__HAL_RCC_GPIOE_CLK_ENABLE()
	
//Motor 脉冲
#define MOTOR_PUL_IRQn                  TIM1_BRK_TIM15_IRQn
#define MOTOR_PUL_IRQHandler            TIM1_BRK_TIM15_IRQHandler

#define MOTOR_PUL_TIM                   TIM15
#define MOTOR_PUL_CLK_ENABLE()  		    __TIM15_CLK_ENABLE()

//#define MOTOR_PUL_PORT       	     		  GPIOI
//#define MOTOR_PUL_PIN             		  GPIO_PIN_5
//#define MOTOR_PUL_GPIO_CLK_ENABLE()		  __HAL_RCC_GPIOI_CLK_ENABLE()
//
//#define MOTOR_PUL_GPIO_AF               GPIO_AF3_TIM8
#define MOTOR_PUL_CHANNEL_x             TIM_CHANNEL_1



/*频率相关参数*/
//定时器实际时钟频率为：168MHz/TIM_PRESCALER
//其中 高级定时器的 频率为168MHz,其他定时器为84MHz
//168/TIM_PRESCALER = 8MHz
//具体需要的频率可以自己计算
//#define TIM_PRESCALER                21
//// 定义定时器周期，输出比较模式周期设置为0xFFFF
//#define TIM_PERIOD                   0xFFFF

/************************************************************/
#define HIGH GPIO_PIN_SET	  //高电平
#define LOW  GPIO_PIN_RESET	//低电平

#define ON  GPIO_PIN_SET	            //开
#define OFF GPIO_PIN_RESET	          //关

//#define CW 	HIGH		        //顺时针
//#define CCW LOW      	      //逆时针
#define FORWARD    0
#define BACKWARD   1
//控制使能引脚
/* 带参宏，可以像内联函数一样使用 */  
#define MOTOR_EN(x)					HAL_GPIO_WritePin(MOTOR_Rst_GPIO_Port,MOTOR_Rst_Pin,x)
//#define MOTOR_PUL(x)				HAL_GPIO_WritePin(MOTOR_PUL_GPIO_PORT,MOTOR_PUL_PIN,x)
#define MOTOR_DIR(x)				HAL_GPIO_WritePin(MOTOR_Dir_GPIO_Port,MOTOR_Dir_Pin,x)

//extern TIM_HandleTypeDef TIM_StepperHandle;

extern void stepper_Init(void);

#endif /* __STEP_MOTOR_INIT_H */
