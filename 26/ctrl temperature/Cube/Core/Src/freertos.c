/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
#include "set.h"
#include "modbus_rtu.h"
#include "usart.h"
#include "spi.h"
#include "tim.h"
#include "string.h"
#include "temp_pid.h"
#include "math.h"
#include "app_fifo.h"
#include "max31856.h"
#include "modbus_mfc.h"
#include "iap.h"
#include "crc.h"
#include "stmflash.h"
#include "iwdg.h"
//#include "mbedtls/entropy.h"
//#include "mbedtls/ctr_drbg.h"
//#include "mbedtls/aes.h"
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/

#include "temperature_controller.h"


/* USER CODE BEGIN Includes */
#include "modbus.h"
//	__IO double temp=0;
//	__IO double temp1=0;
//	__IO uint16_t tmp1=0;
	uint8_t tmp[2];
	volatile 	char flag;
	//extern char RS485_RX_BUFF[512]  ; //接收数据
	extern uint16_t RS485_RX_CNT;//接收计数器
	//extern PID pid; //存放PID算法所需要的数据

//	__IO float tem_2;//存放热电偶2算法所需要的数据
//	float tem_3;//存放热电偶3算法所需要的数据
//	float tem_4;//存放热电偶4算法所需要的数据
	
//	uint8_t buffer[3] = {0};
//	uint8_t buffer1[2] = {0};
//	uint8_t buffer2=0;
//	uint8_t mode[2]={0};
//	uint8_t mask=0;
	uint8_t bff = 0xff;
	uint8_t b00 =0x00;
	int Vfid_flag,Vms_flag,Vheat_flag=0;
	int Tfid_flag,Tms_flag,Tcap_flag,Tdil_flag=0;
	
	//int STfid_flag,STms_flag,STcap_flag,STdil_flag=0;
	
	//int RSfid_flag,RSms_flag,RSheat_flag=0;
	//int val6_flag,val12_flag=0;
		
		
	int Vfid_flag_cnt,Vms_flag_cnt,Vheat_flag_cnt,Tfid_flag_cnt,Tms_flag_cnt,Tcap_flag_cnt=0;
	int Vfid_flag_W,Vms_flag_W,Vheat_flag_W,Tfid_flag_W,Tms_flag_W,Tcap_flag_W=0;
	int Vfid_flag_warning,Vms_flag_warning,Vheat_flag_warning,Tfid_flag_warning,Tms_flag_warning,Tcap_flag_warning=0;	
	
	//int vale1_flag,vale2_flag,vale3_flag,vale4_flag,vale5_flag,vale6_flag,valeA_flag,valeB_flag=0;	
	//float tem_fid,tem_ms,tem_cap=0;
	
	float FVsave,MVsave,HVsave=0;
	//int gcms_working_flag;
	int tx_flag = 0;
	extern union bit32_data num;
	extern union bit32_data tem;
	extern union bit32_data flow_v;
	extern union bit32_data flow_s;
	extern union bit16_data flow;
	extern union bit32_data flow32;
	//extern int sendbegin_flag;
	

/*********************************pid******************

*/

extern temperatureStru temperatureU[3];	
//char vale6_move[] = {0X2F,0X32,0X43,0X57,0X33,0X0D};

#define vale1_move "/1GO1\r"
#define vale2_move "/1GO2\r"
#define vale3_move "/1GO3\r"
#define vale4_move "/1GO4\r"
#define vale5_move "/1GO5\r"
#define vale6_move "/1GO6\r"
#define vale12_moveTOB "/3CC\r"
#define vale12_moveTOA "/3CW\r"
#define SMA "/1SMA\r"
#define SM "/2SM\r"
#define IDset "/2ID0\r"

#define ask6 "/1CP\r"
#define ask12 "/3CP\r"

#define TASK_MFCTX_BIT_1    (0x01 << 0)
#define TASK_WARNING_BIT_2    (0x01 << 1)
#define TASK_VALTX_BIT_3    (0x01 << 2)
#define TASK_TEM_BIT_4    (0x01 << 3)
#define TASK_WARNING_BIT_5    (0x01 << 4)
#define TASK_BIT_6    (0x01 << 5)
#define TASK_BIT_ALL	(TASK_MFCTX_BIT_1  |TASK_WARNING_BIT_2 | TASK_VALTX_BIT_3|TASK_TEM_BIT_4)

int cnt = 0;
uint8_t rx_buffer[20];
uint8_t rx_len=0;
///1上位机   2流量计  3六位阀  4十二通阀
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
EventGroupHandle_t IWDGEventGroup;//声明事件组 
osSemaphoreId uart5_SemHandle;		//声明串口5信号量
/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osThreadId MFC_RXHandle;
osThreadId RS485_sreviceHandle;
osThreadId MFC_TXHandle;
osThreadId Value_TXHandle;
osThreadId WarningHandle;
osThreadId Value_RXHandle;
osThreadId Tem_TXHandle;
osThreadId Tem_RXHandle;
osThreadId get_temTaskHandle;
osThreadId get_pidHandle;
osThreadId tid;
osMessageQId MFC_QueueHandle;
osMessageQId Value_QueueHandle;
osMessageQId TEM_QueueHandle;
osSemaphoreId uart1_SemHandle;
osSemaphoreId uart2_SemHandle;
osSemaphoreId uart3_SemHandle;
osSemaphoreId uart4_SemHandle;
osSemaphoreId TIM3_SemHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

				__IO uint32_t F=0;
				__IO uint32_t M=0;
				__IO uint32_t H=0;
				uint8_t zero = 0x00;
				uint8_t one = 0xff;
int fid_c,ms_c,heat_c;

