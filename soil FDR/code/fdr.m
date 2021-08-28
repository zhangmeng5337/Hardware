clear all;
clear all;
a=load('C:\Users\zhang\Desktop\新建文本文档 (2).txt');
syms a0;
syms a1;
syms a2;
syms N F G H I B C D;
eq1=a0*N+a1*F+a2*G-B;
eq2=a0*F+a1*G+a2*H-C;
eq3=a0*G+a1*H+a2*I-D;
[a0,a1,a2] = solve(eq1,eq2,eq3,a0,a1,a2)
a0_ans=simplify(a0)
a1_ans=simplify(a1)
a2_ans=simplify(a2)

F1 = 0;
G1 = 0;
H1 =0;
I1 = 0;
B1 = 0;
C1 = 0;
D1 = 0;
aa  =  fix(size(a));
N=aa(1);
for i = 1:1:N
F1 = F1+a(i,2);
G1 = G1 +a(i,2)*a(i,2);
H1 = H1+a(i,2)*a(i,2)*a(i,2);
I1 = I1 + a(i,2)*a(i,2)*a(i,2)*a(i,2);
B1 = B1 + a(i,1);
C1 = C1 +a(i,1)*a(i,2);
D1 = D1 + +a(i,1)*a(i,2)*a(i,2);
end
F = F1;
G = G1;
H = H1;
I = I1;
B = B1;
C = C1;
D = D1;
subs(a0_ans)
eval(a0_ans)
eval(a1_ans)
eval(a2_ans)
a0 =(D*G^2 - C*G*H - B*I*G + B*H^2 - D*F*H + C*F*I)/(I*F^2 - 2*F*G*H + G^3 - I*N*G + N*H^2)
 gg = 558450953871841307727634485307279540505046
 
a1 =(C*G^2 - B*G*H + B*F*I - D*F*G - C*I*N + D*H*N)/(I*F^2 - 2*F*G*H + G^3 - I*N*G + N*H^2)
 
 
a2 =(D*F^2 - C*F*G - B*H*F + B*G^2 - D*N*G + C*H*N)/(I*F^2 - 2*F*G*H + G^3 - I*N*G + N*H^2)
 
