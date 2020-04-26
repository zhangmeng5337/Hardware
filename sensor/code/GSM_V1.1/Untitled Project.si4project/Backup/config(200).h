#ifndef CONGIG_H_
#define CONFIG_H_
unsigned int rtctime=300;//10:9s，修改休眠时间
unsigned char one_net_key[]="*296832#571498701#json*";//284261：产品编号；abab：鉴权码；json：脚本
//unsigned char Establish_TCP_Connection[100]="AT+CIPSTART=\"TCP\",\"15k801n729.iok.la\",36732\r";
//unsigned char one_net_key[]="*284261#abab#json* ";
unsigned char Establish_TCP_Connection[100]="AT+CIPSTART=\"TCP\",\"dtu.heclouds.com\",1811\r";
#endif