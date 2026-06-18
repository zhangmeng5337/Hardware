#ifndef MENU_H_
#define MENU_H_
#include "main.h"
#include "devinfo.h"


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
	langPage,
	mainRandomPage = 128
}L1enum;
	
typedef enum
{
    RunICon = 0,
	InfoIcon,
	WarnIcon,
	SetIcon,
	ReturnIcon,
	RandomIcon = 255
}Iconenum;
	

	
typedef enum
{
	date=ReturnIcon + 1,
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
	accVolume=ReturnIcon + 1,
	resetAccVolume,
	totalRunTime,
	motorWorkHour,
	completeStroke,
	PowerSw,
}Counterenum;	
typedef enum
{
 lang = ReturnIcon + 1,
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
	returnIcon = 1
 }langenum;

typedef enum
{
	MaxRunicon = 5,
	MaxInfoIcon = 11+ReturnIcon + 1,
	MaxWarnIcon = 10,
	MaxSetIcon = 14,
	MaxCounterIcon = 6+ReturnIcon + 1,
	MaxLang = 2,
	MaxCtrl = 7,
	MaxAnalog = 4,
	MaxAnalogout = 3,
	MaxSLow = 3,
}MaxIcon_enum;

//typedef struct
//{
//uint16_t status;
//uint32_t image_index;
//uint32_t pointType;//0:status 1 ctrlMode 2:ext display
//uint32_t imageBackIndex;
//unsigned char backEnable;
//unsigned char currentPage;
//unsigned char currentIconNum;
//}image_stru;

typedef struct
{
unsigned char currentPage;
unsigned char currentPageIcon;
uint32_t image_index[32];//
uint16_t arrib;
uint16_t arribSlave;
//uint32_t pointType;//0:status 1 ctrlMode 2:ext display
}image_stru;
typedef struct
{
	 char name[32];
	//unsigned int index;
}bmpMap_stru;
typedef struct
{
unsigned int cmdSpecific;
uint16_t dataType;
unsigned char dataSize;
void*memptr;
}menudatMap;
typedef struct
{
uint16_t index;
unsigned char fontBuf[32];
uint32_t arrib;//0:
unsigned char pollIndex;
menudatMap datMap;
}font_stru;

typedef struct
{
    // unsigned char mode;

    uint16_t currentMenuNum; //当前索引序号:页码
    unsigned char currentIconNum;//当前icon
    unsigned char nextMenuNum;
    unsigned char currMaxIconNum;
    void (*Current_Operation)(void);//当前操作(函数指针)
    unsigned char rootMenuNum;	
    unsigned char changeMenuSig; //1需要跳转
	unsigned char index;
	
    
} Menu_table_t;

typedef struct
{
	unsigned char infocCount;
	unsigned char infoNowNum;
	unsigned char maxInfoCount;
	unsigned char maxCountScreen;
	unsigned char buf[20];
	
}pageStru;



pageStru *getPageInfo(void);

#endif

