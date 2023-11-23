clear all;
clc;
payload = pt100S1;
len = size(payload(:,1))
k=1;
for i=1:1:(len(1))  %1-10   11-20 21   1  11 21 31 
    k = 1+(i-1)*10; 
    if i<21
        m = 11;
    else 
        m = 2;
    end

    for j=k:1:(k+10) 
         dat(j,1)=payload(i,m)
        if i<21
            m = m -1;
        else         
          if m <11
             m = m+1;
          end           
         end
    end

end
dat(:,2) = dat(:,1);
k = -199;
len = size(dat(:,1))
for i=1:1:(len(1))  %1-10   11-20 21   1  11 21 31 
  dat(i,1) =k;  
    if i<200
        k = k+1;
    else 
        
       k = k +1;
    end
    
end

k = 0;

for i=1:1:(len(1))  %1-10   11-20 21   1  11 21 31 
   
    if i<200
      ;
    else 
      dat(i,1) = k;    
       k = k +1;
    end
   
end

plot(dat(:,1),dat(:,2))