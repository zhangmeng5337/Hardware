clc;
step = 1/Fs;
% [max_val max_index ] =max(original);
start_t =44.248;
end_t = start_t +0.05;
% len_t = size(original);
% len_t = len_t(1);
% t = 0:step:len_t*step-step;   % ʱ������
t = 0:step:((end_t-start_t));   % ʱ������
 
%   data_use_size = 2000;
%   x_creat(1:max_index) = x(max_index+data_use_size);
%   x_creat(max_index+data_use_size:end) = x(data_use_size+1);
smooth_xd = smooth(xd,4);
 x = xd(start_t/step:end_t/step); % �ź�
 len = size(t(1:end),2);
 len_t = size(t(1,:))

 x_creat = x(1:len(1));

% �����ź�ͼ
figure(3)
subplot(4,1,1);
plot(t,x_creat);
title('�ź�');
xlabel('ʱ�� (s)');
ylabel('����');

% ����FFT
% ����FFTͼ
N = length(x_creat)-1;

f = Fs*(0:(N/2-1))/N;
X = fft(x_creat)/N ;
subplot(4,1,2);
plot(f(2:end),abs(X(2:N/2)));
title('FFT');
xlabel('Ƶ�� (Hz)');
ylabel('����');

% ���㹦����
P = (2*abs(X(1:N/2)).^2)/N;

% ���ƹ�����ͼ
subplot(4,1,3);
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