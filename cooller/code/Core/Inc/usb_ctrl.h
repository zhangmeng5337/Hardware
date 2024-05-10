#ifndef USB_CTRL_H_
#define USB_CTRL_H_
#include "main.h"
void usb_ctrl(void);
void usb_init(void);
unsigned char get_usb_state(void);
unsigned char *get_usb_wr(void);

#endif

