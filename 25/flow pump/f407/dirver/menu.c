#include "menu.h"
#include "key.h"
#include "st7789.h"
#include "all_icons.h"
#include "devinfo.h"
#include "controller.h"
#include "my_font.h"
#include "font_lib.h"
#include "image_display.h"
#include "spiFileProc.h"

//python .\pic_py.py  .\icons\  .\lib\ -b -w 32 -H 32
//python pic_py.py icons/ lib/all_icons.h -b -c -w 32 -H 32

//python generate_fontlib.py common_chars.txt "C:/Windows/Fonts/msyh.ttc" 16 -o fixed_font.c -n bold_font -W 16 -H 16 --bold 1 --encoding=utf-8-sig
/*
*   [9] flow.bmp (flow)
	[37] 剩余批量.bmp (sheng_yu_pi_liang)
*   [40] 实际背压.bmp (shi_ji_bei_ya)
*   [49] 输入电流.bmp (shu_ru_dian_liu)
*   [32] 下一计量时间.bmp (xia_yi_ji_liang_shi_jian)
*   [42] 总计投加容积.bmp (zong_ji_tou_jia_rong_ji)
*/
//	StandByPage = 0,
//	RunPage,
//	InfoPage,
//	WarnPage,
//	SetPage,const bold_font_char_info_t bold_font_chars16[11] = {
extern image_info_t imageInfo;
extern controller_stru controllerCustom;
font_stru fontTable[] =
{
    //index  			font        len  arri
    //0:manual; 1:pulse 2:0-20ma 3:4-20ma 4:batch 5:addByCycle 6:addByWeekly

    {0, 	{"手动"}, 	2, 		0},
    {1, 	{"脉冲"}, 	2, 		0}, //
    {2, 	{"模拟"}, 	2, 		0},  //
    {3, 	{"模拟"}, 	2, 		0},  //
    {4, 	{"批次"}, 	2, 		0},  //
    {5, 	{"计时器"}, 	3, 		0}, //pulse
    {6, 	{"计时器"}, 	3, 		0}, //pulse
    //    StandByPage = 0,	RunPage,	InfoPage,	WarnPage,	SetPage,
    {StandByPage, {"运行"}, 	2, 		1}, //manual
    {RunPage, 	{"运行"}, 		2, 		1}, //manual
    {InfoPage, 	{"信息"}, 		2, 		1},  //pulse
    {WarnPage, 	{"警告"}, 		2, 		1},  //pulse
    {SetPage, 	{"设置"}, 		2, 		1}  //pulse

};
bmpMap_stru bmpMap[] =
{
    "CIU_1",
    "OoverT_1",
    "airdischarge_1",
    "analog_1",
    "bar_1",
    "cancel_1",
    "cancelsel_1",
    "counter_1",
    "empty_1",
    "flow_1",
    "flowctrl_1",
    "frame1LU_1",
    "frame2RD_1",
    "frame3LD_1",
    "frame4RD_1",
    "liquidLow_1",
    "ok_1",
    "outvale_1",
    "overL_1",
    "powerfault_1",
    "pro_1",
    "pulse_1",
    "return1_1",
    "return2_1",
    "short_1",
    "slowmode_1",
    "standbyback_1",
    "valvedischarge_1",
    "valveposIn_1",
    "valveposOut_1",
    "warn1_1",
    "warn2_1",
    "XYJLSJ",
    "XJCD",
    "XX1",
    "XX2",
    "TJ",
    "SYPL",
    "TP1",
    "WBTJ",
    "SJBY",
    "ZX",
    "ZJTJRJ",
    "SD",
    "XTS",
    "WX",
    "ZDPQ",
    "SZ1",
    "SZ2",
    "SRDL",
    "YX2",
    "YXZ",
    "YXZT1",
    "YXZT2",
    "YXZT3",
    "YXZT4",
    "XZ1",
    "XZ2-1",
    "XZ2-2",
    "SGFH"
};

