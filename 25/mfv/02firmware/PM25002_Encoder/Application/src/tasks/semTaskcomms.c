#include <stdbool.h>
#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include "main.h"
#include "usb_device.h"
#include "commsAPI.h"
#include "protocol.h"
#include "semtaskcomms.h"
#include "nvmem.h"

/*----------------------------------------------------------------------------
 *      Semaphore creation & usage
 *---------------------------------------------------------------------------*/
 
osSemaphoreId_t sid_Semaphore_comms_usbd;                  // semaphore id
 
osThreadId_t tid_Thread_Semaphore_comms_usbd;              // thread id
static uint64_t thread_stk_comms_usbd[256];

const osThreadAttr_t thread_comms_usbd_attr = {
  .stack_mem = &thread_stk_comms_usbd[0],
  .stack_size = sizeof(thread_stk_comms_usbd)
};
 
void Thread_Semaphore_comms_usbd (void *argument);         // thread function
 
int Init_Semaphore_comms_usbd (void) {
 
  sid_Semaphore_comms_usbd = osSemaphoreNew(1U, 0U, NULL);
  if (sid_Semaphore_comms_usbd == NULL) {
    ; // Semaphore object not created, handle failure
  }
 
  tid_Thread_Semaphore_comms_usbd = osThreadNew(Thread_Semaphore_comms_usbd, NULL, &thread_comms_usbd_attr);
  if (tid_Thread_Semaphore_comms_usbd == NULL) {
    return(-1);
  }
  
  MX_USB_Device_Init();
  init_USBD_API();

  return(0);
}

void Resume_thread_comms_usbd(void) {
    osSemaphoreRelease(sid_Semaphore_comms_usbd);              // return a token back to a semaphore
}
 
void Thread_Semaphore_comms_usbd (void *argument) {
  int32_t val;
  
  val = osSemaphoreAcquire(sid_Semaphore_comms_usbd, osWaitForever);       // sync block

  protocol_sm(&usbd_api);
  
  while (1) {
    ; // Insert thread code here...
 
    val = osSemaphoreAcquire(sid_Semaphore_comms_usbd, osWaitForever);       // wait for post
    switch (val) {
      case osOK:
        ; // Use protected code here...
        a_protocol = panalyzer(&usbd_com_driver);      
        switch (a_protocol) {
          default:
            do {
              protocol_sm(&usbd_api);
            } while (usbd_api.current_state != RXWAIT);
            break;        
        }
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
