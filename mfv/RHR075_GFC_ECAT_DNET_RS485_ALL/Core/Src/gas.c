#include "gas.h"
#include "string.h"
gas_datbase_stru gas_datbase;
void gas_init(void)
{
//    unsigned char pb[256];
//    float gas_coff[4];
//	unsigned char i;
//	//************HE***********************
//	i=0;
//	  gas_datbase.id = 1;
//    gas_coff[i++] = 1.667;
//    gas_coff[i++] = 4.002;
//    gas_coff[i++] = 1.115;
//    gas_coff[i++] =0.363025319;
//    sprintf(pb, "%u,%s,%f",gas_datbase.id, "HE", gas_coff);

//	//************AR***********************
//	i=0;
//	  gas_datbase.id = 4;
//    gas_coff[i++] = 1.667;
//    gas_coff[i++] = 39.948;
//    gas_coff[i++] = 1.269;
//    gas_coff[i++] =0.114902092;
//    sprintf(pb, "%u,%s,%f",gas_datbase.id, "AR", gas_coff);	

}
gas_datbase_stru *get_gas(void)
{
    return &gas_datbase;
}