image_stru  imageTable[] =
{
    {StandByPage, RunICon, {36, 51, 26, 2, 29, 28},						0, 0},
    {StandByPage, RunICon, {43, 21, 3, 3, 20, 7},						1, 0},
    {StandByPage, RunICon, {9, 9, 49, 49, 37, 40, 32, 42},				1, 1},
    {StandByPage, RunICon, {25, 10, 44, 41, 46},						2, 0},
    {StandByPage, RunICon, {39, 8, 15, 24, 0, 18, 27, 1, 17},			3, 0},

    {RunPage, RandomIcon, {36, 51, 26, 2, 29, 28},						0, 0},
    {RunPage, RandomIcon, {43, 21, 3, 3, 20, 7},						1, 0},
    {RunPage, RandomIcon, {9, 9, 49, 49, 37, 40, 32, 42},				1, 1},
    {RunPage, RandomIcon, {25, 10, 44, 41, 46},							2, 0},
    {RunPage, RandomIcon, {39, 8, 15, 24, 0, 18, 27, 1, 17},			3, 0},
    {RunPage, RandomIcon, {39, 8, 15, 24, 0, 18, 27, 1, 17},			3, 0},
    //run   info    warn set
    {mainRandomPage, RandomIcon, {38, 34, 30, 47},						4, 0}, //icon can be selected,not selected
    {mainRandomPage, RandomIcon, {50, 35, 31, 48},						4, 1} //icon can be selected, selected
};
/*
*   [38] 图片1.bmp (tu_pian_1)
*   [50] 运行2.bmp (yun_xing_2)
*   [34] 信息1.bmp (xin_xi_1)
*   [35] 信息2.bmp (xin_xi_2)
*   [30] warn1.bmp (warn1)
*   [31] warn2.bmp (warn2)
*   [47] 设置1.bmp (she_zhi_1)
*   [48] 设置2.bmp (she_zhi_2)
*/
//image_stru imageTable[] =
//{
//    {stop,		 							36, 	0, 0, 0,StandByPage,RunICon}, //controllerCustom.settingU.status
//    {run,		 							51, 	0, 0, 0,StandByPage,RunICon},
//    {standby,	 							26, 	0, 0, 0},
//    {discharge,	 							2, 		0, 0, 0},
//    {vOut,		 							29, 	0, 0, 0},
//    {vIn,		 							28, 	0, 0, 0},
//    {manualMode, 							43, 	1, 0, 0}, //controllerCustom.settingU.ctrlMode
//    {pulseMode, 							21, 	1, 0, 0},
//    {cur0To20Mode, 							3, 		1, 0, 0},
//    {cur4To20Mode, 							3, 		1, 0, 0},
//    {batchCtrlMode, 						21, 	1, 0, 0},
//    {addByCycleMode, 						20, 	1, 0, 0},
//    {addByWeeklyMode, 						7, 		1, 0, 0},
//    {manualMode,							9,		2, 0, 0}, //controllerCustom.settingU.ctrlMode
//    {pulseMode,		    					9,		2, 0, 0},
//    {cur0To20Mode, 							49, 	2, 0, 0},
//    {cur4To20Mode, 							49, 	2, 0, 0},
//    {addByCycleMode, 						37, 	2, 0, 0},
//    {addByWeeklyMode, 						37, 	2, 0, 0},
//    {addByWeeklyMode, 						37, 	2, 0, 0},
//    {1,										25,		3, 0, 0}, //controllerCustom.settingU.slowMode
//    {0,										10,		4, 0, 0}, //
//    {1,										44,		5, 0, 0}, //controllerCustom.settingU.sysLock
//    {1,										46,		6, 0, 0}, //controllerCustom.settingU.autoVent
//    {1,										41,		7, 0, 0}, //controllerCustom.settingU.bus.status
//    {emptyFault,							8,		8, 0, 0},
//    {extStop,								39,		8, 0, 0},
//    {liquidlowFault,						15,		8, 0, 0},
//    {lineShortFault,						24,		8, 0, 0},
//    {ciuFault,								0,		8, 0, 0},
//    {maintanceFault,						45,		8, 0, 0},
//    {valvedischargeFault,					27,		8, 0, 0},
//    {overTFault,							1,		8, 0, 0},
//    {overLoadFault,							18,		8, 0, 0},
//    {outValveFault,							17,		8, 0, 0},
//    {1, 34, 9,255,InfoIcon}
//
//};

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
Menu_table_t menuTableU;
Menu_table_t menuTable[] =
{
    // L1
    {StandByPage, RunICon,   		RunPage, 			1,					l1StandbyMenu, 		0, 1}, //0
    {RunPage,     RunICon,    		RunPage,			MaxRunicon,			l1RunPageMenu, 		0, 1}, //1
    {RunPage,     InfoIcon,   		InfoPage,			MaxRunicon,			l1RunPageMenu, 		0, 5}, //2
    {RunPage,     WarnIcon,   		WarnPage,			MaxRunicon,			l1RunPageMenu, 		0, 14}, //3
    {RunPage,     SetIcon,    		SetPage,			MaxRunicon,   	    l1RunPageMenu, 		0, 15}, //4
    //L2
    {InfoPage,    date,      		InfoPage,			MaxInfoIcon,		l2InfoMenu, 		0, 5}, //5
    {InfoPage,    counter,   		CounterPage,		MaxCounterIcon,		l3InfoMenu, 		0,},//6
    {InfoPage,    maintance, 		InfoPage,			MaxInfoIcon,		l2InfoMenu, 		0, 7}, //7
    {InfoPage,    servicePack,  	InfoPage,			MaxInfoIcon,		l2InfoMenu, 		0, 8}, //8
    {InfoPage,    restore, 	    	InfoPage,			MaxInfoIcon,		l2InfoMenu, 		0, 9}, //9
    {InfoPage,    softVer, 	   		InfoPage,			MaxInfoIcon,		l2InfoMenu, 		0, 10}, //10
    {InfoPage,    motorCtrl, 		InfoPage,			MaxInfoIcon,		l2InfoMenu, 		0, 11}, //11
    {InfoPage,    hwVer, 			InfoPage,			MaxInfoIcon,		l2InfoMenu, 		0, 12}, //12
    {InfoPage,    serNo, 			InfoPage,			MaxInfoIcon,		l2InfoMenu, 		0, 13}, //13


    {WarnPage,    WarnIcon, 		WarnPage,			MaxWarnIcon,		l2WarnMenu, 		0, 14}, //14

    //
    {SetPage,    lang, 		    	setLangPage,		MaxLang,	    	l2SetMenu, 		0, 31}, //15
    {SetPage,    ctrl, 		    	setCtrlPage,		MaxCtrl,	    	l3SetCtrlMenu, 		0, 43}, //16
    {SetPage,    analog, 			setAnalogPage,	    MaxAnalog,	    	l3SetAnalogMenu, 	0,},//17
    {SetPage,    analogOut, 		setAnalogOutPage,	MaxAnalogout,		l3SetAnalogOutMenu, 0,},//18
    {SetPage,    slowMode, 	    	setSlowPage,	    MaxAnalogout,		l3SetSlowMenu, 		0,},//19
    {SetPage,    powerAb, 	    	SetPage,	        1,					l2SetMenu, 			0,},//20
    {SetPage,    autoDischarge, 	SetPage,	        1,					l2SetMenu, 			0,},//21
    {SetPage,    calibration, 		calibrationPage,	2,					l3SeCaliMenu, 		0,},//22
    {SetPage,    sysLock, 	    	sysLockPage,	    3,					l3SetsysLockMenu, 	0,},//23
    {SetPage,    display, 	    	disPage,	        3,					l3SetdisplayMenu, 	0,},//24
    {SetPage,    dateTime, 	    	 datePage,	        2,					l3SetdateMenu, 		0,},//25
    {SetPage,    bus, 	        	 busPage,	        2,					l3SetBusMenu, 		0,},//26
    {SetPage,    inOut, 	    	   inOutPage,	        5,					l3SetInoutMenu, 	0,},//27
    {SetPage,    basicSet, 	    	 restorePage,	    3,					l3SetBasicMenu, 	0,},//28
    //L4                                                                                                    //
    {inOutPage,  	relay1, 	       relay1Page,	    		7,					l4SetRelay1Menu, 	0,},//29
    {inOutPage,  	relay2, 	       relay2Page,	    		9,					l4SetRelay2Menu, 	0,},//30

    {setLangPage,	chinese, 	       langPage,	    		2,					l4SetRelay2Menu, 	0, 31}, //31
    {setLangPage,	englsih, 	        langPage,	    		2,					l4SetRelay2Menu, 	0, 32}, //32
    {setLangPage,	returnIcon, 	    langPage,	    		2,					l4SetRelay2Menu, 	0, 15}, //33

    {setCtrlPage,	manualMode, 		  setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 34}, //34
    {setCtrlPage,	pulseMode, 	    	setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 35}, //34
    {setCtrlPage,	cur0To20Mode, 		setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 36}, //34
    {setCtrlPage,	cur4To20Mode, 		setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 37}, //34
    {setCtrlPage,	batchCtrlMode, 		setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 38}, //34
    {setCtrlPage,	addByCycleMode, 	setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 39}, //34
    {setCtrlPage,	addByWeeklyMode, 	setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 40}, //34
    {setCtrlPage,	manufactureMode, 	setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 41}, //34
    {setCtrlPage,	customMode, 		  setCtrlPage,	    7,				l4SetRelay2Menu, 	0, 42}, //34
    //    {setAnalogPage,	, 		  ,	    7,				l4SetRelay2Menu, 	0,43},//34

    {InfoPage,     RunICon,    		RunPage,			MaxRunicon,			l1RunPageMenu, 		0, 1}, //1
    {InfoPage,     InfoIcon,   		InfoPage,			MaxRunicon,			l2InfoMenu, 		0, 5}, //2
    {InfoPage,     WarnIcon,   		WarnPage,			MaxRunicon,			l2WarnMenu, 		0, 14}, //3
    {InfoPage,     SetIcon,    		SetPage,			MaxRunicon,   	    l2SetMenu, 		0, 15}, //4

    {WarnPage,     RunICon,    		RunPage,			MaxRunicon,			l1RunPageMenu, 		0, 1}, //1
    {WarnPage,     InfoIcon,   		InfoPage,			MaxRunicon,			l2InfoMenu, 		0, 5}, //2
    {WarnPage,     SetIcon,    		SetPage,			MaxRunicon,   	    l2SetMenu, 		0, 15}, //4

    {SetPage,     RunICon,    		RunPage,			MaxRunicon,			l1RunPageMenu, 		0, 1}, //1
    {SetPage,     InfoIcon,   		InfoPage,			MaxRunicon,			l2InfoMenu, 		0, 5}, //2
    {SetPage,     WarnIcon,   		WarnPage,			MaxRunicon,			l2WarnMenu, 		0, 14}, //3
    {SetPage,     SetIcon,    		SetPage,			MaxRunicon,   	    l2SetMenu, 		0, 15}, //4

};



