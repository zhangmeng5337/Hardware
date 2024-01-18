clear all;
close all;
clc;
fclose(instrfind); 
% �������ڶ���
s = serial('COM3'); % ��COMx�滻Ϊʵ�ʵĴ��ں�
set(s, 'BaudRate', 921600); % ���ô��ڲ�����
fopen(s);

% ��ʼ�����ݻ�����
N = 4096; % �������ݵ������Ϊ 100
dataBuffer = zeros(N, 1); % NΪ���ݵ���

% ����ͼ�δ���
figure;

% ���ƿ�����
hLine = plot(dataBuffer, '-');
ylim([0, 3]); % ����ʵ���������y�᷶Χ

% ʵʱ��ȡ�ͻ�������
while true
    % ��ȡ����
    fopen(s);
    data = fscanf(s, '%f') % ����ʵ�ʵ����ݸ�ʽ���ж�ȡ
fclose(s);
    % �������ݻ�����
    dataBuffer = [dataBuffer(2:end); data];

    % ������������
    set(hLine, 'YData', dataBuffer);

    % ʵʱˢ��ͼ��
    drawnow;

    % ����ˢ��Ƶ�ʣ�������ʱʱ���Կ���ˢ��Ƶ��
   % pause(0.00001);
end

% �رմ��ڶ���
fclose(s);
delete(s);
clear s;
