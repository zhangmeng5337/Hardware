#include "flash.h"
#include "stmflash.h"
#include "config.h"
flash_struct flash_usr;

/*
header---- len---max---min---warn---record_interval---record_time---power_save
*/
void flash_save()
{
    uint32_t  data_tmp,addr;
    float tmp;
    unsigned char pb[4];
         addr = Application_1_Addr;
	     Erase_page(Application_1_Addr, 1);
		 memcpy(pb, &flash_usr.header, 4);
		 WriteFlash(addr, pb, 4);addr = addr + 4;
		// flash_usr.used_len= flash_usr.used_len +4;
		 
		 memcpy(pb, &flash_usr.used_len, 4);
		 WriteFlash(addr, pb, 4);addr = addr + 4;
		// flash_usr.used_len= flash_usr.used_len +4;

		 floatTouint32(getConfig()->max_T,pb);//上限温度
		 WriteFlash(addr, pb, 4);addr = addr + 4;
		 //flash_usr.used_len= flash_usr.used_len +4;
		 
		 floatTouint32(getConfig()->min_T,pb);//下限温度
		 WriteFlash(addr, pb, 4);addr = addr + 4;
		// flash_usr.used_len= flash_usr.used_len +4;

		 floatTouint32(getConfig()->warn_T,pb);//上限温度
		 WriteFlash(addr, pb, 4);addr = addr + 4;
		 //flash_usr.used_len= flash_usr.used_len +4;
		 
		 memcpy(pb, &getConfig()->record_interval, 4);
		 WriteFlash(addr, pb, 4);addr = addr + 4;
		// flash_usr.used_len= flash_usr.used_len +4;

		 memcpy(pb, &getConfig()->record_time, 4);
		 WriteFlash(addr, pb, 4);addr = addr + 4;
		 //flash_usr.used_len= flash_usr.used_len +4;

		 memcpy(pb, &getConfig()->power_save, 4);
		 WriteFlash(addr, pb, 4);addr = addr + 4;
		//flash_usr.used_len= flash_usr.used_len +4;


	

	
//		 addr = Application_1_Addr;
//		 memcpy(pb, &flash_usr.header, 4);
//		 WriteFlash(addr, pb, 4);
//		 addr = addr + 4;
//	
//		 memcpy(pb, &flash_usr.used_len, 4);
//		 WriteFlash(addr, pb, 4);
//		 addr = addr + 4;
//	
//		 memcpy(pb, &getConfig()->max_T, 4);
//		 WriteFlash(addr, pb, 4);
//		 addr = addr + 4;
//	
//		 memcpy(pb, &getConfig()->min_T, 4);
//		 WriteFlash(addr, pb, 4);
//		 addr = addr + 4;
//	
//		 memcpy(pb, &getConfig()->warn_T, 4);
//		 WriteFlash(addr, pb, 4);
//		 addr = addr + 4;
//	
//		 memcpy(pb, &getConfig()->record_interval, 4);
//		 WriteFlash(addr, pb, 4);
//		 addr = addr + 4;
//	
//	
//		 memcpy(pb, &getConfig()->record_time, 4);
//		 WriteFlash(addr, pb, 4);
//		 addr = addr + 4;
//		 
//		 memcpy(pb, &getConfig()->power_save, 4);
//		 WriteFlash(addr, pb, 4);
//		 addr = addr + 4;
	
	 }
/*
header---- len---max---min---warn---record_interval---record_time---power_save
*/

void flash_init()
{
    uint32_t  data_tmp,addr;
    float tmp;
    unsigned char pb[4];
    ReadFlash(Application_1_Addr, &flash_usr.header, 4);//读header
    if (flash_usr.header == 0x5a)
    {
        addr = Application_1_Addr + 4;
        ReadFlash(addr, pb, 4);addr = addr + 4;
        
        memcpy(&flash_usr.used_len, pb, 4);
        flash_usr.vailabe_len = flash_usr.total_len - flash_usr.used_len;//已写入字节数
        getConfig()->addr = Application_2_Addr+flash_usr.used_len;
		ReadFlash(addr, pb, 4);addr = addr + 4;
		uint32Tofloat(getConfig()->max_T,pb);//上限温度

		ReadFlash(addr, pb, 4);addr = addr + 4;
		uint32Tofloat(getConfig()->min_T,pb);//下限温度

		ReadFlash(addr, pb, 4);addr = addr + 4;
		uint32Tofloat(getConfig()->warn_T,pb);//报警限温度

		ReadFlash(addr, pb, 4);addr = addr + 4;
		memcpy(&getConfig()->record_interval,pb,4);//记录间隔

		ReadFlash(addr, pb, 4);addr = addr + 4;
		memcpy(&getConfig()->record_time,pb,4);//记录时间

		ReadFlash(addr, pb, 4);addr = addr + 4;
		memcpy(&getConfig()->power_save,pb,4);//休眠模式		
    }
    else
    {
         flash_usr.header = 0x5a;
		 flash_usr.total_len = 128 * 1024;
		 flash_usr.used_len = 0;
		 flash_usr.vailabe_len = flash_usr.total_len;

		 getConfig()->addr = Application_2_Addr;
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


