#include "menu.h"
#include "key.h"

void l1StandbyMenu(void);
void l1RunPageMenu(void);
void l2InfoMenu(void);
void l2WarnMenu(void);
void l2SetMenu(void);
void l3InfoMenu(void);
void l3SetMenu(void);
void l3SetLangMenu(void);
void l3SetCtrlMenu(void);
void l3SetAnalogMenu(void);
void l3SetAnalogOutMenu(void);
void l3SetSlowMenu(void);
void l3SeCaliMenu(void);
void l3SetsysLockMenu(void);
void l3SetdisplayMenu(void);
void l3SetdateMenu(void);
void l3SetBusMenu(void);
void l3SetInoutMenu(void);
void l4SetRelay1Menu(void);
void l4SetRelay2Menu(void);
void l3SetBasicMenu(void);
unsigned int menuTaskIndex;
Menu_table_t menuTable[] =
{
    // L1
    {StandByPage, RunICon,   	RunPage, 		MaxRunicon,			l1RunPageMenu, 0, 0},
    {StandByPage, InfoIcon,  	RunPage, 		MaxRunicon,			l1RunPageMenu, 0, 0},
    {StandByPage, WarnIcon,  	RunPage, 		MaxRunicon,			l1RunPageMenu, 0, 0},
    {StandByPage, SetIcon,   	RunPage, 		MaxRunicon,			l1RunPageMenu, 0, 0},

    {RunPage,    RunICon,    	RunPage,		MaxRunicon,			l1StandbyMenu, 0, 0},
    {RunPage,    InfoIcon,   	InfoPage,		MaxInfoIcon,		l2InfoMenu, 0, 0},
    {RunPage,    WarnIcon,   	WarnPage,		MaxWarnIcon,		l2WarnMenu, 0, 0},
    {RunPage,    SetIcon,    	SetPage,		MaxSetIcon,   	l2SetMenu, 0, 0},
    //L2
    {InfoPage,    date,      		InfoPage,			1,							l2InfoMenu, 0, 0},
    {InfoPage,    counter,   		CounterPage,	MaxCounterIcon,	l3InfoMenu, 0, 0},
    {InfoPage,    maintance, 		InfoPage,			1,							l2InfoMenu, 0, 0},
    {InfoPage,    servicePack,  	InfoPage,			1,							l2InfoMenu, 0, 0},
    {InfoPage,    restore, 	    	InfoPage,			1,							l2InfoMenu, 0, 0},
    {InfoPage,    softVer, 	   		InfoPage,			1,							l2InfoMenu, 0, 0},
    {InfoPage,    motorCtrl, 		InfoPage,			1,							l2InfoMenu, 0, 0},
    {InfoPage,    hwVer, 			InfoPage,			1,							l2InfoMenu, 0, 0},
    {InfoPage,    serNo, 			InfoPage,			1,							l2InfoMenu, 0, 0},

    {WarnPage,    WarnIcon, 	WarnPage,		MaxWarnIcon,	l2WarnMenu, 0, 0},

    {SetPage,    lang, 		    	setLangPage,			MaxLang,	    l3SetLangMenu, 0, 0},
    {SetPage,    ctrl, 		    	setCtrlPage,			MaxCtrl,	    l3SetCtrlMenu, 0, 0},
    {SetPage,    analog, 			setAnalogPage,	    	MaxAnalog,	    l3SetAnalogMenu, 0, 0},
    {SetPage,    analogOut, 		setAnalogOutPage,		MaxAnalogout,	l3SetAnalogOutMenu, 0, 0},
    {SetPage,    slowMode, 	    	setSlowPage,	    	MaxAnalogout,	l3SetSlowMenu, 0, 0},
    {SetPage,    powerAb, 	    	SetPage,	        	1,				l2SetMenu, 0, 0},
    {SetPage,    autoDischarge, 	SetPage,	        	1,				l2SetMenu, 0, 0},
    {SetPage,    calibration, 		calibrationPage,		2,				l3SeCaliMenu, 0, 0},
    {SetPage,    sysLock, 	    	sysLockPage,	    	3,				l3SetsysLockMenu, 0, 0},
    {SetPage,    display, 	    	disPage,	        	3,				l3SetdisplayMenu, 0, 0},
    {SetPage,    dateTime, 	    	datePage,	        	2,				l3SetdateMenu, 0, 0},
    {SetPage,    bus, 	        	busPage,	        	2,				l3SetBusMenu, 0, 0},
    {SetPage,    inOut, 	    	inOutPage,	        	5,				l3SetInoutMenu, 0, 0},
    {SetPage,    basicSet, 	    	restorePage,	    	3,				l3SetBasicMenu, 0, 0},
    //L4
    {inOutPage,  relay1, 	      relay1Page,	    		7,				l4SetRelay1Menu, 0, 0},
    {inOutPage,  relay2, 	      relay2Page,	    		9,				l4SetRelay2Menu, 0, 0},

};
void l1StandbyMenu(void)
{

}
void l1RunPageMenu(void)
{

}
void l2InfoMenu(void)
{

}
void l2WarnMenu(void)
{

}

void l2SetMenu(void)
{

}
void l3InfoMenu(void)
{

}
void l3SetMenu(void)
{

}
void l3SetLangMenu(void)
{

}
void l3SetCtrlMenu(void)
{

}
void l3SetAnalogMenu(void)
{

}
void l3SetAnalogOutMenu(void)
{

}
void l3SetSlowMenu(void)
{

}
void l3SeCaliMenu(void)
{

}
void l3SetsysLockMenu(void)
{

}
void l3SetdisplayMenu(void)
{

}
void l3SetdateMenu(void)
{

}
void l3SetBusMenu(void)
{

}
void l3SetInoutMenu(void)
{

}
void l4SetRelay1Menu(void)
{

}
void l4SetRelay2Menu(void)
{

}
void l3SetBasicMenu(void)
{

}



void menuInterfaceMange()
{

    if (getEncoderKey()->keyStatus == 1)
    {
        if (getEncoderKey()->key_dir == FORWARD)
        {
            if (menuTable[menuTaskIndex].currentIconNum <
                    menuTable[menuTaskIndex].currMaxIconNum)
                menuTable[menuTaskIndex].currentIconNum ++;
        }
        else
        {
            if (menuTable[menuTaskIndex].currentIconNum > 0)
                menuTable[menuTaskIndex].currentIconNum --;
        }
        getEncoderKey()->keyStatus = 0;
        menuTable[menuTaskIndex].mode = 1;

    }
    if (getEncoderKey()->keyEnter == 1)
    {
        getEncoderKey()->keyEnter = 0;
        menuTable[menuTaskIndex].changeMenuSig = 1;
    }

}
void menu_manage()
{
    menuInterfaceMange();
    if (menuTable[menuTaskIndex].changeMenuSig == 1) //切换画面
    {
        menuTable[menuTaskIndex].changeMenuSig = 0;//press ok button change menu
        menuTaskIndex = menuTable[menuTaskIndex].nextMenuNum;
        menuTable[menuTaskIndex].currentIconNum = 0;
        menuTable[menuTaskIndex].mode = 0;
    }
    menuTable[menuTaskIndex].Current_Operation();
}
