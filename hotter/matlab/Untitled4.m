clear all;
clc;
delete(instrfindall);

%�½����ڶ���
s1=serial('COM3','BaudRate',921600); 

%���Դ�������
try
  fopen(s1);  %�򿪴��ڶ���
catch err
  fclose(instrfind);  %�رձ�ռ�õĴ���
  error('��ȷ��ѡ������ȷ�Ĵ���');  %���������ʾ
end

h = animatedline('color','r','linestyle','-.','linewidth',2,'marker','o');%ָ����������set(gca,'XLim',[0,8*pi],'YLim',[-1,1]);;  %�����հ׶�̬����
axis([0 40 5 30])  %��������ϵ��Χ
x = linspace(0,40,800);

xlabel('ʱ��');
ylabel('����');

for k = 1:length(x)
    y = (fscanf(arduino,'%d')/100.0);
    addpoints(h,x(k),y);
    drawnow
end

fclose(s1);  %�رմ���