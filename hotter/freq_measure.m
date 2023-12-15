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
y=x(:,1); %��ȡʱ������
 
Fs=1000; %�ɼ�Ƶ��
 
T=1/Fs; %�ɼ�ʱ����
 
N=length(y); %�ɼ��źŵĳ���
 
t=(0:1:N-1)*T; %���������ɼ�ʱ���
 
t=t';  %ת�ó�������
figure(1)
plot(t,y*1000);
title('ԭʼ�ź� ');
xlabel('time/s');
ylabel('voltage/mV');



figure(2);
%  plot(t,y,'b');                                %Mix_Signal_1 ԭʼ�ź�
%  hold on;
 y = kalman(y);
 y=y(:,1);
plot(t,y,'black');
hold on;

[xd,cxd,lxd] = wden(y,'sqtwolog','s','one',2,'db3');
plot(t,xd,'b');                                                %Mix_Signal_1 С���˲����ź�

%plot(t,y,'black');
hold on;
yy=smooth(xd,200);
plot(t,yy,'r');
title('���ǿ�� ');
xlabel('time/s');
ylabel('voltage/V');
legend('�������˲��ź�','С���˲����ź�2','ƽ������'); 
std_vector = std(yy);
disp(std_vector);
% ���ɲ����ź�
start_t = 111;
end_t = 112;
step = 1/Fs;
t = 0:step:(end_t-start_t)-step;   % ʱ������
x = yy(start_t/step:end_t/step-1,1); % �ź�

% �����ź�ͼ
figure(3)
subplot(3,1,1);
plot(t,x);
title('�ź�');
xlabel('ʱ�� (s)');
ylabel('����');

% ����FFT
N = length(x);
% len = N/2;
% N = round(len);
% N = N*2;

X = fft(x);
IX=ifft(X);
plot(t,IX);
f = Fs*(0:(N/2-1))/N;

% ����FFTͼ
subplot(3,1,2);
plot(f,abs(X(1:N/2)));
title('FFT');
xlabel('Ƶ�� (Hz)');
ylabel('����');

% ���㹦����
P = (2*abs(X(1:N/2)).^2)/N;

% ���ƹ�����ͼ
subplot(3,1,3);
plot(f(2:end),P(2:end));
title('�������ܶ�');
xlabel('Ƶ�� (Hz)');
ylabel('����');
% plot3(t,f,abs(X(1:N/2+1)));
figure(4)
plot(t,x,'b');

hold on;
plot(t,IX,'r');