//define of aes and bin
uint8_t *output1;
//mbedtls_aes_context aes;
int rx_len_mid;
uint8_t mid[16];
uint8_t temp_code[16];
uint8_t  decode_len = 0;
uint8_t iv1[] ={0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
unsigned char key[16] ={0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
//end of define


/* USER CODE END FunctionPrototypes */
// 线程函数声明
void Task_Periodic1(void const *argument);
void Task_Periodic(void const *argument);
void StartDefaultTask(void const * argument);
void MFC_RX_Task(void const * argument);
void RS485_srevice_Task(void const * argument);
void MFC_TX_Task(void const * argument);
void Value_TX_Task(void const * argument);
void Warning_Task(void const * argument);
void Value_RX_Task(void const * argument);
void Tem_TX_Task(void const * argument);
void Tem_RX_Task(void const * argument);
void get_tem(void const * argument);
void pid_update(void const * argument);


void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* Hook prototypes */
void configureTimerForRunTimeStats(void);
unsigned long getRunTimeCounterValue(void);

/* USER CODE BEGIN 1 */
/* Functions needed when configGENERATE_RUN_TIME_STATS is on */
__weak void configureTimerForRunTimeStats(void)
{

}

__weak unsigned long getRunTimeCounterValue(void)
{
return 0;
}
/* USER CODE END 1 */

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
	
	 IWDGEventGroup = xEventGroupCreate();	//创建事
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of uart1_Sem */
  osSemaphoreDef(uart1_Sem);
  uart1_SemHandle = osSemaphoreCreate(osSemaphore(uart1_Sem), 1);

  /* definition and creation of uart2_Sem */
  osSemaphoreDef(uart2_Sem);
  uart2_SemHandle = osSemaphoreCreate(osSemaphore(uart2_Sem), 1);

  /* definition and creation of uart3_Sem */
  osSemaphoreDef(uart3_Sem);
  uart3_SemHandle = osSemaphoreCreate(osSemaphore(uart3_Sem), 1);

  /* definition and creation of uart4_Sem */
  osSemaphoreDef(uart4_Sem);
  uart4_SemHandle = osSemaphoreCreate(osSemaphore(uart4_Sem), 1);

  /* definition and creation of TIM3_Sem */
  osSemaphoreDef(TIM3_Sem);
  TIM3_SemHandle = osSemaphoreCreate(osSemaphore(TIM3_Sem), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
	osSemaphoreDef(uart5_Sem);
  uart5_SemHandle = osSemaphoreCreate(osSemaphore(uart5_Sem), 1);
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of MFC_Queue */
  osMessageQDef(MFC_Queue, 4, uint16_t);
  MFC_QueueHandle = osMessageCreate(osMessageQ(MFC_Queue), NULL);

  /* definition and creation of Value_Queue */
  osMessageQDef(Value_Queue, 8, uint16_t);
  Value_QueueHandle = osMessageCreate(osMessageQ(Value_Queue), NULL);

  /* definition and creation of TEM_Queue */
  osMessageQDef(TEM_Queue, 4, uint16_t);
  TEM_QueueHandle = osMessageCreate(osMessageQ(TEM_Queue), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityHigh, 0, 256);	//osPriorityIdle
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of MFC_RX */
  osThreadDef(MFC_RX, MFC_RX_Task, osPriorityHigh, 0, 128);
  MFC_RXHandle = osThreadCreate(osThread(MFC_RX), NULL);

  /* definition and creation of RS485_srevice */
  osThreadDef(RS485_srevice, RS485_srevice_Task, osPriorityHigh, 0, 512);
  RS485_sreviceHandle = osThreadCreate(osThread(RS485_srevice), NULL);

  /* definition and creation of MFC_TX */
  osThreadDef(MFC_TX, MFC_TX_Task, osPriorityAboveNormal, 0, 128);
  MFC_TXHandle = osThreadCreate(osThread(MFC_TX), NULL);

  /* definition and creation of Value_TX */
  osThreadDef(Value_TX, Value_TX_Task, osPriorityAboveNormal, 0, 128);
  Value_TXHandle = osThreadCreate(osThread(Value_TX), NULL);

  /* definition and creation of Warning */
  osThreadDef(Warning, Warning_Task, osPriorityLow, 0, 128);
  WarningHandle = osThreadCreate(osThread(Warning), NULL);

  /* definition and creation of Value_RX */
  osThreadDef(Value_RX, Value_RX_Task, osPriorityHigh, 0, 128);
  Value_RXHandle = osThreadCreate(osThread(Value_RX), NULL);

  /* definition and creation of Tem_TX */
  osThreadDef(Tem_TX, Tem_TX_Task, osPriorityAboveNormal, 0, 128);
  Tem_TXHandle = osThreadCreate(osThread(Tem_TX), NULL);

  /* definition and creation of Tem_RX */
  osThreadDef(Tem_RX, Tem_RX_Task, osPriorityHigh, 0, 128);
  Tem_RXHandle = osThreadCreate(osThread(Tem_RX), NULL);

  /* definition and creation of get_temTask */
  
	
	osThreadDef(get_temTask, get_tem, osPriorityNormal, 0, 128);	//osPriorityNormal
  get_temTaskHandle = osThreadCreate(osThread(get_temTask), NULL);
	// 定义线程
	osThreadDef(Task_Periodic, Task_Periodic1,osPriorityNormal, 0, 128);
	 tid = osThreadCreate(osThread(Task_Periodic), NULL);	
	  /* definition and creation of pid */
  osThreadDef(pidTask, pid_update, osPriorityLow, 0, 128);//osPriorityNormal
  get_pidHandle = osThreadCreate(osThread(pidTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
	HAL_TIM_Base_Start_IT(&htim3);//信号量初始化完成后开定时器中断释放，避免卡死
//	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //使能IDLE中断
//	HAL_UART_Receive_DMA(&huart1,usart1_rx_buffer,BUFFER_SIZE);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	//PID更新命令
	BaseType_t err = pdFALSE;
  /* Infinite loop */
  for(;;)
  {
		if(uart5_SemHandle!=NULL) 
		{
			err = xSemaphoreTake(uart5_SemHandle,portMAX_DELAY);//等待串口2信号量没有就一直阻塞
			if(err==pdTRUE)
			{
				int cmd_err = cmd_rx(rx_buffer5,rx_len5);
				
				
			}
			
		}
		
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_MFC_RX_Task */
/**
* @brief Function implementing the MFC_RX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MFC_RX_Task */
void MFC_RX_Task(void const * argument)
{
  /* USER CODE BEGIN MFC_RX_Task */
	  BaseType_t err = pdFALSE;
  /* Infinite loop */
  for(;;)
  {
     if(uart2_SemHandle!=NULL)  //接收完成标志
			{
				  err = xSemaphoreTake(uart2_SemHandle,portMAX_DELAY);//等待串口2信号量没有就一直阻塞
			    if(err==pdTRUE) //获取信号量成功
			    {						     
				     float dfidv=0;
				     float dmsv=0;	
						 float dheatv =0;
				     //测量值
						 if(rx_buffer2[1] == 0x06)	//	发送清0命令 测量体积清0
						 {
							 if(rx_buffer2[0] == 0x00)
							 {
								 FVsave = 0;
							 }
							 else if(rx_buffer2[0] == 0x02)
							 {
								 MVsave = 0;
							 }
							 	else if(rx_buffer2[0] == 0x04)
							 {
								 HVsave = 0;
							 }
						 }
//						 if(rx_buffer2[1] == 0x06 && rx_buffer2[0] == 0x00)
//							  HAL_UART_Transmit_DMA(&huart6, (uint8_t *)rx_buffer2, 10);//调试发送
				     if(rx_buffer2[1] == 0x03 && rx_buffer2[0] == 0x00)
				     {
								//HAL_UART_Transmit_DMA(&huart6, (uint8_t *)rx_buffer2, 10);//发送
					      Vfid_flag_warning = 0;
					      FID_F[0] = rx_buffer2[3];
					      FID_F[1] = rx_buffer2[4];
							 if((FID_F[0] & 0x80) == 0x80)//最高位数据为F 数据应该为0
								{
									F = 0;
								}
							else if((FID_F[0] & 0x00) == 0x00)
								{
									F = FID_F[0]*256+FID_F[1];//求得流速
									F=round(500*F/4095);//计算流速 结果四舍五入到小数点后一位再乘10
								}
								//将流速分为4byte存入数组中
								flow_s.data=F;
								infoS.FIDF_2.byte3=flow_s.byte.byte3;
								infoS.FIDF_2.byte2=flow_s.byte.byte2;
								infoS.FIDF_2.byte1=flow_s.byte.byte1;
								infoS.FIDF_2.byte0=flow_s.byte.byte0;
								if(Vfid_flag == 1)
								{
									  //采样标志置1												
										dfidv = F/60.0;//每1s采样 的值 换算成ml/s
										FVsave += dfidv;	//每秒采样值累加
										FID_V = round(FVsave);//取出值
										Vfid_flag=0;	
										flow_v.data=FID_V;
										infoS.FIDV_1.byte3=flow_v.byte.byte3;
										infoS.FIDV_1.byte2=flow_v.byte.byte2;
										infoS.FIDV_1.byte1=flow_v.byte.byte1;
										infoS.FIDV_1.byte0=flow_v.byte.byte0;
									
								}

				    }
						else if(rx_buffer2[1] == 0x03 && rx_buffer2[0] == 0x02)
						{
								Vms_flag_warning = 0;
								MS_F[0] = rx_buffer2[3];
								MS_F[1] = rx_buffer2[4];
								if((MS_F[0] & 0x80) == 0x80)//最高位数据为F 数据应该为0
									{
										M = 0;
									}
								else if((MS_F[0] & 0x00) == 0x00)
									{
										M = MS_F[0]*256+MS_F[1];//求得流速
										M = round(500*M/4095);//计算流速 结果四舍五入到小数点后一位再乘10
									}							
								//将流速分为4byte存入数组中
								flow_s.data=M;
								infoS.MSF_4.byte3=flow_s.byte.byte3;
								infoS.MSF_4.byte2=flow_s.byte.byte2;
								infoS.MSF_4.byte1=flow_s.byte.byte1;
								infoS.MSF_4.byte0=flow_s.byte.byte0;
								if(Vms_flag==1)
								{					

										dmsv=M/60.0;
										MVsave += dmsv;	//每秒采样值累加
										MS_V = round(MVsave);//取出值
										Vms_flag=0;
										flow_v.data=MS_V;
										infoS.MSV_3.byte3=flow_v.byte.byte3;
										infoS.MSV_3.byte2=flow_v.byte.byte2;
										infoS.MSV_3.byte1=flow_v.byte.byte1;
										infoS.MSV_3.byte0=flow_v.byte.byte0;
								}

						}
						else if(rx_buffer2[1] == 0x03 && rx_buffer2[0] == 0x04)
						{
								Vheat_flag_warning = 0;
								HEAT_BACK_F[0] = rx_buffer2[3];
								HEAT_BACK_F[1] = rx_buffer2[4];
								if((HEAT_BACK_F[0] & 0x80) == 0x80)//最高位数据为F 超量程 实际测试此时应该是未通气 数据不稳定导致，实际数据应该为0
									{
										H = 0;
									}
								else if((HEAT_BACK_F[0] & 0x00) == 0x00)//
									{
										H=HEAT_BACK_F[0]*256+HEAT_BACK_F[1];//计算流速数据
										//H=round(500*H/4095);//计算流速 结果四舍五入到小数点后一位再乘10 老量程 50ml
										H=round(10000*H/4095);//计算流速 结果四舍五入到小数点后一位再乘10 新量程1000ml
									}
																
								//将流速分为4byte存入数组中
								flow_s.data=H;
								infoS.HEATF_6.byte3=flow_s.byte.byte3;
								infoS.HEATF_6.byte2=flow_s.byte.byte2;
								infoS.HEATF_6.byte1=flow_s.byte.byte1;
								infoS.HEATF_6.byte0=flow_s.byte.byte0;
								if(Vheat_flag==1)
								{					
										dheatv=H/60.0;//计算体积
										HVsave += dheatv;
										HEAT_BACK_V = round(HVsave);//取出值
										Vheat_flag=0;						
										flow_v.data=HEAT_BACK_V;//拆分并写入结构体
										infoS.HEATV_5.byte3=flow_v.byte.byte3;
										infoS.HEATV_5.byte2=flow_v.byte.byte2;
										infoS.HEATV_5.byte1=flow_v.byte.byte1;
										infoS.HEATV_5.byte0=flow_v.byte.byte0;
								
								}
				    }
			    }			
//				  rx_len2 = 0;//清除计数
//				  HAL_UART_Receive_DMA(&huart2,rx_buffer2,BUFFER_SIZE);//重新打开DMA接
					memset(rx_buffer2,0,rx_len2);//清空数组					
				  rx_len2 = 0;//清除计数
				  HAL_UART_Receive_DMA(&huart2,rx_buffer2,BUFFER_SIZE);//重新打开DMA接
					__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE); //开启IDLE中断
				
			}  
  }
  /* USER CODE END MFC_RX_Task */
}

/* USER CODE BEGIN Header_RS485_srevice_Task */
/**
* @brief Function implementing the RS485_srevice thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_RS485_srevice_Task */
void RS485_srevice_Task(void const * argument)
{
  /* USER CODE BEGIN RS485_srevice_Task */
		BaseType_t  err;
		BaseType_t  tem_err;
		BaseType_t  val_err;
		BaseType_t  mfc_err;
    uint16_t tem_state = 0;
		uint16_t value_state = 0;
		uint16_t mfc_state = 0;		
	 
	 ////aes bin
		CRC32_CTX ctx = { 0 };
		unsigned char crc32[4] = { 0 };
	 ////
	/* Infinite loop */
  for(;;)
  {
    if(uart1_SemHandle!=NULL)
		{
			err=xSemaphoreTake(uart1_SemHandle,portMAX_DELAY); //获取信号量
			
			if(err==pdTRUE ) //数据长度正确
			{
					while(fifo_find_frame1(&uart1_rec,rx_buffer,14,&rx_len))
				{				
									
				
			    if(rx_buffer[0] == 0x7E && rx_buffer[1] == 0x01 && rx_buffer[2] == 0x09 && rx_buffer[3] == 0x60 && rx_buffer[4] == 0x01 && rx_len == 14)//起始位，版本号，设备id，数据长度正确
					{
							//printf("rec = %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %x \r\n",rx_buffer[0],rx_buffer[1],rx_buffer[2],rx_buffer[3],rx_buffer[4],rx_buffer[5],rx_buffer[6],rx_buffer[7],rx_buffer[8],rx_buffer[9],rx_buffer[10],rx_buffer[11],rx_buffer[12],rx_buffer[13]);
							if(rx_buffer[5] == 02)//帧类型为命令数据
							{											
											if(rx_buffer[rx_len-2] == calsum((char *)(rx_buffer+1),rx_buffer[2]+2))//rx_buffer[RS485_RX_CNT-2] == calsum(rx_buffer,RS485_RX_CNT)
											{
													
													switch(rx_buffer[7])//根据不同的功能码进行处理
													{													
															case 0x01://设置阀1状态
															{
																			printf("set v1\r\n");
																			
																			if(rx_buffer[2+rx_buffer[2]]==0x01)
																			{
																				//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_SET);
																				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);
																				infoS.V1_9.byte0=1;
																			}
																			else if(rx_buffer[2+rx_buffer[2]]==0x00)
																			{
																				//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6,GPIO_PIN_RESET);
																				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);
																				infoS.V1_9.byte0=0;
																			}																			
																			command[0][8]=0x00;
																			command[0][9]=calsum((char*)(command[0]+1),command[0][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[0], sizeof(command[0]));
																			break;
															}

															case 0x02://设置阀2状态
															{
																			printf("set v2\r\n");
																			//printf("设置阀2状态 \r\n");
																			if(rx_buffer[2+rx_buffer[2]]==0x01)
																			{
																				//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_SET);
																				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);
																				infoS.V2_A.byte0=1;
																			}
																				
																			else if(rx_buffer[2+rx_buffer[2]]==0x00)
																			{
																				//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7,GPIO_PIN_RESET);
																				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);
																				infoS.V2_A.byte0=0;
																			}
																			
																			command[1][8]=0x00;
																			command[1][9]=calsum((char*)(command[1]+1),command[1][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[1], sizeof(command[1]));
																			break;
															}                              
															
															case 0x03: //设置阀3状态
															{
																			printf("set v3\r\n");
																			//printf("设置阀3状态 \r\n");
																			if(rx_buffer[2+rx_buffer[2]]==0x01)
																			{
//																				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_SET);
																				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_SET);
																				infoS.V3_B.byte0=1;
																			}
																			else if(rx_buffer[2+rx_buffer[2]]==0x00)
																			{
//																				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8,GPIO_PIN_RESET);
																				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);
																				infoS.V3_B.byte0=0;
																			}
																			
																			command[2][8]=0x00;
																			command[2][9]=calsum((char*)(command[2]+1),command[2][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[2], sizeof(command[2]));
																			break;
															} 

															 case 0x04: //设置阀4状态
															{
																			printf("set v4\r\n");														
																			if(rx_buffer[2+rx_buffer[2]]==0x01)
																			{
																				//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_SET);
																				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_SET);
																				infoS.V4_C.byte0=1;
																			}
																			else if(rx_buffer[2+rx_buffer[2]]==0x00)
																			{
																				//HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9,GPIO_PIN_RESET);
																				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3,GPIO_PIN_RESET);
																				infoS.V4_C.byte0=0;
																			}
																			command[3][8]=0x00;
																			command[3][9]=calsum((char*)(command[3]+1),command[3][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[3], sizeof(command[3]));
																			break;
															}

															case 0x05://设置阀5状态 备用
															{
																			printf("set v5\r\n");																			
																			command[4][8]=0x00;
																			command[4][9]=calsum((char*)(command[4]+1),command[4][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[4], sizeof(command[4]));
																			break;
															}

															case 0x06: //设置阀6状态 备用
															{
																				printf("set v5\r\n");																		
																				command[5][8]=0x00;
																				command[5][9]=calsum((char*)(command[5]+1),command[5][2]+2);
																				HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[5], sizeof(command[5]));
																			
																			break;
															}

															case 0x07://设置泵状态                               
															{
																			printf("beng\r\n");
																			//printf("rec = %02X %02X %02X \r\n",rx_buffer[1+rx_buffer[2]],rx_buffer[2+rx_buffer[2]],rx_buffer[3+rx_buffer[2]]);
																			if(rx_buffer[2+rx_buffer[2]]==0x01)
																			{
																				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_SET);
																				infoS.Pump_F.byte0=1;
																			}
																			else if(rx_buffer[2+rx_buffer[2]]==0x00)
																			{
																				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4,GPIO_PIN_RESET);
																				infoS.Pump_F.byte0=0;
																			}
																			command[6][8]=0x00;
																			command[6][9]=calsum((char*)(command[6]+1),command[6][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[6], sizeof(command[6]));	
																
																			break;
															}
															case 0x08://设置GCMS                               
															{
																		
																				if(rx_buffer[2+rx_buffer[2]]==0x01)
																			{
																				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_SET);	//电平拉高 PD12

																			}
																			else if(rx_buffer[2+rx_buffer[2]]==0x00)
																			{
																				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_RESET);//电平拉低	PD12
																			}																			
																			command[7][8]=0x00;
																			command[7][9]=calsum((char*)(command[7]+1),command[7][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[7], sizeof(command[7]));
																		
																
																			break;
															}
															case 0x09://设置六位阀
															{
																	if(rx_buffer[2+rx_buffer[2]]==0x01)
																	{
																				printf("1vale6\r\n");	
																				//vale1_flag = 1;
																				value_state = 1;
																				val_err = xQueueSend(Value_QueueHandle,&value_state,0); 
																				if(val_err != pdPASS )
																					{
																							printf( "Could not send to the queue,%d\r\n", value_state);
																					}
																				infoS.Va6_10.byte0=1;
																				command[8][8]=0x00;
																	}
																	else if(rx_buffer[2+rx_buffer[2]]==0x02)
																  {
																				printf("2vale6\r\n");	
																				//vale2_flag = 1;
																				value_state = 2;
																				val_err = xQueueSend(Value_QueueHandle,&value_state,0); 
																				if(val_err != pdPASS )
																					{
																							printf( "Could not send to the queue,%d\r\n", value_state);
																					}
																				infoS.Va6_10.byte0=2;
																				command[8][8]=0x00;
																	}	
																	else if(rx_buffer[2+rx_buffer[2]]==0x03)
																	{
																				printf("3vale6\r\n");	
																				//vale3_flag = 1;
																				value_state = 3;
																				val_err = xQueueSend(Value_QueueHandle,&value_state,0); 
																				if(val_err != pdPASS )
																					{
																							printf( "Could not send to the queue,%d\r\n", value_state);
																					}
																				infoS.Va6_10.byte0=3;
																				command[8][8]=0x00;
																	}
																	else if(rx_buffer[2+rx_buffer[2]]==0x04)
																	{
																				printf("4vale6\r\n");	
																				//vale4_flag = 1;
																				value_state = 4;
																				val_err = xQueueSend(Value_QueueHandle,&value_state,0); 
																				if(val_err != pdPASS )
																					{
																							printf( "Could not send to the queue,%d\r\n", value_state);
																					}
																				infoS.Va6_10.byte0=3;
																				command[8][8]=0x00;
																				infoS.Va6_10.byte0=4;
																				command[8][8]=0x00;
																	}
																	else if(rx_buffer[2+rx_buffer[2]]==0x05)
																	{
																		    printf("5vale6\r\n");	
																				//vale5_flag = 1;
																				value_state = 5;
																				val_err = xQueueSend(Value_QueueHandle,&value_state,0); 
																				if(val_err != pdPASS )
																					{
																							printf( "Could not send to the queue,%d\r\n", value_state);
																					}
																				infoS.Va6_10.byte0=5;	
																				command[8][8]=0x00;
																	}
																	else if(rx_buffer[2+rx_buffer[2]]==0x06)
																	{
																				printf("6vale6\r\n");	
																				//vale6_flag = 1;
																				value_state = 6;
																				val_err = xQueueSend(Value_QueueHandle,&value_state,0); 
																				if(val_err != pdPASS )
																					{
																							printf( "Could not send to the queue,%d\r\n", value_state);
																					}
																				infoS.Va6_10.byte0=6;
																				command[8][8]=0x00;
																	}
																	else 
																	{
																				command[8][8]=0x01;
																	}																			
																	command[8][9]=calsum((char*)(command[8]+1),command[8][2]+2);
																	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[8], sizeof(command[8]));
																	break;
															}  
															case 0x0A://设置十二通阀
															{
																	if(rx_buffer[11]==0x01&&rx_buffer[8]==0x00&&rx_buffer[9]==0x00&&rx_buffer[10]==0x00)
																	{
																				printf("Avale12\r\n");
																				//valeA_flag = 1;
																				value_state = 11;
																				val_err = xQueueSend(Value_QueueHandle,&value_state,0); 
																				if(val_err != pdPASS )
																					{
																							printf( "Could not send to the queue,%d\r\n", value_state);
																					}
																				infoS.Va12_11.byte0=1;
																				command[9][8]=0x00;
																	}
																 else if(rx_buffer[11]==0x02&&rx_buffer[8]==0x00&&rx_buffer[9]==0x00&&rx_buffer[10]==0x00)
																 {
																				printf("Bvale12\r\n");
																				//valeB_flag = 1;
																				value_state = 12;
																				val_err = xQueueSend(Value_QueueHandle,&value_state,0); 
																				if(val_err != pdPASS )
																					{
																							printf( "Could not send to the queue,%d\r\n", value_state);
																					}
																				infoS.Va12_11.byte0=2;
																				command[9][8]=0x00;
																	}
																	else 
																  {
																				command[9][8]=0x01;
																	}
																	command[9][9]=calsum((char*)(command[9]+1),command[9][2]+2);
																	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[9], sizeof(command[9]));
																			
																	break;
															}  
															case 0x0B://设置FID除水温度
															{
																			printf("set t_fig\r\n");
																			FID_T_SET[0] = rx_buffer[10];
																			FID_T_SET[1] = rx_buffer[11];																
																			//Host_write06_slave_4(0x01,0X06,0x0000,(uint16_t*)FID_T_SET);//发送第一路数据
																			//STfid_flag = 1;
//																				tem_state = 1;
//																				tem_err = xQueueSend(TEM_QueueHandle,&tem_state,0); 
//																				if(tem_err != pdPASS )
//																					{
//																							printf( "Could not send to the tem_queue,%c\r\n", tem_state);
//																					}
																			TEM_SET.FID_T = (FID_T_SET[0]<<8) | FID_T_SET[1];
																			if(TEM_SET.FID_T<2000)
																				{
																					//pid1.Sv = TEM_SET.FID_T*0.1;		//取出真实值
																					pid1.ref = TEM_SET.FID_T*0.1;		//取出真实值
																				}
																			else
																				{
																					pid1.ref = -160.0;
																				}
																			
																			command[10][8]=0x00;
																			command[10][9]=calsum((char*)(command[10]+1),command[10][2]+2);
																			//HAL_UART_Transmit(&huart1, (uint8_t *)command[10], sizeof(command[10]),0xFFFF);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[10], sizeof(command[10]));

																			//printf("设置FID除水温度 \r\n");
																			break;
															}  

															case 0x0C://设置MS除水温度
															{
																			printf("t_ms");
																			//MS_T_SET = (rx_buffer[8]*256*256*256+rx_buffer[9]*256*256+rx_buffer[10]*256+rx_buffer[11])*10;
																			MS_T_SET[0] = rx_buffer[10];
																			MS_T_SET[1] = rx_buffer[11];																				
																			//STms_flag = 1;
//																			tem_state = 2;
//																			tem_err = xQueueSend(TEM_QueueHandle,&tem_state,0); 
//																			if(tem_err != pdPASS )
//																				{
//																						printf( "Could not send to the tem_queue,%c\r\n", tem_state);
//																				}
																			TEM_SET.MS_T = (MS_T_SET[0]<<8) | MS_T_SET[1];
																			if(TEM_SET.MS_T<2000)
																					{
//																						pid3.Sv = TEM_SET.MS_T*0.1;		//取出真实值
																						pid2.ref = TEM_SET.MS_T*0.1;		//取出真实值
																					}
																				else
																					{
//																						pid3.Sv = -160.0;
																						pid2.ref = -160.0;
																					}
																			
																			command[11][8]=0x00;
																			command[11][9]=calsum((char*)(command[11]+1),command[11][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[11], sizeof(command[11]));
																			//printf("设置MS除水温度 \r\n");
																			break;
															}  

															case 0x0D://设置捕集管温度
															{
																			printf("t_cap");
																			//TRAP_T_SET = (rx_buffer[8]*256*256*256+rx_buffer[9]*256*256+rx_buffer[10]*256+rx_buffer[11])*10;
																			TRAP_T_SET[0] = rx_buffer[10];
																			TRAP_T_SET[1] = rx_buffer[11];																
																			//Host_write06_slave_4(0x01,0X06,0x0002,(uint16_t*)TRAP_T_SET);//发送第三路数据
																			//STcap_flag = 1;
//																			tem_state = 3;
//																			tem_err = xQueueSend(TEM_QueueHandle,&tem_state,0); 
//																			if(tem_err != pdPASS )
//																				{
//																						printf( "Could not send to the tem_queue,%c\r\n", tem_state);
//																				}
																			TEM_SET.TRAP_T = (TRAP_T_SET[0]<<8) | TRAP_T_SET[1];
																			if(TEM_SET.TRAP_T<2000)
																					{
																						pid3.ref = TEM_SET.TRAP_T*0.1;		//取出真实值
																					}
																				else
																					{
																						pid3.ref = -160.0;
																					}
																			
																			command[12][8]=0x00;
																			command[12][9]=calsum((char*)(command[12]+1),command[12][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[12], sizeof(command[12]));
																			//printf("设置捕集管温度 \r\n");
																			break;
															}  

															case 0x0E://设置备用温度 备用
															{
																			printf("t_beiyong");
																			BY_T_SET[0] = rx_buffer[10];
																			BY_T_SET[1] = rx_buffer[11];
																			//BY_T_SET = (rx_buffer[8]*256*256*256+rx_buffer[9]*256*256+rx_buffer[10]*256+rx_buffer[11])*10;
																			//Host_write06_slave_4(0x01,0X06,0x0003,(uint16_t*)BY_T_SET);//发送第四路数据
																			//STdil_flag = 1;
																			command[13][8]=0x00;
																			command[13][9]=calsum((char*)(command[13]+1),command[13][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[13], sizeof(command[13]));
																			//printf("设置备用温度 备用 \r\n");
																			break;
															}  

															case 0x0F://设置FID采样流量
															{
																			//printf("v_fid");
																			__IO uint16_t fv;
																			__IO uint8_t ABC[2]={0};
																			//FID_F_SET = (rx_buffer[8]*256*256*256+rx_buffer[9]*256*256+rx_buffer[10]*256+rx_buffer[11])*10;
																			//FID_F_SET[0] = rx_buffer[10];
																			//FID_F_SET[1] = rx_buffer[11];
																			fid[0] = rx_buffer[10];
																			fid[1] = rx_buffer[11];
																			//RSfid_flag = 1;	
																			mfc_state = 1;
																			mfc_err = xQueueSend(MFC_QueueHandle,&mfc_state,0); 
																			if(mfc_err != pdPASS )
																				{
																						printf( "Could not send to the mfcqueue,%d\r\n", mfc_state);
																				}																			
																			command[14][8]=0x00;
																			command[14][9]=calsum((char*)(command[14]+1),command[14][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[14], sizeof(command[14]));
																			//printf("设置FID采样流量 \r\n");
																			break;
															}  

															case 0x10://设置MS采样流量
															{
																			//printf("v_ms");
																			//MS_F_SET = (rx_buffer[8]*256*256*256+rx_buffer[9]*256*256+rx_buffer[10]*256+rx_buffer[11])*10;
																			__IO uint16_t mv;
																			__IO uint8_t ABC[2]={0};
																			//MS_F_SET[0] = rx_buffer[10];
																			//MS_F_SET[1] = rx_buffer[11];
																			ms[0] = rx_buffer[10];
																			ms[1] = rx_buffer[11];
																			//RSms_flag = 1;
																			mfc_state = 2;
																			mfc_err = xQueueSend(MFC_QueueHandle,&mfc_state,0); 
																			if(mfc_err != pdPASS )
																				{
																						printf( "Could not send to the mfcqueue,%d\r\n", mfc_state);
																				}			
																			command[15][8]=0x00;
																			command[15][9]=calsum((char*)(command[15]+1),command[15][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[15], sizeof(command[15]));
																			//printf("设置MS采样流量 \r\n");
																			break;
															}  	

															case 0x11://设置加热反吹流量
															{
																			//printf("v_heat");
																			//HEAT_BACK_F_SET = (rx_buffer[8]*256*256*256+rx_buffer[9]*256*256+rx_buffer[10]*256+rx_buffer[11])*10;
																			__IO uint16_t hv;
																			__IO uint8_t ABC[2]={0};
																			//HEAT_BACK_F_SET[0] = rx_buffer[10];
																			//HEAT_BACK_F_SET[1] = rx_buffer[11];
																			heat[0] = rx_buffer[10];
																			heat[1] = rx_buffer[11];
																			//RSheat_flag = 1;
																			mfc_state = 3;
																			mfc_err = xQueueSend(MFC_QueueHandle,&mfc_state,0); 
																			if(mfc_err != pdPASS )
																				{
																						printf( "Could not send to the mfcqueue,%d\r\n", mfc_state);
																				}	
																			command[16][8]=0x00;
																			command[16][9]=calsum((char*)(command[16]+1),command[16][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[16], sizeof(command[16]));
																			//printf("设置加热反吹流量 \r\n");
																			break;
															}  

															case 0x12://设置MFC备用流量 备用
															{
																			//printf("v_beiyong");
																			//HAL_GPIO_WritePin(GPIOG, GPIO_PIN_8 , GPIO_PIN_SET);
																			//HAL_Delay(1);
																			//printf("设置MFC备用流量 备用 \r\n");
																			//BY_MFC_F_SET[0] = rx_buffer[10];
																			//BY_MFC_F_SET[1] = rx_buffer[11];
																			hty[0] = rx_buffer[10];
																			hty[1] = rx_buffer[11];																			
																			//Host_write06_slave(0x00,0X06,0x0011,(uint16_t*)BY_MFC_F_SET);
																
																			//Host_write06_slave(0x02,0X06,0x0011,(uint16_t*)BY_MFC_F_SET);
																			//Host_write06_slave(0x04,0X06,0x0011,(uint16_t*)BY_MFC_F_SET);
																			command[17][8]=0x00;
																			command[17][9]=calsum((char*)(command[17]+1),command[17][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[17], sizeof(command[17]));
																			//Host_write06_slave(0x04,0X06,0x0011,(uint16_t*)BY_MFC_F_SET);
																			//BY_MFC_F_SET = (rx_buffer[8]*256*256*256+rx_buffer[9]*256*256+rx_buffer[10]*256+rx_buffer[11])*10;
																			//printf("value=%u\r\n",BY_MFC_F_SET);
																			//Host_Read03_slave(0x01,0x000A,0x0001);
																			//Host_write06_slave(0x01,0x06,0x000A,0x0200);
																			break;
															}  
															case 0x13://制冷机组
															{

																			if(rx_buffer[2+rx_buffer[2]]==0x01)
																			{
																				//printf("开启制冷机\r\n");
																				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14,GPIO_PIN_SET);
																			}
																			else if(rx_buffer[2+rx_buffer[2]]==0x00)
																			{
																				//printf("关闭制冷机\r\n");
																				HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14,GPIO_PIN_RESET);
																			}
																			command[18][8]=0x00;
																			command[18][9]=calsum((char*)(command[18]+1),command[18][2]+2);
																			HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[18], sizeof(command[18]));
																			break;
															}  
															case 	0x14:
															{
//																			__set_PRIMASK(1);          //关总中断
//																			//printf("aaa\r\n");
//																			int ret,i;
//																			output1 = rx_buffer6;
//																			//printf("resrt");
//																			mbedtls_aes_init(&aes);
//																			mbedtls_aes_setkey_dec(&aes, key, 128);
//																			rx_len_mid = rx_len6;
//																			//mbedtls_aes_crypt_cbc(&aes, 0, rx_len6, iv1, &rx_buffer6[0], &decode[0]);
//																				while (rx_len_mid > 0)
//																						{
//																						//memcpy(temp_code, output1, 16);
//																						for(i=0;i<16;i++)
//																							temp_code[i] = output1[i];
//																						//memcpy(mid, output1, 16);
//																						for(i=0;i<16;i++)
//																							mid[i] = output1[i];
//																						ret = mbedtls_aes_crypt_ecb(&aes, 0, mid, output1);
//																						if (ret == 0)	
//																						{
//																							for(i = 0; i < 16; i++)
//																								output1[i] = (unsigned char)(output1[i] ^ iv1[i]);
//																							//memcpy(iv1, temp_code, 16);
//																						for(i=0;i<16;i++)
//																							iv1[i] = temp_code[i];
//																							output1 += 16;
//																							//output += 16;
//																							rx_len_mid -= 16;
//																						}
//																						ret = 0;
//																						}	
//																			decode_len = rx_buffer6[rx_len6-1];									
//																			if(((*(__IO uint32_t*)(0X20001000+4))&0xFF000000)==0x08000000)//判断是否为0X08XXXXXX.
//																			{	 
//																				CRC32_Init(&ctx);
//																				for(i = 0 ;i< rx_len6-260-decode_len ; i=i+256)
//																				{
//																					CRC32_Update(&ctx, &rx_buffer6[i], 256);
//																				}
//																				CRC32_Update(&ctx, &rx_buffer6[i], rx_len6-i-4-decode_len);
//																				CRC32_Final(&ctx, crc32);
//															//					crc32[0] =  (ctx.crc & 0xFF000000UL) >> 24;
//															//					crc32[1] =  (ctx.crc & 0x00FF0000UL) >> 16;
//															//					crc32[2] =  (ctx.crc & 0x0000FF00UL) >> 8;
//															//					crc32[3] =  (ctx.crc & 0x000000FFUL);
//																				i=0;
//																				if(crc32[0]==rx_buffer6[rx_len6-1-decode_len] && crc32[1]==rx_buffer6[rx_len6-2-decode_len] && crc32[2]==rx_buffer6[rx_len6-3-decode_len] && crc32[3]==rx_buffer6[rx_len6-4-decode_len] )
//																				//uwExpectedCRCValue = HAL_CRC_Calculate(&hcrc, (uint32_t *)abc, 16);
//																				{
//																					Startup_Update[0] = rx_len6-4-decode_len;
//																					Startup_Update[1]=0XAAAAAAAA;
////																					printf("******************************************************\r\n");
////																					printf("                       CRC pass!                      \r\n");
////																					printf("******************************************************\r\n");
////																					printf("                 Firmware update completed!           \r\n");
////																					printf("******************************************************\r\n\r\n");
//																					iap_write_appbin(FLASH_APP2_ADDR,rx_buffer6,rx_len6-4-decode_len);//更新FLASH代码			
//																					STMFLASH_Write(0x080FFFF8,(uint32_t *)Startup_Update,2);																				
//																				}						
//																				else
//																				{
////																					printf("******************************************************\r\n");
////																					printf("                       CRC error!                     \r\n");
////																					printf("******************************************************\r\n\r\n");
//																				//HAL_UART_DeInit(&huart1);
//																				}
//																			}
//																			else
//																				printf("bin error");
//																			__set_PRIMASK(0);          //关总中断
//																			SoftReset();	
//																			break;
															}
																														 
													}
													//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
											}//CRC判断
										
											else//CRC校验错误
											{
												
													RS485_TX_BUFF[0]=rx_buffer[0];
													RS485_TX_BUFF[1]=rx_buffer[1] | 0x80;
													RS485_TX_BUFF[2]=0x04; //异常码													
											}                                                    

							}//功能码判断
							

						}
						else if(rx_buffer[0] == 0x7E && rx_buffer[1] == 0x01 && rx_buffer[2] == 0x05 && rx_buffer[3] == 0x60 && rx_buffer[4] == 0x01 && rx_len == 10 )//起始位，版本号，设备id正确
						{
							if(rx_buffer[5] == 02)//帧类型为命令数据
								{
												if(rx_buffer[rx_len-2] == calsum((char *)(rx_buffer+1),rx_buffer[2]+2))//rx_buffer[RS485_RX_CNT-2] == calsum(rx_buffer,RS485_RX_CNT)
												{
														
														switch(rx_buffer[7])//根据不同的功能码进行处理
														{													
																case 0x00://发送状态数据
																{
																				ptrCSInfo ptrInfoS =  &infoS;
																				uint8_t infoArray[INFOSIZE]={0};
																				CSInfo_2Array_uint8(ptrInfoS,infoArray);//打包数据
																				infoArray[INFOSIZE-2]=calsum((char*)(infoArray+1),infoArray[2]+2);//填入校验和
																				HAL_UART_Transmit_DMA(&huart1, (uint8_t *)infoArray, 123);//发送
																				//tx_flag = 0;
																				break;
																}
																case 0x08://GCMS触发
																{
																				printf("GCMS \r\n");
																				//Host_Read03_slave(0x00,0x0033,0x0001);
																				if(rx_buffer[2+rx_buffer[2]]==0x01)
																				{
																					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_SET);	//电平拉高

																				}
																				else if(rx_buffer[2+rx_buffer[2]]==0x00)
																				{
																					HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12,GPIO_PIN_RESET);//电平拉低
																				}

																				command[7][8]=0x00;
																				command[7][9]=calsum((char*)(command[7]+1),command[7][2]+2);
																				HAL_UART_Transmit_DMA(&huart1, (uint8_t *)command[7], sizeof(command[7]));
																				break;            
																}																																 
													}

												}//CRC判断
												else//CRC校验错误
												{
														//printf("CRC错误\n");
														RS485_TX_BUFF[0]=rx_buffer[0];
														RS485_TX_BUFF[1]=rx_buffer[1] | 0x80;
														RS485_TX_BUFF[2]=0x04; //异常码
														//RS485_respond_err(RS485_TX_BUFF,3);
												}                                                    

								}//功能码判断
						}
				}
					
			}
			
				//rx_len = 0;
		   // memset(rx_buffer,0x00,sizeof(rx_buffer)); //清空数组 
				//rx_buffer[0] = 0;
		    HAL_UART_Receive_DMA(&huart1,usart1_rx_buffer,BUFFER_SIZE);//重新使能接收
				__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //开启IDLE中断
				
				
		}
  }
  /* USER CODE END RS485_srevice_Task */
}

