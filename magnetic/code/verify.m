clear all;
X=load('C:\Users\zhang\Desktop\log\2019.11.11�ش�����.txt');
time_step = 0.05;
len = fix(size(X));
t= 0:time_step:(len(1)-1)*time_step;
% subplot(2,2,1);
% plot(t,X(:,1));
% xlabel('time/sec')
% ylabel('Manetic/gauss')
% title('X axis gauss')
% subplot(2,2,2);
% plot(t,X(:,2));
% xlabel('time/sec')
% title('Y axis gauss')
% ylabel('Manetic/gauss')
% subplot(2,2,3);
% plot(t,X(:,3));
% xlabel('time/sec')
% ylabel('Manetic/gauss')
% title('Z axis gauss')

sumX =0;
sumY =0;
sumZ =0;
N = 20;
Mk = X;%original data
Bk = zeros(len(1),3);%base line data
Fk = zeros(len(1),3);%filtered data
%filter start
for i = 1:1:len(1)
   if i<N
        for j=1:1: i
            sumX = sumX + Mk(j,1);
            sumY = sumY + Mk(j,2);
            sumZ = sumZ + Mk(j,3);
        end
        for j=len(1)-N+i:1: len(1)
            sumX = sumX + Mk(j,1);
            sumY = sumY + Mk(j,2);
            sumZ = sumZ + Mk(j,3);
        end 
        Fk(i,1)= sumX/N;
        Fk(i,2)= sumY/N;
        Fk(i,3)= sumZ/N;  
        sumX =0;
        sumY =0;
        sumZ =0;
   end
   if i>=N
         for j=i+1-N:1:i
            sumX = sumX + Mk(j,1);
            sumY = sumY + Mk(j,2);
            sumZ = sumZ + Mk(j,3);
        end 
        Fk(i,1)= sumX/N;
        Fk(i,2)= sumY/N;
        Fk(i,3)= sumZ/N; 
        sumX =0;
        sumY =0;
        sumZ =0;
   end   
end

alpha = 0.05;
car_flag = zeros(len(1),1);
tmp_reg=zeros(len(1),2);
Bk=Fk;
MAX_THRES = 0;
Cnt_arr = 0;
gamma = 1.2;
%trace base line
for i = 1:1:len(1)

   if car_flag == 1
     if i == 1
        Bk(i,1) = Bk(len(1),1);
        Bk(i,2) = Bk(len(1),2);
        Bk(i,3) = Bk(len(1),3);
     else
        Bk(i,1) = Bk(i-1,1); 
        Bk(i,2) = Bk(i-1,2); 
        Bk(i,3) = Bk(i-1,3); 
     end
     if Bk(i,3) >= 0
     MAX_THRES = Bk(i,3)*(gamma);
     MIN_THRES =  Bk(i,3)*(gamma-1);
     else
     MAX_THRES = -Bk(i,3)*(gamma-1)+Bk(i,3);
     MIN_THRES =  Bk(i,3)*(gamma);
     end
     tmp_reg(i,1) =MAX_THRES;
      tmp_reg(i,2) =MIN_THRES;    
   else
     if i == 1
        Bk(i,1) = Bk(len(1),1)*(1-alpha)+ Mk(i,1)*alpha;
        Bk(i,2) = Bk(len(1),2)*(1-alpha)+ Mk(i,2)*alpha;
        Bk(i,3) = Bk(len(1),3)*(1-alpha)+ Mk(i,3)*alpha;
     else
        Bk(i,1) = Bk(i-1,1)*(1-alpha)+ Mk(i,1)*alpha; 
        Bk(i,2) = Bk(i-1,2)*(1-alpha)+ Mk(i,2)*alpha; 
        Bk(i,3) = Bk(i-1,3)*(1-alpha)+ Mk(i,3)*alpha; 
     end
     if Bk(i,3) >= 0
         MAX_THRES = Bk(i,3)*gamma;
         MIN_THRES =  Bk(i,3)*(gamma-1);
     else
         MAX_THRES = -Bk(i,3)*(gamma-1)+Bk(i,3);
         MIN_THRES =  Bk(i,3)*(gamma);
     end
     tmp_reg(i,1) =MAX_THRES;
      tmp_reg(i,2) =MIN_THRES;
   end   
