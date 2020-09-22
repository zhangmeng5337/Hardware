#ifndef LED_H_
#define LED_H_

#define WORK_STATUS   0X01
#define HIGH_LEVEL    0X02
#define LOW_LEVEL     0X03
#define LED_ALL       0X04

void led_ctrl(unsigned char num,unsigned char on_off);
#endif