image_info2_t *searchTable(unsigned char currentPage,  unsigned char currentIconNum, unsigned char iconIndex,
                           unsigned char arrib, unsigned char arrib2)
{
    image_info2_t *tmp;
    uint16_t i = 0;
    for (i = 0; i < 0xfff; i++)
    {
        if (currentPage == imageTable[i].currentPage)
        {
            if ((currentIconNum == imageTable[i].currentPageIcon ||
                    imageTable[i].currentPageIcon == RandomIcon)
                    && imageTable[i].arrib == arrib &&
                    arrib2 ==  imageTable[i].arribSlave)
            {
                tmp = (image_info2_t *)&images[imageTable[i].image_index[iconIndex]];
                break;
            }
        }
    }
    if (i == 0xfff)
        return NULL;
    else
        return tmp;
}
uint32_t searchBmpTable(unsigned char currentPage,  unsigned char currentIconNum, unsigned char iconIndex,
                        unsigned char arrib, unsigned char arrib2)
{
    uint32_t tmp;
    uint16_t i = 0;
    for (i = 0; i < 0xfff; i++)
    {
        if (currentPage == imageTable[i].currentPage)
        {
            if ((currentIconNum == imageTable[i].currentPageIcon ||
                    imageTable[i].currentPageIcon == RandomIcon)
                    && imageTable[i].arrib == arrib &&
                    arrib2 ==  imageTable[i].arribSlave)
            {
                tmp = imageTable[i].image_index[iconIndex];
                break;
            }
        }
    }
    if (i == 0xfff)
        return NULL;
    else
        return tmp;
}
/*
0:status
1:mode
*/
//image_info_t *searchTable(unsigned char dat, uint16_t pointType, unsigned char bakc, Menu_table_t *pageInfo)
//{
//    image_info_t *tmp;
//    uint32_t i;
//    for (i = 0; i < 0xfff; i++)
//    {
//        if (pageInfo->currentMenuNum == imageTable[i].currentPage)
//        {
//            if (imageTable[i].pointType == pointType)
//            {
//                if (pointType == 8)
//                {
//                    if ((dat & imageTable[i].status) == imageTable[i].status)
//                    {
//                        if (bakc == 1 && imageTable[i].backEnable == 1&&
//							pageInfo->currentIconNum == imageTable[i].currentIconNum)
//                            tmp = (image_info_t *)&images[imageTable[i].imageBackIndex];
//                        else
//                            tmp = (image_info_t *)&images[imageTable[i].image_index];
//                        break;
//                    }
//                }
//                else
//                {
//                    if (dat == imageTable[i].status)
//                    {
//                        if (bakc == 1 && imageTable[i].backEnable == 1&&
//							pageInfo->currentIconNum == imageTable[i].currentIconNum)
//                            tmp = (image_info_t *)&images[imageTable[i].imageBackIndex];
//                        else
//                            tmp = (image_info_t *)&images[imageTable[i].image_index];
//                        break;
//                    }
//                }
//            }
//        }
//
//
//    }
//    if (i == 0xfff)
//        return NULL;
//    else
//        return tmp;
//}
unsigned char searchFontTable(unsigned char index, uint16_t arrib,
                              unsigned int offset, unsigned int x,
                              unsigned int y, unsigned char fontSize)
{

    uint32_t i;
    unsigned int j = 0, offsetTmp = 0;
    bold_font_char_info_t *info;
    for (i = 0; i < 0xfff; i++)
    {

        if (arrib == fontTable[i].arrib)
        {
            if (index == fontTable[i].index)
            {
                offsetTmp = 0;
               
                for (j = 0; j < fontTable[i].len; j++)
                {
                    info = find_char(fontTable[i].fontBuf[j], fontSize);
                    ST7789_DrawImage(x + offsetTmp, y, info->width, info->height, (uint16_t *)piexls(info->offset, fontSize));
                    offsetTmp = offsetTmp + info->width + 5;
                }
                break;
            }
        }

    }
    if (i == 0xfff)
        return NULL;
    else
        return 0;
}
unsigned char *searchFont2Table(unsigned char index, uint16_t arrib)
{

    uint32_t i;
    unsigned int j = 0, offsetTmp = 0;
    bold_font_char_info_t *info;
    for (i = 0; i < 0xfff; i++)
    {

        if (arrib == fontTable[i].arrib)
        {
            if (index == fontTable[i].index)
            {
                offsetTmp = 0;
				return fontTable[i].fontBuf;
                break;
            }
        }

    }
}
void menuStatusBar()
{
    unsigned char i, j;
    uint16_t offset;
	uint32_t bmpIndex;
    image_info2_t *images_point;
    offset = 0;
    //	for(i = 0;i< MaxRunicon; i++)
    {

		


	
        for (j = 1; j < menuTableU.currentMenuNum; j++)
        {

            if (menuTableU.currentIconNum == (j - 1))
                bmpIndex = searchBmpTable(mainRandomPage,
                                           RandomIcon,
                                           j - 1,
                                           4, 1);
            else
                bmpIndex = searchBmpTable(mainRandomPage,
                                           RandomIcon,
                                           j - 1,
                                           4, 0);
            //  images_point = searchTable(controllerCustom.settingU.status, 0, 0,&menuTable[menuTaskIndex]);
           // ST7789_DrawImage(offset,     0, images_point->width, images_point->height, (uint16_t *)images_point->data);//36
			show_image(bmpMap[bmpIndex].name, offset, 0);//run stop 0 60 34   TJ

			offset = offset + imageInfo.width;
        }
//			unsigned char *p = searchFont2Table(controllerCustom.settingU.ctrlMode, 0);
//		draw_string_ex(50, 136, BLACK, 24, p, WHITE);	



		
        unsigned char *p = searchFont2Table(menuTableU.currentMenuNum, 1);
		draw_string_ex(offset, 10, BLACK, 32, p, WHITE);
        j = j + 1;
        offset = offset + 64;
        offset = 320 - (SetPage - j + 1) * 48;
        for (; j <= SetPage; j++)
        {
            //            images_point = searchTable(mainRandomPage,
            //                                       RandomIcon,
            //                                       j - 1,
            //                                       4, 0);

            if (menuTableU.currentIconNum == (j - 1))
                bmpIndex = searchBmpTable(mainRandomPage,
                                           RandomIcon,
                                           j - 1,
                                           4, 1);
            else
                bmpIndex = searchBmpTable(mainRandomPage,
                                           RandomIcon,
                                           j - 1,
                                           4, 0);

            //  images_point = searchTable(controllerCustom.settingU.status, 0, 0,&menuTable[menuTaskIndex]);
//            ST7789_DrawImage(offset,     0, images_point->width, images_point->height, (uint16_t *)images_point->data);//36
			show_image(bmpMap[bmpIndex].name, offset, 0);//run stop 0 60 34   TJ

			offset = offset + imageInfo.width;
        }
    }
}
void l1StandbyMenu(void)
{


    image_info2_t *images_point;
    uint32_t bmpIndex;
    unsigned char buf[64];
//    images_point = searchTable(menuTableU.currentMenuNum,
//                               menuTableU.currentIconNum,
//                               controllerCustom.settingU.status,
//                               0, 0);

    show_image(bmpMap[36].name, 0, 60);//run stop 0 60 34   TJ
    for (unsigned char i = 0; i < 64; i++)
        buf[i] = 0;

    if (controllerCustom.settingU.display.units == 0)
        sprintf(buf, "%.1f l/h", controllerCustom.dispFlow);
    else
        sprintf(buf, "%.1f gal/h", controllerCustom.dispFlow);

    draw_string_ex(100, 69, BLACK, 24, buf, WHITE);

    show_image("YXZT1", 240, 60);//run stop 0 60 34	 TJ

    bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                              menuTableU.currentIconNum,
                              controllerCustom.settingU.ctrlMode,
                              1, 0);

    show_image(bmpMap[bmpIndex].name, 0, 120);//run stop 0 60 34   TJ
	unsigned char *p = searchFont2Table(controllerCustom.settingU.ctrlMode, 0);
	draw_string_ex(50, 146, BLACK, 24, p, WHITE);


