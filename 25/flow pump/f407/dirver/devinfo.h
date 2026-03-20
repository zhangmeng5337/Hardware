#ifndef DEVINFO_H
#define DEVINFO_H
#include "main.h"

#define SoftVer   "V1.0.1"
#define MotorCtrl "V01"
#define HardwareVer "V1"
#define TypeNo  gd

typedef struct
{
    float totalFlow;
    float resetTotalFlow;
    float runHours;
    uint32_t motorRunHours;
    uint32_t pulseTotal;
    uint32_t powerTimes;
} counter_stru;

typedef struct
{
    float flowFullRange;
    float currentFullRange;
    float currentZero;
    float flowZero;
} analogFraction_stru;

typedef struct
{
    float volume;
    unsigned char status;//1:stop 2:start 0:no effort
    float currentZero;
    float flowZero;
} calibration_stru;
typedef struct
{
    unsigned char units;//0:met 1:gal
    unsigned char contrast; //0.58
    unsigned char defatult;
    unsigned char totalAddVolume;
} display_stru;

typedef struct
{
    unsigned char year;
    unsigned char month;
    unsigned char date;
    unsigned char hour;
    unsigned char minutes;
	unsigned char week;
} timeAddDate_stru;

typedef struct
{
    unsigned char status;//0:stop
    unsigned char addr;

} bus_stru;

typedef struct
{
    unsigned char warn;//0:disable 1:enable
    unsigned char fault;//0:disable 1:enable
    unsigned char onePulse;
	unsigned char pumpAdd;
	unsigned char pulseInput;
	unsigned char busCtrl;
	unsigned char TouchPointDef;
} relay1_stru;

typedef struct
{
    unsigned char warn;//0:disable 1:enable
    unsigned char fault;//0:disable 1:enable
    unsigned char onePulse;
	unsigned char pumpAdd;
	
	unsigned char pulseInput;
	unsigned char recycleCounter;
	unsigned char weeklyRecycleCounter;
	unsigned char busCtrl;
	unsigned char TouchPointDef;
} relay2_stru;

typedef struct
{
    unsigned char extStop;//0:stop
    unsigned char emptySig;
	relay1_stru liquidlow;
} diDo_stru;

typedef struct
{
    unsigned char status;//0:stop
    unsigned char addr;
	relay1_stru relay1u;
	relay2_stru relay2u;
	diDo_stru diDo;
} inOut_stru;
typedef enum
{
	manualMode = 0,
	pulseMode,
	cur0To20Mode,
	cur4To20Mode,
	batchCtrlMode,
	addByCycleMode,
	addByWeeklyMode,
	manufactureMode,
	customMode
}settingCtrlMode; //0:manual; 1:pulse 2:0-20ma 3:4-20ma 4:batch 5:addByCycle 6:addByWeekly

typedef enum
{
	stop = 0,
    run,
    standby,
    discharge,
    vOut,
    vIn
    
}statusEnum;

typedef enum
{
	chinese = 0,
	englsih
}languageMode;	
typedef enum
{
	extStop = 1,
	liquidlowFault = 4,
	lineShortFault = 2,
	emptyFault = 8,
	ciuFault = 16,
	maintanceFault = 32,
	valvedischargeFault = 64,
	overTFault =128,
	overLoadFault = 256,
	outValveFault = 512
}faultEnum;
typedef struct
{
    unsigned char 		language; //0:chinese 1:english
    unsigned char 		ctrlMode; //0:manual; 1:pulse 2:0-20ma 3:4-20ma 4:batch 5:addByCycle 6:addByWeekly
    analogFraction_stru analogFraction;
    unsigned char 		analogOut;//0:in=out 1:actual 2:bus ctrl
    unsigned char 		slowMode;//0:stop 1:0.5 speed 1:0.25speed
    unsigned char 		powerFault;//0:normal 1:fault stop
    unsigned char 		autoVent;//0:no 1:support
    calibration_stru 	calibration;
    unsigned char 		sysLock;//0:stop 1:password setting 2：进制更改按键
    display_stru 		display;  //
    timeAddDate_stru 	timeAddDate;
    bus_stru 			bus;
    inOut_stru 			inOut;
    unsigned char 		baseSet;//0:no effort 1:recovery manufacture set 2:backup custom 3:recovery custom
    unsigned char       status;//0：Stop 1run 2：standby 4:discharge 5:valve out 6:valve in
    unsigned int 		fault; //bit:0---ext stop bit1:empty bit2:lowlevel bit3:line short
    //4:manufacture setting 5:custom setting
} setting_stru;

#endif

