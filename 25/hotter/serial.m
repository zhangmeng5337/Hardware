clear all;
close all;
clc;
newobjs = instrfind
fclose(newobjs)
s = serial('COM5'); % COMx为串口号，根据实际情况修改
set(s,'BaudRate',2250000); % 设置波特率为9600bps
%set(s, 'OutputBufferSize', 1000000);
%s=serial('COM5','terminator','CR/LF');
fopen(s); % 打开串口
xData = 0;yData= 0;
% 创建一个空白的图形对象
figure;
hold on; % 保持当前图形并添加其他元素到同一图形上
% axis([0 40 5 30])  %设置坐标系范围
x = linspace(0,800);
% x = 0; % x轴数据点
y = 0; % y轴数据点
plot(x, y, 'r', 'LineWidth', 2); % 初始绘制静态曲线
title('动态曲线');
xlabel('X轴');
ylabel('Y轴');
grid on;
k=0;
tic;
fs=9000;
step = 1/9000;
%x = linspace(0,step, 100);
%axis([0 100 0 0.2]); 
while true
     formatSpec = '%f'; % 格式化输入，此处举例为两个整数之间的空格分隔
     %y =fscanf(s, formatSpec) % 读取两个整数值;
     
         try
        data = fgets(s); % 读取一行数据
        y=str2double(data);
        k = [k y];
     x=x+step;
    set(gca,'Color','white'); % 清除之前的曲线
    plot(x, y, 'b--', 'LineWidth', 2); % 重新绘制新的曲线

    drawnow; % 立即显示更新后的图形
    catch ME
        if isequal(ME.identifier, 'MATLAB:serial:SerialPortError')
            errorMessage = sprintf('An error occurred while reading from the serial port.\n\tError message: %s', ME.message);
            disp(errorMessage);
            
            break; % 结束循环
        else
            rethrow(ME);
        end
    end
end



fclose(s); % 关闭串口
delete(s); % 删除串口对象
clear s; % 清空变量