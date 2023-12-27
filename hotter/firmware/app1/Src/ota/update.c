#include "update.h"
#include "stmflash.h"
#include "config.h"
/* 升级完成,并写入一个标志位 */
void Update_Success(void)
{
    uint8_t update_flag = 0xAA;
    WriteFlash((Application_2_Addr + Application_Size - 8), &update_flag, 1);
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
