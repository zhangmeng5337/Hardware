
//--------------------------------------------------------------
//串口发送程序，把Send_data[16]的值传出，其他程序要改变好senddata的值，并置Uart.Send_flag=1;
//-----------------------------------------------------------------

void Send_command(){
  if( Uart.SendTimeCnt>50){   //500ms才发送一次 不用sendflag做为标准，而是用时间做标准
      Uart.SendTimeCnt=0;
     //对校验和进行计算
     Send_data[14]=0;
     for(int i=0;i<14;i++){
        Send_data[14] += Send_data[i];
     }
     //把指令传送出去
     for(int j=0;j<16;j++){
        Serial.write(Send_data[j]);
     }
        Uart.Send_flag=0;   
   }  
}
//---------------------------------------------------------------------------------------
//在程序中循环执行接受任务，并做效验和
//--------------------------------------------------------------------------------------
int ErrorCnt;   //计数错误，
void Receive_command(){
     INT8U inByte,temp;
     if(Serial.available() > 0) {
        Uart.IntervalCnt=0;
        inByte = Serial.read();
     
        Receive_data_temp[Receive_item]=inByte;
        Receive_item++;
        if(inByte==0x55&&Receive_item>14){  //要判断是第15位的值,否则中间位也有可能有0x55的值,从而判断失败！！
                    temp=0;
                     Receive_item=0; 
                    //对接受的数据进行累计校验
                    for(int i=0;i<14;i++){
                        temp += Receive_data_temp[i];
                    }  
                    if(temp==Receive_data_temp[14]){
                        for(int j=0;j<16;j++){Receive_data[j]=Receive_data_temp[j];}    //如果接受到的数据正确则copy到Receive_data数组中
                        if(Receive_data[9]>0){
                               ErrorCnt++;
                               if(ErrorCnt>3){
                                  if(Error<20)Error=Receive_data[9];
                               }
                        }else{
                             ErrorCnt=0;
                        }
                      
                    //    for(int k=0;k<16;k++){ Serial.write(Receive_data[k]);}   //测试语句，把收到的返回
                        Timers.UART_timer=0;
                    }
                
           Uart.Receive_flag=1;    
        }
  
     }
}


