function fft_fig(original,kal_man,Fs)
close (figure(3));
clc;
step = 1/Fs;
cpy_size = size(original);
i= 1;
sel_tick = 0;
last_tick = 0;
for i = 1:1:cpy_size(1)-1
    if abs(original(i+1)-original(i))>=20
        i
        i*step
        if(abs(i*step-last_tick)>=0.01)
            sel_tick= [sel_tick; i*step];
            last_tick = i*step
        end
    end
end
cpy_size = size(sel_tick);
start_index = 0;
end_index = 0;
original_cpy = [];
for i = 1:1:cpy_size(1)
    if sel_tick(i)>0
    start_index = round((sel_tick(i)+0.05)/step-50);
    end_index = round((sel_tick(i)+0.2)/step);
    
    original_cpy = [original_cpy;kal_man(start_index:end_index)];        
    end

end
% original_cpy=smooth(original_cpy,20);
% start_t = 1;
% [max_val max_index ] =max(original);
%  average_baseline = mean(original);
% 
% start_t =max_index*step;
% start_t =start_t+0.05;
% end_t = start_t +0.05;
% 
len = size(original_cpy);
t = 0:step:len(1)*step-step;   % 时间向量
% smooth_xd = smooth(kal_man,4);
 x = original_cpy; % 信号
%  len = size(t(1:end),2);
%  len_t = size(t(1,:))

 x_creat = x(1:len(1));

% 绘制信号图
figure(3)
subplot(4,1,1);
plot(t,x_creat);
title('信号');
xlabel('时间 (s)');
ylabel('幅度');

hold on;
disp('输入时间范围（例如：0.5 1）:');
start_fft_time  = input('','s');
start_fft_time = str2num(start_fft_time);

start_fft_time(1) = round(start_fft_time(1)/step);
start_fft_time(2) = round(start_fft_time(2)/step);

% 计算FFT
% 绘制FFT图
N = length(x_creat(start_fft_time(1):start_fft_time(2)))-1;

f = Fs*(0:(N/2-1))/N;
X = fft(x_creat(start_fft_time(1):start_fft_time(2)))/N ;
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
[ma, I]=max(P(2:end));	%ma为数组的最大值，I为下标值
fprintf(1, '信号频率最强处的频率值=%f\n', I/length(P)*Fs/2);
% plot3(t,f,abs(X(1:N/2+1)));
% figure(4)
% plot(t,x,'b');
% 
% hold on;
% plot(t,IX,'r');