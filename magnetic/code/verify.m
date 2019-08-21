clear all;
X=load('C:\Users\zhang\Desktop\log.txt');
len = size(X);
t= 0:0.02:(len(1)-1)*0.02;
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
Mk = X;
Bk = zeros(len(1),3);
Fk = zeros(len(1),3);
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
for i = 1:1:len(1)
   if Mk(i,3)> MAX_THRES
       Cnt_arr = Cnt_arr +1;
       if Cnt_arr >= 1
           car_flag(i,1) = 1;
       end
   end
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
     MAX_THRES = Bk(i,3)*(gamma)
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
         MAX_THRES = Bk(i,3)*gamma
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
subplot(2,2,1);
plot(t,Mk(:,1),'b',t,Fk(:,1),'r',t,Bk(:,1),'g','linewidth',3);
legend('原始数据','滤波后','基线');
xlabel('time/sec')
ylabel('Manetic/gauss')
title('X axis gauss')
subplot(2,2,2);
plot(t,Mk(:,2),'b',t,Fk(:,2),'r',t,Bk(:,2),'g','linewidth',3);
legend('原始数据','滤波后','基线');
xlabel('time/sec')
title('Y axis gauss')
ylabel('Manetic/gauss')
subplot(2,2,3);
plot(t,Mk(:,3),'b',t,Fk(:,3),'y',t,Bk(:,3),'g',t,tmp_reg(:,1),'r',t,tmp_reg(:,2),'y','linewidth',3);
legend('z轴原始数据','z轴滤波后数据','z轴基线','z轴阈值上限','z轴阈值下限');
xlabel('time/sec')
ylabel('Manetic/gauss')
title('Z axis gauss')
subplot(2,2,4);
plot(t,car_flag(:,1),'b','linewidth',3);
xlabel('time/sec')
ylabel('Manetic/gauss')
title('Z axis gauss')
