

% ����һ���հ׵�ͼ�ζ���
figure;
hold on; % ���ֵ�ǰͼ�β��������Ԫ�ص�ͬһͼ����
x = linspace(0, 2*pi, 100); % x�����ݵ�
y = sin(x); % y�����ݵ�
plot(x, y, 'r', 'LineWidth', 2); % ��ʼ���ƾ�̬����
title('��̬����');
xlabel('X��');
ylabel('Y��');
grid on;
 
% ����һ����ʱ����ÿ��һ��ʱ���������
tic;
while true
    t = toc; % ��ȡ������ʱ�䣨��λΪ�룩
    
    % ����ʱ�������µ�yֵ
    new_y = sin(x + t/5); % ���ｫԭ�е�sin������Ϊ��sin(x+t)��ʹ�����߻�����ʱ����ƶ�
    
    % ��������
    set(gca,'Color','white'); % ���֮ǰ������
    plot(x, new_y, 'b--', 'LineWidth', 2); % ���»����µ�����
    
    drawnow; % ������ʾ���º��ͼ��
end

