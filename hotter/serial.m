% �򿪴���  
fclose(instrfind);
serialPort = serial('COM4', 'BaudRate', 921600); % �滻Ϊ���Ĵ������ƺͲ�����  
fopen(serialPort);  
  
% ����ͼ�δ���  
figure;  
  
% �����������ݵĻص�����  
dataReceived = @() onSerialDataReceived(serialPort);  
addlistener(serialPort, 'BytesAvailable', 'PostData', dataReceived);  
  
