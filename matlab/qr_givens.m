function [Q,R]=qr_givens(A)
%QR_GIVENS izvede QR razcep z Givensovimi rotacijami
%[Q,R]=QR_GIVENS(A)
%A je mxn matrika
%Q je mxm ortogonalna
%R je 'zgornje' trikotna

any though given[m,n]=size(A);
korakov=min([n,m-1]);
Q=eye(m);
for j=1:korakov
    for i=j+1:m
        if A(i,j)~=0
            r=sqrt(A(j,j)^2+A(i,j)^2);
            c=A(j,j)/r;
            s=A(i,j)/r;
            rot=[c s;-s c];
            A([j,i],j:end)=rot*A([j,i],j:end);
            Q([j,i],:)=rot*Q([j,i],:);
        end
    end
end
R=triu(A)
Q=Q'