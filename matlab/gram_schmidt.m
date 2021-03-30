function [Q,R]=gram_schmidt(A,tip)
%GRAM_SCHMIDT je Gram-Schmidtova ortogonalizacija
%[Q,R]=GRAM_SCHMIDT(A,tip)
%Q je ortogonalna, R zgornja trikotna
%A je matrika mxn, m>=n, rang(A)=n
%tip='gs' za klasicni Gram-Schmidt
%tip='mgs' za modificirani Gram-Schmidt

[m,n]=size(A);
Q=zeros(m,n);
R=zeros(n);
switch tip
  case 'gs'
    for k=1:n
      Q(:,k)=A(:,k);
      for i=1:k-1
        R(i,k)=Q(:,i)'*A(:,k);
        Q(:,k)=Q(:,k)-R(i,k)*Q(:,i);
      end
      R(k,k)=norm(Q(:,k),2);
      Q(:,k)=Q(:,k)/R(k,k);
    end
  case 'mgs' 
    for k=1:n
      Q(:,k)=A(:,k);
      for i=1:k-1
        R(i,k)=Q(:,i)'*Q(:,k);
        Q(:,k)=Q(:,k)-R(i,k)*Q(:,i);
      end
      R(k,k)=norm(Q(:,k),2);
      Q(:,k)=Q(:,k)/R(k,k);
    end
 end
