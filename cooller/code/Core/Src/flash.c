#include "flash.h"
#include "stmflash.h"
#include "config.h"
flash_struct flash_usr;


void flash_save()
{
    uint32_t addr, data_tmp;
    float tmp;
    unsigned char pb[4];

	     Erase_page(Application_1_Addr, 1);

	
		 addr = Application_1_Addr;
		 memcpy(pb, &flash_usr.header, 4);
		 WriteFlash(addr, pb, 4);
		 addr = addr + 4;
	
		 memcpy(pb, &flash_usr.used_len, 4);
		 WriteFlash(addr, pb, 4);
		 addr = addr + 4;
	
		 memcpy(pb, &getConfig()->max_T, 4);
		 WriteFlash(addr, pb, 4);
		 addr = addr + 4;
	
		 memcpy(pb, &getConfig()->min_T, 4);
		 WriteFlash(addr, pb, 4);
		 addr = addr + 4;
	
		 memcpy(pb, &getConfig()->warn_T, 4);
		 WriteFlash(addr, pb, 4);
		 addr = addr + 4;
	
		 memcpy(pb, &getConfig()->record_interval, 4);
		 WriteFlash(addr, pb, 4);
		 addr = addr + 4;
	
	
		 memcpy(pb, &getConfig()->record_time, 4);
		 WriteFlash(addr, pb, 4);
		 addr = addr + 4;
		 
		 memcpy(pb, &getConfig()->power_save, 4);
		 WriteFlash(addr, pb, 4);
		 addr = addr + 4;
	
	 }

void flash_init()
{
    uint32_t addr, data_tmp;
    float tmp;
    unsigned char pb[4];
    ReadFlash(Application_1_Addr, &flash_usr.header, 1);//读header
    if (flash_usr.header == 0x5a)
    {
        addr = Application_1_Addr + 1;
        ReadFlash(addr, pb, 4);
        addr = addr + 4;
        memcpy(&flash_usr.used_len, pb, 4);
        flash_usr.vailabe_len = flash_usr.total_len - flash_usr.used_len;//已写入字节数


        getConfig()->max_T = *(float *)(addr);//上限温度
        addr = addr + 4;
        getConfig()->min_T = *(float *)(addr);//下限温度
        addr = addr + 4;
        getConfig()->warn_T = *(float *)(addr);//报警限温度
        addr = addr + 4;
        getConfig()->record_interval = *(uint32_t *)(addr);//记录间隔
        addr = addr + 4;

        getConfig()->record_time = *(uint32_t *)(addr);//记录时间
        addr = addr + 4;
        getConfig()->power_save = *(uint32_t *)(addr);//休眠模式
        addr = addr + 4;
    }
    else
    {
         flash_usr.header = 0x5a;
		 flash_usr.total_len = 128 * 1024;
		 flash_usr.used_len = 0;
		 flash_usr.vailabe_len = flash_usr.total_len;
	
		 getConfig()->max_T = -10;
		 getConfig()->min_T = -30;
		 getConfig()->record_interval = 30;//s
		 getConfig()->warn_T = -35;
		 getConfig()->power_save = 15;//s
		 getConfig()->update_T = 0;
		 getConfig()->record_time = 0;
		flash_save();


    }

}

flash_struct *get_flash_status()
{
    return &flash_usr;
}


