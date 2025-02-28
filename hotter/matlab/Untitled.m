clear all;
close all;
clc;
% 打开要读取的文本文件
 fileID = fopen('Z:\D\soft\SerialDebug\dat_mks2.txt');% data.txt为包含浮点数据的文本文件名
%fileID = fopen('C:\Users\zhang\Desktop\dat_mks2.txt');% data.txt为包含浮点数据的文本文件名


if fileID == -1
    error('无法打开文件！')
end
 
% 创建一个空的变量来存储读取到的浮点数据
data = [];
 i=0;
figure;
hold on; % 保持当前图形并添加其他元素到同一图形上
x = 0; % x轴数据点
y = 0; % y轴数据点
xValue =0;
plot(x, y, 'r', 'LineWidth', 2); % 初始绘制静态曲线

title('动态曲线');
xlabel('X轴');
ylabel('Y轴');
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
% axis([0 40 5 30])  %设置坐标系范围
% x = linspace(0,800);
while ~feof(fileID) % 当没有达到文件结尾时进行循环
    start_time = tic;
    lineData = fgetl(fileID); % 逐行读取文本内容
    
    if lineData == 0 % 如果该行为注释或者为空则跳过
        continue;
    end
    tokens = textscan(lineData, '%f'); % 将每行按照浮点格式提取数值部分
    floatValues = tokens{1}; % 获得所有提取到的浮点数值
    data = [data; floatValues]; % 将新读取到的浮点数值添加到已有数据后面   
    xValue=xValue+step;

   % 将新的点添加到坐标列表中
     x = [x xValue];
    y = [y floatValues];
%     i=i+1
    % 清除原先的图形
    count = count +1;
     if(count ==9000)
         set(p,'XData',x,'YData',y); 
        drawnow
        k = k+0.5;
        axis([k  k+5 0 4]);
%          drawnow; % 立即显示更新后的图形   
        count = 0;
        end_time = toc(start_time);
        fprintf('程序执行时间为 %.4fs\n', end_time);
     end
end
 
