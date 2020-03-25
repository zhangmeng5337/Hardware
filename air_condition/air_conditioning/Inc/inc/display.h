#ifndef __DISPLAY_H__
#define	__DISPLAY_H__
#define DISPLAY_PERIOD   5
#define DISPLAY_BLINK    200
#define DOT		    0x01
#define MINUS		0xfd


 




#define SET_CM      0x79  //cold mode    'c'
#define SET_CHM     0x65  //cold humid modec^- 
#define SET_HM      0xc7  //hot mode 'u'
#define SET_FHM     0x47  //fast cold 'u^-'
#define SET_H       0Xd7  //humid '||'
#define SET_ST      0Xc9  //setting temperature 's'
#define SET_WL      0Xe7  //low  'L'
#define SET_I       0x11  //current     'A'
#define SET_AT      0xe4  //actual temperature  'c.'
#define ACTUAL_SPEED      0xD4  //actual temperature  'N.'

#define RECYCLE_TIME    2000

void display_off(void);

void display_dat_deal(float dat,unsigned char updateflag,unsigned char dattypes);
void display_proc(unsigned char flag);
void init_seg(void);

#endif

