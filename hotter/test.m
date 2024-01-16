clc;
step = 1/Fs;
% [max_val max_index ] =max(original);
start_t =44.248;
end_t = start_t +0.05;
% len_t = size(original);
% len_t = len_t(1);
% t = 0:step:len_t*step-step;   % 时间向量
t = 0:step:((end_t-start_t));   % 时间向量
 
%   data_use_size = 2000;
%   x_creat(1:max_index) = x(max_index+data_use_size);
%   x_creat(max_index+data_use_size:end) = x(data_use_size+1);
smooth_xd = smooth(xd,4);
 x = xd(start_t/step:end_t/step); % 信号
 len = size(t(1:end),2);
 len_t = size(t(1,:))

 x_creat = x(1:len(1));

% 绘制信号图
figure(3)
subplot(4,1,1);
plot(t,x_creat);
title('信号');
xlabel('时间 (s)');
ylabel('幅度');

% 计算FFT
% 绘制FFT图
N = length(x_creat)-1;

f = Fs*(0:(N/2-1))/N;
X = fft(x_creat)/N ;
subplot(4,1,2);
plot(f(2:end),abs(X(2:N/2)));
title('FFT');
xlabel('频率 (Hz)');
ylabel('幅度');

% 计算功率谱
P = (2*abs(X(1:N/2)).^2)/N;

% 绘制功率谱图
subplot(4,1,3);
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