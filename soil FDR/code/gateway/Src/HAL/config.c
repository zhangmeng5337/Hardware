#include "config.h"
config_stru config;
void configInit(void)
{
	 //tcp server ip
    config.TcpServer[0] = 192;
    config.TcpServer[1] = 168;
    config.TcpServer[2] = 1;
    config.TcpServer[3] = 2;
    config.TcpServerPort = 8080;
 //终端地址
    config.srcaddr[0] = 0;
    config.srcaddr[1] = 0;
    config.srcaddr[2] = 0;
    config.srcaddr[3] = 0;
    config.srcaddr[4] = 0;
    config.srcaddr[5] = 0;
    config.srcaddr[6] = 0;
    config.srcaddr[7] = 2;
 // 网关地址
    config.destaddr[0] = 0;
    config.destaddr[1] = 0;
    config.destaddr[2] = 0;
    config.destaddr[3] = 0;
    config.destaddr[4] = 0;
    config.destaddr[5] = 0;
    config.destaddr[6] = 0;
    config.destaddr[7] = 3;
	config.airrate = 7;//空速
	config.channel = 30;//信道
}
config_stru * getConfig(void)
{

    return &config;
}