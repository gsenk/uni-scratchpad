#include <stdio.h>

//int func_yee(double *[2], double *[2], double *[2], double *[2], double, double, int, int);
/*
int func_yee(double *Eold, double *Bold, double *Enew, double *Bnew, double delta_t, double delta_x, int N, int J){
   printf("prisel sem v func_yee");
   int j, n;
   for (n=0; n<=N; n++){
      for (j=0; j<=J; j++){
         Enew[j][n+1]=(Bold[j+1][n]-Bold[j][n])*delta_t/delta_x+Eold[j][n];
         Bnew[j+1][n+1]=(Eold[j+1][n+1]-Eold[j][n+1])*delta_t/delta_x+Bold[j+1][n];
      }
   }

   return 0;
}*/
int main(){
   int N=100, J=100, j, n;
   double Eold[J][N], Bold[J][N], Enew[J][N], Bnew[J][N], delta_x=0.1;//delta_j=0.1,
   for (j=0; j<J; j++){
      for (n=0; n<N; n++){
         Eold[j][n]=0;
         Bold[j][n]=0;
         Enew[j][n]=0;
         Bnew[j][n]=0;
         if (j>44 && j<56) Eold[j][n]=1;
         if (n>44 && n<56) Bold[j][n]=1;
      }
   }

   // initial conditions
//=================================================================
   // start with propagating in time

   double t, delta_t=0.1;
   for (t=0; t<1.05; t=t+delta_t){

      // so we can write intial condition to folder
      if (t>-0.0001 && t<0.0001){
         FILE *yeeE = fopen("Maxwell_yeeE_00.txt", "wt");
         FILE *yeeB = fopen("Maxwell_yeeB_00.txt", "wt");

         for (j=0; j<J; j++){
            for (n=0; n<N; n++){
               fprintf(yeeE, "%.4f  %.4f  %.15f\n", j*delta_t, n*delta_x, Eold[j][n]);
               fprintf(yeeB, "%.4f  %.4f  %.15f\n", j*delta_t, n*delta_x, Bold[j][n]);
            }
            fprintf(yeeE, "\n");
            fprintf(yeeB, "\n");
         }
         fclose(yeeE);
         fclose(yeeB);
      }
      else if(t>0.4999 && t<0.5001){
         FILE *yeeE = fopen("Maxwell_yeeE_05.txt", "wt");
         FILE *yeeB = fopen("Maxwell_yeeB_05.txt", "wt");

         for (j=0; j<J; j++){
            for (n=0; n<N; n++){
               fprintf(yeeE, "%.4f  %.4f  %.15f\n", j*delta_t, n*delta_x, Eold[j][n]);
               fprintf(yeeB, "%.4f  %.4f  %.15f\n", j*delta_t, n*delta_x, Bold[j][n]);
            }
            fprintf(yeeE, "\n");
            fprintf(yeeB, "\n");
         }
         fclose(yeeE);
         fclose(yeeB);
      }
      else if(t>0.9999 && t<1.0001){
         FILE *yeeE = fopen("Maxwell_yeeE_10.txt", "wt");
         FILE *yeeB = fopen("Maxwell_yeeB_10.txt", "wt");

         for (j=0; j<J; j++){
            for (n=0; n<N; n++){
               fprintf(yeeE, "%.4f  %.4f  %.15f\n", j*delta_t, n*delta_x, Eold[j][n]);
               fprintf(yeeB, "%.4f  %.4f  %.15f\n", j*delta_t, n*delta_x, Bold[j][n]);
            }
            fprintf(yeeE, "\n");
            fprintf(yeeB, "\n");
         }
         fclose(yeeE);
         fclose(yeeB);
      }


   //func_yee(Eold, Bold, Enew, Bnew, delta_j, delta_x, N, J);
   //int j, n;
   for (n=0; n<N-1; n++){
      for (j=0; j<J-1; j++){
         Enew[j][n+1]=(Bold[j+1][n]-Bold[j][n])*delta_t/delta_x+Eold[j][n];
         Bnew[j+1][n+1]=(Eold[j+1][n+1]-Eold[j][n+1])*delta_t/delta_x+Bold[j+1][n];
         //Enew[j][n+1]=(Bold[j+1][n]-Bold[j][n])+Eold[j][n];
         //Bnew[j+1][n+1]=(Eold[j+1][n+1]-Eold[j][n+1])+Bold[j+1][n];
      }
   }


   //printf("prisel sem do menjave arrayev");
   // usher in new generation of arrays
   for (j=0; j<J; j++){
      for (n=0; n<N; n++){
         Eold[j][n]=Enew[j][n];
         Bold[j][n]=Bnew[j][n];
      }
   }


   }
   return 0;
}
