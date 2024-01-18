clear all;
clc;
delete(instrfindall);

%新建串口对象
s1=serial('COM3','BaudRate',921600); 

%尝试代开串口
try
  fopen(s1);  %打开串口对象
catch err
  fclose(instrfind);  %关闭被占用的串口
  error('请确认选择了正确的串口');  %输出错误提示
end

h = animatedline('color','r','linestyle','-.','linewidth',2,'marker','o');%指定线条特性set(gca,'XLim',[0,8*pi],'YLim',[-1,1]);;  %创建空白动态曲线
axis([0 40 5 30])  %设置坐标系范围
x = linspace(0,40,800);

xlabel('时间');
ylabel('距离');

for k = 1:length(x)
    y = (fscanf(arduino,'%d')/100.0);
    addpoints(h,x(k),y);
    drawnow
end

fclose(s1);  %关闭串口