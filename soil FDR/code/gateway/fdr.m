clear all;
a=load('C:\Users\Administrator\Desktop\data.txt');
b=a(:,1);
c=a(:,2);
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
size = length(a);

F = 0;
G = 0;
H =0;
I = 0;
B = 0;
C = 0;
D = 0;
N  = size;
for i = 1:1:size
F = F+a(i,1);
G = G +a(i,1)*a(i,1);
H = H+a(i,1)*a(i,1)*a(i,1);
I = I + a(i,1)*a(i,1)*a(i,1)*a(i,1);
B = B + a(i,2);
C = C +a(i,1)*a(i,2);
D = D + +a(i,2)*a(i,1)*a(i,1);
end

eval(a0_ans)
eval(a1_ans)
eval(a2_ans)


