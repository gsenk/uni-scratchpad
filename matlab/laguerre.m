function z=laguerre(p,z0,eps,maxit)
%LAGUERRE poisce niclo polinoma z Laguerreovo metodo
%z=LAGUERRE(p,z0,eps,maxit)
%p je seznam koeficientov polinoma p=[a_n,a_{n-1},...,a_0]
%z0 je zacetni priblizek, eps je natancnost
%maxit je maksimalno stevilo iteracij

n=length(p)-1;
dp=polyder(p);
ddp=polyder(dp);

napaka=eps+1;
k=0;
while napaka>eps && k<maxit
  pz=polyval(p,z0);
  dpz=polyval(dp,z0);
  ddpz=polyval(ddp,z0);
  S1=dpz/pz;
  S2=(dpz^2-pz*ddpz)/pz^2;
  S3=sqrt((n-1)*(n*S2-S1^2));
  if abs(S1-S3)>abs(S1+S3)
    z1=z0-n/(S1-S3);
  else
    z1=z0-n/(S1+S3);
  end
  napaka=abs(z1-z0);
  z0=z1;
  k=k+1;
end
z=z1;  
