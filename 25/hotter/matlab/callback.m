

% 创建一个空白的图形对象
figure;
hold on; % 保持当前图形并添加其他元素到同一图形上
x = linspace(0, 2*pi, 100); % x轴数据点
y = sin(x); % y轴数据点
plot(x, y, 'r', 'LineWidth', 2); % 初始绘制静态曲线
title('动态曲线');
xlabel('X轴');
ylabel('Y轴');
grid on;
 
% 定义一个计时器，每隔一段时间更新曲线
tic;
while true
    t = toc; % 获取经过的时间（单位为秒）
    
    % 根据时间生成新的y值
    new_y = sin(x + t/5); % 这里将原有的sin函数改为了sin(x+t)，使得曲线会随着时间而移动
    
    % 更新曲线
    set(gca,'Color','white'); % 清除之前的曲线
    plot(x, new_y, 'b--', 'LineWidth', 2); % 重新绘制新的曲线
    
    drawnow; % 立即显示更新后的图形
end