/* USER CODE BEGIN Header_MFC_TX_Task */
/**
* @brief Function implementing the MFC_TX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_MFC_TX_Task */
void MFC_TX_Task(void const * argument)
{
  /* USER CODE BEGIN MFC_TX_Task */
	BaseType_t err=pdFALSE;
	uint16_t mfc_state;
	int wait_times = 2;//防止过冲等待次数 等待时间 = 阻塞时间*wait 
  /* Infinite loop */
  for(;;)
  {
    err = xQueueReceive( MFC_QueueHandle,&mfc_state,500);//读队列，无数据阻塞
			if(fid_c != 0)
				fid_c--;
			if(ms_c != 0)
				ms_c--;
			if(heat_c != 0)
				heat_c--;
			if(err != pdTRUE)//队列无数据
			{
				printf("mfc_queue队列无数据，发送查询数据\r\n");
				if(fid_c == 0)
				{
					Host_Read03_slave(0x00,0x0010,0x0001);//查询气体流量
					Vfid_flag=1;
					Vfid_flag_warning=1;
					vTaskDelay(100);
				}
				if(ms_c == 0)
				{
					Host_Read03_slave(0x02,0x0010,0x0001);
					Vms_flag=1;
					Vms_flag_warning=1;
					vTaskDelay(100);//等待回复
				}
				if(heat_c == 0)
				{
					Host_Read03_slave(0x04,0x0010,0x0001);
					Vheat_flag=1;
					Vheat_flag_warning=1;
					vTaskDelay(100);
				}
			}
			else
			{
				printf("mfc_queue读取队列成功，数据：%d\r\n",mfc_state);
				switch(mfc_state)
				{
					case 1://设置FID
					{
						__IO uint16_t fv;
						__IO uint8_t ABC[2]={0};
						fv =	(fid[0] << 8) | fid[1];//计算流速
						fv = fv*4095/500;						 //换算成刻度					
						flow.data = fv;
						ABC[1]=flow.byte.byte0;	//取高位
						ABC[0]=flow.byte.byte1;	//取低位
						Host_write06_slave(0x00, 0X06, 0x0011, (uint16_t*)ABC);
						fid_c =wait_times;
						//RSfid_flag = 0;
						vTaskDelay(100);
						break;
					}
					case 2:
					{
					  __IO uint16_t mv;
						__IO uint8_t ABC[2]={0};
						mv =	ms[0]*256+ms[1];//计算流速
						mv = mv*4095/500;		//换算成刻度
						flow.data = mv;
						ABC[1]=flow.byte.byte0;
						ABC[0]=flow.byte.byte1;
						Host_write06_slave(0x02, 0X06, 0x0011, (uint16_t*)ABC);
						ms_c = wait_times;
						//RSms_flag = 0;
						vTaskDelay(100);
						break;
					}
					case 3:
					{
						__IO uint16_t hv;
						__IO uint8_t ABC[2]={0};
						hv =	heat[0]*256+heat[1];//计算流速
						//hv =	hv*4095/500;	//老量程50ml
						hv =	hv*4095/10000;	//新量程1000ml					
						flow.data = hv;		
						ABC[1]=flow.byte.byte0;
						ABC[0]=flow.byte.byte1;
						Host_write06_slave(0x04, 0X06, 0x0011, (uint16_t*)ABC);
						heat_c = wait_times;
						//RSheat_flag = 0;
						vTaskDelay(100);
						break;
					}
					case 4:
					{
					break;
					}
					default:
					{
					break;
					}
				
				}
			}
			xEventGroupSetBits(IWDGEventGroup, TASK_MFCTX_BIT_1);//标志位置三
			//printf("mfc iwdg! \r\n");
  }
  /* USER CODE END MFC_TX_Task */
}

