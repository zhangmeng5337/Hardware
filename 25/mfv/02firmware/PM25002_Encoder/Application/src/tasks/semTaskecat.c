#include <stdbool.h>
#include "main.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "applInterface.h"
#include "9252_HW.h"
#include "digitalio.h"
#include "awe.h"

/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
extern UINT16 ulMacId;
extern TIM_HandleTypeDef htim4;
extern void init_IWDG();

bool init = false;
osSemaphoreId_t sid_Semaphore_ec;                  // semaphore id
 
osThreadId_t tid_Thread_Semaphore_ec;              // thread id
static uint64_t thread1_stk_ec[1024];
 
const osThreadAttr_t thread_ec_attr = {
  .stack_mem  = &thread1_stk_ec[0],
  .stack_size = sizeof(thread1_stk_ec),
};
 
void Thread_Semaphore_ec(void *argument);         // thread function
 
int Init_Semaphore_ec(void) {
 
  sid_Semaphore_ec = osSemaphoreNew(1U, 0U, NULL);
  if (sid_Semaphore_ec == NULL) {
    ; // Semaphore object not created, handle failure
  }
 
  tid_Thread_Semaphore_ec = osThreadNew(Thread_Semaphore_ec, NULL, &thread_ec_attr);
  if (tid_Thread_Semaphore_ec == NULL) {
    return(-1);
  }
 
  return(0);
}

void Resume_thread_comms_ecat(void) {
  osSemaphoreRelease(sid_Semaphore_ec);              // return a token back to a semaphore
}
 
void Thread_Semaphore_ec (void *argument) {
  int32_t val;
	
  LAN9252_RESET
	val = osSemaphoreAcquire(sid_Semaphore_ec, osWaitForever);	// sync block 
  LAN9252_RUN
  if (LAN9252_Init() == 1) {
		return;
	}
  wMX_I2C1_Init();
  MainInit();
  ecat_userobj_init(1);
  HAL_TIM_Base_Start_IT(&htim4);
  bRunApplication = TRUE;
  //init_IWDG();
  while (bRunApplication) {
    ; // Insert thread code here...
    val = osSemaphoreAcquire(sid_Semaphore_ec, osWaitForever);	// block forever
    switch (val) {
      case osOK:
        ; // Use protected code here...    
        //SP_SET  
        MainLoop();
        /* Refresh IWDG: reload counter */
        //HAL_IWDG_Refresh(&hiwdg); 
        //SP_RESET   
        ; // return a token back to a semaphore
        break;
      case osErrorResource:
        break;
      case osErrorParameter:
        break;
      default:
        break;
    }
 
    osThreadYield();                                    // suspend thread
  }
  
}
