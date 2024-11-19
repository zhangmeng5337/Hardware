#include "config.h"
#include "stmflash.h"



static CONFIG_stru config_usr;


void config_save()
{
    //uint32_t addr_count = 0;
    uint32_t buf[1024], len, index;
    if (config_usr.update_setting)
    {

        index = 1;
        buf[0] = 0x5a;
        len = strlen(get_config()->user);
        memcpy(buf + index, get_config()->user, len);
        index = index + len;

        len = strlen(get_config()->password);
        memcpy(buf + index, get_config()->password, len);
        index = index + len;


        len = strlen(get_config()->mqtt_ip);
        memcpy(buf + index, get_config()->mqtt_ip, len);
        index = index + len;


        len = strlen(get_config()->mqtt_port);
        memcpy(buf + index, get_config()->mqtt_port, len);
        index = index + len;



        len = strlen(get_config()->version);
        memcpy(buf + index, get_config()->version, len);
        index = index + len;


        len = strlen(get_config()->http_ip);
        memcpy(buf + index, get_config()->http_ip, len);
        index = index + len;


        len = strlen(get_config()->http_port);
        memcpy(buf + index, get_config()->http_port, len);
        index = index + len;

        len = sizeof(get_config()->set_tout);
        memcpy(buf + index, &get_config()->set_tout, len);
        index = index + len;

        len = sizeof(get_config()->set_tindoor);
        memcpy(buf + index, &get_config()->set_tindoor, len);
        index = index + len;

        len = sizeof(get_config()->set_up_period);
        memcpy(buf + index, &get_config()->set_up_period, len);
        index = index + len;

        len = sizeof(get_config()->mode);
        memcpy(buf + index, &get_config()->mode, len);
        index = index + len;


        len = sizeof(get_config()->fault_mask);
        memcpy(buf + index, &get_config()->fault_mask, len);
        index = index + len;


        Erase_page(CONFIG_Addr, 1); //²Á³ý2ÉÈÇø
        WriteFlash((CONFIG_Addr), (uint8_t *)(buf), index);
        config_usr.update_setting = 0;

    }
}
void config_init()
{

    uint32_t head, index, len;
    unsigned char buf[4];
    ReadFlash(CONFIG_Addr, (uint8_t *)&head, 1);


//    if (head == 0x5a)
//    {


//        index = 1;
//        len = sizeof(get_config()->user);
//        ReadFlash(CONFIG_Addr + index, (uint8_t *)get_config()->user, len);

//        index = index + len;
//        len = sizeof(get_config()->password);
//        ReadFlash(CONFIG_Addr + index, (uint8_t *)get_config()->password, len);

//        index = index + len;
//        len = sizeof(get_config()->mqtt_ip);
//        ReadFlash(CONFIG_Addr + index, (uint8_t *)get_config()->mqtt_ip, len);

//        index = index + len;
//        len = sizeof(get_config()->mqtt_port);
//        ReadFlash(CONFIG_Addr + index, (uint8_t *)get_config()->mqtt_port, len);


//        index = index + len;
//        len = sizeof(get_config()->version);
//        ReadFlash(CONFIG_Addr + index, (uint8_t *)get_config()->version, len);


//        index = index + len;
//        len = sizeof(get_config()->http_ip);
//        ReadFlash(CONFIG_Addr + index, (uint8_t *)get_config()->http_ip, len);

//        index = index + len;
//        len = sizeof(get_config()->http_port);
//        ReadFlash(CONFIG_Addr + index, (uint8_t *)get_config()->http_port, len);


//        index = index + len;
//        len = sizeof(get_config()->set_tout);
//        ReadFlash(CONFIG_Addr + index, buf, len);
//        memcpy(&get_config()->set_tout, buf, len);

//        index = index + len;
//        len = sizeof(get_config()->set_tindoor);
//        ReadFlash(CONFIG_Addr + index, buf, len);
//        memcpy(&get_config()->set_tindoor, buf, len);

//        index = index + len;
//        len = sizeof(get_config()->set_up_period);
//        // ReadFlash(CONFIG_Addr + index, &get_config()->set_up_period, len);
//        ReadFlash(CONFIG_Addr + index, buf, len);
//        memcpy(&get_config()->set_up_period, buf, len);


//        index = index + len;
//        len = sizeof(get_config()->mode);
//        ReadFlash(CONFIG_Addr + index, &get_config()->mode, len);

//        index = index + len;
//        len = sizeof(get_config()->fault_mask);
//        ReadFlash(CONFIG_Addr + index, buf, len);
//        memcpy(&get_config()->fault_mask, buf, len);

//    }
//    else
    {
        sprintf(get_config()->user, "%s", "usr");
        sprintf(get_config()->password, "%s", "7895621");
        sprintf(get_config()->mqtt_ip, "%s", "39.106.131.169");
        sprintf(get_config()->mqtt_port, "%s", "1883");
        sprintf(get_config()->version, "%s", VERSION);
		                    if(OTA_UPDATE_TO == APP2)
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
        get_config()->set_up_period = 10;
        get_config()->mode = 2;
        get_config()->fault_mask = 0xffffffff;
        get_config()->update_setting = 1;
        config_save();


    }




    sprintf(get_config()->mqtt_mpubtopic, "%s", "dev_pub_");
    //sprintf(get_config()->mqtt_subtopic,"%s%s", SUB1,get_config()->user_id);
    get_config()->seq_count = 1;

    //memcpy(&get_config()->sub_sring[1][0],"dev_sub_temp_",strlen("dev_sub_temp_"));
    //memcpy(&get_config()->sub_sring[2][0],"dev_sub_ctrl_",strlen("dev_sub_ctrl_"));
    get_config()->Erase_flag = 1;




    //setting params



        unsigned char read_flag = APP_NUM;
        Erase_page(OTA_NUM_ADDR, 1);
        WriteFlash((OTA_NUM_ADDR), &read_flag, 1);





}
CONFIG_stru *get_config()
{
    return &config_usr;
}

