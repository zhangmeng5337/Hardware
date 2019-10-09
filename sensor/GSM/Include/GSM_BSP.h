#ifndef GSM_BSP
#define GSM_BSP

#define ON  1
#define OFF 0
#define MAX_STEP   5

#define Max_Network_thres 30
void modulePowerOn(unsigned char pwr_ctrl);
void Send_Comm(unsigned char* comm,unsigned short len);
#endif
