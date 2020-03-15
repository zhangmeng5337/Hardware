#ifndef __DISPLAY_H__
#define	__DISPLAY_H__
#define DISPLAY_PERIOD   5
#define DISPLAY_BLINK    200
void display_off(void);

void display(float dat,unsigned char updateflag,unsigned char dattypes);
void display_proc(void);

#endif

