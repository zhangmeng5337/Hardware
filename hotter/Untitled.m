clear all;
close all;
clc;
% ��Ҫ��ȡ���ı��ļ�
 fileID = fopen('Z:\D\soft\SerialDebug\dat_mks2.txt');% data.txtΪ�����������ݵ��ı��ļ���
%fileID = fopen('C:\Users\zhang\Desktop\dat_mks2.txt');% data.txtΪ�����������ݵ��ı��ļ���


if fileID == -1
    error('�޷����ļ���')
end
 
% ����һ���յı������洢��ȡ���ĸ�������
data = [];
 i=0;
figure;
hold on; % ���ֵ�ǰͼ�β��������Ԫ�ص�ͬһͼ����
x = 0; % x�����ݵ�
y = 0; % y�����ݵ�
xValue =0;
plot(x, y, 'r', 'LineWidth', 2); % ��ʼ���ƾ�̬����

title('��̬����');
xlabel('X��');
ylabel('Y��');
% axis manual;
grid on;
k=-5;
tic;
fs=9000;
step = 1/9000;
count = 0;
last_x = 1;
last_y=1;
p = plot(x,y,'EraseMode','background','MarkerSize',5);
% axis([0 40 5 30])  %��������ϵ��Χ
% x = linspace(0,800);
while ~feof(fileID) % ��û�дﵽ�ļ���βʱ����ѭ��
    start_time = tic;
    lineData = fgetl(fileID); % ���ж�ȡ�ı�����
    
    if lineData == 0 % �������Ϊע�ͻ���Ϊ��������
        continue;
    end
    tokens = textscan(lineData, '%f'); % ��ÿ�а��ո����ʽ��ȡ��ֵ����
    floatValues = tokens{1}; % ���������ȡ���ĸ�����ֵ
    data = [data; floatValues]; % ���¶�ȡ���ĸ�����ֵ��ӵ��������ݺ���   
    xValue=xValue+step;

   % ���µĵ���ӵ������б���
     x = [x xValue];
    y = [y floatValues];
%     i=i+1
    % ���ԭ�ȵ�ͼ��
    count = count +1;
     if(count ==9000)
         set(p,'XData',x,'YData',y); 
        drawnow
        k = k+0.5;
        axis([k  k+5 0 4]);
%          drawnow; % ������ʾ���º��ͼ��   
        count = 0;
        end_time = toc(start_time);
        fprintf('����ִ��ʱ��Ϊ %.4fs\n', end_time);
     end
end
 
