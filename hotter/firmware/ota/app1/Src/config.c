#include "config.h"
#include "stmflash.h"



CONFIG_stru config_usr;
void config_save()
{
    //uint32_t addr_count = 0;
    uint32_t buf[1024], len, index;
    if (config_usr.update_setting )
    {

        index = 1;
        buf[0] = 0x5a;
        len = sizeof(get_config()->user);
        memcpy(buf+index, get_config()->user, len);
        index = index + len;

        len = sizeof(get_config()->password);
        memcpy(buf+index, get_config()->password, len);
        index = index + len;


        len = sizeof(get_config()->mqtt_ip);
        memcpy(buf+index, get_config()->mqtt_ip, len);
        index = index + len;


        len = sizeof(get_config()->mqtt_port);
        memcpy(buf+index, get_config()->mqtt_port, len);
        index = index + len;



        len = sizeof(get_config()->version);
        memcpy(buf+index, get_config()->version, len);
        index = index + len;


        len = sizeof(get_config()->http_ip);
        memcpy(buf+index, get_config()->http_ip, len);
        index = index + len;


        len = sizeof(get_config()->http_port);
        memcpy(buf+index, get_config()->http_port, len);
        index = index + len;

        len = sizeof(get_config()->set_tout);
        memcpy(buf+index, &get_config()->set_tout, len);
        index = index + len;

        len = sizeof(get_config()->set_tindoor);
        memcpy(buf+index, &get_config()->set_tindoor, len);
        index = index + len;

        len = sizeof(get_config()->set_up_period);
        memcpy(buf+index, &get_config()->set_up_period, len);
        index = index + len;

        len = sizeof(get_config()->mode);
        memcpy(buf+index, &get_config()->mode, len);
        index = index + len;


        len = sizeof(get_config()->fault_mask);
        memcpy(buf+index, &get_config()->fault_mask, len);
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


    if (head == 0x5a)
    {


        index = 1;
        len = sizeof(get_config()->user);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_config()->user, len);

        index = index + len;
        len = sizeof(get_config()->password);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_config()->password, len);

        index = index + len;
        len = sizeof(get_config()->mqtt_ip);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_config()->mqtt_ip, len);

        index = index + len;
        len = sizeof(get_config()->mqtt_port);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_config()->mqtt_port, len);


        index = index + len;
        len = sizeof(get_config()->version);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_config()->version, len);


        index = index + len;
        len = sizeof(get_config()->http_ip);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_config()->http_ip, len);

        index = index + len;
        len = sizeof(get_config()->http_port);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_config()->http_port, len);


        index = index + len;
        len = sizeof(get_config()->set_tout);
        ReadFlash(CONFIG_Addr + index, buf, len);
        memcpy(&get_config()->set_tout, buf, len);

        index = index + len;
        len = sizeof(get_config()->set_tindoor);
        ReadFlash(CONFIG_Addr + index, buf, len);
        memcpy(&get_config()->set_tindoor, buf, len);

        index = index + len;
        len = sizeof(get_config()->set_up_period);
        // ReadFlash(CONFIG_Addr + index, &get_config()->set_up_period, len);
        ReadFlash(CONFIG_Addr + index, buf, len);
        memcpy(&get_config()->set_up_period, buf, len);


        index = index + len;
        len = sizeof(get_config()->mode);
        ReadFlash(CONFIG_Addr + index, &get_config()->mode, len);

	index = index + len;
	len = sizeof(get_config()->fault_mask);
	ReadFlash(CONFIG_Addr + index, buf, len);
 memcpy(&get_config()->fault_mask , buf, len);

    }
    else
    {
        sprintf(get_config()->user, "%s", "usr");
        sprintf(get_config()->password, "%s", "7895621");
        sprintf(get_config()->mqtt_ip, "%s", "192.168.1.23");
        sprintf(get_config()->mqtt_port, "%s", "8080");
        sprintf(get_config()->version, "%s", "V3.0");
        sprintf(get_config()->http_ip, "%s", "192.168.1.23");
        sprintf(get_config()->http_port, "%s", "8080");
        get_config()->machine = 1;
        get_config()->update_firm = 0;
        get_config()->set_tout = 45;
        get_config()->set_tindoor = 35;
        get_config()->reboot = 0;
        get_config()->set_up_period = 60;
        get_config()->mode = 1;
		get_config()->fault_mask = 0xffffffff;
        get_config()->update_setting = 1;
        config_save();


    }



    sprintf(get_config()->mqtt_mpubtopic, "%s/%s", "mqtt_mub",get_config()->user_id);
    sprintf(get_config()->mqtt_subtopic, "%s/%s", "mqtt_sub",get_config()->user_id);





    //setting params






}
CONFIG_stru *get_config()
{
    return &config_usr;
}

