clc;




step = 1/Fs;
start_t = 9.345;
end_time = 9.35;
end_count = round((end_time-start_t)/step);
end_t = start_t+step*end_count; 

t = 0:step:(end_t-start_t)-step;   % ʱ������
x = xd(start_t/step:end_t/step-1,1); % �ź�

% �����ź�ͼ
figure(3)
subplot(3,1,1);
plot(t,x);
title('�ź�');
xlabel('ʱ�� (s)');
ylabel('����');

% ����FFT
N = length(x)/2;
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
% figure(4)
% plot(t,x,'b');
% 
% hold on;
% plot(t,IX,'r');