//    searchFontTable(controllerCustom.settingU.ctrlMode, 0, 16, 50,  146, 16);
    for (unsigned char i = 0; i < 64; i++)
        buf[i] = 0;
    if (controllerCustom.settingU.display.units == 0)
        sprintf(buf, "%.1fl/h", controllerCustom.dispFlow);
    else
        sprintf(buf, "%.1fgal/h", controllerCustom.dispFlow);

    draw_string_ex(140, 144, BLACK, 24, buf, WHITE);
    bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                              menuTableU.currentIconNum,
                              controllerCustom.settingU.ctrlMode,
                              1, 1);

    show_image(bmpMap[bmpIndex].name, 240, 120);//run stop 0 60 34   TJ

    unsigned char index;

    if (controllerCustom.settingU.slowMode == 1)
        index = 0;
    else   if (controllerCustom.settingU.sysLock == 1)
        index = 1;
    else   if (controllerCustom.settingU.autoVent == 1)
        index = 2;
    else   if (controllerCustom.settingU.bus.status == 1)
        index = 3;
    else
        index = 4;
    bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                              menuTableU.currentIconNum,
                              index,
                              2, 0);

    show_image(bmpMap[bmpIndex].name, 0, 180);//run stop 0 60 34   TJ

    bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                              menuTableU.currentIconNum,
                              controllerCustom.settingU.fault,
                              3, 0);

    show_image(bmpMap[bmpIndex].name, 260, 180);//run stop 0 60 34   TJ


}
extern const unsigned char gImage_L127[480];
void l1RunPageMenu(void)
{

    image_info2_t *images_point;
    unsigned char buf[64];
	 uint32_t bmpIndex;
    menuStatusBar();


   bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                               menuTableU.currentIconNum,
                               controllerCustom.settingU.status,
                               0, 0);

    show_image(bmpMap[bmpIndex].name, 0, 60);//run stop 0 60 34   TJ
	
    for (unsigned char i = 0; i < 64; i++)
        buf[i] = 0;

    if (controllerCustom.settingU.display.units == 0)
        sprintf(buf, "%.1f l/h", controllerCustom.dispFlow);
    else
        sprintf(buf, "%.1f gal/h", controllerCustom.dispFlow);
  
   // ST7789_WriteString(100,  69, buf, Font_16x26, BLACK, WHITE);
