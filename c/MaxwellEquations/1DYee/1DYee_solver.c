// we write a 1D yee solver on FDTD grid with a source current j_z(x,t)=\sigma E_y(x,t)

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define Time 100

int main(){
  int J=100, j, buffer=1, i;

// delta has to be less then 1, because of dispersion
  int resolution=4;
  double delta_x=1/(double)resolution, delta=0.99, delta_t=delta*delta_x;

// declare arrays
  double E[J*resolution+2*buffer], B[J*resolution+2*buffer], upor=0.05, sigma=10;

// intital conditions
  for (j=0; j<(J*resolution+2*buffer); j++){
    E[j]=10*exp(-((j/(double)resolution-(double)J/2)*(j/(double)resolution-(double)J/2))/(sigma*sigma))*sin(j/(double)resolution/2);
    //E[j]=0;
    B[j]=10*exp(-((j/(double)resolution+0.5-(double)J/2)*(j/(double)resolution+0.5-(double)J/2))/(sigma*sigma))*sin((j/(double)resolution+0.5)/2);
  }


//=================================================================
// start with propagating in time
  FILE *yeeE = fopen("Maxwell_yeeE.txt", "wt");
  FILE *yeeB = fopen("Maxwell_yeeB.txt", "wt");
  FILE *yeeEP = fopen("PMaxwell_yeeE.txt", "wt");
  FILE *yeeBP = fopen("PMaxwell_yeeB.txt", "wt");

  for (i=0; i<Time*resolution;i++){
    if (i%resolution==0){
      for (j=1; j<(J*resolution+1); j++){
        fprintf(yeeE, "%.4f  %.4f  %.15f\n", (double)i/(double)resolution, (double)j/(double)resolution, E[j]);
        fprintf(yeeB, "%.4f  %.4f  %.15f\n", (double)i/(double)resolution, (double)j/(double)resolution, B[j]);
        fprintf(yeeEP, "%.15f\n", E[j]);
        fprintf(yeeBP, "%.15f\n", B[j]);
      }
      fprintf(yeeE, "\n");
      fprintf(yeeB, "\n");
    }

// propagation od E in time
    for (j=1; j<(J*resolution+1); j++){
    //E[j][n+1]=(B[j+1][n]-B[j][n])*delta+E[j][n];
      E[j]=(delta*(B[j+1]-B[j])+(1.0-upor*delta_t/2.0)*E[j])/(1.0+upor*delta_t/2.0);
    }
// boundary conditions
    E[0]=E[J*resolution];
    E[J*resolution+1]=E[1];

// propagation of B in time
    for (j=1; j<(J*resolution+1); j++){
      B[j]=delta*(E[j]-E[j-1])+B[j];
    }
// boundary conditions
    B[0]=B[J*resolution];
    B[J*resolution+1]=B[1];

  }
  fclose(yeeE);
  fclose(yeeB);
  return 0;
}
