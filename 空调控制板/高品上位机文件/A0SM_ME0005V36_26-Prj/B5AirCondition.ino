
//--------------------------------------------------------------------------------
//根据MenuID 处理按键功能
//--------------------------------------------------------------------------------

void AirConditionKey(){
    char Key1_status, Key2_status,Key3_status,Key4_status;
    Key1_status=Key_Return[1];                      
    Key2_status=Key_Return[2]; 
    Key3_status=Key_Return[3]; 
    Key4_status=Key_Return[4]; 

   //根据不同的菜单显示层级对按键进行处理，并显示恰当的LED值
    switch(Menu_ID){
        case 1:        
         
            if(Key1_status==3){   
                   if(AutoDispChange==0)AutoDispChange=1;
                   Clean_KeyV();
               }
           if(Key2_status==3){
                Fun_Para[0]=2;
                WorkModeSwitch=0x02;
                WorkMode=Fun_Para[0]; 
                Clean_KeyV();
                ParaSaveFlag=1;
                Timers.ParaSave_Count=0;
             }
           //处理短按键
           if(Key1_status==1){
              if(SysWork==0){
                   Menu_ID_DS=6;    //显示Fu
               }else{
                   Menu_ID_DS++;
                   if(Menu_ID_DS>4)Menu_ID_DS=1;
               }
               RestDsCount=0;
               Update_Disp==1;                  
               AutoDispChange=0;   
               Clean_KeyV();
           }
           if(Key2_status==1){
                Fun_Para[6]++;
                if(Fun_Para[6]>MAXSET_AIR_THERM )Fun_Para[6]=MAXSET_AIR_THERM ;
                Menu_ID_DS=3;
                RestDsCount=0;
                Update_Disp=1;
                ParaSaveFlag=1;
                Timers.ParaSave_Count=0;
                Clean_KeyV();
           }

           if(Key3_status==1){
                Fun_Para[6]--;
                if(Fun_Para[6]<MINSET_AIR_THERM)Fun_Para[6]=MINSET_AIR_THERM;
                Menu_ID_DS=3;
                RestDsCount=0;
                Update_Disp=1;
                ParaSaveFlag=1;
                Timers.ParaSave_Count=0;
                Clean_KeyV();
           }
           
           if(Key4_status==1){
                if(SysWork==1){
                   SysWork=0;
                   Menu_ID_DS=0;
                   DelayCountDown=AFTER_CTDOWN_NUM;
                }else{
                   Error=0;   //如果有错误,则清理故障
                   SysWork=1;
                   Menu_ID_DS=1;
                   DelayCountDown=BEGIN_CTDOWN_NUM;   
                }
                RestDsCount=0;
               Update_Disp=1;
               Clean_KeyV();
            }       
          
          break;      
        case 2:  //Menu_ID=2；界面是系统参数显示界面
         
           break;
       
    }
}


//--------------------------------------------------------------
//根据菜单条目进行显示
//-----------------------------------------------------------------
void AirConditionDisp(){
    int tempV ,vspeed;
    char i;
    if(Update_Disp==1){   
         switch(Menu_ID){
             case 1:
                    switch(Menu_ID_DS){
                        case 0:                       
                          for(i=0;i<4;i++){DIS_BUF[i]=11;}     
                          break;
                        case 1: //空气温度
                           if(AD_Value[1]<0){
                                 Get_data_dis(-AD_Value[1]/10);
                                 DIS_BUF[1]=11;
                            }else{
                                 Get_data_dis(AD_Value[1]/10);
                                 DIS_BUF[1]=10;                        
                             }
                            DIS_BUF[0]=13; 
                          break;
                         case 2: //湿度
                         
                           Get_data_dis(AD_Value[0]/10);
                           DIS_BUF[1]=10;    
                           DIS_BUF[0]=14;  
                           
                            break;
                         case 3: //设定湿度
                            
                           Get_data_dis(Fun_Para[6]);
                           DIS_BUF[1]=10;    
                           DIS_BUF[0]=19; 
                           break;
                       case 4: //速度 
                           vspeed=Receive_data[3]*0x100+Receive_data[2];
                           Get_data_dis(vspeed);                    
                           Point_value[0]=0;  Point_value[1]=0;  Point_value[2]=0;  Point_value[3]=0;        
                          break;
                       case 5://显示错误
                           Get_data_dis(Error);
                           DIS_BUF[0]=12;
                           DIS_BUF[1]=16; 
                           break;
                        case 6: //显示模式
                          Get_data_dis(Fun_Para[0]);
                          DIS_BUF[0]=15;
                          DIS_BUF[1]=10;
                          DIS_BUF[2]=10;
                          break;
                    }
               
                  
                  break;
              case 2:
                   
                  break;
              case 3:
                  
                  break;    
         } 
         Update_Disp=0;
       }
}

