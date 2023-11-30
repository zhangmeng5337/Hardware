clear all;
clc;
delete(instrfindall);   %�ȹرմ��ڣ�������ܵ��³���

global x            %ȫ�ֱ������������жϺ���ʹ��

global t;			%ȫ�ֱ��������������Ҫ����ͼ�εĸ�����Ҫ����
global m;
global i;

t = [0];      %ʱ����
m = [0];      %������
i = 0;        %���ڼ���

p = plot(t,m);
axis([i-50 i+50 -1 10]);
grid on;

%%----------------------------���ò���----------------------------

%�������ڶ���ֱ�������������serial('com1')�ɲ鿴���ڲ���
x = serial('com1');
%���û��޸Ĵ���ͨ�Ų���
set(x,'BaudRate',115200);

%�����жϴ�����ʽ

set(x,'BaudRate',9600,'DataBits',8,'StopBits',1,'Parity','none','FlowControl','none');
x.BytesAvailableFcnCount = 1;%�����жϵ��ֽ���
x.BytesAvailableFcnMode = 'byte';
x.InputBufferSize = 4096;

% set(x,'BytesAvailableFcnMode','byte')     %���ݴ��������ݷ����������ݳ����������ж�
% set(x,'BytesAvailableFcnCount',1)


x.BytesAvailableFcn = @Callback  %�����ж���Ӧ���������������жϺ�����

% fopen(x);                %�򿪴��ڣ������ڿ���
try
    fopen(x);
    fwrite(x,'hello')
catch err
    msgbox('���ڴ�ʧ�ܣ�');
end
% fwrite(x,255); 	  %���������ź�0xff��stm32���յ���ʼ�ش�����
% pause           	  %��Ҫ��ͣ�������ջش�����
%      
% fclose(x);
