#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "main.h"
#include "nvmem.h"
#include "semtaskbgnd.h"

uint8_t reset_device = 0;

/**
  @brief shutdown sequence
*/
void shutdown(void) {
  static uint16_t countup = 0;
  if (reset_device) { // check if we are ready to reset. Run house keeping here
    countup++;
    if (countup >= 1) {
      if (is_flash_busy() == 0) {
        // Processor Reset
        NVIC_SystemReset();
      }
    }
  }
}

/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
 
osSemaphoreId_t sid_Semaphore_bgnd;                  // semaphore id
 
osThreadId_t tid_Thread_Semaphore_bgnd;              // thread id
static uint64_t thread_stk_bgnd[256];

const osThreadAttr_t thread_bgdn_attr = {
  .stack_mem = &thread_stk_bgnd[0],
  .stack_size = sizeof(thread_stk_bgnd),
  //.priority = osPriorityNormal
};

void Thread_Semaphore_bgnd (void *argument);         // thread function
 
int Init_Semaphore_bgnd (void) {
 
  sid_Semaphore_bgnd = osSemaphoreNew(1U, 1U, NULL);
  if (sid_Semaphore_bgnd == NULL) {
    ; // Semaphore object not created, handle failure
  }
 
  tid_Thread_Semaphore_bgnd = osThreadNew(Thread_Semaphore_bgnd, NULL, &thread_bgdn_attr);
  if (tid_Thread_Semaphore_bgnd == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Resume_thread_taskbgnd(void) {
    osSemaphoreRelease(sid_Semaphore_bgnd);              // return a token back to a semaphore
}

void Thread_Semaphore_bgnd (void *argument) {
  int32_t val;
  
//  val = osSemaphoreAcquire(sid_Semaphore_bgnd, osWaitForever);       // sync block
  
  while (1) {
    ; // Insert thread code here... 
    osDelay(100);       // wait 100 mSec
    switch (val) {
      case osOK:
        ; // Use protected code here...
        #ifndef BOOTLOADER     
        
        #endif        
        shutdown();
        osSemaphoreRelease(sid_Semaphore_bgnd);              // return a token back to a semaphore
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
