#include <FlexiTimer2.h>
#include <DHT.h>
#include <EEPROM.h>

#define INT8U   unsigned char             //定义0-255数字
#define INT16U  unsigned int              //定义0-65535数字

//595引脚定义
#define SH_CP 11 //移位时钟脉冲
#define DS    4 //串行数据输入
#define ST_CP 3 //输出锁存器控制脉冲

//595引脚操作定义
#define SH_CP_0   digitalWrite(SH_CP, LOW);
#define SH_CP_1   digitalWrite(SH_CP, HIGH);
#define DS_0      digitalWrite(DS, LOW);
#define DS_1      digitalWrite(DS, HIGH);
#define ST_CP_0   digitalWrite(ST_CP, LOW);
#define ST_CP_1   digitalWrite(ST_CP, HIGH);

#define Light_Value 250  //数码管亮度值，即LED点亮的时间（us），值越大则越亮 
#define HighLight_Value 650  //数码管亮度值，即LED点亮的时间（us），值越大则越亮 
INT8U  HighLight_Pos[4]={0,0,0,0};  //高亮数码管位置
INT8U  Point_value[4]={0,0,0,0};   //小数点是否亮

//LED 显示掩码定义 0,1,2,3,4,5,6,7,8,9,关闭//10，-/11,E/12,C/13,H/14,F/15,r/16,n/17,u/18,s/19,A/20;b/21
INT8U SEG_CODE[] ={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xFF,0xBF,0x86,0xC6,0x89,0x8E,0xAF,0xAB,0x9D,0xAD,0x88,0x83};

INT8U DIS_BUF[4] = {11,11,11,11};    //用于保存要显示数据值
INT8U Data_bit[3]; //保存数据分位后的数据
//INT8U Key_count[4],Key_is_down[4]; //按键检测参数


char Menu_ID=1,WorkRun=0,Error=0;
char AutoDispChange=0;

//按键定义
#define KEY1  8
#define KEY2  9
#define KEY3  10
#define KEY4  2

//定义水位开关
#define SW1  12   //水位开关1
#define SW2  13   //水位开关2

//输出定义
#define OUT1  7
#define OUT2  6
#define OUT3  5

//通讯定义发送和接受变量
#define EN_UART_ERR   0  //是否检测通讯错误.
INT8U Send_data[16]={0xAA,0x00,0x00,0xB8,0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xDA,0x55};    //发送变量
INT8U Receive_data[16]={0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55};  //接受变量
INT8U Receive_data_temp[16]={0xAA,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x55};  //零时保持接受的变量,未计算校验和
INT8U Receive_item=0;

#define SPEED_MAX 5000
#define SPEED_MIN 2000

 struct UARTCOM{
   INT8U Send_flag;
   INT8U Receive_flag;
   unsigned int SendTimeCnt;
   unsigned int IntervalCnt;
   unsigned int Send_speed;
}Uart;

//设置参数
int Para_ID=1;
INT16U Para_Value[10];

char WorkState,NoWorkState;
bool DelayCountDownFlag,NoWorkCountDown;
int Logic_Update=0;
long DelayCountDown;
int RestDsCount;
//int StopCountDown;
char SysWork=0;

char Menu_ID_DS=0;

#define _AutoMode   2
#define _ManualMode 9

// 选择温度传感器类型
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#define DHTPIN A4     // 定义温湿度连接pin
DHT dht( DHTPIN,DHTTYPE);
#define  MAXSET_THERM    35
#define  MINSET_THERM   -10
#define  MAXSET_HUMIDITY  99
#define  MINSET_HUMIDITY  30
#define  MAXSET_AIR_THERM    35
#define  MINSET_AIR_THERM    15

int AD_Value[9]={500,280,280,280,0x0005,0x0006,0x0007,0x0008,0x0009};//0-湿度，1-Dht温度,2-A6测量温度值
int Sys_ID=0;
INT8U Update_Disp=0;

//系统功能保存参数
//Fun_Par的值需要保存

