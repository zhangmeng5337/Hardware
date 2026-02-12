#ifndef __SEMTASKCOMMS_H_
#define __SEMTASKCOMMS_H_

extern int Init_Semaphore_comms_uart (void);
extern void Resume_thread_comms_uart(void);
extern int Init_Semaphore_comms_usbd (void);
extern void Resume_thread_comms_usbd(void);
extern int Init_Semaphore_comms_uart_ascii (void);
extern void Resume_thread_comms_uart_ascii(void);

#endif
