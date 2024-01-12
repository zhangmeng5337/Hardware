clear all;
close all;
clc;
filename = 'Z:\D\soft\SerialDebug\dat_mks2.txt'
dat=textread(filename,'%s');
dat = hex2dec(dat);
dat_len = size(dat(1:end),1)

j=0;
original = 0;
dat_4byte = 0;
result = 0;
head = 0;
index = 0;
find_index=dat_len-3;
cycle = 0;
start_index = 0;
for i=1:1:dat_len-3
    data_H = dat(i);
    data_M = dat(1+i);
    data_S = dat(2+i); 
    data_L = dat(3+i);
    if(data_H == 255&&data_M==255&&data_S==255&&data_L==255)
        if(data_L==255)
            head = head+1;
            if((7+i+4096*4)<=dat_len)
                data_H = dat(i+4+4096*4);%查看数据是否完整
                data_M = dat(5+i+4096*4);
                data_S = dat(6+i+4096*4); 
                data_L = dat(7+i+4096*4);
                if(data_H == 255&&data_M==255&&data_S==255&&data_L==254)%数据完整
                     for k = i+4:4:i+4096
                        data_H = dat(k+3);
                        data_M = dat(k+2);
                        data_S = dat(k+1); 
                        data_L = dat(k);
                        mergedByte = bitshift((bitshift(data_H, 8) + data_M), 16) + (bitshift(data_S, 8) + data_L); %402500 f7f9
                        values = typecast(uint32(mergedByte),'single');
                        j=j+1;
                        original(j) = values;
                     end
                    index = index + 1024*4;%数据帧数计数
                    
                end                
            end
            
        end
    end
end
original = original';

x = original;

len = size(original);
len = len(1)
len = len/2;
len = round(len);
len = len*2
x = x(1:len,1)*1000;
original = x;
 y=x(:,1); %读取时域数据


Fs=9000; %采集频率
T=1/Fs; %采集时间间隔
N=length(y); %采集信号的长度
t=0:T:(N-1)*T; %定义整个采集时间点
t=t';  %转置成列向量
 y = kalman(original);
 y=y(:,1);
 kal_man = y;


figure(3)
[xd,cxd,lxd] = wden(original','rigrsure','s','one',2,'db3');

% plot(t,original);
% hold on;
plot(t,xd,'r');                                                %Mix_Signal_1 小波滤波后信号

title('小波变换信号 ');
xlabel('time/s');
ylabel('voltage/mV');

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