char Fun_Para[10]={
         9,  //0, 模式（1：冷水机，2：除湿机，3：干衣机，4移动空调,9，手动模式），3,....  -128~128;
         16, //1, 设定温度，
         84, //2，设定最高转速(Hz)， 内部最高限速5500；
         38, //3，设定最低速度(Hz),  内部也现在速度2000；
         01, //4,
         60, //5, 设定湿度
         25, //6, 设定温度
         01,
         01,
         01
    };  //0，


//时间定义
 struct TIM{
   INT16U DHT_timer;
   INT16U PID_timer; 
   unsigned long PIDState0_timer;
   INT16U UART_timer;
   INT16U DspUpdate_timer;
   INT16U Recover_Count;
   INT16U ParaSave_Count;
};
TIM Timers;
char ParaSaveFlag=0;
char WorkMode;
char WorkModeSwitch;
char PIDState;
char SetKey=0;
char ParaSet=0;
#define KeyPassword  13    //当SetKey=KeyValue的时候才能变更

//用于AYPump;
int WorkACycCount=0,WorkBCycCount=0,WorkTotalCount=0;
char WorkTime10ms=0;
int WorkTime1s=0;
int S1Time=0;
char PumpWorkOn=0;
char ValveWorkOn=0;

//----
int SWlastHTime[2],SWlastLTime[2];
#define SWLDectTime  4000   //持续检测水位错误时间
#define SWHDectTime  1000    //持续检测水位错误恢复时间

#define BEGIN_CTDOWN_NUM   500  //5s
#define AFTER_CTDOWN_NUM   1000  //10s

#define M_KP 30
#define M_KI 10
#define M_KD 0
#define M_SCALE 8
#define M_MAXPWM  1016
#define M_MINPWM  0
int NowPwm;
int EK, EK1,EK2,CEK,CEK1,CEK2,YK;

//--------------------------------------------------------------------------------
//按键子程序，烧苗更新按键数组状态
//0，没有按键，1，短按，3，长按
//--------------------------------------------------------------------------------
char Key_Step[]={0,0,0,0,0};   //
char Key_Return[]={0,0,0,0,0};
int Key_Time[]={0,0,0,0,0};
char Key_Get_Deal=0;
#define KEY_SHORT_PRESS 1
#define KEY_LONG_PRESS 3


void Get_data_dis(INT16U dat);
//--------------------------------------------------------
//10ms任务
//--------------------------------------------------------

void task_10ms(){
  
     Display_scan();  //10ms刷新显示LED
     //工作计时  秒
     WorkTime10ms++;
     if(WorkTime10ms==100){
        WorkTime1s++;
        WorkTime10ms=0;
     }
     Uart.IntervalCnt++;
     if(Uart.IntervalCnt>80){ //大于0.8s,则认为是重新开始接受数据
        Uart.IntervalCnt=0;
        Receive_item=0;  //接受的数据起始归零
     }
     Uart.SendTimeCnt++;
     RestDsCount++;      
     Logic_Update++;
     Key_Get_Deal++;
     if(Key_Get_Deal==2){        //20ms才扫描按键一次
          Get_Key(KEY1,0);
          Get_Key(KEY2,2);
          Get_Key(KEY3,3);
          Get_Key(KEY4,4);
          Get_Key(KEY1,1);
          Key_Get_Deal=0;
   }
     if(ParaSaveFlag==1)Timers.ParaSave_Count++;
     Timers.DHT_timer++;   //DHT读取间隔时间
     Timers.PID_timer++;  //PID计算时间
     Timers.DspUpdate_timer++;
     if(Timers.DspUpdate_timer>50){
           Update_Disp=1;  //0.5更新显示数值.
           Timers.DspUpdate_timer=0;
     }
     if(PIDState==0)Timers.PIDState0_timer++;
     if(DelayCountDownFlag==1)DelayCountDown--;
     if(SysWork==1){
         Timers.UART_timer++;
         if(Timers.UART_timer>2000){
             if((Error==0)&&(EN_UART_ERR==1))Error=17;   //无法通信错误
             Timers.UART_timer=2001;        
         }else{
             if(Error==17)Error=0;   
         }
     }
    if(Error!=0)Timers.Recover_Count++;
}

