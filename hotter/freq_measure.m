clear all;
close all;
clc;
load('Z:\D\soft\SerialDebug\dat.txt');
len = size(dat);
len = len(1)
len = len/2;
len = round(len);
len = len*2
x = dat(1:len-2,1)*1000;
y=x(:,1); %读取时域数据
 
Fs=1000; %采集频率
 
T=1/Fs; %采集时间间隔
 
N=length(y); %采集信号的长度
 
t=(0:1:N-1)*T; %定义整个采集时间点
 
t=t';  %转置成列向量
figure(1)
plot(t,y*1000);
title('原始信号 ');
xlabel('time/s');
ylabel('voltage/mV');



figure(2);
%  plot(t,y,'b');                                %Mix_Signal_1 原始信号
%  hold on;
 y = kalman(y);
 y=y(:,1);
plot(t,y,'black');
hold on;

[xd,cxd,lxd] = wden(y,'sqtwolog','s','one',2,'db3');
plot(t,xd,'b');                                                %Mix_Signal_1 小波滤波后信号

%plot(t,y,'black');
hold on;
yy=smooth(xd,200);
plot(t,yy,'r');
title('光感强度 ');
xlabel('time/s');
ylabel('voltage/V');
legend('卡尔曼滤波信号','小波滤波后信号2','平滑处理'); 
std_vector = std(yy);
disp(std_vector);
% 生成测试信号
start_t = 111;
end_t = 112;
step = 1/Fs;
t = 0:step:(end_t-start_t)-step;   % 时间向量
x = yy(start_t/step:end_t/step-1,1); % 信号

% 绘制信号图
figure(3)
subplot(3,1,1);
plot(t,x);
title('信号');
xlabel('时间 (s)');
ylabel('幅度');

% 计算FFT
N = length(x);
% len = N/2;
% N = round(len);
% N = N*2;

X = fft(x);
IX=ifft(X);
plot(t,IX);
f = Fs*(0:(N/2-1))/N;

% 绘制FFT图
subplot(3,1,2);
plot(f,abs(X(1:N/2)));
title('FFT');
xlabel('频率 (Hz)');
ylabel('幅度');

% 计算功率谱
P = (2*abs(X(1:N/2)).^2)/N;

% 绘制功率谱图
subplot(3,1,3);
plot(f(2:end),P(2:end));
title('功率谱密度');
xlabel('频率 (Hz)');
ylabel('功率');
% plot3(t,f,abs(X(1:N/2+1)));
figure(4)
plot(t,x,'b');

hold on;
plot(t,IX,'r');