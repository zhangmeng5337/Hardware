#include "com.h"
#include "sys.h"

com_stru com_usr;
void recv_timeout(void)
{
    if (com_usr.lock_port > 0)
    {
        com_usr.timeout = com_usr.timeout + 1;
    }
    else
        com_usr.timeout = 0;

    if (com_usr.timeout >= TIME_OUT)
    {
        com_usr.lock_port = 0;
        com_usr.timeout = 0;

    }

}
unsigned char slave_to_master()
{
    unsigned char result = 1;
    switch (com_usr.lock_port)
    {
        case 1:
            P31 = P02;
            break;
        case 2:
            P31 = P25;
            break;
        case 3:
            P31 = P23;
            break;
        case 4:
            P31 = P21;
            break;
        case 5:
            P31 = P43;
            break;
        case 6:
            P31 = P42;
            break;
        case 7:
            P31 = P36;
            break;
        case 8:
            P31 = P50;
            break;
        default:
            break;
    }
    return result;
}
void com_recv_proc()
{ 		
    if (P31 == 0)
        com_usr.master_recv = 1;
    if (P02 == 0)
    {    com_usr.timeout = 0;
        com_usr.rec_num[0] = 1;//recv 1
        com_usr.lock_port = 1;
			
			 		P04 = !P04;				//²âÊÔ¶Ë¿Ú
    }

    else   if (P25 == 0)
    {com_usr.timeout = 0;
        com_usr.rec_num[1] = 1;//recv 2
        com_usr.lock_port = 2;
    }

    else   if (P23 == 0)
    {com_usr.timeout = 0;
        com_usr.rec_num[2] = 1;//recv 3
        com_usr.lock_port = 3;
    }

    else   if (P21 == 0)
    {com_usr.timeout = 0;
        com_usr.rec_num[3] = 1;//recv 4
        com_usr.lock_port = 4;
    }

    else   if (P43 == 0)
    {com_usr.timeout = 0;
        com_usr.rec_num[4] = 1;//recv 5
        com_usr.lock_port = 5;
    }

    else   if (P42 == 0)
    {com_usr.timeout = 0;
        com_usr.rec_num[5] = 1;//recv 6
        com_usr.lock_port = 6;
    }

    else   if (P36 == 0)
    {com_usr.timeout = 0;
        com_usr.rec_num[6] = 1;//recv 7
        com_usr.lock_port = 7;
    }

    else   if (P50 == 0)
    {com_usr.timeout = 0;
        com_usr.rec_num[7] = 1;//recv 8
        com_usr.lock_port = 8;
    }
   // else
       // com_usr.lock_port = 0;
}
void com_tx_slaver()
{
    P03 = P30;
    P24 = P30;
    P22 = P30;
    P20 = P30;
    P44 = P30;
    P41 = P30;
    P37 = P30;
    P51 = P30;


}
void com_tx_to_master()
{
//    if (com_usr.lock_port != 0)
//    {
//        if (slave_to_master() == 1)
//            P31 = 1;
//        else
//            P31 = 0;

//    }
}
