#include "update.h"
#include "stmflash.h"
#include "config.h"
/* 升级完成,并写入一个标志位 */
void boot_jump(void)
{
	uint8_t update_flag = 0xAA;
	   uint8_t tmp;
	   if(OTA_UPDATE_TO == APP2)
	   {
		   WriteFlash((Application_2_Addr + Application_Size - 8), &update_flag, 1);
		   update_flag = 0x10;
		   ReadFlash(OTA_NUM_ADDR,&tmp,1);
		   if(tmp!=update_flag)
		   {
			   Erase_page(OTA_NUM_ADDR, 1);
			   WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
	
		   }
	   }
	
	   if(OTA_UPDATE_TO == APP1)
	   {
		   WriteFlash((Application_1_Addr + Application_Size - 8), &update_flag, 1);
		   update_flag = 0x20;
		   ReadFlash(OTA_NUM_ADDR,&tmp,1);
	
		   if(tmp!=update_flag)
		   {
			   Erase_page(OTA_NUM_ADDR, 1);
			   WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
	
		   }
	   }

}
void Update_Success(void)
{
    uint8_t update_flag = 0xAA;
    uint8_t tmp;
    if(OTA_UPDATE_TO == APP2)
    {
        WriteFlash((Application_2_Addr + Application_Size - 8), &update_flag, 1);
        update_flag = 0x20;
        ReadFlash(OTA_NUM_ADDR,&tmp,1);
        if(tmp!=update_flag)
        {
            Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);

        }
    }

    if(OTA_UPDATE_TO == APP1)
    {
        WriteFlash((Application_1_Addr + Application_Size - 8), &update_flag, 1);
        update_flag = 0x10;
        ReadFlash(OTA_NUM_ADDR,&tmp,1);

        if(tmp!=update_flag)
        {
            Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);

        }
    }



    //uint8_t update_flag = 0xBB;
    //WriteFlash((Application_1_Addr + Application_Size - 8), &update_flag, 1);
}

/* 升级任务 */
void OTA_Task(void)
{
    if (get_config()->reboot_flag == 1)
    {
        get_config()->reboot_flag = 0;
        Update_Success();
        HAL_NVIC_SystemReset();
    }
}
