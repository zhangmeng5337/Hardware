clc;
clear;
close all;

%% 提取信号
M = importdata('3.txt');
fsample=1000;%采样率为1KHz

[mx,my]=size(M);
Signal=M(:,2);%M的第一列为时间，第二列为信号

length=floor(mx/2);%取原始信号的一半。
S=Signal(1:length);

%% 高通滤波，去除基线漂移的影响
disp('-------------------------------------------');
disp('1：工具箱巴特沃斯高通滤波器');
disp('2：IIR高通滤波');
disp('3：FIR高通滤波');
disp('4：中值滤波');
disp('5：稀疏小波滤波');
disp('6：中值+小波滤波');
disp('-------------------------------------------');
choose=input('选择滤波方式choose=');
