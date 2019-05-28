#ifndef LED_H
#define LED_H
#include "ddl.h"
//#include "stm8l15x_gpio.h"
#define LED_PORT1          0
#define LED_PORT2          1
#define LED_PIN1          3
#define LED_PIN2          5

#define LED_RED           0X00
#define LED_GREEN         0X01
void LED_ctrl(unsigned char led_sel,boolean_t status);
void LED_blink(unsigned char led_sel);
 void LED_Init(void);
#endif