//    ST7789_DrawImage(240,   60, images[52].width, images[52].height, (uint16_t *)images[52].data);

//    images_point = searchTable(menuTableU.currentMenuNum,
//                               menuTableU.currentIconNum,
//                               controllerCustom.settingU.ctrlMode,
//                               1, 0);
//    ST7789_DrawImage(0,     120, images_point->width, images_point->height, (uint16_t *)images_point->data);
   // searchFontTable(controllerCustom.settingU.ctrlMode, 0, 24, 50,  136, 16);
		
		 draw_string_ex(100, 69, BLACK, 24, buf, WHITE);
		 show_image("YXZT1", 240, 60);//run stop 0 60 34	 TJ
     bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                               menuTableU.currentIconNum,
                               controllerCustom.settingU.ctrlMode,
                               1, 0);

    show_image(bmpMap[bmpIndex].name, 0, 120);//run stop 0 60 34   TJ		
		unsigned char *p = searchFont2Table(controllerCustom.settingU.ctrlMode, 0);
	draw_string_ex(50, 136, BLACK, 24, p, WHITE);	
		
		
    for (unsigned char i = 0; i < 64; i++)
        buf[i] = 0;
    if (controllerCustom.settingU.display.units == 0)
        sprintf(buf, "%.1fl/h", controllerCustom.dispFlow);
    else
        sprintf(buf, "%.1fgal/h", controllerCustom.dispFlow);
    //
    //ST7789_WriteString(140, 144, buf, Font_11x18, BLACK, WHITE);

   // images_point = searchTable(menuTableU.currentMenuNum,
   //                            menuTableU.currentIconNum,
   //                            controllerCustom.settingU.ctrlMode,
    //                           1, 1);
    //ST7789_DrawImage(240,     120, images_point->width, images_point->height, (uint16_t *)images_point->data);//36

		
		draw_string_ex(140, 144, BLACK, 24, buf, WHITE);
     bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                               menuTableU.currentIconNum,
                               controllerCustom.settingU.ctrlMode,
                               1, 1);

    show_image(bmpMap[bmpIndex].name, 240, 120);//run stop 0 60 34   TJ		
		
    unsigned char index;

    if (controllerCustom.settingU.slowMode == 1)
        index = 0;
    else   if (controllerCustom.settingU.sysLock == 1)
        index = 1;
    else   if (controllerCustom.settingU.autoVent == 1)
        index = 2;
    else   if (controllerCustom.settingU.bus.status == 1)
        index = 3;
    else
        index = 4;
    bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                               menuTableU.currentIconNum,
                               index,
                               2, 0);
		show_image(bmpMap[bmpIndex].name, 0, 180);//run stop 0 60 34   TJ	
   // ST7789_DrawImage(0,     180, images_point->width, images_point->height, (uint16_t *)images_point->data);//36
    bmpIndex = searchBmpTable(menuTableU.currentMenuNum,
                               menuTableU.currentIconNum,
                               controllerCustom.settingU.fault,
                               3, 0);
   // ST7789_DrawImage(260,     180, images_point->width, images_point->height, (uint16_t *)images_point->data);//36