/* USER CODE BEGIN Header_Value_TX_Task */
/**
* @brief Function implementing the Value_TX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Value_TX_Task */
void Value_TX_Task(void const * argument)
{
  /* USER CODE BEGIN Value_TX_Task */
	uint16_t val_state = 0;
  BaseType_t err = 0;
	
  /* Infinite loop */
  for(;;)
  {
      err = xQueueReceive( Value_QueueHandle,&val_state,500); //读队列，无数据阻塞0.5s
      if(err != pdTRUE)	//无数据
      {
					HAL_UART_Transmit_DMA(&huart3, (uint8_t *)ask12, sizeof(ask12));
					vTaskDelay(50);
					HAL_UART_Transmit_DMA(&huart3, (uint8_t *)ask6, sizeof(ask6));
					vTaskDelay(50);
      }
		  else //队列中有数据
      {
				  switch(val_state)
					{
						case 1:
						{
							 printf("value:1\r\n");
							 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&vale1_move, strlen(vale1_move));
							 break;
						}
						case 2:
						{
							 printf("value:2\r\n");
							 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&vale2_move, strlen(vale2_move));
							 break;
						}
						case 3:
						{
							 printf("value:3\r\n");
							 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&vale3_move, strlen(vale3_move));
							 break;
						}
						case 4:
						{
							 printf("value:4\r\n");
							 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&vale4_move, strlen(vale4_move));
							 break;
						}
						case 5:
						{
							 printf("value:5\r\n");
							 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&vale5_move, strlen(vale5_move));
							 break;
						}
						case 6:
						{
							 printf("value:6\r\n");
							 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&vale6_move, strlen(vale6_move));
							 break;
						}
						case 11:
						{
							 printf("value:A\r\n");
							 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&vale12_moveTOA, strlen(vale12_moveTOA));
							 break;
						}
						case 12:
						{
							 printf("value:B\r\n");
							 HAL_UART_Transmit_DMA(&huart3, (uint8_t *)&vale12_moveTOB, strlen(vale12_moveTOB));
							 break;
						}
						default:break;
					}
      }
			xEventGroupSetBits(IWDGEventGroup, TASK_VALTX_BIT_3); //标志位置三
  }
  /* USER CODE END Value_TX_Task */
}

