clc;




step = 1/Fs;
start_t = 9.345;
end_time = 9.35;
end_count = round((end_time-start_t)/step);
end_t = start_t+step*end_count; 

t = 0:step:(end_t-start_t)-step;   % 时间向量
x = xd(start_t/step:end_t/step-1,1); % 信号

% 绘制信号图
figure(3)
subplot(3,1,1);
plot(t,x);
title('信号');
xlabel('时间 (s)');
ylabel('幅度');

% 计算FFT
N = length(x)/2;
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
% figure(4)
% plot(t,x,'b');
% 
% hold on;
% plot(t,IX,'r');