show_image(bmpMap[bmpIndex].name, 260, 180);//run stop 0 60 34   TJ	


}
void l2InfoMenu(void)
{
    menuStatusBar();
}
void l2WarnMenu(void)
{
    menuStatusBar();
}

void l2SetMenu(void)
{
    menuStatusBar();
}
void l3InfoMenu(void)
{
    menuStatusBar();
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
    if (menuTaskIndex == 0 &&
            (getEncoderKey(ENCODER_KEY_INDEX)->keyStatus == 1 ||
             getEncoderKey(SW2_KEY_INDEX)->keyEnter == 1 ||
             getEncoderKey(SW1_KEY_INDEX)->keyEnter == 1))
    {
        menuTableU.changeMenuSig = 1;
        menuTableU.currentMenuNum = RunPage;
        getEncoderKey(ENCODER_KEY_INDEX)->keyStatus = 0;
        getEncoderKey(SW2_KEY_INDEX)->keyEnter = 0;
        getEncoderKey(SW1_KEY_INDEX)->keyEnter = 0;
    }

    if (getEncoderKey(ENCODER_KEY_INDEX)->keyStatus == 1)
    {
        if (getEncoderKey(ENCODER_KEY_INDEX)->key_dir == BACKWORD)
        {
            if (menuTableU.currentIconNum <
                    menuTable[menuTaskIndex].currMaxIconNum)
                menuTableU.currentIconNum ++;
        }
        else
        {
            if (menuTableU.currentIconNum > 0)
                menuTableU.currentIconNum --;
        }
        getEncoderKey(ENCODER_KEY_INDEX)->keyStatus = 0;
        // menuTableU.mode = 1;

    }
    if (getEncoderKey(ENCODER_KEY_INDEX)->keyEnter == 1)
    {
        getEncoderKey(ENCODER_KEY_INDEX)->keyEnter = 0;
        menuTableU.changeMenuSig = 1;
    }
    if (getEncoderKey(SW1_KEY_INDEX)->keyEnter == 1)
    {
        getEncoderKey(SW1_KEY_INDEX)->keyEnter = 0;
        if (controllerCustom.settingU.status == stop)
            controllerCustom.settingU.status = run;
        else  if (controllerCustom.settingU.status == run)
            controllerCustom.settingU.status = stop;
    }

}
void menu_init(void)
{
    menuTaskIndex = 0;
    menuTableU.changeMenuSig = 0;
    menuTableU.currentIconNum = 0;
    menuTableU.currentMenuNum = 1;
}

