clear all;
clc;
delete(instrfindall);   %先关闭串口，否则可能导致出错

global x            %全局变量，供串口中断函数使用

global t;			%全局变量，这里根据需要绘制图形的个人需要而设
global m;
global i;

t = [0];      %时间轴
m = [0];      %数据轴
i = 0;        %用于计数

p = plot(t,m);
axis([i-50 i+50 -1 10]);
grid on;

%%----------------------------配置部分----------------------------

%创建串口对象，直接在命令框输入serial('com1')可查看串口参数
x = serial('com1');
%设置或修改串口通信参数
set(x,'BaudRate',115200);

%设置中断触发方式

set(x,'BaudRate',9600,'DataBits',8,'StopBits',1,'Parity','none','FlowControl','none');
x.BytesAvailableFcnCount = 1;%进入中断的字节数
x.BytesAvailableFcnMode = 'byte';
x.InputBufferSize = 4096;

% set(x,'BytesAvailableFcnMode','byte')     %数据触发，根据发送来的数据长度来触发中断
% set(x,'BytesAvailableFcnCount',1)


x.BytesAvailableFcn = @Callback  %定义中断响应函数对象，类似于中断函数名

% fopen(x);                %打开串口，类似于开关
try
    fopen(x);
    fwrite(x,'hello')
catch err
    msgbox('串口打开失败！');
end
% fwrite(x,255); 	  %发送握手信号0xff，stm32接收到后开始回传数据
% pause           	  %需要暂停，来接收回传数据
%      
% fclose(x);
