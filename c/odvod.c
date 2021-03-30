#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

int odvod(char *vhodna, char *izhodna){
   FILE *vh=fopen(vhodna, "rt");
   if (vh==NULL)return 1;
   FILE *iz=fopen(izhodna, "wt");
   if (iz==NULL){
      fclose(vh);
      return 2;
   }
   int i=0;
   double X1[10000], Y1[10000], od[10000], X2[10000];
   while (1){
      int a=fscanf(vh,"%lf %lf", &X1[i], &Y1[i]);
      //for i from t to n-1 do u[i] = (y[i+1]-y[i])/(x[i+1]-x[i]); <---- odvod
      if (a==EOF) break;
      i=i+1;
   }
   for(int j=0; j<i ;j++){
      X2[j]=(X1[j]+X1[j+1])/2;
      od[j]=(Y1[j+1]-Y1[j])/(X1[j+1]-X1[j]);
      fprintf(iz,"%f %f\n", X2[j], od[j]);
   }

   fclose(vh);
   fclose(iz);
   return 0;
}

int main (){
   int a=odvod("vhodna","izhodna");
   printf("%d", a);
}
