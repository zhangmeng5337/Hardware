% 定义回调函数，用于处理接收到的数据  
function onSerialDataReceived(serialPort)  
    % 读取接收到的数据  
    data = fread(serialPort, serialPort.BytesAvailable);  
      
    % 将数据转换为数字格式（如果需要）  
    data = data - ' ' - 1; % 将字符数组转换为数字数组，假设数据是空格分隔的数字  
    data = str2num(data);  
      
    % 创建图形并显示数据  
    plot(data);  
    drawnow; % 更新图形窗口  
end