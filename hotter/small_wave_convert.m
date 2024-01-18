%% 1.生成仿真信号，x为无噪声信号，sig为添加噪声后的信号
rng(123); %设置随机种子，保证每次生成噪声的一致性
N = 200;
t = linspace(0,1,N);
x = 4*sin(4*pi*t);
x = x - sign(t-0.3) - sign(0.72-t);
sig = x + 0.2*randn(size(t));
figure('Color','w');subplot(2,1,1);plot(t,x);title('未加入白噪声信号')
subplot(2,1,2);plot(t,sig);title('加入白噪声信号')

s = filterWaveletTh(sig,'db3','s',3,'visushrink',[]);  %调用函数进行滤波，filterWaveletTh函数获取方法见文末