/* USER CODE BEGIN Header_Warning_Task */
/**
* @brief Function implementing the Warning thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Warning_Task */
void Warning_Task(void const * argument)
{
  /* USER CODE BEGIN Warning_Task */
	
  /* Infinite loop */
  for(;;)
  {
    // 报警信息
		if(Vfid_flag_W == 1)
		{
			 infoS.WARINING_18.byte0 = 0x06;
			 Vfid_flag_W = 0;
		}
		else if(Vms_flag_W == 1)
		{
			 infoS.WARINING_18.byte0 = 0x07;
			 Vms_flag_W = 0;
		}
		else if(Vheat_flag_W == 1)
		{
			 infoS.WARINING_18.byte0 = 0x08;
			 Vheat_flag_W = 0;
		}
		else if(Tfid_flag_W == 1)
		{
			 infoS.WARINING_18.byte0 = 0x03;
			 Tfid_flag_W = 0;
		}
		else if(Tms_flag_W == 1)
		{
			 infoS.WARINING_18.byte0 = 0x04;
			 Tms_flag_W = 0;
		}
		else if(Tcap_flag_W == 1)
		{
			 infoS.WARINING_18.byte0 = 0x05;
			 Tcap_flag_W = 0;
		}
		else 
		{
			 infoS.WARINING_18.byte0 = 0x00;
		}
		xEventGroupSetBits(IWDGEventGroup, TASK_WARNING_BIT_2); //标志位置四
		osDelay(1000);
  }
  /* USER CODE END Warning_Task */
}

