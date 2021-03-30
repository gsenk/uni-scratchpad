#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

double premica(char *vhodna, char *izhodna){
   FILE *vh=fopen(vhodna, "rt");
   if (vh==NULL)return 1;
   FILE *iz=fopen(izhodna, "wt");
   if (iz==NULL){
      fclose(vh);
      return 2;
   }
   int j=0;
   double x[1000], y[1000], napaka[1000];
   double vsotax=0, vsotay=0, vsotax2=0, vsotaxy=0, vsota1=0;
   double xx, yy, zz;
   while (1){
      int a=fscanf(vh,"%lf %lf %lf", &xx, &yy, &zz);
      if (a==EOF) break;

      x[j]=xx;
      y[j]=yy;
      napaka[j]=zz;
      j=j+1;
   }
   for(int i=0; i<j;i++){
      vsota1=vsota1+1/(napaka[i]*napaka[i]);
      vsotax=vsotax+x[i]/(napaka[i]*napaka[i]);
      vsotay=vsotay+y[i]/(napaka[i]*napaka[i]);
      vsotax2=vsotax2+x[i]*x[i]/(napaka[i]*napaka[i]);
      vsotaxy=vsotaxy+x[i]*y[i]/(napaka[i]*napaka[i]);
   }


   double k, n;
   k=(vsota1*vsotaxy-vsotax*vsotay)/(vsota1*vsotax2-vsotax*vsotax);
   n=(vsotax2*vsotay-vsotax*vsotaxy)/(vsota1*vsotax2-vsotax*vsotax);

   double vsotan=0;
   for(int i=0; i<j;i++){
      vsotan=vsotan+(y[i]-k*x[i]-n)*(y[i]-k*x[i]-n);
   }

   fprintf(iz, "%f\n%f\n%f", k, n,vsotan);

   fclose(vh);
   fclose(iz);
   return 0;
}

int main (){
   int a=premica("HitrostTokaOdFrekvence.dat", "1NHitrostTokaOdFrekvence.dat");
   printf("%d", a);
}
