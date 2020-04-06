#ifndef __DISPLAY_H__
#define	__DISPLAY_H__
#define DISPLAY_PERIOD   5
#define DISPLAY_BLINK_ON    500
#define DISPLAY_BLINK_PERIOD    1000

#define DOT		    0x01
#define MINUS		0xfd


 


#define SEG_R		0x83
#define SEG_E		0x31
#define SEG_A		0xc5
#define SEG_D		0xd5
#define SEG_Y		0x13


#define SET_CM      0x63  //cold mode    'c'
#define SET_CHM     0x65  //cold humid modec^- 
#define SET_HM      0x91  //hot mode 'u'
#define SET_FHM     0x71  //fast cold 'u^-'
#define SET_H       0X93  //humid '||'
#define SET_ST      0X49  //setting temperature 's'
#define SET_WL      0Xe7  //low  'L'
#define SET_I       0x11  //current     'A'
#define SET_AT      0xc5  //actual temperature  'c.'
#define ACTUAL_SPEED      0xD4  //actual temperature  'N.'

#define RECYCLE_TIME    1000

void display_off(void);

void display_dat_deal(float dat,unsigned char *header_code,unsigned char dattypes);
void display_proc(unsigned char flag);
void init_seg(void);

#endif

