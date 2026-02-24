#include "config.h"
#include "stmflash.h"
#include "ai_proc.h"
#include "modbus.h"
#include "FuzzyPID.h"



 CONFIG_stru config_usr;

//get_config()->valid_flag = 1;
//	  index = 256;
//
//	 // len = sizeof(get_config()->mode);
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)&head, 1);
//	  get_config()->mode = head;
//	  if(get_config()->mode>=OFF_MODE)
//		  get_config()->mode = OFF_MODE;
//	  index = index + 1;
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)&head, 1);
//	  if(head>=10)
//	  get_config()->dev_size = DEV_SIZE;
//	  else
//	  get_config()->dev_size = head;		  
//	  index = index + 1;
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)&head, 1);
//	  if(head>=10)
//	  get_config()->energy_size = ENERGY_SIZE;
//	  else
//	  get_config()->energy_size = head; 		  
//	  index = index + 1;
//
//	  
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
//	  index = index + 4;
//	  get_config()->set_up_period = uint32Tofloat(buf);
//
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
//	  index = index + 4;
//	  get_pid_params()->kp_u = uint32Tofloat(buf);
//
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
//	  index = index + 4;
//	  get_pid_params()->ki_u = uint32Tofloat(buf);
//
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
//	  index = index + 4;
//	  get_pid_params()->kd_u = uint32Tofloat(buf);
//	  
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 1);
//	  index = index + 1;
//	  get_config()->instru_num = buf[0];
//	  
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
//	  index = index + 4;
//	  get_pid_params()->out_max = uint32Tofloat(buf);
//
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
//	  index = index + 4;
//	  get_pid_params()->out_min = uint32Tofloat(buf);
//	  ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
//	  index = index + 4;
//	  get_config()->set_up_period = uint32Tofloat(buf);
//
// len = sizeof(get_config()->mode);