// L1
//    {StandByPage, RunICon,   		    RunPage, 			1,							l1StandbyMenu, 		0, 0,1},
//    {RunPage,     RunICon,    		RunPage,			MaxRunicon,					l1RunPageMenu, 		0, 0,1},
//    {RunPage,     InfoIcon,   		InfoPage,			MaxRunicon,					l2InfoMenu, 		0, 0,5},
//    {RunPage,     WarnIcon,   		WarnPage,			MaxRunicon,					l2WarnMenu, 		0, 0,14},
//    {RunPage,     SetIcon,    		SetPage,			MaxRunicon,   	    		l2SetMenu, 			0, 0,15},
//    //L2
//    {InfoPage,    date,      		InfoPage,			1,							l2InfoMenu, 		0, 0},
//    {InfoPage,    counter,   		CounterPage,		MaxCounterIcon,				l3InfoMenu, 		0, 0},
//    {InfoPage,    maintance, 		InfoPage,			1,							l2InfoMenu, 		0, 0},
//    {InfoPage,    servicePack,  	InfoPage,			1,							l2InfoMenu, 		0, 0},
//    {InfoPage,    restore, 	    	InfoPage,			1,							l2InfoMenu, 		0, 0},
//    {InfoPage,    softVer, 	   		InfoPage,			1,							l2InfoMenu, 		0, 0},
//    {InfoPage,    motorCtrl, 		InfoPage,			1,							l2InfoMenu, 		0, 0},
//    {InfoPage,    hwVer, 			InfoPage,			1,							l2InfoMenu, 		0, 0},
//    {InfoPage,    serNo, 			InfoPage,			1,							l2InfoMenu, 		0, 0},

