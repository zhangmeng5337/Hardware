#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "semtaskmem.h"
#include "semtasksigproc.h"
#include "semtaskecat.h"
#include "semtaskcomms.h"
#include "semtaskbgnd.h" 
#include "nvmem.h"
/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/

osSemaphoreId_t sid_Semaphore_mem;                  // semaphore id
 
osThreadId_t tid_Thread_Semaphore_mem;              // thread id
static uint64_t thread_stk_mem[256];

const osThreadAttr_t thread_mem_attr = {
  .stack_mem = &thread_stk_mem[0],
  .stack_size = sizeof(thread_stk_mem),
  //.priority = osPriorityAboveNormal1
};
 
void Thread_Semaphore_mem (void *argument);         // thread function
 
int Init_Semaphore_mem (void) {
 
  sid_Semaphore_mem = osSemaphoreNew(1U, 1U, NULL);
  if (sid_Semaphore_mem == NULL) {
    ; // Semaphore object not created, handle failure
  }
 
  tid_Thread_Semaphore_mem = osThreadNew(Thread_Semaphore_mem, NULL, &thread_mem_attr);
  if (tid_Thread_Semaphore_mem == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_Semaphore_mem (void *argument) {

  nvmem_stat_machine();
#ifndef BOOTLOADER
//  Resume_thread_sigproc();
//  Resume_thread_comms_ecat();  
#endif
//  Resume_thread_comms_uart();
  Resume_thread_comms_usbd();  
  Resume_thread_taskbgnd();
    
  while (1) {
    ; // Insert thread code here...
    osDelay(FLASH_THREAD_PERIOD);
    while (nvmem_stat_machine() >= NVMEM_PAGE_WRITE) {
      __nop();
    };
#ifndef BOOTLOADER    
    //rtb_stat_machine();
#endif
    osThreadYield();                                    // suspend thread
  }
}
