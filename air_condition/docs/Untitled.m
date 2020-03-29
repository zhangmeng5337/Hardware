clear all;
X=load('F:\projects\git\Hardware\air_condition\docs\data.txt');
x=X(:,1);
y=X(:,2);
%x=0.1:0.1:15
plot(x,y,'k.','markersize',15) 
hold on    
x=x';
axis([x(1,1) x(1,150) y(150,1) y(1,1)])    
y=y';
P3=polyfit(x,y,5)  

t=0.1:0.1:15;

S3=polyval(P3,x);  
plot(x,S3,'r','linewidth',1);
for i=1:1:150
ss(i,1)=-0.0007*((x(1,i))^3)+0.0342*((x(1,i))^2)-1.1909*(x(1,i))+27.3576;
end
plot(x,y,'r',x,ss,'b','linewidth',1);




yy=polyfit(x,y,4)
for i=1:1:150
x2(i,1)=-0.001*x(i,1)^3+0.0342*x(i,1)^2-1.7785*x(i,1)+30.0332;
end
subplot(1,2,1);
plot(x,y,'r','linewidth',3);
ylabel('阻值（K）')
xlabel('温度')
subplot(1,2,2);
plot(x,y,'r','linewidth',3);
plot(x,y,'r',x,x2(:,1),'b','linewidth',3);
ylabel('阻值（K）')
xlabel('温度')

clear all;
x=0:0.1:1     

y=[-0.447,1.978,3.28,6.16,7.08,7.34,7.66,9.56,9.48,9.3,11.2]    

plot(x,y,'k.','markersize',25)  

hold on    

axis([0 1.3 -2 16])    

P3=polyfit(x,y,3)  

t=0:0.1:1.2;

S3=polyval(P3,t);  

plot(t,S3,'r');