void searchMenuTable()
{
    unsigned int i, j;
    unsigned char flag;
    flag = 0;
    for (i = 0; i < 255; i++)
    {
        if (menuTable[i].currentMenuNum == menuTableU.currentMenuNum)
        {
            if (menuTable[i].currentIconNum == menuTableU.currentIconNum)
            {
                menuTaskIndex = menuTable[i].index;
                menuTableU.currentIconNum = menuTable[menuTaskIndex].currentIconNum ;
                menuTableU.currentMenuNum = menuTable[menuTaskIndex].currentMenuNum;
                break;
            }
        }
    }


    //    for (j = 0; j < 255; j++)
    //    {
    //        if (menuTable[i].nextMenuNum == menuTable[j].currentMenuNum
    //                && (menuTable[i].nextMenuNum != menuTable[i].currentMenuNum))
    //        {
    //            menuTaskIndex = j;
    //            menuTableU.currentMenuNum = menuTable[j].currentMenuNum;
    //            flag = 1;
    //            if (menuTable[i].nextMenuNum != menuTable[i].currentMenuNum)
    //                menuTableU.currentIconNum = 0;
    //            break;
    //        }
    //        else
    //        {
    //            if (menuTable[i].nextMenuNum == menuTable[j].currentMenuNum
    //                    && (menuTable[j].nextMenuNum == menuTable[j].currentMenuNum))
    //            {
    //                if (menuTable[j].currentIconNum == menuTableU.currentIconNum)
    //                {
    //                    menuTaskIndex = j;
    //                    menuTableU.currentMenuNum = menuTable[j].currentMenuNum;
    //                    break;
    //                }
    //            }
    //        }
    //    }

}
void menu_manage()
{
    menuInterfaceMange();
    if (menuTableU.changeMenuSig == 1) //切换画面
    {
        menuTableU.changeMenuSig = 0;//press ok button change menu
        searchMenuTable();
        //        menuTaskIndex = menuTable[ menuTaskIndex].index;

        //menuTableU. = 0;
        ST7789_Fill_Color(WHITE);
    }
    menuTable[menuTaskIndex].Current_Operation();
}
