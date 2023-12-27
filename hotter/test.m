clc;




step = 1/Fs;
[max_index max_val] =max(original);
len_t = size(original);
len_t = len_t(1);
t = 0:step:len_t*step-step;   % ʱ������

  x_creat = original;
  data_use_size = 200;
  x_creat(1:max_index) = x(max_index+data_use_size);
  x_creat(max_index+data_use_size:end) = x(data_use_size+1);

% x = original(start_t/step:start_t/step+100); % �ź�


% �����ź�ͼ
figure(3)
subplot(3,1,1);
plot(t,x_creat);
title('�ź�');
xlabel('ʱ�� (s)');
ylabel('����');

% ����FFT
N = length(x_creat)/2;
% len = N/2;
% N = round(len);
% N = N*2;

X = fft(x_creat);
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