void config_save()
{
    //uint32_t addr_count = 0;
    uint8_t buf[512];
    unsigned int len, index;
	float tmp;
    if (config_usr.update_setting)
    {
        ReadFlash(CONFIG_Addr, buf, 512);
			  buf[0] = 0x5a;
        Erase_page(CONFIG_Addr, 1); //²Á³ý2ÉÈÇø
        index = 256;

        len = sizeof(get_config()->mode);
        memcpy(buf + index, &get_config()->mode, len);
        index = index + len;
        len = sizeof(get_config()->dev_size);
        memcpy(buf + index, &get_config()->dev_size, len);
        index = index + len;
		len = sizeof(get_config()->energy_size);

        memcpy(buf + index, &get_config()->energy_size, len);
        index = index + len;

		
		unsigned char buf2[4];
			tmp = get_config()->set_up_period;
		floatTouint32(tmp, buf2);
		memcpy(buf + index, buf2, 4);
		len = 4;
		index = index + len;

	   floatTouint32(get_pid_params()->kp_u, buf2);
		memcpy(buf + index, buf2, 4);
	   len = 4;
	   index = index + len;
	   
	   floatTouint32(get_pid_params()->ki_u, buf2);
		memcpy(buf + index, buf2, 4);
	   len = 4;
	   index = index + len;
	   
	   floatTouint32(get_pid_params()->kd_u, buf2);
		memcpy(buf + index, buf2, 4);
	   len = 4;
	   index = index + len;
	  
		memcpy(buf + index, &get_config()->instru_num, 1);
	   len = 1;
	   index = index + len;
	   
	   floatTouint32(get_pid_params()->out_max, buf2);
		memcpy(buf + index, buf2, 4);
	   len = 4;
	   index = index + len;
	   floatTouint32(get_pid_params()->out_min, buf2);
		memcpy(buf + index, buf2, 4);
	   len = 4;
	   index = index + len;
	   floatTouint32(get_config()->set_up_period, buf2);
		memcpy(buf + index, buf2, 4);
	   len = 4;
	   index = index + len;

       WriteFlashBytes((CONFIG_Addr), (uint8_t *)(buf), 512);
        config_usr.update_setting = 0;

    }
}
void config_init()
{

    unsigned char head;
    unsigned int index;
    unsigned char buf[4];
    index = 0;
    ReadFlash(CONFIG_Addr + index, (uint8_t *)&head, 1);
    //len = 0;
    config_usr.count = 0;
	get_config()->valid_flag = 0;
    if ((head & 0xff) == 0x5a)
    {

        get_config()->valid_flag = 1;
        index = 256;

       // len = sizeof(get_config()->mode);
        ReadFlash(CONFIG_Addr + index, (uint8_t *)&head, 1);
        get_config()->mode = head;
        if(get_config()->mode>=OFF_MODE)
			get_config()->mode = OFF_MODE;
        index = index + 1;
        ReadFlash(CONFIG_Addr + index, (uint8_t *)&head, 1);
		if(head>=10)
        get_config()->dev_size = DEV_SIZE;
		else
        get_config()->dev_size = head;			
		index = index + 1;
        ReadFlash(CONFIG_Addr + index, (uint8_t *)&head, 1);
		if(head>=10)
        get_config()->energy_size = ENERGY_SIZE;
		else
        get_config()->energy_size = head;			
		index = index + 1;

		
        ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
		index = index + 4;
		get_config()->set_up_period = uint32Tofloat(buf);

        ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
		index = index + 4;
		get_pid_params()->kp_u = uint32Tofloat(buf);

        ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
		index = index + 4;
		get_pid_params()->ki_u = uint32Tofloat(buf);

        ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
		index = index + 4;
		get_pid_params()->kd_u = uint32Tofloat(buf);
		
        ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 1);
		index = index + 1;
		get_config()->instru_num = buf[0];
		
        ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
		index = index + 4;
		get_pid_params()->out_max = uint32Tofloat(buf);

        ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
		index = index + 4;
		get_pid_params()->out_min = uint32Tofloat(buf);
        ReadFlash(CONFIG_Addr + index, (uint8_t *)buf, 4);
		index = index + 4;
		get_config()->set_up_period = uint32Tofloat(buf);


		//ReadFlash(CONFIG_Addr + index, buf, 4);
        //uint32Tofloat(unsigned char *buf)

   
//        index = index + len;
//        len = sizeof(get_config()->fault_mask);
//        ReadFlash(CONFIG_Addr + index, buf, len);
//        memcpy(&get_config()->fault_mask, buf, len);

    }
    else
    {
        sprintf(get_config()->user, "%s", "usr");
        sprintf(get_config()->password, "%s", "7895621");
        sprintf(get_config()->mqtt_ip, "%s", "39.106.131.169");
        sprintf(get_config()->mqtt_port, "%s", "1883");
        sprintf(get_config()->version, "%s", VERSION);
        if (OTA_UPDATE_TO == UAPP2)
            sprintf(get_config()->http_ip, "%s",
                    "http://39.106.131.169:666/ota/b/");

        else
            sprintf(get_config()->http_ip, "%s",
                    "http://39.106.131.169:666/ota/a/");
        sprintf(get_config()->http_port, "%s", "666");
        get_config()->machine = 1;
        get_config()->update_firm = 0;
        get_config()->set_tout = 45;
        get_config()->set_tindoor = 25;
        get_config()->reboot = 0;
        get_config()->set_up_period =5;
		get_pid_params()->out_max = 50;
		get_pid_params()->out_min = 20;
        get_config()->mode = OFF_MODE;
		get_config()->instru_num = DELI;
        get_config()->dev_size = DEV_SIZE;
        get_config()->energy_size = ENERGY_SIZE;

        get_config()->fault_mask = 0xffffffff;
        get_config()->update_setting = 1;
        get_config()->timeout = 0;
        config_save();


    }

    sprintf(get_config()->user, "%s", "usr");
    sprintf(get_config()->password, "%s", "7895621");
    sprintf(get_config()->mqtt_ip, "%s", "39.106.131.169");
    sprintf(get_config()->mqtt_port, "%s", "1883");
    sprintf(get_config()->version, "%s", VERSION);
    if (OTA_UPDATE_TO == UAPP2)
        sprintf(get_config()->http_ip, "%s",
                "http://39.106.131.169:666/ota/b/");

    else
        sprintf(get_config()->http_ip, "%s",
                "http://39.106.131.169:666/ota/a/");
    sprintf(get_config()->http_port, "%s", "666");
    get_config()->machine = 1;
    get_config()->update_firm = 0;
    get_config()->set_tout = 45;
    get_config()->set_tindoor = 25;
    get_config()->reboot = 0;
    get_config()->set_up_period = 5;
    //get_config()->mode = 2;
    get_config()->fault_mask = 0xffffffff;
    get_config()->update_setting = 1;
    get_config()->timeout = 0;
    //config_save();



    sprintf(get_config()->mqtt_mpubtopic, "%s", "dev/pub/");
    //sprintf(get_config()->mqtt_subtopic,"%s%s", SUB1,get_config()->user_id);
    get_config()->seq_count = 1;

    //memcpy(&get_config()->sub_sring[1][0],"dev_sub_temp_",strlen("dev_sub_temp_"));
    //memcpy(&get_config()->sub_sring[2][0],"dev_sub_ctrl_",strlen("dev_sub_ctrl_"));
    get_config()->Erase_flag = 1;

    get_config()->tin_index = AI_WATER_T_IN_INDEX;
    get_config()->to_index = AI_WATER_T_O_INDEX;

    get_config()->pin_index = AI_PUMP_F_INDEX;
    get_config()->po_index = AI_PUMP_E_INDEX;
	get_config()->connectTimeOut = 0;
    get_config()->private_reg = 0xff;
	//get_config()->instru_num = DELI;



}
CONFIG_stru *get_config()
{
    return &config_usr;
}

