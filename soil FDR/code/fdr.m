clear all;
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





a0 =(D*G^2 - C*G*H - B*I*G + B*H^2 - D*F*H + C*F*I)/(I*F^2 - 2*F*G*H + G^3 - I*N*G + N*H^2)
 
 
a1 =(C*G^2 - B*G*H + B*F*I - D*F*G - C*I*N + D*H*N)/(I*F^2 - 2*F*G*H + G^3 - I*N*G + N*H^2)
 
 
a2 =(D*F^2 - C*F*G - B*H*F + B*G^2 - D*N*G + C*H*N)/(I*F^2 - 2*F*G*H + G^3 - I*N*G + N*H^2)
 
