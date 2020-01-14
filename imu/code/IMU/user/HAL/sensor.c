
#include "sensor.h"
//校准角速度 
//void Angular_Velocity_Calibration(float *x,float *y,float *z)//校准的不够准，待改进
//{
//	
//	float gyroX,gyroY,gyroZ;
//	
//	GetGyro(&gyroX,&gyroY,&gyroZ);
//	
//	//校准
//	*x -= gyroX;
//	*y -= gyroY;
//	*z -= gyroZ;

//	
//}
////与重力加速度的夹角,返回角度值
//void MPU_Angle_Analysis(float *rX,float *rY,float *rZ)
//{
//	
//	float accelX,accelY,accelZ;//分向量
//	float accelXSum,accelYSum,accelZSum;//分向量
//	int i;
//	float norm;//模长

//	//滤波
//	for(i=0;i<10;i++)
//	{
//		GetAccel(&accelX,&accelY,&accelZ);
//		
//		//防止越界
//		if(accelX>1)       accelX = 1;
//		else if(accelX<-1) accelX = -1;
//		
//		if(accelY>1)       accelY = 1;
//		else if(accelY<-1) accelY = -1;
//		
//		if(accelZ>1)       accelZ = 1;
//		else if(accelZ<-1) accelZ = -1;

//		accelXSum += accelX;
//		accelYSum += accelY;
//		accelZSum += accelZ;
//		
//	}
//	accelX = accelXSum / 10.0f;
//	accelY = accelYSum / 10.0f;
//	accelZ = accelZSum / 10.0f;
//	
//	norm = sqrt(accelX*accelX+accelY*accelY+accelZ*accelZ);
//	norm>1?(norm=1):(norm=norm);
//	
//	accelX = acos(accelX/norm);
//	accelY = acos(accelY/norm);
//	accelZ = acos(accelZ/norm);
//	
//	accelX = accelX*180.0f/Pi;//弧度转角度
//	accelY = accelY*180.0f/Pi;
//	accelZ = accelZ*180.0f/Pi;
//	
//	
//	*rX = accelX;
//	*rY = accelY;
//	*rZ = accelZ;
//	
//	accelX = 0;
//	accelY = 0;
//	accelZ = 0;
//}



  
//**************************************************************************************************  
//定义类型及变量  
//**************************************************************************************************  

int dis_data;                   //变量  
  
  
//******角度参数************  
    float Gyro_y;        //Y轴陀螺仪数据暂存  
float Angle_gy;      //由角速度计算的倾斜角度  
float Accel_x;  
    //X轴加速度值暂存  
float Angle_ax;      //由加速度计算的倾斜角度  
float Angle;         //小车最终倾斜角度  
unsigned char value;  //角度正负极性标记   
  
  
//void getangle()   
//{  
//    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);//读取原始6个数据  
//    angleAx=atan2(ax,az)*180/PI;//计算与x轴夹角  
//    gyroGy=-gy/131.00;//计算角速度  
//    Yijielvbo(angleAx,gyroGy);//一阶滤波  
//    Erjielvbo(angleAx,gyroGy);//二阶滤波  
//    Kalman_Filter(angleAx,gyroGy);   //卡尔曼滤波  
//}  
//  
//  
//  
//void Yijielvbo(float angle_m, float gyro_m)  
//{  
//    angle1 = K1 * angle_m+ (1-K1) * (angle1 + gyro_m * dt);  
//}  
//  
//void Erjielvbo(float angle_m,float gyro_m)  
//{  
//    x1=(angle_m-angle2)*(1-K2)*(1-K2);  
//    y1=y1+x1*dt;  
//    x2=y1+2*(1-K2)*(angle_m-angle2)+gyro_m;  
//    angle2=angle2+ x2*dt;  
//}  
  
  
//*********************************************************  
// 卡尔曼滤波  
//*********************************************************  
//Kalman滤波，20MHz的处理时间约0.77ms；  
  
float Kalman_Filter(float Accel,float Gyro)    
{  
 
  static const float Q_angle=0.001;  
	static const float Q_gyro=0.003;
	static const float R_angle=0.5;
	static const float dt=0.01;	                  //dt为kalman滤波器采样时间;
	static const char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	
Angle+=(Gyro - Q_bias) * dt; //先验估计  
  
Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-先验估计误差协方差的微分  
  
Pdot[1]=- PP[1][1];  
Pdot[2]=- PP[1][1];  
Pdot[3]=Q_gyro;  
  
PP[0][0] += Pdot[0] * dt;   // Pk-先验估计误差协方差微分的积分  
PP[0][1] += Pdot[1] * dt;   // =先验估计误差协方差  
PP[1][0] += Pdot[2] * dt;  
PP[1][1] += Pdot[3] * dt;  
  
Angle_err = Accel - Angle;  
//zk-先验估计  
  
PCt_0 = C_0 * PP[0][0];  
PCt_1 = C_0 * PP[1][0];  
  
E = R_angle + C_0 * PCt_0;  
  
K_0 = PCt_0 / E;  
K_1 = PCt_1 / E;  
  
t_0 = PCt_0;  
t_1 = C_0 * PP[0][1];  
  
PP[0][0] -= K_0 * t_0;  
//后验估计误差协方差  
PP[0][1] -= K_0 * t_1;  
PP[1][0] -= K_1 * t_0;  
PP[1][1] -= K_1 * t_1;  
  
Angle += K_0 * Angle_err;  
//后验估计  
Q_bias  += K_1 * Angle_err; //后验估计  
Gyro_y   = Gyro - Q_bias;  
//输出值(后验估计)的微分=角速度  
return  Gyro_y;  
}    
  
//*********************************************************  
// 倾角计算（卡尔曼融合）  
//*********************************************************  
  
  
void Angle_Calcu(float acc,float gyro)    
{  
  
//------加速度--------------------------  
  
//范围为2g时，换算关系：16384 LSB/g  
//角度较小时，x=sinx得到角度（弧度）, deg = rad*180/3.14  
//因为x>=sinx,故乘以1.3适当放大  
  
//Accel_x  = GetData(ACCEL_XOUT_H);  
 //读取X轴加速度 
Accel_x = acc;	
Angle_ax = (Accel_x - 1100) /16384;   //去除零点偏移,计算得到角度（弧度）  
Angle_ax = Angle_ax*1.4*180/3.14;     //弧度转换为度,  
  
//Display10BitData(Angle_ax,2,1);  
  
//-------角速度-------------------------  
  
//范围为2000deg/s时，换算关系：16.4 LSB/(deg/s)  
//Gyro_y = GetData(GYRO_YOUT_H);  
     //静止时角速度Y轴输出为-30左右
Gyro_y = gyro;	  
Gyro_y = -(Gyro_y + 30)/16.4;         //去除零点偏移，计算角速度值,负号为方向处理   
//Angle_gy = Angle_gy + Gyro_y*0.01;  //角速度积分得到倾斜角度.  
  
//Display10BitData(Gyro_y,8,1);  
  
//-------卡尔曼滤波融合-----------------------  
Kalman_Filter(Angle_ax,Gyro_y);       //卡尔曼滤波计算倾角  
//Display10BitData(Kalman_Filter(Angle_ax,Gyro_y));  
/*//-------互补滤波-----------------------  
  
//补偿原理是取当前倾角和加速度获得倾角差值进行放大，然后与  
    //陀螺仪角速度叠加后再积分，从而使倾角最跟踪为加速度获得的角度  
//0.5为放大倍数，可调节补偿度；0.01为系统周期10ms  
Angle = Angle + (((Angle_ax-Angle)*0.5 + Gyro_y)*0.01);*/  
}   

