clear;clf;
X=load('F:\projects\git\Hardware\air_condition\docs\resis.txt');
t=-40:1:109; %输入温度 t 的实验数据
%R=[500.3,421.0,357.3,306.0,260.0,224.0,194.0,168.0,145.0,127.0]; %输入热敏电阻值 R 的实验数据 
R=X'; %输入热敏电阻值 R 的实验数据 
a=ones(size(t)); 
T=273.15*a+t; %讲摄氏温度变为绝对温度 
fT=a./T; %绝对温度求倒数 
fR=log(R);%求电阻值的自然对数
p=polyfit(fT,fR,1); %最小二乘法线性拟合
A=p(2);%截距 A 
B=p(1);%斜率 B 
r=corrcoef(fT,fR);%相关系数 r 
A,B,r,1;%显示参数 A,B 及相关因子 r
plot(fT,fR,'*'); %以下是绘制热敏电阻温度特性曲线
hold on;
plot(fT,A+B*fT,'-'); xlabel('1/T/K'); ylabel('log(R)/\Omega');

for t=1:1:150
TT(t)=B/(log(X(t))-A);
%T=273.15*a+t; %讲摄氏温度变为绝对温度 
tt(t)=TT(t)-273.15;
end
time = 1:1:150;
t=-40:1:109; %输入温度 t 的实验数据
plot(time,tt(1:150),'b',time,t(1:150),'r');



