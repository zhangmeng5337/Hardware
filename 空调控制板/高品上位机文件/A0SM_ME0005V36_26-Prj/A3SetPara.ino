
char NowIndex=0;
void SetParaKey(){
    char Key1_status, Key2_status,Key3_status,Key4_status;
    Key1_status=Key_Return[1];                      
    Key2_status=Key_Return[2]; 
    Key3_status=Key_Return[3]; 
    Key4_status=Key_Return[4]; 
     switch(Menu_ID){
        case 1:        
          //处理短按键
           if(Key1_status==1){
              if(SetKey==KeyPassword){
                 Menu_ID=2;
                 HighLight_Pos[0]=1;HighLight_Pos[1]=0;  HighLight_Pos[2]=0;HighLight_Pos[3]=0;
                }else{
                ParaSet=0; 
                 Menu_ID=1;
              }

                Clean_KeyV();
           }
           if(Key2_status==1){
                SetKey++;
                Update_Disp=1;
                  Clean_KeyV();
           }

           if(Key3_status==1){
                SetKey--;
                if(SetKey<0)SetKey=0;
                Update_Disp=1;
                  Clean_KeyV();
           }
          
          break;      
        case 2:  //Menu_ID=2；界面是系统参数显示界面
              if(Key1_status==3){
                 WrtieEEprom();
                 ParaSet=0; 
                 Menu_ID=1;
                 WorkMode=Fun_Para[0];   // 设定工作模式
                 Uart.Send_speed=Send_data[4]*0x100+Send_data[3];
                 HighLight_Pos[0]=0;HighLight_Pos[1]=0;  HighLight_Pos[2]=0;HighLight_Pos[3]=0;
                 Clean_KeyV();
              }
             if(Key1_status==1){
                 Menu_ID=3;
                 HighLight_Pos[0]=0;HighLight_Pos[1]=0;  HighLight_Pos[2]=1;HighLight_Pos[3]=1;
                 Clean_KeyV();
              }
              
           if(Key2_status==1){
                 NowIndex++;     
                 if(NowIndex>8)NowIndex=0;      
                 Update_Disp=1;
                   Clean_KeyV();
           }

           if(Key3_status==1){
                   NowIndex--;
                   if(NowIndex<0)NowIndex=0;
                   Update_Disp=1;
                     Clean_KeyV();
           }
          
          if(Key4_status==1){
                ParaSet=0; 
                Menu_ID=1;
                  Clean_KeyV();
           }
           
           break;
       case 3:  //Menu_ID=2\3；界面是系统参数具体值设定界面
              if(Key1_status==3){
                 WrtieEEprom();
                 ParaSet=0; 
                 Menu_ID=1;
                 WorkMode=Fun_Para[0]; 
                 Uart.Send_speed=Send_data[4]*0x100+Send_data[3];
                 HighLight_Pos[0]=0;HighLight_Pos[1]=0;  HighLight_Pos[2]=0;HighLight_Pos[3]=0;
                  Clean_KeyV();
              }
             if(Key1_status==1){
                 Menu_ID=2;
                 HighLight_Pos[0]=1;HighLight_Pos[1]=0;  HighLight_Pos[2]=0;HighLight_Pos[3]=0;
                  Clean_KeyV();
              }
              
           if(Key2_status==1){
                   Fun_Para[NowIndex]++;                
                   if(Fun_Para[0]>9)Fun_Para[0]=9;
                   if(Fun_Para[1]>35)Fun_Para[1]=35;                
                   Update_Disp=1;
                   Clean_KeyV();
           }

           if(Key3_status==1){
                
                   Fun_Para[NowIndex]--;
                   if(Fun_Para[NowIndex]<0)Fun_Para[NowIndex]=0;
                   if(Fun_Para[0]<1)Fun_Para[0]=1;
                  Update_Disp=1;
                    Clean_KeyV();
           }
          
          if(Key4_status==1){
                ParaSet=0; 
                Menu_ID=1;
                HighLight_Pos[0]=0;HighLight_Pos[1]=0;  HighLight_Pos[2]=1;HighLight_Pos[3]=0;
                Clean_KeyV();
           }
           
           break;
       
    }
}

void SetParaDisp(){
        char i;
        unsigned int temp;
        if(Update_Disp==1){   
            switch(Menu_ID){
             case 1:
                    Get_data_dis(SetKey);
                    DIS_BUF[0]=19;
                    DIS_BUF[1]=19;                  
                    break;
             case 2:
             case 3:
                   temp=(NowIndex+1)*1000+Fun_Para[NowIndex];
                   Get_data_dis(temp);      
              //     DIS_BUF[1]=10;      
                   break;
            } 
            Update_Disp=0;
       }
}

