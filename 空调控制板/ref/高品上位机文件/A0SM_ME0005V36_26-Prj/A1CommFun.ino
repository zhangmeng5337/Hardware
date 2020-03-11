
int Get_Key(int key_v, int key_select){
     bool Key_value;
     Key_value=digitalRead(key_v);
     switch(Key_Step[key_select]){
             case 0:
                  if(!Key_value)Key_Step[key_select]=1;
                  break;
             case 1:
                 if(!Key_value){
                     Key_Time[key_select]=1;
                     Key_Step[key_select]=2;
                 }else{
                     Key_Step[key_select]=0;
                 }
                 break;
             case 2:
                if(Key_value){
                   Key_Return[key_select]=KEY_SHORT_PRESS;
                   Key_Step[key_select]=0;                  
                }
                else if(Key_Time[key_select]++>80){
                   Key_Return[key_select]=KEY_LONG_PRESS;
                   Key_Step[key_select]=3;                  
                }
                 break;
             case 3:
                 if(Key_value){
                    Key_Step[key_select]=0;  
                 }
                 break;
            default:
                 Key_Return[key_select]=0;
                 break;
      
     }  
}

//处理完按键功能后需要清除Key状态

void Clean_KeyV(){
    Key_Return[0]=0;
    Key_Return[1]=0;
    Key_Return[2]=0;
    Key_Return[3]=0;
    Key_Return[4]=0;
}



void ReadSW(bool DectSw1,bool DectSw2){

   if(SysWork==1){
     if(DectSw1==1){
       if(digitalRead(SW1)==0){
           SWlastHTime[0]++; 
           if( SWlastHTime[0]>SWLDectTime&&Error==0)Error=21;
           SWlastLTime[0]=0;
       }else{
           SWlastLTime[0]++;
           if( SWlastLTime[0]>SWHDectTime&&Error==21)Error=0;  
           SWlastHTime[0]=0;
       }
     }
     if(DectSw2==1){
      if(digitalRead(SW2)==0){
          SWlastHTime[1]++;
          if( SWlastHTime[1]>SWLDectTime&&Error==0)Error=22;
          SWlastLTime[1]=0;
      }else{
          SWlastLTime[1]++;
         if( SWlastLTime[1]>SWHDectTime&&Error==22)Error=0;
          SWlastHTime[1]=0;
      }
     }
   }else{
     SWlastLTime[0]=0;
     SWlastHTime[0]=0;
     SWlastLTime[1]=0;
     SWlastHTime[1]=0;
   }
}

//读取温度值
char Temp1ErrCount=0,Temp2ErrCount=0,DhtErrCount=0;
void ReadTemp(bool Dht,bool Temp1,bool Temp2){ 
 float Humidity,Temperature,Temperature2,Temperature3;  
 double Digital_Value,Voltage_Value,Rt_Value;
int AD0_Temp, AD1_Temp;
   if(Timers.DHT_timer>100){  //不用太频繁的去检测温度，1s检测一次
       if(Dht==1){
             Humidity = dht.readHumidity();
          
              AD0_Temp= AD_Value[0]*3/4+Humidity*10/4; //每次只取1/8的值（LPC滤波） //湿度
              if(AD0_Temp==0){
                  DhtErrCount++;
                  if(DhtErrCount>10){if(Error==0)Error=29;}
              }else{ 
                    AD_Value[0]=AD0_Temp;
                   DhtErrCount=0;
               }
             
             Temperature = dht.readTemperature();
             AD1_Temp=AD_Value[1]*3/4+Temperature*10/4;                            //温度
             if(AD1_Temp==0){
                  DhtErrCount++;
                 if(DhtErrCount>10){if(Error==0)Error=29;}
              
                
             }else{
                  AD_Value[1]=AD1_Temp;
                    DhtErrCount=0;
             }
       }
      if(Temp1==1){
      //A1值 对应是单片机的A6 内
       Digital_Value=analogRead(A6);   //读取串联电阻上的电压值（数字量）
       if(Digital_Value>900){
              Temp1ErrCount++;
              if(Temp1ErrCount>5){
              if(Error==0)Error=28;
           }
       }else{
           Temp1ErrCount=0;      
           if(Error==28)Error=0;
           Voltage_Value=(Digital_Value/1023)*5.00;//换算成模拟量的电压值
           Rt_Value= Voltage_Value*10/(5-Voltage_Value);  //计算出热敏电阻的阻值
           Temperature2=(double)1/(log(Rt_Value/10)/3980 + 1/( 25 + 273.15)) - 273.15; //计算所感知的温度并发送
           AD_Value[2]= AD_Value[2]*7/8+ Temperature2*10/8;
          
       }
     
      }
      if(Temp2==1){
           //A2值 对应是单片机的A7 外           
          Digital_Value=analogRead(A7);   //读取串联电阻上的电压值（数字量）
          if(Digital_Value>900){
               Temp2ErrCount++;
               if(Temp2ErrCount>5){
               if(Error==0)Error=27;
            }
          }else{
              Temp2ErrCount=0;  
              if(Error==27)Error=0;
              Voltage_Value=(Digital_Value/1023)*5.00;//换算成模拟量的电压值
              Rt_Value= Voltage_Value*10/(5-Voltage_Value);  //计算出热敏电阻的阻值
              Temperature3=(double)1/(log(Rt_Value/10)/3980 + 1/( 25 + 273.15)) - 273.15; //计算所感知的温度并发送
               AD_Value[3]= AD_Value[3]*7/8+ Temperature3*10/8;            
          }
       }
       Timers.DHT_timer=0;  
 }
}


void readEEProm(){
     char EEMark;
      EEMark=EEPROM.read(0);//读标志位
      if(EEMark==88){
            for(int i=1;i<10;i++){
               Fun_Para[i-1]=EEPROM.read(i);
            }        
      }
}

void WrtieEEprom(){
       EEPROM.write(0, 88);  //写入标志
       for(int i=1;i<10;i++){
            EEPROM.write(i,Fun_Para[i-1]);
        }     
}