//--------------------------------------------------------------
//工作逻辑,这里进行工作逻辑状态扫描
//-----------------------------------------------------------------

void AirConditionLogic(){
    int Diff,OutSpeed;
     if(SysWork==1){  //工作状态
            if(Error==0){
              
                    if(RestDsCount>200){
                      if(AutoDispChange==1){
                        Menu_ID_DS++;
                        if(Menu_ID_DS>4)Menu_ID_DS=1;
                      }else{
                         Menu_ID_DS=1;
                    
                      }                      
                       RestDsCount=0;  
                    }  //重置显示到水温
                    
                    if(  WorkModeSwitch==0x04){
                         WorkModeSwitch=0x00;
                         WorkState=0;
                    }
                   switch(WorkState){
                        case 0:
                                 digitalWrite(OUT1,HIGH);
                                 digitalWrite(OUT2,HIGH);
                                 digitalWrite(OUT3,HIGH);
                                 PIDState=0;
                                 Timers.PIDState0_timer=0;
                                 Menu_ID_DS=1;
                                 DelayCountDownFlag=1;                                
                                 if(DelayCountDown<0){
                                    Diff=AD_Value[1]-Fun_Para[6]*10;  //检测温度与设定温度差值
                                    if(Diff<5){
                                         WorkState=5;          
                                    }else{
                                        WorkState=1; 
                                        DelayCountDownFlag=0;  
                                    }
                                  }
                                 break;
                         case 1:
                               Uart.Send_flag=1;
                               Send_data[2]=1;
                               Uart.Send_speed=Send_data[4]*0x100+Send_data[3]; //组合数据
                               WorkState=2;
                               break;
                         case 2:
                              if(Receive_data[10]==0){
                                  WorkState=1;  //没有收到压缩机在工作的信号，则重新去发送数据
                              }else{
                                  WorkState=3;
                              }
                              break;
                          case 3: //根据温度进行,
                              if(Logic_Update>200){    //2s才去更新一次差异检测 不能太频繁的检测
                                   Diff=AD_Value[1]-Fun_Para[6]*10;  //检测温度与设定温度差值
                                   if(Diff<-16){                    //最大差值1.6
                                        WorkState=4;         
                                        DelayCountDown=AFTER_CTDOWN_NUM;
                                   }
                                  Uart.Send_speed=36*NowPwm/10+2000;    //根据PID的计算结果对应到压缩机的转速。NowPwm=0对应2000，NowPwm=256duiy
                                  if(Uart.Send_speed>Fun_Para[2]*60)Uart.Send_speed=Fun_Para[2]*60; //参数限速
                                  if(Uart.Send_speed<Fun_Para[3]*60)Uart.Send_speed=Fun_Para[3]*60; //参数限速
                                  if(Uart.Send_speed<2000)Uart.Send_speed=2000;                     //系统极限限速参数限速
                                  if(Uart.Send_speed>5500) Uart.Send_speed=5500;                   //系统极限限速
                                  Send_data[3]=Uart.Send_speed%0x100;
                                  Send_data[4]=Uart.Send_speed/0x100; 
                                  Uart.Send_flag=1;
                                  Logic_Update=0;
                              }
                              break;

                           case 4:             
                                  Uart.Send_flag=1;
                                  Send_data[2]=0;
                                  if(Receive_data[10]==0){  
                                     DelayCountDownFlag=1; 
                                     if(DelayCountDown<0){
                                         WorkState=5;                                     
                                         DelayCountDownFlag=0; 
                                     }
                                  }
                                break;
                                   
                           case 5: //等待温度上升重新启动系统
                               Diff=AD_Value[1]-Fun_Para[6]*10;  //检测温度与设定温度差值
                               if(Diff>15){                     //大于1.5度则重新启动
                                    WorkState=0;
                                }else{
                                    Uart.Send_flag=1;
                                    Send_data[2]=0;
                                    digitalWrite(OUT1,HIGH);   //这个时候水泵不停机，保持循环开启
                                    digitalWrite(OUT2,LOW);
                                    digitalWrite(OUT3,LOW);
                               }
                               break;
                    }
              
            }else{
                 Menu_ID_DS=5;
                 switch(Error){ 
                        case 1:
                        case 2:
                             digitalWrite(OUT1,LOW);   //如果是电流驱动器故障，这个时候水泵不停机，保持循环开启
                             digitalWrite(OUT2,LOW);
                             digitalWrite(OUT3,LOW);
                             break;
                       case 21:
                       case 22:
                       case 27:
                       case 28:
                             Uart.Send_flag=1;
                             Send_data[2]=0;
                             digitalWrite(OUT1,LOW);  
                             digitalWrite(OUT2,LOW);
                             digitalWrite(OUT3,LOW);
                            break;
                   
                       case 85:
                             Error=0;
                             Menu_ID_DS=1;
                             break;
                       case 17:      
                       default:
                             Uart.Send_flag=1;
                             Send_data[2]=0;
                             digitalWrite(OUT1,LOW);  
                             digitalWrite(OUT2,LOW);
                             digitalWrite(OUT3,LOW);
                            if(Timers.Recover_Count>2000){
                                 Menu_ID_DS=1;
                                 Error=0;
                                 Uart.Send_flag=1;   //尝试发送重新启动指令。看能否恢复启动
                                 Send_data[2]=1;
                                 Timers.Recover_Count=0;                              
                            }
                 }
                               
                 WorkState=0;//syswork工作是则从0开始
            }
      
     }else{
             Timers.UART_timer=0;
             if(RestDsCount>600){
                 Menu_ID_DS=0;
                 RestDsCount=0;
             } 
             //重置显示到- - - -
             Uart.Send_flag=1;
             Send_data[2]=0; 
             digitalWrite(OUT1,LOW);
             if(Receive_data[10]==0){    //延时管风扇
                   DelayCountDownFlag=1; 
                   if(DelayCountDown<0){                     
                         DelayCountDownFlag=0;                        
                         digitalWrite(OUT2,LOW);
                         digitalWrite(OUT3,LOW); 
                    }
              }           
             WorkState=0;//syswork工作是则从0开始
        
     }
}

void AirConditionPIDCompute(){
  
 if (Timers.PID_timer>100){
      
           CEK = M_KP + M_KI + M_KD ;
           CEK1 = 0 - M_KP - 2*M_KD ;
           CEK2 = M_KD ;
           EK2 = EK1 ;
           EK1 = EK ;
           EK = AD_Value[1]-Fun_Para[6]*10;    //湿度差异
           YK = EK * CEK + EK1 * CEK1 + EK2 *CEK2 ;
           YK=YK/M_SCALE;
           NowPwm=NowPwm+YK;
           
           if(NowPwm <= M_MINPWM){
                 NowPwm = M_MINPWM;
            }else if(NowPwm > M_MAXPWM){
                 NowPwm = M_MAXPWM;
             }
             Timers.PID_timer=0;
   }
}

void AirConditionParaSave(){
  if(Timers.ParaSave_Count>400){
      Timers.ParaSave_Count=0;
      ParaSaveFlag=0;
      WrtieEEprom();
  }
}

