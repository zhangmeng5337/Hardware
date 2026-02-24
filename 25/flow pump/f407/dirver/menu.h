#ifndef MENU_H_
#define MENU_H_
#include "main.h"
typedef enum
{
	StandByPage = 0,
	RunPage,
	InfoPage,
	WarnPage,
	SetPage,
	CounterPage,
	setLangPage,
	setCtrlPage,
	setAnalogPage,
	setAnalogOutPage,
	setSlowPage,
	calibrationPage,
	sysLockPage,
	disPage,
	datePage,
	busPage,
	inOutPage,
	relay1Page,
	relay2Page,
	restorePage,
}L1enum;
typedef enum
{
    RunICon = 0,
	InfoIcon,
	WarnIcon,
	SetIcon
}Iconenum;
typedef enum
{
	date=0,
	counter,
	maintance,
	servicePack,
	restore,
	softVer,
	motorCtrl,
	hwVer,
	serNo,
	
}Infoenum;
typedef enum
{
 lang = 0,
 ctrl,
 analog,
 analogOut,
 slowMode,
 powerAb,
 autoDischarge,
 calibration,
 sysLock,
 display,
 dateTime,
 bus,
 inOut,
 basicSet
}Setenum;
 typedef enum
 {
	relay1=0,
	relay2,
	powerOffExt,
	empty,
	lowLevel
 }inoutenum;
 
typedef enum
{
	MaxRunicon = 4,
	MaxInfoIcon = 11,
	MaxWarnIcon = 10,
	MaxSetIcon = 14,
	MaxCounterIcon = 6,
	MaxLang = 2,
	MaxCtrl = 7,
	MaxAnalog = 4,
	MaxAnalogout = 3,
	MaxSLow = 3,
}MaxIcon_enum;
typedef struct
{
    // unsigned char mode;
    uint8_t currentMenuNum; //当前索引序号:页码
    unsigned char currentIconNum;//当前icon
    unsigned char nextMenuNum;
    unsigned char currMaxIconNum;
    void (*Current_Operation)(void);//当前操作(函数指针)
    unsigned char changeMenuSig; //1需要跳转
    unsigned char mode;//1：选中
} Menu_table_t;
#endif

