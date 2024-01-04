clear all;
close all;
clc;
 %dat=load('Z:\D\soft\SerialDebug\dat.txt');
dat=load('Z:\D\soft\SerialDebug\dat_mks2.txt');
len = size(dat);
len = len(1)
len = len/2;
len = round(len);
len = len*2
x = dat(1:len-2,1)*1000;
original = x;
 y=x(:,1); %读取时域数据


Fs=2000; %采集频率
T=1/Fs; %采集时间间隔
N=length(y); %采集信号的长度
t=(0:1:N-1)*T; %定义整个采集时间点
t=t';  %转置成列向量
 y = kalman(original);
 y=y(:,1);
 kal_man = y;


%iifr

 fs = 4000; % 采样率
fc = 1500; % 截止频率
N =20; % 阶数
b = fir1(N, fc/(fs/2)); % 使用fir1函数设计低通滤波器系数
 x1get=filter(b,1,original);

% figure(2)
% % plot(t,original);
% xlabel('time/s');
% ylabel('voltage/mV');
% % hold on;
% plot(t,x1get,'r');
% title('fir信号 ');
% xlabel('time/s');
% ylabel('voltage/mV');
% % legend('原始信号','fir滤波');
% 
figure(3)
[xd,cxd,lxd] = wden(original,'rigrsure','s','one',2,'db3');

% plot(t,original);
% hold on;
plot(t,xd,'r');                                                %Mix_Signal_1 小波滤波后信号

title('小波变换信号 ');
xlabel('time/s');
ylabel('voltage/mV');
% legend('原始信号','小波变换信号');
% figure(4)
% yy=smooth(xd,4);
% smooth_dat = yy;
% % plot(t,original);
% % hold on
% plot(t,yy,'r'); 
% title('平滑信号 ');
% xlabel('time/s');
% ylabel('voltage/mV');
% % legend('原始信号','平滑信号');
figure(5)
plot(t,original);
hold on
plot(t,kal_man,'r'); 


title('卡尔曼信号 ');
xlabel('time/s');
ylabel('voltage/mV');
legend('原始信号','卡尔曼信号');
% std_vector = std(yy);
% disp(std_vector);
% 生成测试信号

% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% 
% clc;
% start_t =15;
% end_t = start_t+5;    
% 
% 
% 
% step = 1/Fs;
% t = 0:step:(end_t-start_t)-step;   % 时间向量
% x = orinal_smooth(start_t/step:end_t/step-1,1); % 信号
% 
% % 绘制信号图
% figure(4)
% subplot(3,1,1);
% plot(t,x);
% title('信号');
% xlabel('时间 (s)');
% ylabel('幅度');
% 
% % 计算FFT
% N = length(x)/2;
% % len = N/2;
% % N = round(len);
% % N = N*2;
% 
% X = fft(x);
% IX=ifft(X);
% plot(t,IX);
% f = Fs*(0:(N/2-1))/N;
% 
% % 绘制FFT图
% subplot(3,1,2);
% plot(f,abs(X(1:N/2)));
% title('FFT');
% xlabel('频率 (Hz)');
% ylabel('幅度');
% 
% % 计算功率谱
% P = (2*abs(X(1:N/2)).^2)/N;
% 
% % 绘制功率谱图
% subplot(3,1,3);
% plot(f(2:end),P(2:end));
% title('功率谱密度');
% xlabel('频率 (Hz)');
% ylabel('功率');
% % plot3(t,f,abs(X(1:N/2+1)));
% figure(5)
% plot(t,x,'b');
% 
