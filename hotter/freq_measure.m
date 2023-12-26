clear all;
close all;
clc;
dat=load('Z:\D\soft\SerialDebug\dat.txt');

len = size(dat);
len = len(1)
len = len/2;
len = round(len);
len = len*2
x = dat(1:len-2,1)*1000;
original = x;
y=x(:,1); %��ȡʱ������



Fs=4000; %�ɼ�Ƶ��
T=1/Fs; %�ɼ�ʱ����
N=length(y); %�ɼ��źŵĳ���
t=(0:1:N-1)*T; %���������ɼ�ʱ���
t=t';  %ת�ó�������





 y = kalman(original);
 y=y(:,1);
 kal_man = y;


%iifr

 fs = 4000; % ������
fc = 1500; % ��ֹƵ��
N =20; % ����
b = fir1(N, fc/(fs/2)); % ʹ��fir1������Ƶ�ͨ�˲���ϵ��
%  freqz(b, 1); % �����˲�����Ƶ����Ӧ����
 x1get=filter(b,1,original);
% figure(2)
%  plot(t,original,'+b');
%  hold on;


% plot(t,kal_man,'g');
% hold on;
figure(1)
plot(t,original);
title('ԭʼ�ź� ');
xlabel('time/s');
ylabel('voltage/mV');
figure(2)



subplot(4,1,2);
plot(t,x1get,'r');
title('fir�˲���ԭʼ�ź� ');
xlabel('time/s');
ylabel('voltage/mV');

[xd,cxd,lxd] = wden(original,'rigrsure','s','one',2,'db3');
subplot(4,1,3);
plot(t,xd,'b');                                                %Mix_Signal_1 С���˲����ź�
title('С���任����ǿ�� ');
xlabel('time/s');
ylabel('voltage/mV');
%plot(t,y,'black');

yy=smooth(xd,5);
orinal_smooth = yy;
subplot(4,1,4);
plot(t,yy,'r');
title('ƽ������ǿ�� ');
xlabel('time/s');
ylabel('voltage/mV');
% legend('С���˲����ź�2','ƽ������'); 

% legend('�������˲��ź�','С���˲����ź�2','ƽ������'); 
std_vector = std(yy);
disp(std_vector);
% ���ɲ����ź�

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
% t = 0:step:(end_t-start_t)-step;   % ʱ������
% x = orinal_smooth(start_t/step:end_t/step-1,1); % �ź�
% 
% % �����ź�ͼ
% figure(4)
% subplot(3,1,1);
% plot(t,x);
% title('�ź�');
% xlabel('ʱ�� (s)');
% ylabel('����');
% 
% % ����FFT
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
% % ����FFTͼ
% subplot(3,1,2);
% plot(f,abs(X(1:N/2)));
% title('FFT');
% xlabel('Ƶ�� (Hz)');
% ylabel('����');
% 
% % ���㹦����
% P = (2*abs(X(1:N/2)).^2)/N;
% 
% % ���ƹ�����ͼ
% subplot(3,1,3);
% plot(f(2:end),P(2:end));
% title('�������ܶ�');
% xlabel('Ƶ�� (Hz)');
% ylabel('����');
% % plot3(t,f,abs(X(1:N/2+1)));
% figure(5)
% plot(t,x,'b');
% 
