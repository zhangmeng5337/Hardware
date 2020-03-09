
void ManualKey(){
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
           //处理短按键
             if(Key1_status==1){                
                  RestDsCount=0;
                  Update_Disp==1;      
                  AutoDispChange=0;  
                  Menu_ID_DS++;
                 if(Menu_ID_DS>8)Menu_ID_DS=1;
                   Clean_KeyV();
                 
             }
                   //处理增加按键 Key2
             if(Key2_status==1){
                    Uart.Send_speed=Send_data[4]*0x100+Send_data[3];
                    Uart.Send_speed +=100;
                    if( Uart.Send_speed>5900) Uart.Send_speed =5900;
                    Send_data[3]=Uart.Send_speed%0x100;
                    Send_data[4]=Uart.Send_speed/0x100; 
                    Get_data_dis(Uart.Send_speed);
                    Point_value[0]=0;  Point_value[1]=0;  Point_value[2]=0;  Point_value[3]=0; 
                    Uart.Send_flag=1;
                    Clean_KeyV();               
             }
            //处理减少按键 Key3
             if(Key3_status==1){
                    Uart.Send_speed=Send_data[4]*0x100+Send_data[3];
                    Uart.Send_speed -=100;
                    if( Uart.Send_speed<1800) Uart.Send_speed =1800;
                    Send_data[3]=Uart.Send_speed%0x100;
                    Send_data[4]=Uart.Send_speed/0x100;    
                    Get_data_dis(Uart.Send_speed);
                    Point_value[0]=0;  Point_value[1]=0;  Point_value[2]=0;  Point_value[3]=0; 
                    Uart.Send_flag=1;
                    Clean_KeyV();
             }
              if(Key4_status==1){
                if(SysWork==1){
                   SysWork=0;
                   Menu_ID_DS=0;
                   Point_value[0]=0;  Point_value[1]=0;  Point_value[2]=0;  Point_value[3]=0;  
                  Uart.Send_flag=1;                  
                  Send_data[2]=0;
              
                }else{
                   Error=0;   //如果是驱动器的错误,则清理故障
                   SysWork=1;
                   Menu_ID_DS=1;
                   
                  Uart.Send_flag=1;
                  Send_data[2]=1;
                }
                   RestDsCount=0;
                   Update_Disp=1;
                   Clean_KeyV();
            }         
           break;
       default:
           break;
    }           
}



void ManualDisp(){
     int tempV ,vspeed;
     char i;
        if(Update_Disp==1){   
            switch(Menu_ID){
                case 1:
                     switch(Menu_ID_DS){
                         case 0:    //- - - -                   
                              for(i=0;i<4;i++){DIS_BUF[i]=11;}     
                              break;
                         case 1:  //显示数值
                             Send_data[3]=Uart.Send_speed%0x100;
                             Send_data[4]=Uart.Send_speed/0x100;    
                             Get_data_dis(Uart.Send_speed);
                             break;        
                        case 2: //速度
                            vspeed=Receive_data[3]*0x100+Receive_data[2];
                            Get_data_dis(vspeed);                    
                             Point_value[0]=0;  Point_value[1]=0;  Point_value[2]=0;  Point_value[3]=0;        
                          break;
                        case 3: //电流
                           tempV=Receive_data[5]*0x100+Receive_data[4];
                           Get_data_dis(tempV);
                           Point_value[0]=0;  Point_value[1]=1;  Point_value[2]=0;  Point_value[3]=0;                                        
                          break;
                        case 4: //电压
                           tempV=Receive_data[7]*0x100+Receive_data[6];
                           Get_data_dis(tempV); 
                            Point_value[0]=0;  Point_value[1]=1;  Point_value[2]=0;  Point_value[3]=0;    
                           break;
                        case 5: //驱动器温度
                            Get_data_dis(Receive_data[8]); 
                             DIS_BUF[0]=13;
                             DIS_BUF[1]=10;
                             Point_value[0]=0;  Point_value[1]=0;  Point_value[2]=0;  Point_value[3]=0;  
                            break;
                        case 6: //故障代码
                              Get_data_dis(Error); 
                              DIS_BUF[0]=12;
                              DIS_BUF[1]=16; 
                     
                          break;
                        case 7://工作状态
                           Get_data_dis(Receive_data[10]); 
                              DIS_BUF[0]=10;
                             DIS_BUF[1]=10;                              
                           break;
                        case 8: //显示模式
                          Get_data_dis(Fun_Para[0]);
                          DIS_BUF[0]=15;
                          DIS_BUF[1]=10;
                          DIS_BUF[2]=10;
                          break;
                    }
                 
                  
                  break;
             case 2:
             
                 break;
            } 
            Update_Disp=0;
       }
}


void ManualLogic(){
     if(SysWork==1){  //工作状态
            if(Error==0){
                   if(RestDsCount>200){
                      if(AutoDispChange==1){
                        Menu_ID_DS++;
                        if(Menu_ID_DS>8)Menu_ID_DS=1;
                      }else{
                         Menu_ID_DS=2;
                      }                      
                       RestDsCount=0;  
                       Uart.Send_flag=1;
                       Send_data[2]=1;
                    }  //重置显示
                     
                      
                         digitalWrite(OUT1,HIGH);
                         digitalWrite(OUT2,HIGH);
                         digitalWrite(OUT3,HIGH);
                    
              
            }else{
                  Menu_ID_DS=6;
                 switch(Error){ 
                        case 1:
                        case 2:
                              //  digitalWrite(OUT1,LOW);   //如果是电流驱动器故障，这个时候水泵不停机，保持循环开启
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
                             Menu_ID_DS=2;
                             break;
                       case 17:
                       default:
                             Uart.Send_flag=1;
                             Send_data[2]=0;
                             digitalWrite(OUT1,LOW);  
                             digitalWrite(OUT2,LOW);
                             digitalWrite(OUT3,LOW);
                            if(Timers.Recover_Count>3000){
                                 Error=0;
                                 Menu_ID_DS=2;
                                 Uart.Send_flag=1;   //尝试发送重新启动指令。看能否恢复启动
                                 Send_data[2]=1;
                                 Timers.Recover_Count=0;                              
                            }
                 }
                               
                 WorkState=0;//syswork工作是则从0开始
            }
      
     }else{
              Timers.UART_timer=0;
             if(RestDsCount>200){
                   if(AutoDispChange==1){
                        Menu_ID_DS++;
                       if(Menu_ID_DS>8)Menu_ID_DS=0;
                       }else{
                         Menu_ID_DS=0;
                       }                      
                       RestDsCount=0;  
               }  //重置显示
              Uart.Send_flag=1;
              Send_data[2]=0;
              digitalWrite(OUT1,LOW);  
              digitalWrite(OUT2,LOW);
              digitalWrite(OUT3,LOW);
        
     }
}