/* USER CODE BEGIN Header_Value_RX_Task */
/**
* @brief Function implementing the Value_RX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Value_RX_Task */
void Value_RX_Task(void const * argument)
{
  /* USER CODE BEGIN Value_RX_Task */
	BaseType_t err = pdFALSE;
	
  /* Infinite loop */
  for(;;)
  {
     if(uart3_SemHandle != NULL)
		 {
				err = xSemaphoreTake(uart3_SemHandle,portMAX_DELAY);	// 等待串口3信号量
			  if(err == pdTRUE)
			  {
					 //printf("ValeR:L=%d,D=%02X %02X %02X %02X %02X %02X %02X %02X\r\n",rx_len3,rx_buffer3[0],rx_buffer3[1],rx_buffer3[2],rx_buffer3[3],rx_buffer3[4],rx_buffer3[5],rx_buffer3[6],rx_buffer3[7]);
					 if(rx_buffer3[0] == 0x2F && rx_buffer3[1] == 0x31 && rx_len3 == 20) // 六通阀
					 {
							 switch(rx_buffer3[18])
							 {
									case 0x31:infoS.Va6_10.byte0 = 1;break;
									case 0x32:infoS.Va6_10.byte0 = 2;break;
									case 0x33:infoS.Va6_10.byte0 = 3;break;
									case 0x34:infoS.Va6_10.byte0 = 4;break;
									case 0x35:infoS.Va6_10.byte0 = 5;break;
									case 0x36:infoS.Va6_10.byte0 = 6;break;
									default:  infoS.Va6_10.byte0 = 0;break;
							 }
					 }
					 else if(rx_buffer3[0] == 0x2F && rx_buffer3[1] == 0x33 && rx_len3 == 19) // 12通阀
					 {
							 if(rx_buffer3[16] == 0x41)
									infoS.Va12_11.byte0 = 1;
							 else if(rx_buffer3[16] == 0x42)
									infoS.Va12_11.byte0 = 2;
							 else
									infoS.Va12_11.byte0 = 0;
					 }
				}
				
				rx_len3       = 0;
        rx_buffer3[0] = 0;
				HAL_UART_Receive_DMA(&huart3,rx_buffer3,BUFFER_SIZE);
				__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
		 }	
  }
  /* USER CODE END Value_RX_Task */
}

/* USER CODE BEGIN Header_Tem_TX_Task */
/**
* @brief Function implementing the Tem_TX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tem_TX_Task */
void Tem_TX_Task(void const * argument)
{
  /* USER CODE BEGIN Tem_TX_Task */
	uint16_t tem_state = 0;
  BaseType_t err = 0;
	
  /* Infinite loop */
  for(;;)
  {
			err = xQueueReceive(TEM_QueueHandle,&tem_state,500);//读队列，无数据阻塞
      if(err != pdTRUE)
      {
//            printf("tem_queue队列无数据，发送查询数据\r\n");
//						Host_Read03_slave_4(0x01,0x010B,0x0001);//查询fid温度 第四路
//						Tfid_flag=1;
//						Tfid_flag_warning=1;
//						vTaskDelay(50);					
//						Host_Read03_slave_4(0x01,0x0109,0x0001);//查询ms温度
//						Tms_flag =1;
//						Tms_flag_warning =1;
//						vTaskDelay(50);
//						Host_Read03_slave_4(0x01,0x010A,0x0001);//查询捕集管温度
//						Tcap_flag=1;
//						Tcap_flag_warning=1;
//						vTaskDelay(50);					
      }
		  else //队列中有数据
      {
//            printf("tem_queue读取队列成功，数据：%d\r\n",tem_state);
//						switch(tem_state)
//						{
//							case 1:
//							{
//								Host_write06_slave_4(0x01,0X06,0x0003,(uint16_t*)FID_T_SET);//发送第四路数据
//								vTaskDelay(50);
//								break;
//							}
//							case 2:
//							{
//								Host_write06_slave_4(0x01,0X06,0x0001,(uint16_t*)MS_T_SET);//发送第二路数据
//								vTaskDelay(50);
//								break;
//							}
//							case 3:
//							{
//								Host_write06_slave_4(0x01,0X06,0x0002,(uint16_t*)TRAP_T_SET);//发送第三路数据
//								vTaskDelay(50);
//								break;
//							}
//							default:
//							{
//								break;
//							}
//						}
      }
  }
  /* USER CODE END Tem_TX_Task */
}

/* USER CODE BEGIN Header_Tem_RX_Task */
/**
* @brief Function implementing the Tem_RX thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Tem_RX_Task */
void Tem_RX_Task(void const * argument)
{
  /* USER CODE BEGIN Tem_RX_Task */
	BaseType_t err=pdFALSE;
	
  /* Infinite loop */
  for(;;)
  {
     if(uart4_SemHandle!=NULL) //接收完成标志
		 {
				err=xSemaphoreTake(uart4_SemHandle,portMAX_DELAY);//获取信号量
				if(err==pdTRUE) //获取信号量成功
				{								
	//				uint32_t tD=0;
					//判断有没有断联	
					//测量值
//					if(rx_buffer4[1] == 0x03 && rx_buffer4[0] == 0x01 && rx_buffer4[2] == 0x02) //前三位 01 03 02
//					{
//						//Tfid_flag,Tms_flag,Tcap_flag,Tdil_flag=0;
//						if(Tfid_flag==1)
//						{
//							Tfid_flag_warning = 0;
//							Tfid_flag=0;
//							FID_T[0] = rx_buffer4[3];
//							FID_T[1] = rx_buffer4[4];
//							tF=FID_T[0]*256+FID_T[1];//求得温度
//							//tF_printf  =  tF * 0.1;
//							if((FID_T[0] & 0x80 )== 0x80)//正负判断
//							{
//								infoS.FIDT_12.byte3=0xFF;
//								infoS.FIDT_12.byte2=0xFF;
//								infoS.FIDT_12.byte1=FID_T[0];
//								infoS.FIDT_12.byte0=FID_T[1];
//								
//							}
//							else if((FID_T[0] & 0x80 )== 0x00)
//							{
//								infoS.FIDT_12.byte3=0x00;
//								infoS.FIDT_12.byte2=0x00;
//								infoS.FIDT_12.byte1=FID_T[0];
//								infoS.FIDT_12.byte0=FID_T[1];														
//							}
//						}
//						else if(Tms_flag==1)
//						{
//							Tms_flag_warning = 0;
//							Tms_flag=0;
//							MS_T[0] = rx_buffer4[3];
//							MS_T[1] = rx_buffer4[4];
//							tM=MS_T[0]*256+MS_T[1];//求得温度
//							tF_printf  =  tM * 0.1;
//							if((MS_T[0] & 0x80 )== 0x80)
//							{
//								infoS.MST_13.byte3=0xFF;
//								infoS.MST_13.byte2=0xFF;
//								infoS.MST_13.byte1=MS_T[0];
//								infoS.MST_13.byte0=MS_T[1];
//							}
//							else if((MS_T[0] & 0x80 )== 0x00)
//							{
//								infoS.MST_13.byte3=0x00;
//								infoS.MST_13.byte2=0x00;
//								infoS.MST_13.byte1=MS_T[0];
//								infoS.MST_13.byte0=MS_T[1];
//							}
//						}
//						else if(Tcap_flag==1)
//						{
//							Tcap_flag_warning = 0;
//							Tcap_flag=0;
//							TRAP_T[0] = rx_buffer4[3];
//							TRAP_T[1] = rx_buffer4[4];
//							tC=TRAP_T[0]*256+TRAP_T[1];//求得温度
//							if((TRAP_T[0] & 0x80 )== 0x80)
//							{
//								infoS.CAPT_14.byte3=0xFF;
//								infoS.CAPT_14.byte2=0xFF;
//								infoS.CAPT_14.byte1=TRAP_T[0];
//								infoS.CAPT_14.byte0=TRAP_T[1];
//							}
//							else if((TRAP_T[0] & 0x80 )== 0x00)
//							{
//								infoS.CAPT_14.byte3=0x00;
//								infoS.CAPT_14.byte2=0x00;
//								infoS.CAPT_14.byte1=TRAP_T[0];
//								infoS.CAPT_14.byte0=TRAP_T[1];
//							}


//						}

//					}				
					memset(rx_buffer4,0,rx_len4);//清空数组
					rx_len4 = 0;//清除计数
					HAL_UART_Receive_DMA(&huart4,rx_buffer4,BUFFER_SIZE);//重新打开DMA接
					//__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //关闭IDLE中断
			  }
		 }
  }
  /* USER CODE END Tem_RX_Task */
}

