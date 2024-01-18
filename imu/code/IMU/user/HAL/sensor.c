
#include "sensor.h"
//У׼���ٶ� 
//void Angular_Velocity_Calibration(float *x,float *y,float *z)//У׼�Ĳ���׼�����Ľ�
//{
//	
//	float gyroX,gyroY,gyroZ;
//	
//	GetGyro(&gyroX,&gyroY,&gyroZ);
//	
//	//У׼
//	*x -= gyroX;
//	*y -= gyroY;
//	*z -= gyroZ;

//	
//}
////���������ٶȵļн�,���ؽǶ�ֵ
//void MPU_Angle_Analysis(float *rX,float *rY,float *rZ)
//{
//	
//	float accelX,accelY,accelZ;//������
//	float accelXSum,accelYSum,accelZSum;//������
//	int i;
//	float norm;//ģ��

//	//�˲�
//	for(i=0;i<10;i++)
//	{
//		GetAccel(&accelX,&accelY,&accelZ);
//		
//		//��ֹԽ��
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
//	accelX = accelX*180.0f/Pi;//����ת�Ƕ�
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
//�������ͼ�����  
//**************************************************************************************************  

int dis_data;                   //����  
  
  
//******�ǶȲ���************  
    float Gyro_y;        //Y�������������ݴ�  
float Angle_gy;      //�ɽ��ٶȼ������б�Ƕ�  
float Accel_x;  
    //X����ٶ�ֵ�ݴ�  
float Angle_ax;      //�ɼ��ٶȼ������б�Ƕ�  
float Angle;         //С��������б�Ƕ�  
unsigned char value;  //�Ƕ��������Ա��   
  
  
//void getangle()   
//{  
//    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);//��ȡԭʼ6������  
//    angleAx=atan2(ax,az)*180/PI;//������x��н�  
//    gyroGy=-gy/131.00;//������ٶ�  
//    Yijielvbo(angleAx,gyroGy);//һ���˲�  
//    Erjielvbo(angleAx,gyroGy);//�����˲�  
//    Kalman_Filter(angleAx,gyroGy);   //�������˲�  
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
// �������˲�  
//*********************************************************  
//Kalman�˲���20MHz�Ĵ���ʱ��Լ0.77ms��  
  
float Kalman_Filter(float Accel,float Gyro)    
{  
 
  static const float Q_angle=0.001;  
	static const float Q_gyro=0.003;
	static const float R_angle=0.5;
	static const float dt=0.01;	                  //dtΪkalman�˲�������ʱ��;
	static const char  C_0 = 1;
	static float Q_bias, Angle_err;
	static float PCt_0, PCt_1, E;
	static float K_0, K_1, t_0, t_1;
	static float Pdot[4] ={0,0,0,0};
	static float PP[2][2] = { { 1, 0 },{ 0, 1 } };
	
Angle+=(Gyro - Q_bias) * dt; //�������  
  
Pdot[0]=Q_angle - PP[0][1] - PP[1][0]; // Pk-����������Э�����΢��  
  
Pdot[1]=- PP[1][1];  
Pdot[2]=- PP[1][1];  
Pdot[3]=Q_gyro;  
  
PP[0][0] += Pdot[0] * dt;   // Pk-����������Э����΢�ֵĻ���  
PP[0][1] += Pdot[1] * dt;   // =����������Э����  
PP[1][0] += Pdot[2] * dt;  
PP[1][1] += Pdot[3] * dt;  
  
Angle_err = Accel - Angle;  
//zk-�������  
  
PCt_0 = C_0 * PP[0][0];  
PCt_1 = C_0 * PP[1][0];  
  
E = R_angle + C_0 * PCt_0;  
  
K_0 = PCt_0 / E;  
K_1 = PCt_1 / E;  
  
t_0 = PCt_0;  
t_1 = C_0 * PP[0][1];  
  
PP[0][0] -= K_0 * t_0;  
//����������Э����  
PP[0][1] -= K_0 * t_1;  
PP[1][0] -= K_1 * t_0;  
PP[1][1] -= K_1 * t_1;  
  
Angle += K_0 * Angle_err;  
//�������  
Q_bias  += K_1 * Angle_err; //�������  
Gyro_y   = Gyro - Q_bias;  
//���ֵ(�������)��΢��=���ٶ�  
return  Gyro_y;  
}    
  
//*********************************************************  
// ��Ǽ��㣨�������ںϣ�  
//*********************************************************  
  
  
void Angle_Calcu(float acc,float gyro)    
{  
  
//------���ٶ�--------------------------  
  
//��ΧΪ2gʱ�������ϵ��16384 LSB/g  
//�ǶȽ�Сʱ��x=sinx�õ��Ƕȣ����ȣ�, deg = rad*180/3.14  
//��Ϊx>=sinx,�ʳ���1.3�ʵ��Ŵ�  
  
//Accel_x  = GetData(ACCEL_XOUT_H);  
 //��ȡX����ٶ� 
Accel_x = acc;	
Angle_ax = (Accel_x - 1100) /16384;   //ȥ�����ƫ��,����õ��Ƕȣ����ȣ�  
Angle_ax = Angle_ax*1.4*180/3.14;     //����ת��Ϊ��,  
  
//Display10BitData(Angle_ax,2,1);  
  
//-------���ٶ�-------------------------  
  
//��ΧΪ2000deg/sʱ�������ϵ��16.4 LSB/(deg/s)  
//Gyro_y = GetData(GYRO_YOUT_H);  
     //��ֹʱ���ٶ�Y�����Ϊ-30����
Gyro_y = gyro;	  
Gyro_y = -(Gyro_y + 30)/16.4;         //ȥ�����ƫ�ƣ�������ٶ�ֵ,����Ϊ������   
//Angle_gy = Angle_gy + Gyro_y*0.01;  //���ٶȻ��ֵõ���б�Ƕ�.  
  
//Display10BitData(Gyro_y,8,1);  
  
//-------�������˲��ں�-----------------------  
Kalman_Filter(Angle_ax,Gyro_y);       //�������˲��������  
//Display10BitData(Kalman_Filter(Angle_ax,Gyro_y));  
/*//-------�����˲�-----------------------  
  
//����ԭ����ȡ��ǰ��Ǻͼ��ٶȻ����ǲ�ֵ���зŴ�Ȼ����  
    //�����ǽ��ٶȵ��Ӻ��ٻ��֣��Ӷ�ʹ��������Ϊ���ٶȻ�õĽǶ�  
//0.5Ϊ�Ŵ������ɵ��ڲ����ȣ�0.01Ϊϵͳ����10ms  
Angle = Angle + (((Angle_ax-Angle)*0.5 + Gyro_y)*0.01);*/  
}   

