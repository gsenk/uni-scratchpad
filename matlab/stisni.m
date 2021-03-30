function stisni(slika)
%STISNI rise aproksimacije slike
%STISNI(slika)
%slika je ime slike (slika.jpg)
%rang aproksimacije vnesemo (0 za konec)
%14.04.2010
A=imread(slika,'jpg');
Adouble=double(A);
A1=Adouble(:,:,1);%nivo R (red)
A2=Adouble(:,:,2);%nivo G (green)
A3=Adouble(:,:,3);%nivo B (blue)

[U1,S1,V1]=svd(A1);
[U2,S2,V2]=svd(A2);
[U3,S3,V3]=svd(A3);

[m,n]=size(A1);
clc;
fprintf('Dimenzija slike je  %d x %d.\n',m,n)
k=input('Vnesite rang aproksimacije (0 za konec):  ');

while k>0
    A1k=zeros(size(A1));
    A2k=A1k;
    A3k=A1k;
    
    for j=1:k
      A1k=A1k+U1(:,j)*S1(j,j)*V1(:,j)';
      A2k=A2k+U2(:,j)*S2(j,j)*V2(:,j)';
      A3k=A3k+U3(:,j)*S3(j,j)*V3(:,j)';
    end
    Akdouble=zeros(size(Adouble));
    Akdouble(:,:,1)=A1k;
    Akdouble(:,:,2)=A2k;
    Akdouble(:,:,3)=A3k;
    
    Ak=uint8(round(Akdouble - 1)); %pretvorba nazaj v 'slikovni' format
    clf
    subplot(1,2,1);
    zoom(1.5);
    image(A);
    axis equal;
    axis off;
    subplot(1,2,2);
    zoom(1.5);
    image(Ak);
    axis equal;
    axis off;
    clc;
    fprintf('Dimenzija slike je  %d x %d.\n',m,n)
    k=input('Vnesite rang aproksimacije (0 za konec):  ');
end