/* USER CODE BEGIN Header_get_tem */
/**
* @brief Function implementing the get_temTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_get_tem */
int32_t duty = 0;
int32_t duty1 = 0;
int32_t duty2 = 0;
uint32_t delayms = 0;
float filtered_temp;
float alpha = 0.85;
uint32_t t1,t2,t3;
unsigned char divT = 10;
void Task_Periodic1(void const *argument)
{
    uint32_t last_wake_time = xTaskGetTickCount();  // 获取当前系统滴答计数
    static unsigned char div = 10;
    while (1)
    {
			// div = divT;
			kalmanProc(0);
			kalmanProc(1);
			kalmanProc(2);
			uint32_t period = temperatureU[0].periodMeter;
        // 周期性任务代码
        // ...
			t1 = xTaskGetTickCount();
			temperatureU[0].temperatureOri = TEM1.temp_tc;
			temperatureU[0].temperatureTarget = pid1.ref;

			temperatureU[1].temperatureOri = TEM2.temp_tc;
			temperatureU[1].temperatureTarget = pid2.ref;

			temperatureU[2].temperatureOri = TEM3.temp_tc;
			temperatureU[2].temperatureTarget = pid3.ref;
      if(temperatureU[0].div == 0)
			{
				controller(0); 	
			temperatureU[0].div = temperatureU[0].periodTask;				
			}
			else 
				temperatureU[0].div--;
			
      if(temperatureU[1].div == 0)
			{
			controller(1);
			temperatureU[1].div = temperatureU[1].periodTask;				
			}
			else 
				temperatureU[1].div--;
			
      if(temperatureU[2].div == 0)
			{
			controller(2); 	
			temperatureU[2].div = temperatureU[2].periodTask;				
			}
			else 
				temperatureU[2].div--;
			
			t2 = xTaskGetTickCount();
			t3 = t2 - t1;
        // 精确延时 100 个系统滴答（假设系统滴答为 1ms，则延时 100ms）
        osDelayUntil(&last_wake_time, period);
    }
}
void get_tem(void const * argument)
{
	/* USER CODE BEGIN get_tem */
//	pidInit();

	  controllerInit();
  /* Infinite loop */
  for(;;)
  {
		__IO  int32_t raw_temp = 0;
		__IO  int32_t raw_temp1 = 0;
		 uint8_t txData[3]={0x00};
		__IO uint8_t rxData[3]={0x00};
		__IO uint32_t FID,MS,HEAT;
		union int_bit32_data tem32;
		
		//读第一路温度
		TEM1.fault = Max31856_READ_Fault(cs1);
		osDelay(1);
		if(TEM1.fault != 0)
		{
			//printf("tem1:error,fault:%x \r\n",TEM1.fault);
			//后续加入保护措施，停止加热
			pid1.Sv   = -200;
      //pid2.Sv = -200;
      //pid3.Sv = -200;
			Tfid_flag_W = 1;

			infoS.FIDT_12.byte0=0x3c;
			infoS.FIDT_12.byte1=0xf6;
			infoS.FIDT_12.byte2=0xff;
			infoS.FIDT_12.byte3=0xff;
		}
		else
		{
			
			Max31856_READ_MODE(cs1,TEM1.mode);
			if(TEM1.mode[0] != 0xB0)
			{
				//printf("MODE1,ERROR\r\n");
				Max31856_SET_MODE(cs1,0xB0);
			}
			if(TEM1.mode[1] != 0x07)
			{
				//printf("MODE2,ERROR\r\n");
				Max31856_SET_TYPE(cs1,0x07);
			}
			
			TEM1.temp_tc = Get_Max31856_TC(cs1);
			osDelay(1);
			//FID = round(10 * TEM1.temp_tc);
			tem32.data = round(10 * TEM1.temp_tc);
			infoS.FIDT_12.byte0=tem32.byte.byte0;
			infoS.FIDT_12.byte1=tem32.byte.byte1;
			infoS.FIDT_12.byte2=tem32.byte.byte2;
			infoS.FIDT_12.byte3=tem32.byte.byte3;
			osDelay(1);
			TEM1.temp_cold = Get_Max31856_Cold(cs1);
			osDelay(1);
			//printf("tem1：tc= %0.2f,cold=%0.2f\r\n",TEM1.temp_tc,TEM1.temp_tc);
			if(TEM1.temp_tc < 200 )
			{
					//更新pid温度
				filtered_temp = alpha*TEM1.temp_tc + (1-alpha)*filtered_temp;
					pid1.fd = filtered_temp;//TEM1.temp_tc;	//更新pid温度
				
//					current_temp = TEM1.temp_tc;
//					target_temp = pid1.ref;

			}
			else
			{
				//过温报警
				printf("warning!tem1>200! \r\n");
				pid1.Sv   = -200;
        //pid2.Sv = -200;
        //pid3.Sv = -200;
				Tfid_flag_W = 1;
			
			}
		}
			//读第二路温度
		TEM2.fault = Max31856_READ_Fault(cs2);
		osDelay(1);
		if(TEM2.fault != 0)
		{
			//printf("tem2:error,fault:%x \r\n",TEM2.fault);
			//后续加入保护措施，停止加热
//				pid1.Sv = -200;
				pid2.Sv = -200;
//				pid3.Sv = -200;
				Vms_flag_W = 1;

		}
		else
		{
			Max31856_READ_MODE(cs2,TEM2.mode);
			if(TEM2.mode[0] != 0xB0)
			{
				//printf("MODE1,ERROR\r\n");
				Max31856_SET_MODE(cs2,0xB0);
			}
			if(TEM2.mode[1] != 0x07)
			{
				Max31856_SET_TYPE(cs2,0x07);
			}
			
			TEM2.temp_tc = Get_Max31856_TC(cs2);
			osDelay(1);
			tem32.data = round(10 * TEM2.temp_tc);//乘10 四舍五入
			infoS.MST_13.byte0=tem32.byte.byte0;
			infoS.MST_13.byte1=tem32.byte.byte1;
			infoS.MST_13.byte2=tem32.byte.byte2;
			infoS.MST_13.byte3=tem32.byte.byte3;
			TEM2.temp_cold = Get_Max31856_Cold(cs2);
			osDelay(1);
				if(TEM2.temp_tc < 200)
			{
					//更新pid温度
						pid2.fd = TEM2.temp_tc;	//更新pid温度

			}
			else
			{
				//过温报警
					printf("warning!tem2>200! \r\n");

					pid2.Sv = -200;
					Vms_flag_W = 1;

			}
		}
		
		//读第三路温度
		TEM3.fault = Max31856_READ_Fault(cs3);
		osDelay(1);
		if(TEM3.fault != 0)
		{
				pid3.Sv = -200;
				Tcap_flag_W = 1;
		}
		else
		{
			Max31856_READ_MODE(cs3,TEM3.mode);
			if(TEM3.mode[0] != 0xB0)
			{
				Max31856_SET_MODE(cs3,0xB0);
			}
			if(TEM3.mode[1] != 0x07)
			{
				Max31856_SET_TYPE(cs3,0x07);
			}
			
			TEM3.temp_tc = Get_Max31856_TC(cs3);
			osDelay(1);
			tem32.data = round(10 * TEM3.temp_tc);//乘10 四舍五入
			infoS.CAPT_14.byte0=tem32.byte.byte0;
			infoS.CAPT_14.byte1=tem32.byte.byte1;
			infoS.CAPT_14.byte2=tem32.byte.byte2;
			infoS.CAPT_14.byte3=tem32.byte.byte3;
			TEM3.temp_cold = Get_Max31856_Cold(cs3);
			osDelay(1);
			if(TEM3.temp_tc < 200)
			{
					//更新pid温度
					pid3.fd = TEM3.temp_tc;	//更新pid温度

			}
			else
			{
				printf("warning!tem2>200! \r\n");

				pid3.Sv = -200;
				Tcap_flag_W = 1;
			}
		}


		
		 //PID1_Calc();	//控制TEM1   对应V1  MS
 
		duty = (int32_t)pid1.out;
		//if(TEM1.temp_tc>pid1.ref) duty = 0;
//		if(TEM1.temp_tc > 200 ) duty = 0;
//		if(duty<0) duty = 0;
		
//		PID2_Calc();	//控制TEM2  对应V3   FID
//		duty1 = (int32_t)pid2.out;
//		if(TEM2.temp_tc>pid2.ref) duty1 = 0;
//		if(TEM2.temp_tc > 200 ) duty1 = 0;
//		if(duty1<0) duty1 = 0;
//		
//		PID3_Calc();	//控制TEM3  对应V2   捕集
//		duty2 = (int32_t)pid3.out;
//		if(TEM3.temp_tc>pid3.ref) duty2 = 0;
//		if(TEM3.temp_tc > 200 ) duty2 = 0;
//		if(duty2<0) duty2 = 0;
		
		xEventGroupSetBits(IWDGEventGroup, TASK_TEM_BIT_4);//标志位置四
		
		//__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,duty);
//		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_3,duty2);
//		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,duty1);
		 osDelay(delayms);
  }
	#if 0
  /* USER CODE BEGIN get_tem */
  /* Infinite loop */
  for(;;)
  {
		__IO  int32_t raw_temp = 0;
		__IO  int32_t raw_temp1 = 0;
		 uint8_t txData[3]={0x00};
		__IO uint8_t rxData[3]={0x00};
		__IO uint32_t FID,MS,HEAT;
		union int_bit32_data tem32;
		
		//读第一路温度
		TEM1.fault = Max31856_READ_Fault(cs1);
		osDelay(1);
		if(TEM1.fault != 0)
		{
			//printf("tem1:error,fault:%x \r\n",TEM1.fault);
			//后续加入保护措施，停止加热
			pid1.Sv   = -200;
      //pid2.Sv = -200;
      //pid3.Sv = -200;
			Tfid_flag_W = 1;
      //HAL_TIM_Base_Stop_IT(&htim2);//关闭PID中断 停止加热
      //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET); //停止加热
      //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET); //停止加热	
      //HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET); //停止加热
			infoS.FIDT_12.byte0=0x3c;
			infoS.FIDT_12.byte1=0xf6;
			infoS.FIDT_12.byte2=0xff;
			infoS.FIDT_12.byte3=0xff;
		}
		else
		{
			Max31856_READ_MODE(cs1,TEM1.mode);
			if(TEM1.mode[0] != 0xB0)
			{
				//printf("MODE1,ERROR\r\n");
				Max31856_SET_MODE(cs1,0xB0);
			}
			if(TEM1.mode[1] != 0x07)
			{
				//printf("MODE2,ERROR\r\n");
				Max31856_SET_TYPE(cs1,0x07);
			}
			
			TEM1.temp_tc = Get_Max31856_TC(cs1);
			osDelay(1);
			//FID = round(10 * TEM1.temp_tc);
			tem32.data = round(10 * TEM1.temp_tc);
			infoS.FIDT_12.byte0=tem32.byte.byte0;
			infoS.FIDT_12.byte1=tem32.byte.byte1;
			infoS.FIDT_12.byte2=tem32.byte.byte2;
			infoS.FIDT_12.byte3=tem32.byte.byte3;
			osDelay(10);
			TEM1.temp_cold = Get_Max31856_Cold(cs1);
			osDelay(1);
			//printf("tem1：tc= %0.2f,cold=%0.2f\r\n",TEM1.temp_tc,TEM1.temp_tc);
			if(TEM1.temp_tc < 200 )
			{
					//更新pid温度
					pid1.Pv = TEM1.temp_tc;	//更新pid温度
			}
			else
			{
				//过温报警
				printf("warning!tem1>200! \r\n");
				pid1.Sv   = -200;
        //pid2.Sv = -200;
        //pid3.Sv = -200;
				Tfid_flag_W = 1;
			  //后续加入保护措施，停止加热
				//HAL_TIM_Base_Stop_IT(&htim2);//关闭PID中断 全部停止加热
				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);    //停止加热
				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);    //停止加热	
				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);    //停止加热
				//infoS.FIDT_12.byte0=0x3c;
				//infoS.FIDT_12.byte1=0xf6;
				//infoS.FIDT_12.byte2=0xff;
				//infoS.FIDT_12.byte3=0xff;
			}
		}
			
		//printf("%0.1f,%0.1f\n",pid1.Pv,pid1.Sv);
		//if(pid1.Pv > pid1.Sv)
		//pid1.OUT = 1;

		//读第二路温度
		TEM2.fault = Max31856_READ_Fault(cs2);
		osDelay(1);
		if(TEM2.fault != 0)
		{
			//printf("tem2:error,fault:%x \r\n",TEM2.fault);
			//后续加入保护措施，停止加热
//				pid1.Sv = -200;
				pid2.Sv = -200;
//				pid3.Sv = -200;
				Vms_flag_W = 1;
//			HAL_TIM_Base_Stop_IT(&htim2);//关闭PID中断 停止加热
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);    //停止加热
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);    //停止加热	
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);    //停止加热
//				infoS.MST_13.byte0=0x3c;
//				infoS.MST_13.byte1=0xf6;
//				infoS.MST_13.byte2=0xff;
//				infoS.MST_13.byte3=0xff;
		}
		else
		{
			Max31856_READ_MODE(cs2,TEM2.mode);
			if(TEM2.mode[0] != 0xB0)
			{
				//printf("MODE1,ERROR\r\n");
				Max31856_SET_MODE(cs2,0xB0);
			}
			if(TEM2.mode[1] != 0x07)
			{
				//printf("MODE2,ERROR\r\n");
				Max31856_SET_TYPE(cs2,0x07);
			}
			
			TEM2.temp_tc = Get_Max31856_TC(cs2);
			osDelay(1);
			//MS = round(10 * TEM2.temp_tc);//乘10 四舍五入
			tem32.data = round(10 * TEM2.temp_tc);//乘10 四舍五入
			infoS.MST_13.byte0=tem32.byte.byte0;
			infoS.MST_13.byte1=tem32.byte.byte1;
			infoS.MST_13.byte2=tem32.byte.byte2;
			infoS.MST_13.byte3=tem32.byte.byte3;
			TEM2.temp_cold = Get_Max31856_Cold(cs2);
			osDelay(1);
				if(TEM2.temp_tc < 200)
			{
					//更新pid温度
						pid2.Pv = TEM2.temp_tc;	//更新pid温度
			}
			else
			{
				//过温报警
					printf("warning!tem2>200! \r\n");
			//后续加入保护措施，停止加热
//					pid1.Sv = -200;
					pid2.Sv = -200;
//					pid3.Sv = -200;
					Vms_flag_W = 1;
//				HAL_TIM_Base_Stop_IT(&htim2);//关闭PID中断 全部停止加热
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);    //停止加热
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);    //停止加热	
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);    //停止加热
			}
		
