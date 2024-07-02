clear all;
clc;
close all;

 
% 创建模糊PID控制器的模糊规则
fpid = readfis('FUZZY_PID.fis'); % 模糊P的规则



ts=0.005;  %采样时间=0.005s
sys=tf(0.998,[0.021,1]);   %建立被控对象传递函数，即式4.1
dsys=c2d(sys,ts,'z');      %离散化
[num,den]=tfdata(dsys,'v');   %
e_1=0;      %前一时刻的偏差  
e_2=0;
Ee=0;       %累积偏差
u_1=0.0;    %前一时刻的控制量
y_1=0;       %前一时刻的输出
%PID参数

u=zeros(1,1000);
time=zeros(1,1000);
fuzzyk=[];
% r(1) = 0;
for k=1:1:1000
    time(k)=k*ts;   %时间参数
    r(k)=1500;
    if mod(k,100)==0
    r(k)=r(k-1)+150;      %给定量
    else 
        if k>1
        r(k) = r(k-1);
        else 
            r(k) = 0;
        end
    end
    y(k)=-1*den(2)*y_1+num(2)*u_1+num(1)*u(k);
    e(k)=r(k)-y(k);   %偏差
    kp_I_d = evalfis(fpid, [e(k) e(k)-e_1]); % 模糊计算P的输出
    kp = kp_I_d(1); % 模糊计算D的输出
    ki = kp_I_d(2); % 模糊计算D的输出
    kd = kp_I_d(3); % 模糊计算D的输出
     fuzzyk = [ fuzzyk;kp_I_d];
%     kp =3; % 模糊计算D的输出
%     ki = 0.8; % 模糊计算D的输出
%     kd = 0.25; % 模糊计算D的输出

    if(k>1)
    detalu = u(k-1);
    else 
        detalu = 0;
    end
    u(k)=detalu+kp*(e(k)-e_1)+ki*e(k)+kd*(e(k)-2*e_1+e_2);  
   
    Ee=Ee+e(k);    
    u_1=u(k);    
    y_1=y(k); 
    e_2=e_1;
    e_1=e(k);
    
end
figure(1)
p1=plot(time,r);xlim([0,1]);hold on;
p2=plot(time,y);xlim([0,1]);
legend([p1,p2],'设定值','实际值');
figure(2)
p3=plot(time,fuzzyk(:,1));xlim([0,1]);hold on;
p4=plot(time,fuzzyk(:,2));xlim([0,1]);hold on;
p5=plot(time,fuzzyk(:,3));xlim([0,1]);
legend([p3,p4,p5],'kp','ki','kd');



