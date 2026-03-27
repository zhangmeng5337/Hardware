#include "controller.h"
#include "di_do.h"

controller_stru controllerManufacture;
controller_stru controllerCustom;
void faultUpdate(void)
{
    unsigned int diLevel;
	diLevel = getDi()&0x000f;
	    controllerCustom.settingU.fault = controllerCustom.settingU.fault & 0xfff0;
		controllerCustom.settingU.fault = controllerCustom.settingU.fault|diLevel;
}
void controlInit(void)
{
	controllerCustom.settingU.ctrlMode = manualMode;
	controllerCustom.mode = customMode;
}