//			printf("%0.1f,%0.1f\n",pid2.Pv,pid2.Sv);
			
			
//			//printf("tem2：tc= %0.2f,cold=%0.2f\r\n",TEM2.temp_tc,TEM2.temp_tc);
		}
//		
//		
		//读第三路温度
		TEM3.fault = Max31856_READ_Fault(cs3);
		osDelay(1);
		if(TEM3.fault != 0)
		{
			//printf("tem3:error,fault:%x \r\n",TEM3.fault);
			
//				pid1.Sv = -200;
//				pid2.Sv = -200;
				pid3.Sv = -200;
				Tcap_flag_W = 1;
//			HAL_TIM_Base_Stop_IT(&htim2);//关闭PID中断 停止加热
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);    //停止加热
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);    //停止加热	
//			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);    //停止加热
//			infoS.CAPT_14.byte0=0x3c;
//			infoS.CAPT_14.byte1=0xf6;
//			infoS.CAPT_14.byte2=0xff;
//			infoS.CAPT_14.byte3=0xff;
		}
		else
		{
			Max31856_READ_MODE(cs3,TEM3.mode);
			if(TEM3.mode[0] != 0xB0)
			{
				//printf("MODE1,ERROR\r\n");
				Max31856_SET_MODE(cs3,0xB0);
			}
			if(TEM3.mode[1] != 0x07)
			{
				//printf("MODE2,ERROR\r\n");
				Max31856_SET_TYPE(cs3,0x07);
			}
			
			TEM3.temp_tc = Get_Max31856_TC(cs3);
			osDelay(1);
			//HEAT = round(10 * TEM3.temp_tc);//乘10 四舍五入
			tem32.data = round(10 * TEM3.temp_tc);//乘10 四舍五入
			infoS.CAPT_14.byte0=tem32.byte.byte0;
			infoS.CAPT_14.byte1=tem32.byte.byte1;
			infoS.CAPT_14.byte2=tem32.byte.byte2;
			infoS.CAPT_14.byte3=tem32.byte.byte3;
			TEM3.temp_cold = Get_Max31856_Cold(cs3);
			osDelay(1);
			if(TEM3.temp_tc < 200)
			{
					//更新pid温度
						pid3.Pv = TEM3.temp_tc;	//更新pid温度
			}
			else
			{
				//过温报警
				printf("warning!tem2>200! \r\n");
				
//				pid1.Sv = -200;
//				pid2.Sv = -200;
				pid3.Sv = -200;
				Tcap_flag_W = 1;
			//后续加入保护措施，停止加热
//				HAL_TIM_Base_Stop_IT(&htim2);//关闭PID中断 全部停止加热
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);    //停止加热
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);    //停止加热	
//				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2,GPIO_PIN_RESET);    //停止加热
			}
			
			//printf("%0.1f,%0.1f\n",pid3.Pv,pid3.Sv);
			//printf("tem3：tc= %0.2f,cold=%0.2f\r\n",TEM3.temp_tc,TEM3.temp_tc);
			
		}

		
		 PID1_Calc();	
		printf("temp:%0.1f,set_temp:%0.1f,speed:%0.1f\n",pid1.Pv,pid1.Sv,pid1.speed);
		if(pid1.low_cnt>MAX_TIME_CNT && pid1.Ek>MAX_EK)	//加热15s离目标温度还是差20度以上 加热器件损坏停止加热
		{
				//
				
				pid1.Sv = -190;
				printf("error! set_temp = %0.1f\n",pid1.Sv);
				pid1.low_cnt = 0;
		}
		 //PID_Calc_overshoot();
		 PID2_Calc();
		if(pid2.low_cnt>MAX_TIME_CNT && pid2.Ek>MAX_EK)	//加热15s离目标温度还是差20度以上 加热器件损坏停止加热
		{
				
				pid2.Sv = -200;
				pid2.low_cnt = 0;
		}
		 PID3_Calc();
		if(pid3.low_cnt>MAX_TIME_CNT && pid3.Ek>MAX_EK) //加热15s离目标温度还是差20度以上 加热器件损坏停止加热
		{				
				pid3.Sv = -200;
				pid3.low_cnt = 0;
		}
		xEventGroupSetBits(IWDGEventGroup, TASK_TEM_BIT_4);//标志位置四
		//printf("tem2 iwdg! \r\n");
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_1,duty);
		__HAL_TIM_SetCompare(&htim4,TIM_CHANNEL_2,duty);
		 osDelay(240);
  }
	#endif
  /* USER CODE END get_tem */
}


void pid_update(void const * argument)
{
  /* USER CODE BEGIN get_tem */
  /* Infinite loop */
	EventBits_t uxBits;
  for(;;)
  {
		 //PID1_Calc();
		 //osDelay(100);
		/* 等待所有任务发来事件标志 */
		uxBits = xEventGroupWaitBits(IWDGEventGroup, /* 事件标志组句柄 */
									 TASK_BIT_ALL,       /* 等待TASK_BIT_ALL被设置 */
									 pdTRUE,             /* 退出前TASK_BIT_ALL被清除，这里是TASK_BIT_ALL都被设置才表示“退出”*/
									 pdTRUE,             /* 设置为pdTRUE表示等待TASK_BIT_ALL都被设置*/
									 2000);      /* 等待延迟时间 */
		if((uxBits & TASK_BIT_ALL) == TASK_BIT_ALL)//判断各个任务是否执行
		{
			HAL_IWDG_Refresh(&hiwdg);
			//printf("iwdg seccess！\r\n");
		}
		else
		{
			//printf("iwdg error！\r\n");
		}

  }
  /* USER CODE END get_tem */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void DMA_Usart_Send1(uint8_t *buf,uint8_t len)//串口发送封装
{
 if(HAL_UART_Transmit_DMA(&huart1, buf,len)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
  {
   Error_Handler();
  }
}

void DMA_Usart_Send2(uint8_t *buf,uint8_t len)//串口发送封装
{
 if(HAL_UART_Transmit_DMA(&huart2, buf,len)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
  {
   Error_Handler();
  }
}

void DMA_Usart_Send3(uint8_t *buf,uint8_t len)//串口发送封装
{
 if(HAL_UART_Transmit_DMA(&huart3, buf,len)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
  {
   Error_Handler();
  }
}

void DMA_Usart_Send4(uint8_t *buf,uint8_t len)//串口发送封装
{
 if(HAL_UART_Transmit_DMA(&huart4, buf,len)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
  {
   Error_Handler();
  }
}

void DMA_Usart_Send5(uint8_t *buf,uint8_t len)//串口发送封装
{
 if(HAL_UART_Transmit_DMA(&huart5, buf,len)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
  {
   Error_Handler();
  }
}

void DMA_Usart_Send36(uint8_t *buf,uint8_t len)//串口发送封装
{
 if(HAL_UART_Transmit_DMA(&huart6, buf,len)!= HAL_OK) //判断是否发送正常，如果出现异常则进入异常中断函数
  {
   Error_Handler();
  }
}
/*
*********************************************************************************************************
* 函 数 名: DMA_Usart1_Read
* 功能说明: 串口接收功能函数
* 形  参: Data,len
* 返 回 值: 无
*********************************************************************************************************
*/

void DMA_Usart1_Read(uint8_t *Data,uint8_t len)//串口接收封装
{
	HAL_UART_Receive_DMA(&huart1,Data,len);//重新打开DMA接收
}
void DMA_Usart2_Read(uint8_t *Data,uint8_t len)//串口接收封装
{
	HAL_UART_Receive_DMA(&huart2,Data,len);//重新打开DMA接收
}
void DMA_Usart3_Read(uint8_t *Data,uint8_t len)//串口接收封装
{
	HAL_UART_Receive_DMA(&huart3,Data,len);//重新打开DMA接收
}
void DMA_Usart4_Read(uint8_t *Data,uint8_t len)//串口接收封装
{
	HAL_UART_Receive_DMA(&huart4,Data,len);//重新打开DMA接收
}
void DMA_Usart5_Read(uint8_t *Data,uint8_t len)//串口接收封装
{
	HAL_UART_Receive_DMA(&huart5,Data,len);//重新打开DMA接收
}
void DMA_Usart6_Read(uint8_t *Data,uint8_t len)//串口接收封装
{
	HAL_UART_Receive_DMA(&huart6,Data,len);//重新打开DMA接收
}
/* USER CODE END Application */
