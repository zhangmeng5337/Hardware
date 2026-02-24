#ifndef LED_H
#define LED_H
#define LED_ADC_STATUS	        0
#define LED_MASTER_STATUS	1


#define ON				0
#define OFF				1
#define BLINKINF			2
#define FREQUENCE		3
#define DELAY_TIME	 1000/FREQUENCE
void led_ctrl(unsigned char blink);
#endif

