#include "main.h"
#include "bsp.h"
#include "rm3100.h"
#include "sensor.h"

		static BOOL flag;
unsigned int temptime;

float elapsed_time;

/** Temporization functions */
float get_time (void)
{


    // Colocar em fun??o
    if(flag == 0)
    {
        flag = 1;
		temptime = HAL_GetTick(); // Reads time elapsed
       // elapsed_time = HAL_GetTick() -temptime;// tempo = valortimer*overflow*prescaler*4/fosc ;
	}
	else
		elapsed_time = HAL_GetTick() -temptime;// tempo = valortimer*overflow*prescaler*4/fosc ;
    return elapsed_time;
}

void reset_timer (void)
{
   flag = 0;
   temptime = HAL_GetTick();
}
  short int ManeticBuffer[3];
int sensor_process(void)
{
	//ThreeD3100_magic_GetData(&dataMd);

//   unsigned char i;
//    i = getRM3100Status ();
//    RM3100_init_SM_Operation ();
//    sensor_xyz raw;
////    BYTE buf[64];

//    float cycle_time,time_elapsed=0;
//    float gain = getRM3100Gain ();

//    float time_to_send=0;
//    //TRISAbits.TRISA2  = 0;	// set RA2 out

//    while(1){

//        cycle_time = get_time();

//        if (cycle_time >10)
//				//	&& !time_to_send)
//				{
//            //time_to_send = cycle_time;
//            reset_timer();
//            requestSingleMeasurement ();
//        }

//        if(getDataReadyStatus ()){
////        if(new_data){
//           // LATAbits.LATA2 = 1;
//            time_elapsed = time_elapsed + time_to_send;

////            new_data=0;

//            raw = ReadRM3100Raw ( );

//            ManeticBuffer[0] = (short int) (raw.x / gain) ;
//            ManeticBuffer[1] = (short int) (raw.y / gain) ;
//            ManeticBuffer[2] = (short int) (raw.z / gain) ;

//            //sprintf(buf,"%.1f   %.1f   %.1f   %f\n",converted_x,converted_y,converted_z, time_to_send);
//            //SendDataBuffer(buf, strlen(buf));
//            time_to_send=0;
//           // LATAbits.LATA2 = 0;
//        }
//    }

	}

