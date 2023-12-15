function X_data  = kalman(x)
%卡尔曼滤波估计小车的运动，这是一个含有不确定性噪声的动态系统，需要做最优估计
%clc;
%clear;
%Z = (1:500);          %假设小车理想情况下是匀速运动，每隔1s观测一次。后面会加上噪声
%noise = randn(1,500); 
Z = x(:,1);        %观测系统的小车的状态，可以适当突出噪声演示kalman的作用
len = size(Z);

X = [0; 0];           %预测的初始值，设为多少不重要

P = [1 0; 0 1];       %预测系统的初始协方差，设为多少不重要

F = [1 1; 0 1];       %状态转移矩阵，由于1s观测一次，delta_t为1

Q = [0.000001 0; 0 0.000001];   %相信观测转移矩阵，误差设置的较小

H = [1 0];                  %观测矩阵，只观测到小车位置

R = 1;                      %由于noise设置为标准正态分布，这里观测的协方差设置为1

X_data = zeros(len(1), 2);     %存储小车最优状态估计，初始设置为0，后面会不断更新，500行数据需要迭代500次完成更新

for t=1:len(1)

    X_ = F * X;

    P_ = F * P * F' + Q;

    K = P_ * H' / (H * P_ * H' + R);

    X = X_ + K * (Z(t) - H * X_);

    X_data(t, 1) = X(1);

    X_data(t, 2) = X(2);

    P = (eye(2) - K * H) * P_;

end

 
% plot(X_data(:, 1), 'r-','linewidth',3);
% xlabel('时间/s')
% 
% ylabel('小车位置/m')
% 
% title('kalman filter');
% 
%  
% 
% hold on;
% 
% plot(Z, 'b');
% 
% legend('kalman最优估计值', '带噪声的观测值');
% 
% hold off;
