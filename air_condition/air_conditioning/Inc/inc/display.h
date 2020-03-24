#ifndef __DISPLAY_H__
#define	__DISPLAY_H__
#define DISPLAY_PERIOD   5
#define DISPLAY_BLINK    200
#define DOT		    0x01
#define MINUS		0xfd
#define SET_TT      0Xd5  //current     'n'
#define SET_TA      0Xb9  //actual temperature  'u' 
#define SET_TW      0Xc9  //setting temperature 's'
#define SET_H       0Xd1  //humid 'h'
#define SET_WL      0Xe7  //low  'L'
#define RECYCLE_TIME    2000

void display_off(void);

void display_dat_deal(float dat,unsigned char updateflag,unsigned char dattypes);
void display_proc(unsigned char flag);

#endif