void setup ()
{ 
  //595引脚设置
  pinMode(SH_CP,OUTPUT);
  pinMode(DS,OUTPUT);
  pinMode(ST_CP,OUTPUT);
  //LED共阳位段 
  pinMode(A0,OUTPUT); 
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT); 
  pinMode(A3,OUTPUT);
  //控制输出引脚设置
  pinMode(OUT1,OUTPUT);
  pinMode(OUT2,OUTPUT);
  pinMode(OUT3,OUTPUT);
  //按键设置
  pinMode(KEY1, INPUT); //
  pinMode(KEY1, INPUT_PULLUP); //由于在默认状态时IO口读到的是一个不稳定的电平值，加上拉使之默认接高电平。
  pinMode(KEY2, INPUT); //
  pinMode(KEY2, INPUT_PULLUP); //由于在默认状态时IO口读到的是一个不稳定的电平值，加上拉使之默认接高电平。
  pinMode(KEY3, INPUT); //
  pinMode(KEY3, INPUT_PULLUP); //由于在默认状态时IO口读到的是一个不稳定的电平值，加上拉使之默认接高电平。
  pinMode(KEY4, INPUT); //
  pinMode(KEY4, INPUT_PULLUP); //由于在默认状态时IO口读到的是一个不稳定的电平值，加上拉使之默认接高电平。

  //水位开关
  pinMode(SW1, INPUT); //水位开关1
  pinMode(SW1, INPUT_PULLUP); 
  pinMode(SW2, INPUT); //水位开关2
  pinMode(SW2, INPUT_PULLUP); 
  
  FlexiTimer2::set(10, task_10ms);    //装载显示函数，定时运行防止闪屏，10ms一次
  FlexiTimer2::start();                  //开始运行
  dht.begin();                            //初始化DHT
  Serial.begin(9600);
  readEEProm();  
  WorkMode=Fun_Para[0];   // 设定工作模式
  Uart.Send_speed=Send_data[4]*0x100+Send_data[3];
  delay(10);  
  if(digitalRead(KEY4)==0){ //开机检测是否要进行参数设置;
     ParaSet=1;
     DIS_BUF[0]=19;
     DIS_BUF[1]=19;
     DIS_BUF[2]=19;
     DIS_BUF[3]=19;
     Menu_ID=1;
   }else{
    ParaSet=0;
   }
      S1Time=Fun_Para[4]*60;
}



void loop()
{    
  //如果要设定参数则这里循环
  while(ParaSet==1){
 
   //参数设置逻辑与显示
     SetParaKey();
     SetParaDisp();
   
  }  
  //通讯处理
  
   Send_command();      //发送数据
   Receive_command();   //处理接受的数据       
     
//工作模式程序
   switch(WorkMode){
        case 1: //冷水机模式
             ReadSW(1,1);               //检测水位开关
             ReadTemp(0,1,1);           //读温度
             CoolWaterKey();
             CoolWaterDisp();
             CoolWaterLogic();
             CoolWaterPIDCompute();              //针对该模式的PID计算  
             break;
        case 2:
            //抽湿机模式
         //    ReadSW(1,1);               //检测水位开关
             ReadTemp(1,0,0);           //读温度
             HumidityKey();
             HumidityDisp();
             HumidityLogic();
             HumidityPIDCompute();              //针对该模式的PID计算  
             HumidityParaSave();
            break; 
        case 3:
           //干衣机模式
           
         //  break;
        case 4:
           //空调模式
           //  ReadSW(1,1);                           //检测水位开关
             ReadTemp(1,0,0);                          //读温度
             AirConditionKey();
             AirConditionDisp();
             AirConditionLogic();
             AirConditionPIDCompute();              //针对该模式的PID计算  
             AirConditionParaSave();
             break;      
        case 5:
           //制冰机模式
             ReadTemp(0,1,0);           //读温度
             IceMakerKey();
             IceMakerDisp();
             IceMakerLogic();
             break;
        case 6:
        case 9:
        default:
           //手动控制模式    
            ManualKey();
            ManualDisp();
            ManualLogic();           
         break;    
   } 
}

