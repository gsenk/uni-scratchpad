function [L,U,P]=LU_delno(A)
%LU_DELNO je LU razcep z celnim pivotiranjem
%[L,U,P]=LU_DELNO(A)
%A je vhodna matrika
%L je spodnja trikotna z enkami na diagonali
%U je zgornja trikotna
%P je permutacijska

n=size(A,1);
P=eye(n);
L=zeros(n);
U=zeros(n);

for i=1:n-1
  [M,maxi]=max(abs(A(i:n,i)));
  maxi=maxi+i-1;%popravimo na globalni indeks
  P([i,maxi],:)=P([maxi,i],:);
  A([i,maxi],:)=A([maxi,i],:);
  A(i+1:n,i)=A(i+1:n,i)/A(i,i);
  A(i+1:n,i+1:n)=A(i+1:n,i+1:n)-A(i+1:n,i)*A(i,i+1:n);
end
P
L=tril(A,-1)+eye(n)
U=triu(A)