end
%plot(t,X(:,1),'b',t,Fk(:,1),'r',t,Bk(:,1),'g','linewidth',3);
% plot(t,X(:,1),'b',t,Fk(:,1),'r','linewidth',3);
% plot(t,X(:,1),'b',t,Fk(:,1),'r','linewidth',3);
% plot(t,X(:,1),'b',t,Fk(:,1),'r','linewidth',3);
subplot(2,3,1);
plot(t,Mk(1:len(1),1),'b',t,Fk(1:len(1),1),'r',t,Bk(1:len(1),1),'g','linewidth',3);
legend('ԭʼ����','�˲���','����');
xlabel('time/sec')
ylabel('Manetic/gauss')
title('X axis gauss')
subplot(2,3,2);
plot(t,Mk(1:len(1),2),'b',t,Fk(1:len(1),2),'r',t,Bk(1:len(1),2),'g','linewidth',3);
legend('ԭʼ����','�˲���','����');
xlabel('time/sec')
title('Y axis gauss')
ylabel('Manetic/gauss')
subplot(2,3,3);
plot(t,Mk(1:len(1),3),'b',t,Fk(1:len(1),3),'y',t,Bk(1:len(1),3),'g','linewidth',3);
legend('z��ԭʼ����','z���˲�������','z�����');
xlabel('time/sec')
ylabel('Manetic/gauss')
title('Z axis gauss')


% subplot(2,3,4);
% plot(t,Mk(:,3),'b',t,Fk(:,3),'y',t,Bk(:,3),'g','linewidth',3);
% legend('z��ԭʼ����','z���˲�������','z�����');
% xlabel('time/sec')
% ylabel('Manetic/gauss')
% title('Z axis gauss')
len_base=size(Bk); j=1
for i=1:1:len_base(1)-N
    z_tmp = Mk(i:N+i-1,:);
    zz_tmp = Bk(i:N+i-1,:);
sd_x(j,:) = var(z_tmp(:,:),1);
sd_xx(j,:) = var(zz_tmp(:,:),1);

j=j+1;
end
len_sd=fix(size(sd_xx));
%len_sd =6000;
t=0:time_step:time_step*(len_sd(1)-1);
%plot(t,sd_x(:,1),'b','linewidth',3);
figure(1);
%plot(t,sd_xx(1:len_sd(1),1),'b',t,sd_xx(1:len_sd(1),2),'y',t,sd_xx(1:len_sd(1),3),'r',t,X(1:len_sd(1),11),'r+',t,X(1:len_sd(1),12),'b','linewidth',3);

plot(t,sd_xx(1:len_sd(1),3),'g',t,X(1:len_sd(1),11),'r',t,X(1:len_sd(1),12),'b','linewidth',3);
legend('z�᷽��','z��ԭʼ����','z���');
xlabel('time/sec')
ylabel('������')
title('������')

figure(2);
[ax,h1,h2]=plotyy(t,X(1:len_sd(1),16),t,X(1:len_sd(1),12));
%plot(t,sd_x(1:len_sd(1)));
grid on
legend([h1,h2],'��������','Z�������',1); %��ע�����ߵ�ͼ��












subplot(2,3,4);
t=0:time_step:time_step*(len_sd(1)-1);
%plot(t,sd_x(:,1),'b','linewidth',3);
plot(t,Mk(1:len_sd(1),8),'g',t,sd_x(1:len_sd(1),3),'b','linewidth',3);
%legend('st Z����߷���','st Z�᷽��','Z����߼��㷽��','Z����㷽��');
% plot(t,Mk(1:len_sd(1),7),'y',t,Mk(1:len_sd(1),8),'g',t,sd_xx(:,3),'r',t,sd_x(:,3),'b','linewidth',3);
% legend('st Z����߷���','st Z�᷽��','Z����߼��㷽��','Z����㷽��');
xlabel('time/sec')
ylabel('������')
title('������')





