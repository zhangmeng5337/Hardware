close all;
clear all;
ts=0.005;  %采样时间=0.005s
sys=tf(0.998,[0.021,1]);   %建立被控对象传递函数，即式4.1
dsys=c2d(sys,ts,'z');      %离散化
[num,den]=tfdata(dsys,'v');   %
e_1=0;      %前一时刻的偏差      
Ee=0;       %累积偏差
u_1=0.0;    %前一时刻的控制量
y_1=0;       %前一时刻的输出
%PID参数
kp=5.92;    
ki=0.13;
kd=0;
u=zeros(1,1000);
time=zeros(1,1000);
for k=1:1:1000
    time(k)=k*ts;   %时间参数
    r(k)=1500;      %给定量
    y(k)=-1*den(2)*y_1+num(2)*u_1+num(1)*u(k);
    e(k)=r(k)-y(k);   %偏差
    u(k)=kp*e(k)+ki*Ee+kd*(e(k)-e_1);   
    Ee=Ee+e(k);    
    u_1=u(k);    
    y_1=y(k);    
    e_1=e(k);
end
p1=plot(time,r,'-.');xlim([0,1]);hold on;
p2=plot(time,y,'--');xlim([0,1]);
hold on;
a=1;%控制积分分离的二值数
e_1=0;Ee=0;u_1=0.0;y_1=0;%重新初始化 

kp=0.22;    
ki=0.13;
kd=0;
for k=1:1:1000
    time(k)=k*ts;   %时间参数
    r(k)=1500;      %给定量
    y(k)=-1*den(2)*y_1+num(2)*u_1;
    e(k)=r(k)-y(k);   %偏差
    u(k)=kp*e(k)+ki*Ee+kd*(e(k)-e_1);   
     if ((u(k)>r(k)) && (e(k)>0))||((u(k)<0) && (e(k)<0))
         a=0;
     else 
         a=1;
     end     
    Ee=Ee+a*e(k);    
    u_1=u(k);    
    y_1=y(k);    
    e_1=e(k);
end
p3=plot(time,y,'-');xlim([0,1]);

hold on;

a=1;%控制积分分离的二值数
e_1=0;Ee=0;u_1=0.0;y_1=0;%重新初始化 
kp=0.22;    
ki=0.23;
kd=0.5;
for k=1:1:1000
    time(k)=k*ts;   %时间参数
    r(k)=1500;      %给定量
    y(k)=-1*den(2)*y_1+num(2)*u_1;
    e(k)=r(k)-y(k);   %偏差
    u(k)=kp*e(k)+ki*Ee+kd*(e(k)-e_1);   
     if ((u(k)>r(k)) && (e(k)>0))||((u(k)<0) && (e(k)<0))
         a=0;
     else 
         a=1;
     end     
    Ee=Ee+a*e(k);    
    u_1=u(k);    
    y_1=y(k);    
    e_1=e(k);
end
p4=plot(time,y,'black');xlim([0,1]);
title('含积分分离与不含积分分离的对比');
legend([p1,p2,p3,p4],'指令信号','不含积分分离','含积分分离','含积分分离调参数');
title('含积分分离与不含积分分离的对比');
% legend([p1,p2,p3],'指令信号','不含积分分离','含积分分离');