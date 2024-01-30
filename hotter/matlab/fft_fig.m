function fft_fig(original,kal_man,Fs)
close (figure(3));
clc;
step = 1/Fs;
cpy_size = size(original);
i= 1;
sel_tick = 0;
last_tick = 0;
for i = 1:1:cpy_size(1)-100
    if abs(original(i+1)-original(i))>=5
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
for i = 1:1:cpy_size(1)-10
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
t = 0:step:len(1)*step-step;   % ʱ������
% smooth_xd = smooth(kal_man,4);
 x = original_cpy; % �ź�
%  len = size(t(1:end),2);
%  len_t = size(t(1,:))

 x_creat = x(1:len(1));

% �����ź�ͼ
figure(3)
subplot(4,1,1);
plot(t,x_creat);
title('�ź�');
xlabel('ʱ�� (s) �����ʣ�6000Hz');
ylabel('����');

hold on;
disp('����ʱ�䷶Χ�����磺0.5 1��:');
start_fft_time  = input('','s');
start_fft_time = str2num(start_fft_time);

if start_fft_time(1) == 0
    start_fft_time(1) = step;
end
start_fft_time(1) = round(start_fft_time(1)/step);
start_fft_time(2) = round(start_fft_time(2)/step);

% ����FFT
% ����FFTͼ

N = length(x_creat(start_fft_time(1):start_fft_time(2)))-1;

f = Fs*(0:(N/2-1))/N;
X = fft(x_creat(start_fft_time(1):start_fft_time(2)))/N ;
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
[ma, I]=max(P(2:end));	%maΪ��������ֵ��IΪ�±�ֵ
fprintf(1, '*****************************************************\r\n');
fprintf(1, '          �ź�Ƶ����ǿ����Ƶ��ֵ=%f\n', I/length(P)*Fs/2);
fprintf(1, '*****************************************************\r\n');

% plot3(t,f,abs(X(1:N/2+1)));
% figure(4)
% plot(t,x,'b');
% 
% hold on;
% plot(t,IX,'r');