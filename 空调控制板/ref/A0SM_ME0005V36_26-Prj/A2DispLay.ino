
//------------------------------------------------------------------
//按位取得数字并保存在数组中,用于应用于LED的DIS_BUF中
//------------------------------------------------------------------
void Get_data_dis(INT16U dat)
{  INT8U i;
    for (i=0; i<4; i++)
  {
    Data_bit[i] = dat%10;   //取余数，
    dat /= 10;
    DIS_BUF[3-i]=Data_bit[i];   //更新在显示数组中
  }
}  

//-----------------------------------------------------------------
// 串行输出子程序
//-----------------------------------------------------------------
void Serial_input_595(INT8U dat)
{ 
  ST_CP_0;                               
  shiftOut(DS,SH_CP,MSBFIRST,dat);  //数据逐位输出到595
  ST_CP_1;                         //上升沿将数据送到输出锁存器
  ST_CP_0;          
}
 
//数码管显示共阳位选
void Led_select(INT8U num,INT8U mode) 
{   
  if(num<4)   
  {       
    switch(num)       
    {          
      case 0: num = A0; break;            //A0 第一位LED的MCU引脚 ，也可以是采用数字引脚14来定义 
      case 1: num = A1; break;            //同上  
      case 2: num = A2; break;          
      case 3: num = A3; break;        
    }      
    digitalWrite(num,mode);   
   }
} 
//----------------------------------------------------------------------
//数码管扫描显示输出
//----------------------------------------------------------------------
 void Display_scan(void)
 {  
   INT8U i=0,dis;
    for(i=0;i<4;i++)
    {
       dis=SEG_CODE[DIS_BUF[i]];
       if(Point_value[i]==1){dis=SEG_CODE[DIS_BUF[i]]&0x7F;}     //小数点由point_value控制,    
       Serial_input_595(dis);         //将数字I的段码字节串行输入595
       Led_select(i,LOW);            //第I位数码通电显示
       if(HighLight_Pos[i]==1){delayMicroseconds(HighLight_Value); }else{ delayMicroseconds(Light_Value);}  //位段亮度控制
       Led_select(i,HIGH);             //数码管消隐
    }
 }

