#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

int funkcija(char *vhodna, char *izhodna, int predali){
   FILE *vh=fopen(vhodna, "rt");
   if(vh==NULL) return 1;

   FILE *iz=fopen(izhodna, "wt");
   if(iz==NULL){
      fclose(vh);
      return 2;
   }
   double vrednost[predali];
   for(int i=0;i<predali;i++){
     vrednost[i]=0;
   }
   double X[10000];
   double Y[10000];
   int n=0;

   while(1){
      if(fgetc(vh)==EOF) break;
      double xx, yy;
      fscanf(vh, "   %lf      %lf", &xx, &yy);
      X[n]=xx;
      Y[n]=yy;
      n++;
   }

   //dolocitev max in min za Y os
   double ymin = Y[0];
   double ymax = Y[0];
   for (int i = 1; i < n; i++) {
      if (Y[i] < ymin) ymin = Y[i];
      if (Y[i] > ymax) ymax = Y[i];
   }
   double korak =((ymax - ymin))/((double)predali);

   double meje[predali+1];
   for(int i=0; i<(predali+1); i++){
   meje[i]=ymin+i*korak;}

   //ovrednotenje po dolzini na X osi
   for(int i=1; i<(n-1); i++){
      for(int j=0; j<predali; j++){
         if (Y[i]>meje[j]&& Y[i]<meje[j+1])
         {vrednost[j]=vrednost[j]+(0.5*(X[i+1]-X[i-1]));
         break;}
      }
   }


    //energijski interval
   double xmin = X[0];
   double xmax = X[0];
   for (int i = 1; i < n; i++) {
      if (X[i] < xmin) xmin = X[i];
      if (X[i] > xmax) xmax = X[i];
   }

   double dolzina=(xmax-xmin);

   for(int i=0; i<predali; i++)
   fprintf(iz, "%f %f\n",(meje[i]+meje[i+1])/2, vrednost[i]/dolzina);
   //fprintf(iz,"%f\n",vrednost[i]/dolzina);

   fclose(vh);
   fclose(iz);
   return 0;
}

int main(){
   int a;
   int stevilo;
   printf("Vnesi stevilo predalckov: ");
   scanf("%d", &stevilo);

   a=funkcija( "Vhodna", "Izhodna", stevilo);
   printf("%d", a);
}
