% ����ص����������ڴ�����յ�������  
function onSerialDataReceived(serialPort)  
    % ��ȡ���յ�������  
    data = fread(serialPort, serialPort.BytesAvailable);  
      
    % ������ת��Ϊ���ָ�ʽ�������Ҫ��  
    data = data - ' ' - 1; % ���ַ�����ת��Ϊ�������飬���������ǿո�ָ�������  
    data = str2num(data);  
      
    % ����ͼ�β���ʾ����  
    plot(data);  
    drawnow; % ����ͼ�δ���  
end