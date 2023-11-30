

function Callback(obj,event)   %创建中断服务函数，绘制图像

global t; %时间
global m; %纵坐标
global i; %时间变化值


    disp('I receive!')
    out = fscanf(obj);
    data = str2num(out)	%将接收到的字符转换为数值
     
     %%----------------------以下根据需要自行编写-------------------------------
    t = [t i];							
    m = [m data];
    plot(t,m)
    xlabel('t');
    ylabel('data');
    axis([i-50 i+50 0 100]);
    grid on;
    
    i=i+1;

end
