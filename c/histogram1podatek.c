#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

//histogram za 1 stolpec, torej samo vrednosti
int histogram(char *vhodna, char *izhodna, int predali){
   FILE *vh=fopen(vhodna, "rt");
   if(vh==NULL)return 1;

   FILE *iz=fopen(izhodna, "wt");
   if(iz==NULL){
      fclose(vh);
      return 2;
   }

   int vrednost[predali];
   for(int i=0;i<predali;i++){
      vrednost[i]=0;
   }
   double X[10000];
   int n=0;
   double xx;
   while(1){
      if(fgetc(vh)==EOF) break;
      fscanf(vh, "%lf", &xx);
      X[n]=xx;
      n++;
   }
   //dolocitev max in min za X os
   double min = X[0];
   double max = X[0];
   for (int i = 1; i < n; i++) {
      if (X[i] < min) min = X[i];
      if (X[i] > max) max = X[i];
   }
   double Xkorak =((max - min))/((double)predali);

   double Xmeje[predali+1];
   for(int i=0; i<(predali+1); i++){
      Xmeje[i]=min+i*Xkorak;
   }
   for(int i=0;i<n;i++){
      for(int j=0;j<predali;j++){
         if( X[i] >= Xmeje[j] && X[i] <= Xmeje[j+1]){
            vrednost[j]=vrednost[j]+1;
            break;
         }
      }
   }
   for(int i=0; i<predali; i++)
   fprintf(iz, "%f   %d\n",(Xmeje[i]+Xmeje[i+1])/2, vrednost[i]);

   // for(int i=0; i<predali; i++)
   // fprintf(iz, "%d\n", vrednost[i]);

   fclose(vh);
   fclose(iz);
   return n;
}

int main(){
   int x;
   printf("koliko predalckov zelis: ");
   scanf("%d", &x);
   int a=histogram("Vhodna","Izhodna", x);
   printf("%d", a);
}
