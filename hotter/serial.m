% 打开串口  
fclose(instrfind);
serialPort = serial('COM4', 'BaudRate', 921600); % 替换为您的串口名称和波特率  
fopen(serialPort);  
  
% 创建图形窗口  
figure;  
  
% 创建接收数据的回调函数  
dataReceived = @() onSerialDataReceived(serialPort);  
addlistener(serialPort, 'BytesAvailable', 'PostData', dataReceived);  
  
