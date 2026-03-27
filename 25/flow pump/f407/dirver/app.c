#include "app.h"
#include "aiAo.h"
#include "di_do.h"
#include "bldc_ctronl.h"
#include "dac8551.h"
#include "pid.h"
#include "st7789.h"
#include "bsp_encoder.h"
#include "stmflash.h"
#include "sys.h"
#include "controller.h"
config_stru config_u;
extern nvmem_struct nvmem;
void loadPrams(void)
{
    load_data_from_flash();
    if (nvmem.magic_number == FLASH_HEADER)
    {
        config_u.lcdRefreshTime = nvmem.app.lcdRefreshTime;
        config_u.adcRefreshTime = nvmem.app.adcRefreshTime;
        config_u.flashRefreshTime = nvmem.app.flashRefreshTime;
		getPid()->outh = nvmem.app.outh;
		getPid()->outL = nvmem.app.outL;
    }
    else
    {
        nvmem.app.lcdRefreshTime = LCD_REFRESH_TIME;
        nvmem.app.adcRefreshTime = ADC_SAMP_TIME;
        nvmem.app.flashRefreshTime = FLASH_REFRESH_TIME;
		nvmem.app.outh = 60000;
		nvmem.app.outL = 0;	
		
		getPid()->outh = nvmem.app.outh;
		getPid()->outL = nvmem.app.outL;
        config_u.lcdRefreshTime = nvmem.app.lcdRefreshTime;
        config_u.adcRefreshTime = nvmem.app.adcRefreshTime;
        config_u.flashRefreshTime = nvmem.app.flashRefreshTime;
        nvmem.magic_number = FLASH_HEADER;
        write_to_flash(50 );
    }
    controlInit();

}
void RegisterTick(void)
{
    reset_registerTick(ADC_TICK_NO);
    reset_registerTick(LCD_TICK_NO);
    reset_registerTick(FLASH_TICK_NO);
    registerTick(ADC_TICK_NO, config_u.adcRefreshTime);
    registerTick(LCD_TICK_NO, config_u.lcdRefreshTime);
    registerTick(FLASH_TICK_NO, config_u.flashRefreshTime);
}
unsigned char initComplete = 0;
void init(void)
{
    HAL_NVIC_SetPriority(FLASH_IRQn, 1, 0); // 优先级数值根据你的项目需求设定
    HAL_NVIC_EnableIRQ(FLASH_IRQn);
    loadPrams();
    aiAoInit();
    doInit();
    dacInit();
    PID_param_init();
    bldcm_init();
    ST7789_Init();
    Encoder_Init();
    menu_init();
    RegisterTick();
	initComplete = 1;
    // TFT_init();	//ST7789
}
float cur_set = 0;
void app(void)
{
    
    if (GetTickResult(ADC_TICK_NO) == 1)
    {
        reset_registerTick(ADC_TICK_NO);
        registerTick(ADC_TICK_NO, config_u.adcRefreshTime);
        aiAoProc();
        diDoProc();
        DAC8552_Out_Put_Voltage(cur_set);
    }
    if (GetTickResult(LCD_TICK_NO) == 1)
    {
        reset_registerTick(LCD_TICK_NO);
        registerTick(LCD_TICK_NO, config_u.lcdRefreshTime);
        menu_manage();
    }
    if (GetTickResult(FLASH_TICK_NO) == 1)
    {
        reset_registerTick(FLASH_TICK_NO);
        registerTick(FLASH_TICK_NO, config_u.flashRefreshTime);
        nvmem_stat_machine();
    }
    urtc_proc();
}
