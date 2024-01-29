clear all;
close all;
clc;
newobjs = instrfind
fclose(newobjs)
s = serial('COM5'); % COMxΪ���ںţ�����ʵ������޸�
set(s,'BaudRate',2250000); % ���ò�����Ϊ9600bps
%set(s, 'OutputBufferSize', 1000000);
%s=serial('COM5','terminator','CR/LF');
fopen(s); % �򿪴���
xData = 0;yData= 0;
% ����һ���հ׵�ͼ�ζ���
figure;
hold on; % ���ֵ�ǰͼ�β��������Ԫ�ص�ͬһͼ����
% axis([0 40 5 30])  %��������ϵ��Χ
x = linspace(0,800);
% x = 0; % x�����ݵ�
y = 0; % y�����ݵ�
plot(x, y, 'r', 'LineWidth', 2); % ��ʼ���ƾ�̬����
title('��̬����');
xlabel('X��');
ylabel('Y��');
grid on;
k=0;
tic;
fs=9000;
step = 1/9000;
%x = linspace(0,step, 100);
%axis([0 100 0 0.2]); 
while true
     formatSpec = '%f'; % ��ʽ�����룬�˴�����Ϊ��������֮��Ŀո�ָ�
     %y =fscanf(s, formatSpec) % ��ȡ��������ֵ;
     
         try
        data = fgets(s); % ��ȡһ������
        y=str2double(data);
        k = [k y];
     x=x+step;
    set(gca,'Color','white'); % ���֮ǰ������
    plot(x, y, 'b--', 'LineWidth', 2); % ���»����µ�����

    drawnow; % ������ʾ���º��ͼ��
    catch ME
        if isequal(ME.identifier, 'MATLAB:serial:SerialPortError')
            errorMessage = sprintf('An error occurred while reading from the serial port.\n\tError message: %s', ME.message);
            disp(errorMessage);
            
            break; % ����ѭ��
        else
            rethrow(ME);
        end
    end
end



fclose(s); % �رմ���
delete(s); % ɾ�����ڶ���
clear s; % ��ձ���