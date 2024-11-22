#include "update.h"
#include "stmflash.h"
#include "config.h"
HAL_StatusTypeDef status = HAL_ERROR;

/* 升级完成,并写入一个标志位 */
void boot_jump(void)
{
    uint8_t update_flag = 0xAA;
    uint8_t tmp,result;
    result = 0;
    if(OTA_UPDATE_TO == APP2)
    {

        Erase_page(OTA_NUM_ADDR, 1);
        update_flag = 0x10;//now is app 1
        WriteFlash((Application_2_Addr + Application_Size - 8), &update_flag, 1);

        ReadFlash(OTA_NUM_ADDR,&tmp,1);
        while(tmp!=update_flag)
        {
            Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
            ReadFlash(OTA_NUM_ADDR,&tmp,1);
            if(result>=5)
                break;
            else
                result++;

        }
    }
    result = 0;
    if(OTA_UPDATE_TO == APP1)
    {
        Erase_page(OTA_NUM_ADDR, 1);
        update_flag = 0x20;//now is app 2
        WriteFlash((Application_1_Addr + Application_Size - 8), &update_flag, 1);

        ReadFlash(OTA_NUM_ADDR,&tmp,1);

        while(tmp!=update_flag)
        {
            Erase_page(OTA_NUM_ADDR, 1);
            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
            if(result>=5)
                break;
            else
                result++;
        }
    }

}
void Update_Success(void)
{
    uint8_t update_flag = 0xAA;
    uint8_t tmp,result;
    if(OTA_UPDATE_TO == APP2)
    {
        result = 0;
		status = HAL_ERROR;
        while(status == HAL_ERROR)
        {
            if(result>=5)
                break;
            else
                result++;
            status=Erase_page(OTA_NUM_ADDR, 1);

        }

        update_flag = 0x20;
        WriteFlash(OTA_NUM_ADDR, &update_flag, 1);
        ReadFlash(OTA_NUM_ADDR,&tmp,1);
		result = 0;
		status = HAL_ERROR;
        if(tmp!=update_flag)
        {
            while(status == HAL_ERROR)
            {
                if(result>=5)
                    break;
                else
                    result++;
                status=Erase_page(OTA_NUM_ADDR, 1);

            }

            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
            ReadFlash(OTA_NUM_ADDR,&tmp,1);


        }
    }


    if(OTA_UPDATE_TO == APP1)
    {  
       result = 0;
	   status = HAL_ERROR;
        while(status == HAL_ERROR)
        {
            if(result>=5)
                break;
            else
                result++;
            status=Erase_page(OTA_NUM_ADDR, 1);

        }

        update_flag = 0x10;
        WriteFlash(OTA_NUM_ADDR, &update_flag, 1);
        ReadFlash(OTA_NUM_ADDR,&tmp,1);
		result = 0;
		status = HAL_ERROR;

        if(tmp!=update_flag)
        {
            while(status == HAL_ERROR)
            {
                if(result>=5)
                    break;
                else
                    result++;
                status=Erase_page(OTA_NUM_ADDR, 1);

            }
            WriteFlash((OTA_NUM_ADDR), &update_flag, 1);
            ReadFlash(OTA_NUM_ADDR,&tmp,1);


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
