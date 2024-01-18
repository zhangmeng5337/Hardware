clear;clf;
X=load('F:\projects\git\Hardware\air_condition\docs\resis.txt');
t=-40:1:109; %�����¶� t ��ʵ������
%R=[500.3,421.0,357.3,306.0,260.0,224.0,194.0,168.0,145.0,127.0]; %������������ֵ R ��ʵ������ 
R=X'; %������������ֵ R ��ʵ������ 
a=ones(size(t)); 
T=273.15*a+t; %�������¶ȱ�Ϊ�����¶� 
fT=a./T; %�����¶����� 
fR=log(R);%�����ֵ����Ȼ����
p=polyfit(fT,fR,1); %��С���˷��������
A=p(2);%�ؾ� A 
B=p(1);%б�� B 
r=corrcoef(fT,fR);%���ϵ�� r 
A,B,r,1;%��ʾ���� A,B ��������� r
plot(fT,fR,'*'); %�����ǻ������������¶���������
hold on;
plot(fT,A+B*fT,'-'); xlabel('1/T/K'); ylabel('log(R)/\Omega');

for t=1:1:150
TT(t)=B/(log(X(t))-A);
%T=273.15*a+t; %�������¶ȱ�Ϊ�����¶� 
tt(t)=TT(t)-273.15;
end
time = 1:1:150;
t=-40:1:109; %�����¶� t ��ʵ������
plot(time,tt(1:150),'b',time,t(1:150),'r');



