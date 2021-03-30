function V=cholesky(A)
%CHOLESKY je razcep Choleskega
%V=CHOLESKY(A)
%A je simetricna  pozitivno definitna
%V je spodnja trikotna, da je V*V'=A

n=size(A,1);
V=zeros(n);
V(1,1)=sqrt(A(1,1));
V(2:n,1)=A(2:n,1)/V(1,1);
for k=2:n
  V(k,k)=sqrt(A(k,k)-V(k,1:k-1)*V(k,1:k-1)');
  for j=k+1:n
      V(j,k)=(A(j,k)-V(k,1:k-1)*V(j,1:k-1)')/V(k,k);
  end
end
