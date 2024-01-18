clear all;
close all;
clc;
fclose(instrfind); 
% 创建串口对象
s = serial('COM3'); % 将COMx替换为实际的串口号
set(s, 'BaudRate', 921600); % 设置串口波特率
fopen(s);

% 初始化数据缓冲区
N = 4096; % 假设数据点的数量为 100
dataBuffer = zeros(N, 1); % N为数据点数

% 创建图形窗口
figure;

% 绘制空曲线
hLine = plot(dataBuffer, '-');
ylim([0, 3]); % 根据实际情况设置y轴范围

% 实时读取和绘制数据
while true
    % 读取数据
    fopen(s);
    data = fscanf(s, '%f') % 根据实际的数据格式进行读取
fclose(s);
    % 更新数据缓冲区
    dataBuffer = [dataBuffer(2:end); data];

    % 更新曲线数据
    set(hLine, 'YData', dataBuffer);

    % 实时刷新图形
    drawnow;

    % 控制刷新频率，调整延时时间以控制刷新频率
   % pause(0.00001);
end

% 关闭串口对象
fclose(s);
delete(s);
clear s;
