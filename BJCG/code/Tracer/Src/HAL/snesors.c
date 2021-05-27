#include "sensors.h"
#include "lis2mdl.h"
#include "lsm6ds3tr.h"
#include "calman.h"
#include "BMP3882.h"
#include "sys.h"
#include "gps.h"
sensors_stru *sensorsp;
AccAng_stru *AccAng_usrp;
bmp3_data   *comp_datap;



unsigned char gpsBuf[GPS_COUNT];
float FilterData[SN];////滤波后的imu数据
void sensors_Init(void)
{
    if(BMP388_Init()==1)
        printf("BMP388 Init Failure\n");  //输出测量累积误差
    if(lis2mdl_init()==1)
        printf("lis2mdl Init Failure\n");  //输出测量累积误差
    if(lsm6ds3tr_init()==1)
        printf("lsm6ds3tr Init Failure\n");  //输出测量累积误差

}

void sensors_sample(void)
{
    BMP388_Get_PaT();
    lis2mdl_read_data_simple();
    lsm6ds3tr_c_read_data_polling();


}
unsigned char *getGPS()//获取gps位置信息
{
    unsigned char *p;
    unsigned char result;

    parseGpsBuffer(gpsBuf);
    return gpsBuf;
}
//float *GetSensorsData(void)
//{
//   return 0;//;FilterData;
//}
void snesors_process(void)
{
    float pbt[SENSORS_COUNT];
    unsigned char i,j;
    unsigned char len;
    len = 0;

    sensors_sample();
    getGPS();//获取gps位置信息
    sensorsp = GetMagnetic();
    AccAng_usrp = GetAccAngtic();
    comp_datap = GetPressure();

    for(i = 0; i<3; i++)
    {
        FilterData[i+len]=AccAng_usrp->acceleration_mg[i];
    }
    len =len +3;
    for(i = 0; i<3; i++)
    {
        FilterData[i+len]=AccAng_usrp->angular_rate_mdps[i];
    }
    len =len +3;
    FilterData[len]=AccAng_usrp->temperature_degC;
    len =len +1;
    for(i = 0; i<3; i++)
    {
        FilterData[i+len]=sensorsp->magnetic_mG[i];
    }
    // memcpy(FilterData+len,sensorsp->magnetic_mG,3);
    len =len +3;
    //memcpy(FilterData+len,&comp_datap->pressure,1);
    FilterData[len]=comp_datap->pressure;
    len =len +1;
    // memcpy(FilterData+len,&comp_datap->temperature,1);
    FilterData[len]=comp_datap->temperature;
    len =len +1;



    len = 0;
    for(i = 0; i<3; i++)
    {
        pbt[i+len]=AccAng_usrp->acceleration_mg[i];
    }
    len =len +3;
    for(i = 0; i<3; i++)
    {
        pbt[i+len]=AccAng_usrp->angular_rate_mdps[i];
    }
    len =len +3;
    pbt[len]=AccAng_usrp->temperature_degC;
    len =len +1;
    for(i = 0; i<3; i++)
    {
        pbt[i+len]=sensorsp->magnetic_mG[i];
    }
    // memcpy(FilterData+len,sensorsp->magnetic_mG,3);
    len =len +3;
    //memcpy(FilterData+len,&comp_datap->pressure,1);
    pbt[len]=comp_datap->pressure;
    len =len +1;
    // memcpy(FilterData+len,&comp_datap->temperature,1);
    pbt[len]=comp_datap->temperature;
    len =len +1;


//    len = 0;
//    memcpy(pbt,AccAng_usrp->acceleration_mg,3);
//    len =len +3;
//    memcpy(pbt+len,AccAng_usrp->angular_rate_mdps,3);
//    len =len +3;
//    memcpy(pbt+len,&AccAng_usrp->temperature_degC,1);
//    len =len +1;
//    memcpy(pbt+len,sensorsp->magnetic_mG,3);
//    len =len +3;
//    memcpy(pbt+len,&comp_datap->pressure,1);
//    len =len +1;
//    memcpy(pbt+len,&comp_datap->temperature,1);
//    len =len +1;

    /***********************************************************
                      数据滤波处理
    ************************************************************/


    for(j = 0; j<SN; j++)
    {
        FilterData[j] = Claman(&(FilterData[j]),0.004,10,j+1);

    }
    /*pb[i++] = Claman(&(AccAng_usrp->acceleration_mg[0]),0.008,10,1);
    pb[i++] = Claman(&(AccAng_usrp->acceleration_mg[1]),0.008,10,1);
    pb[i++] =Claman(&(AccAng_usrp->acceleration_mg[2]),0.008,10,1);

    pb[i++]= Claman(&(AccAng_usrp->angular_rate_mdps[0]),0.008,10,1);
    pb[i++]= Claman(&(AccAng_usrp->angular_rate_mdps[1]),0.008,10,1);
    pb[i++]= Claman(&(AccAng_usrp->angular_rate_mdps[2]),0.008,10,1);

    pb[i++]= Claman(&(sensorsp->magnetic_mG[0]),0.008,10,1);
    pb[i++]= Claman(&(sensorsp->magnetic_mG[1]),0.008,10,1);
    pb[i++]= Claman(&(sensorsp->magnetic_mG[2]),0.008,10,1);
    pb[i++]= Claman(&(AccAng_usrp->temperature_degC),0.008,10,1);
    pb[i++]= Claman(&(AccAng_usrp->temperature_degC),0.008,10,1);
    pb[i++]= Claman(&(AccAng_usrp->temperature_degC),0.008,10,1);*/

    for(j =0; j<(SN/2-1); j++)
    {
        printf("   %f    %f  ",pbt[j],FilterData[j]);  //输出测量累积误差

    }
    printf("	%f	 %f\n",pbt[j],FilterData[j]);  //输出测量累积误差



}

