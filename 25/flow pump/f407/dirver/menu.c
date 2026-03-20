#include "menu.h"
#include "key.h"
#include "st7789.h"
#include "all_icons.h"
#include "devinfo.h"
#include "controller.h"
#include "my_font.h"
//python .\pic_py.py  .\icons\  .\lib\ -b -w 32 -H 32
//python pic_py.py icons/ lib/all_icons.h -b -c -w 32 -H 32
//python font.py "手动" "C:/Windows/Fonts/msyh.ttc" 16 hello.h
//python generate_fontlib.py chars.txt "C:/Windows/Fonts/msyh.ttc" 16 -o fixed_font.h -n my_font -W 32 -H 32 --encoding=gbk

//python generate_fontlib.py chars.txt "C:/Windows/Fonts/msyh.ttc" 16 -o fixed_font.c -n bold_font -W 16 -H 16 --bold 1 --encoding=gbkextern controller_stru controllerCustom;
//python generate_fontlib.py common_chars.txt "C:/Windows/Fonts/msyh.ttc" 16 -o fixed_font.c -n bold_font -W 16 -H 16 --bold 1 --encoding=gbk
//python gb2312_fontlib.py common_chars.txt "C:/Windows/Fonts/msyh.ttc" 16 -o gb2312_font.h -n gb2312_font -W 16 -H 16 --bold 1 --encoding=gb2312
/*
*   [9] flow.bmp (flow)
	[37] 剩余批量.bmp (sheng_yu_pi_liang)
*   [40] 实际背压.bmp (shi_ji_bei_ya)
*   [49] 输入电流.bmp (shu_ru_dian_liu)
*   [32] 下一计量时间.bmp (xia_yi_ji_liang_shi_jian)
*   [42] 总计投加容积.bmp (zong_ji_tou_jia_rong_ji)
*/
extern controller_stru controllerCustom;
font_stru fontTable[] =
{
    {0, {0x624B, 0x52A8}, 2, 0}
};
image_stru imageTable[] =
{
    {stop,		 							36, 	0,0,0},//controllerCustom.settingU.status
    {run,		 							51, 	0,0,0},
    {standby,	 							26, 	0,0,0},
    {discharge,	 							2, 		0,0,0},
    {vOut,		 							29, 	0,0,0},
    {vIn,		 							28, 	0,0,0},
    {manualMode, 							43, 	1,0,0},//controllerCustom.settingU.ctrlMode
    {pulseMode, 							21, 	1,0,0},
    {cur0To20Mode, 							3, 		1,0,0},
    {cur4To20Mode, 							3, 		1,0,0},
    {batchCtrlMode, 						21, 	1,0,0},
    {addByCycleMode, 						20, 	1,0,0},
    {addByWeeklyMode, 						7, 		1,0,0},
    {manualMode,							9,		2,0,0},//controllerCustom.settingU.ctrlMode
    {pulseMode,		    					9,		2,0,0},
    {cur0To20Mode, 							49, 	2,0,0},
    {cur4To20Mode, 							49, 	2,0,0},
    {addByCycleMode, 						37, 	2,0,0},
    {addByWeeklyMode, 						37, 	2,0,0},
    {addByWeeklyMode, 						37, 	2,0,0},
    {1,										25,		3,0,0},//controllerCustom.settingU.slowMode
    {0,										10,		4,0,0},//
    {1,										44,		5,0,0},//controllerCustom.settingU.sysLock
    {1,										46,		6,0,0},//controllerCustom.settingU.autoVent
    {1,										41,		7,0,0},//controllerCustom.settingU.bus.status
    {emptyFault,							8,		8,0,0},
    {extStop,								39,		8,0,0},
    {liquidlowFault,						15,		8,0,0},
    {lineShortFault,						24,		8,0,0},
    {ciuFault,								0,		8,0,0},
    {maintanceFault,						45,		8,0,0},
    {valvedischargeFault,					27,		8,0,0},
    {overTFault,							1,		8,0,0},
    {overLoadFault,							18,		8,0,0},
    {outValveFault,							17,		8,0,0},
    {1, 34, 9}

};

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
    {StandByPage, RunICon,   		RunPage, 		1,			l1StandbyMenu, 0, 0},


    {RunPage,    RunICon,    		RunPage,		MaxRunicon,			l1RunPageMenu, 0, 0},
    {RunPage,    InfoIcon,   		InfoPage,		MaxInfoIcon,		l2InfoMenu, 0, 0},
    {RunPage,    WarnIcon,   		WarnPage,		MaxWarnIcon,		l2WarnMenu, 0, 0},
    {RunPage,    SetIcon,    		SetPage,		MaxSetIcon,   	    l2SetMenu, 0, 0},
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

    {WarnPage,    WarnIcon, 		WarnPage,		MaxWarnIcon,	l2WarnMenu, 0, 0},

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
    {inOutPage,  relay1, 	      	relay1Page,	    		7,				l4SetRelay1Menu, 0, 0},
    {inOutPage,  relay2, 	      	relay2Page,	    		9,				l4SetRelay2Menu, 0, 0},

};
/*
0:status
1:mode
*/
image_info_t *searchTable(unsigned char dat, uint16_t mode,unsigned char bakc)
{
    image_info_t *tmp;
    uint32_t i;
    for (i = 0; i < 0xfff; i++)
    {
        if (imageTable[i].pointType == mode)
        {
            if (mode == 8)
            {
                if ((dat & imageTable[i].status) == imageTable[i].status)
                {  
                   if(bakc == 1 && imageTable[i].backEnable == 1)
				   	tmp = (image_info_t *)&images[imageTable[i].imageBackIndex];
				   else
                    tmp = (image_info_t *)&images[imageTable[i].image_index];
                    break;
                }
            }
            else
            {
                if (dat == imageTable[i].status)
                {
                   if(bakc == 1 && imageTable[i].backEnable == 1)
				   	tmp = (image_info_t *)&images[imageTable[i].imageBackIndex];
				   else
                    tmp = (image_info_t *)&images[imageTable[i].image_index];
                    break;
                }
            }


        }

    }
    if (i == 0xfff)
        return NULL;
    else
        return tmp;
}
unsigned char searchFontTable(unsigned char index, uint16_t mode,
                              unsigned int offset, unsigned int x,
                              unsigned int y)
{

    uint32_t i;
    unsigned int j = 0, offsetTmp = 0;
    bold_font_char_info_t *info;
    for (i = 0; i < 0xfff; i++)
    {

        if (index == fontTable[i].index)
        {
            offsetTmp = 0;
            for (j = 0; j < fontTable[i].len; j++)
            {
                info = find_char(fontTable[i].fontBuf[j]);
                ST7789_DrawImage(x + offsetTmp, y, info->width, info->height, (uint16_t *)piexls(info->offset));
                offsetTmp = offsetTmp + offset;
            }
            break;
        }
    }
    if (i == 0xfff)
        return NULL;
    else
        return 0;
}
void l1StandbyMenu(void)
{


    image_info_t *images_point;
    unsigned char buf[64];
//    if (menuTable[menuTaskIndex].currentIconNum > RunICon)
//    {
//         images_point = searchTable(controllerCustom.settingU.status, 0,1);
//    }
//	else
    images_point = searchTable(controllerCustom.settingU.status, 0,0);
    ST7789_DrawImage(0,     60, images_point->width, images_point->height, (uint16_t *)images_point->data);//36

	for (unsigned char i = 0; i < 64; i++)
        buf[i] = 0;
	
    if (controllerCustom.settingU.display.units == 0)
        sprintf(buf, "%.1f   l/h", controllerCustom.dispFlow);
    else
        sprintf(buf, "%.1f   gal/h", controllerCustom.dispFlow);
	
    ST7789_WriteString(80,  69, buf, Font_16x26, BLACK, WHITE);
    ST7789_DrawImage(240,   60, images[52].width, images[52].height, (uint16_t *)images[52].data);
    images_point = searchTable(controllerCustom.settingU.ctrlMode, 1,0);
    ST7789_DrawImage(0,     120, images_point->width, images_point->height, (uint16_t *)images_point->data);
    for (unsigned char i = 0; i < 64; i++)
        buf[i] = 0;
    if (controllerCustom.settingU.display.units == 0)
        sprintf(buf, "%.1fl/h", controllerCustom.dispFlow);
    else
        sprintf(buf, "%.1fgal/h", controllerCustom.dispFlow);
	
    ST7789_WriteString(120, 144, buf, Font_11x18, BLACK, WHITE);

    images_point = searchTable(controllerCustom.settingU.ctrlMode, 2,0);
    ST7789_DrawImage(240,     120, images_point->width, images_point->height, (uint16_t *)images_point->data);//36

    if (controllerCustom.settingU.slowMode == 1)
        images_point = searchTable(controllerCustom.settingU.slowMode, 3,0);
    else   if (controllerCustom.settingU.sysLock == 1)
        images_point = searchTable(controllerCustom.settingU.sysLock, 5,0);
    else   if (controllerCustom.settingU.autoVent == 1)
        images_point = searchTable(controllerCustom.settingU.autoVent, 6,0);
    else   if (controllerCustom.settingU.bus.status == 1)
        images_point = searchTable(controllerCustom.settingU.bus.status, 7,0);
    else
        images_point = searchTable(0, 4,0);
    ST7789_DrawImage(0,     180, images_point->width, images_point->height, (uint16_t *)images_point->data);//36


    images_point = searchTable(controllerCustom.settingU.fault, 8,0);
    ST7789_DrawImage(260,     180, images_point->width, images_point->height, (uint16_t *)images_point->data);//36
    searchFontTable(0, 0, 24, 50,  136);
    // ST7789_DrawImage(0,     60, images[36].width, images[36].height, (uint16_t *)images[36].data);//36
    //    ST7789_WriteString(80,  69, "12.3  l/h", Font_16x26, BLACK, WHITE);
    //    ST7789_DrawImage(240,   60, images[52].width, images[52].height, (uint16_t *)images[52].data);
    //    ST7789_DrawImage(0,     120, images[43].width, images[43].height, (uint16_t *)images[43].data);
    //    ST7789_WriteString(120, 144, "12.3l/h", Font_11x18, BLACK, WHITE);
    //    ST7789_DrawImage(240,   120, images[9].width, images[9].height, (uint16_t *)images[9].data);
    //    ST7789_DrawImage(0,     180, images[10].width, images[10].height, (uint16_t *)images[10].data);
    //    ST7789_DrawImage(260,   180, images[15].width, images[15].height, (uint16_t *)images[15].data);

    //    if (controllerCustom.settingU.language == chinese)
    //    {
    //        bold_font_char_info_t *info;
    //        uint16_t offset = 24, sx = 50, sy = 136;
    //
    //        info = find_char(0x624B);
    //        ST7789_DrawImage(sx, sy, info->width, info->height, (uint16_t *)piexls(info->offset));
    //        info = find_char(0x52A8);
    //        ST7789_DrawImage(sx + offset, sy, info->width, info->height, (uint16_t *)piexls(info->offset));
    //
    //    }

}
extern const unsigned char gImage_L127[480];
void l1RunPageMenu(void)
{
    //    if(menuTable[menuTaskIndex].currentIconNum == RunICon)
    //	ST7789_WriteString(10, 10, "Filled Cir.", Font_11x18, YELLOW, BLACK);
    //    if(menuTable[menuTaskIndex].currentIconNum == InfoIcon)
    //	ST7789_WriteString(10, 10, "Filled Cir.", Font_11x18, YELLOW, WHITE);
    //    if(menuTable[menuTaskIndex].currentIconNum == WarnIcon)
    //	ST7789_WriteString(10, 10, "Filled Cir.", Font_11x18, YELLOW, BLUE);
    //    if(menuTable[menuTaskIndex].currentIconNum == SetIcon)
    //	ST7789_WriteString(10, 10, "Filled Cir.", Font_11x18, YELLOW, RED);
    //ST7789_DrawImage(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT,(uint16_t *)warn1_16x18);
    //	ST7789_Test();
    //ST7789_DrawImage(0, 0, 128, 128, (uint16_t *)saber);

    /*
    -------------------------------------------------------------------------------
    -------------------------------------------------------------------------------
    -------------------------------------------------------------------------------
    -------------------------------------------------------------------------------
    */
    /*
    [26] standbyback.bmp (standbyback)
    [36] 停机.bmp (ting_ji)
    [51] 运行中.bmp (yun_xing_zhong)
    [2] airdischarge.bmp (airdischarge)
    [28] valveposIn.bmp (valveposin)
    [29] valveposOut.bmp (valveposout)*/
    /**
    	l/h
    	gal/h
    */
    /*
    *   [52] 运行状态1.bmp (yun_xing_zhuang_tai_1)
    *   [53] 运行状态2.bmp (yun_xing_zhuang_tai_2)
    *   [54] 运行状态3.bmp (yun_xing_zhuang_tai_3)
    *   [55] 运行状态4.bmp (yun_xing_zhuang_tai_4)
    */
    /*
    *   [43] 手动.bmp (shou_dong)
    	[3] analog.bmp (analog)
    *   [21] pulse.bmp (pulse)
    	[20] pro.bmp (pro)
    *   [7] counter.bmp (counter)
    */
    /*
    	l/h
    	gal/h
    */
    /*
    *   [9] flow.bmp (flow)
    	[37] 剩余批量.bmp (sheng_yu_pi_liang)
    *   [40] 实际背压.bmp (shi_ji_bei_ya)
    *   [49] 输入电流.bmp (shu_ru_dian_liu)
    *   [32] 下一计量时间.bmp (xia_yi_ji_liang_shi_jian)
    *   [42] 总计投加容积.bmp (zong_ji_tou_jia_rong_ji)
    */
    /*
    *   [25] slowmode.bmp (slowmode)
    *   [10] flowctrl.bmp (flowctrl)
    *   [44] 系统锁.bmp (xi_tong_suo)
    *   [46] 自动排气.bmp (zi_dong_pai_qi)
    *   [41] 总线.bmp (zong_xian)

    */
    /*
    *   [8] empty.bmp (empty)
    *   [39] 外部停机.bmp (wai_bu_ting_ji)
    *   [15] liquidLow.bmp (liquidlow)
    *   [24] short.bmp (short)
    *   [0] CIU.bmp (ciu)
    *   [45] 维修.bmp (wei_xiu)
    *   [18] overL.bmp (overl)
    *   [27] valvedischarge.bmp (valvedischarge)
    *   [1] OoverT.bmp (oovert)
    *   [17] outvale.bmp (outvale)
    */
    /*
    	手动
    	模拟
    	脉冲
    	批次
    	计时器
    */
    image_info_t *images_point;
    unsigned char buf[64];
    images_point = searchTable(1, 9,0);
    ST7789_DrawImage(0,     0, images_point->width, images_point->height, (uint16_t *)images_point->data);//36

    images_point = searchTable(controllerCustom.settingU.status, 0,0);
    ST7789_DrawImage(0,     60, images_point->width, images_point->height, (uint16_t *)images_point->data);//36
    for (unsigned char i = 0; i < 64; i++)
        buf[i] = 0;
    if (controllerCustom.settingU.display.units == 0)
        sprintf(buf, "%.1f   l/h", controllerCustom.dispFlow);
    else
        sprintf(buf, "%.1f   gal/h", controllerCustom.dispFlow);
    ST7789_WriteString(80,  69, buf, Font_16x26, BLACK, WHITE);

    ST7789_DrawImage(240,   60, images[52].width, images[52].height, (uint16_t *)images[52].data);
    images_point = searchTable(controllerCustom.settingU.ctrlMode, 1,0);
    ST7789_DrawImage(0,     120, images_point->width, images_point->height, (uint16_t *)images_point->data);
    for (unsigned char i = 0; i < 64; i++)
        buf[i] = 0;
    if (controllerCustom.settingU.display.units == 0)
        sprintf(buf, "%.1fl/h", controllerCustom.dispFlow);
    else
        sprintf(buf, "%.1fgal/h", controllerCustom.dispFlow);
    ST7789_WriteString(120, 144, buf, Font_11x18, BLACK, WHITE);

    images_point = searchTable(controllerCustom.settingU.ctrlMode, 2,0);
    ST7789_DrawImage(240,     120, images_point->width, images_point->height, (uint16_t *)images_point->data);//36

    if (controllerCustom.settingU.slowMode == 1)
        images_point = searchTable(controllerCustom.settingU.slowMode, 3,0);
    else   if (controllerCustom.settingU.sysLock == 1)
        images_point = searchTable(controllerCustom.settingU.sysLock, 5,0);
    else   if (controllerCustom.settingU.autoVent == 1)
        images_point = searchTable(controllerCustom.settingU.autoVent, 6,0);
    else   if (controllerCustom.settingU.bus.status == 1)
        images_point = searchTable(controllerCustom.settingU.bus.status, 7,0);
    else
        images_point = searchTable(0, 4,0);
    ST7789_DrawImage(0,     180, images_point->width, images_point->height, (uint16_t *)images_point->data);//36


    images_point = searchTable(controllerCustom.settingU.fault, 8,0);
    ST7789_DrawImage(260,     180, images_point->width, images_point->height, (uint16_t *)images_point->data);//36
    searchFontTable(0, 0, 24, 50,  136);
    // ST7789_DrawImage(0,     60, images[36].width, images[36].height, (uint16_t *)images[36].data);//36
    //    ST7789_WriteString(80,  69, "12.3  l/h", Font_16x26, BLACK, WHITE);
    //    ST7789_DrawImage(240,   60, images[52].width, images[52].height, (uint16_t *)images[52].data);
    //    ST7789_DrawImage(0,     120, images[43].width, images[43].height, (uint16_t *)images[43].data);
    //    ST7789_WriteString(120, 144, "12.3l/h", Font_11x18, BLACK, WHITE);
    //    ST7789_DrawImage(240,   120, images[9].width, images[9].height, (uint16_t *)images[9].data);
    //    ST7789_DrawImage(0,     180, images[10].width, images[10].height, (uint16_t *)images[10].data);
    //    ST7789_DrawImage(260,   180, images[15].width, images[15].height, (uint16_t *)images[15].data);

    //    if (controllerCustom.settingU.language == chinese)
    //    {
    //        bold_font_char_info_t *info;
    //        uint16_t offset = 24, sx = 50, sy = 136;
    //
    //        info = find_char(0x624B);
    //        ST7789_DrawImage(sx, sy, info->width, info->height, (uint16_t *)piexls(info->offset));
    //        info = find_char(0x52A8);
    //        ST7789_DrawImage(sx + offset, sy, info->width, info->height, (uint16_t *)piexls(info->offset));
    //
    //    }


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
	if(menuTaskIndex == 0 &&(getEncoderKey(ENCODER_KEY_INDEX)->keyStatus == 1||
		getEncoderKey(SW2_KEY_INDEX)->keyEnter == 1||
		getEncoderKey(SW1_KEY_INDEX)->keyEnter == 1))
	{
      menuTable[menuTaskIndex].changeMenuSig = 1;
	}

    if (getEncoderKey(ENCODER_KEY_INDEX)->keyStatus == 1)
    {
        if (getEncoderKey(ENCODER_KEY_INDEX)->key_dir == BACKWORD)
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
        getEncoderKey(ENCODER_KEY_INDEX)->keyStatus = 0;
        menuTable[menuTaskIndex].mode = 1;

    }
    if (getEncoderKey(ENCODER_KEY_INDEX)->keyEnter == 1)
    {
        getEncoderKey(ENCODER_KEY_INDEX)->keyEnter = 0;
        menuTable[menuTaskIndex].changeMenuSig = 1;
    }
    if (getEncoderKey(SW1_KEY_INDEX)->keyEnter == 1)
    {
        getEncoderKey(SW1_KEY_INDEX)->keyEnter = 0;
		if(controllerCustom.settingU.status == stop)
       controllerCustom.settingU.status = run;
		else  if(controllerCustom.settingU.status == run)
       controllerCustom.settingU.status = stop;
    }

}
void menu_init(void)
{
    menuTaskIndex = 0;
    menuTable[menuTaskIndex].changeMenuSig = 0;
    menuTable[menuTaskIndex].currentIconNum = 0;
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
        ST7789_Fill_Color(WHITE);
    }
    menuTable[menuTaskIndex].Current_Operation();
}
