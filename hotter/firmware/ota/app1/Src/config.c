#include "config.h"
#include "stmflash.h"



CONFIG_stru config_usr;
void config_save()
{
    //uint32_t addr_count = 0;
    uint32_t buf[1024], len, index;
    if (config_usr.update_setting == 1)
    {

        index = 1;
        buf[0] = 0x5a;
        len = sizeof(get_congfig()->user);
        memcpy(buf+index, get_congfig()->user, len);
        index = index + len;

        len = sizeof(get_congfig()->password);
        memcpy(buf+index, get_congfig()->password, len);
        index = index + len;


        len = sizeof(get_congfig()->mqtt_ip);
        memcpy(buf+index, get_congfig()->mqtt_ip, len);
        index = index + len;


        len = sizeof(get_congfig()->mqtt_port);
        memcpy(buf+index, get_congfig()->mqtt_port, len);
        index = index + len;



        len = sizeof(get_congfig()->version);
        memcpy(buf+index, get_congfig()->version, len);
        index = index + len;


        len = sizeof(get_congfig()->http_ip);
        memcpy(buf+index, get_congfig()->http_ip, len);
        index = index + len;


        len = sizeof(get_congfig()->http_port);
        memcpy(buf+index, get_congfig()->http_port, len);
        index = index + len;

        len = sizeof(get_congfig()->set_tout);
        memcpy(buf+index, &get_congfig()->set_tout, len);
        index = index + len;

        len = sizeof(get_congfig()->set_tindoor);
        memcpy(buf+index, &get_congfig()->set_tindoor, len);
        index = index + len;

        len = sizeof(get_congfig()->set_up_period);
        memcpy(buf+index, &get_congfig()->set_up_period, len);
        index = index + len;

        len = sizeof(get_congfig()->mode);
        memcpy(buf+index, &get_congfig()->mode, len);
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
        len = sizeof(get_congfig()->user);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_congfig()->user, len);

        index = index + len;
        len = sizeof(get_congfig()->password);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_congfig()->password, len);

        index = index + len;
        len = sizeof(get_congfig()->mqtt_ip);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_congfig()->mqtt_ip, len);

        index = index + len;
        len = sizeof(get_congfig()->mqtt_port);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_congfig()->mqtt_port, len);


        index = index + len;
        len = sizeof(get_congfig()->version);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_congfig()->version, len);


        index = index + len;
        len = sizeof(get_congfig()->http_ip);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_congfig()->http_ip, len);

        index = index + len;
        len = sizeof(get_congfig()->http_port);
        ReadFlash(CONFIG_Addr + index, (uint8_t * )get_congfig()->http_port, len);


        index = index + len;
        len = sizeof(get_congfig()->set_tout);
        ReadFlash(CONFIG_Addr + index, buf, len);
        memcpy(&get_congfig()->set_tout, buf, len);

        index = index + len;
        len = sizeof(get_congfig()->set_tindoor);
        ReadFlash(CONFIG_Addr + index, buf, len);
        memcpy(&get_congfig()->set_tindoor, buf, len);

        index = index + len;
        len = sizeof(get_congfig()->set_up_period);
        // ReadFlash(CONFIG_Addr + index, &get_congfig()->set_up_period, len);
        ReadFlash(CONFIG_Addr + index, buf, len);
        memcpy(&get_congfig()->set_up_period, buf, len);


        index = index + len;
        len = sizeof(get_congfig()->mode);
        ReadFlash(CONFIG_Addr + index, &get_congfig()->mode, len);


    }
    else
    {
        sprintf(get_congfig()->user, "%s", "usr");
        sprintf(get_congfig()->password, "%s", "7895621");
        sprintf(get_congfig()->mqtt_ip, "%s", "192.168.1.23");
        sprintf(get_congfig()->mqtt_port, "%s", "8080");
        sprintf(get_congfig()->version, "%s", "V3.0");
        sprintf(get_congfig()->http_ip, "%s", "192.168.1.23");
        sprintf(get_congfig()->http_port, "%s", "8080");
        get_congfig()->machine = 1;
        get_congfig()->update_firm = 0;
        get_congfig()->set_tout = 45;
        get_congfig()->set_tindoor = 35;
        get_congfig()->reboot = 0;
        get_congfig()->set_up_period = 60;
        get_congfig()->mode = 1;
        get_congfig()->update_setting = 1;
        config_save();


    }



    sprintf(get_congfig()->mqtt_msubtopic, "%s/%s", "mqtt_mub",get_congfig()->user_id);
    sprintf(get_congfig()->mqtt_subtopic, "%s/%s", "mqtt_sub",get_congfig()->user_id);





    //setting params






}
CONFIG_stru *get_congfig()
{
    return &config_usr;
}

