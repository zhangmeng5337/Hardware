%% 1.���ɷ����źţ�xΪ�������źţ�sigΪ�����������ź�
rng(123); %����������ӣ���֤ÿ������������һ����
N = 200;
t = linspace(0,1,N);
x = 4*sin(4*pi*t);
x = x - sign(t-0.3) - sign(0.72-t);
sig = x + 0.2*randn(size(t));
figure('Color','w');subplot(2,1,1);plot(t,x);title('δ����������ź�')
subplot(2,1,2);plot(t,sig);title('����������ź�')

s = filterWaveletTh(sig,'db3','s',3,'visushrink',[]);  %���ú��������˲���filterWaveletTh������ȡ��������ĩ